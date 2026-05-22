import re
import sys
from pathlib import Path
import os
from typing import Optional

BLOCK_HEADERS = {
    'if', 'for', 'while', 'def', 'class', 'try', 'except', 'finally', 'with', 'elif', 'else'
}

BRACKET_STRINGS_RE = re.compile(r"\[([^\]]*)\]")
LAMBDA_RE = re.compile(r"\blambda\s+([A-Za-z_][A-Za-z0-9_]*(?:\s+[A-Za-z_][A-Za-z0-9_]*)*?)\s+(.+)")
DEF_RE = re.compile(r'^def\s+([A-Za-z_][A-Za-z0-9_]*)\s*(.*)$')
CLASS_RE = re.compile(r'^class\s+([A-Za-z_][A-Za-z0-9_]*)(?:\s+(.*))?$')
SIMPLE_CALL_RE = re.compile(r'[+\-*/%<>=&|^!:,()\[\]{}]')
IDENTIFIER_RE = re.compile(r'^[A-Za-z_][A-Za-z0-9_\.]*$')
WORD_ARG_RE = re.compile(r'^[A-Za-z0-9_\.]+$')
CALL_LINE_RE = re.compile(r'^[A-Za-z_][A-Za-z0-9_\.]*\s+[A-Za-z0-9_\.\s]+$')
KEYWORD_LINE_RE = re.compile(r'^(?:else|try|finally|except|elif|if|while|for|with)(?:\b|$)')
INDENT_RE = re.compile(r'^\s*')

# Security constants
MAX_FILE_SIZE = 10 * 1024 * 1024  # 10MB max file size
MAX_LINE_LENGTH = 10000  # Maximum line length to prevent DoS
ALLOWED_EXTENSIONS = {'.newpy', '.py', '.txt'}


def convert_bracket_strings(line: str) -> str:
    return BRACKET_STRINGS_RE.sub(lambda m: f'"{m.group(1).replace("\"", "\\\"")}"', line)


def convert_lambda(line: str) -> str:
    match = LAMBDA_RE.search(line)
    if not match:
        return line
    args = ", ".join(match.group(1).split())
    body = match.group(2).strip()
    return line[:match.start()] + f"lambda {args}: {body}"


def convert_print(line: str) -> str:
    indent = INDENT_RE.match(line).group(0)
    stripped = line.strip()[len('print '):].strip()
    tokens = re.findall(r'"[^"]*"|\'[^\']*\'|\S+', stripped)
    converted = []
    i = 0
    while i < len(tokens):
        token = tokens[i]
        if IDENTIFIER_RE.match(token) and i + 1 < len(tokens) and WORD_ARG_RE.match(tokens[i + 1]):
            args = [tokens[i + 1]]
            i += 2
            while i < len(tokens) and WORD_ARG_RE.match(tokens[i]):
                args.append(tokens[i])
                i += 1
            converted.append(f"{token}({', '.join(args)})")
            continue
        converted.append(token)
        i += 1
    return indent + f"print({', '.join(converted)})"


def convert_def(line: str) -> str:
    match = DEF_RE.match(line)
    if not match:
        return line
    name, params = match.groups()
    params = ', '.join(params.split()) if params else ''
    return f"def {name}({params}):"


def convert_class(line: str) -> str:
    match = CLASS_RE.match(line)
    if not match:
        return line
    name, bases = match.groups()
    if not bases:
        return f"class {name}():"
    bases = ', '.join(bases.split())
    return f"class {name}({bases}):"


def convert_block_header(line: str) -> str:
    stripped = line.strip()
    if stripped == 'else':
        return 'else:'
    if stripped == 'finally':
        return 'finally:'
    if stripped == 'except':
        return 'except:'
    if stripped.startswith('except '):
        return stripped + ':'
    if stripped.startswith('elif '):
        return stripped + ':'
    if stripped.startswith('with '):
        return stripped + ':'
    if stripped.startswith('if ') or stripped.startswith('while '):
        return stripped + ':'
    if stripped.startswith('for '):
        head = stripped[4:].strip()
        if ' in ' in head:
            target, iterable = head.split(' in ', 1)
            iterable = convert_simple_call(iterable.strip())
            return f"for {target.strip()} in {iterable}:"
        return stripped + ':'
    if stripped == 'try':
        return 'try:'
    return stripped


def convert_simple_call(expr: str) -> str:
    if SIMPLE_CALL_RE.search(expr):
        return expr
    tokens = expr.split()
    if len(tokens) <= 1:
        return expr
    target = tokens[0]
    if not IDENTIFIER_RE.match(target):
        return expr
    return f"{target}({', '.join(tokens[1:])})"


def convert_assignment_or_expression(line: str) -> str:
    indent = INDENT_RE.match(line).group(0)
    stripped = line.strip()
    if stripped.startswith('return '):
        rest = stripped[len('return '):].strip()
        return indent + 'return ' + convert_simple_call(rest)
    if '=' in stripped and '==' not in stripped:
        left, right = stripped.split('=', 1)
        return indent + left.strip() + ' = ' + convert_simple_call(right.strip())
    if CALL_LINE_RE.match(stripped):
        return indent + convert_simple_call(stripped)
    return line


def convert_line(line: str) -> str | None:
    stripped = line.strip()
    if not stripped:
        return line
    if stripped == 'end':
        return None
    line = convert_bracket_strings(line)
    line = convert_lambda(line)
    if stripped.startswith('print '):
        return convert_print(line)
    indent = INDENT_RE.match(line).group(0)
    stripped = line.strip()
    if stripped.startswith('def '):
        return indent + convert_def(stripped)
    if stripped.startswith('class '):
        return indent + convert_class(stripped)
    if KEYWORD_LINE_RE.match(stripped):
        return indent + convert_block_header(line)
    return convert_assignment_or_expression(line)


def parse_newpython(source: str) -> str:
    lines = source.splitlines()
    end_lines = [line for line in lines if line.strip() == 'end']
    if len(end_lines) > 1:
        raise ValueError("NewPython v2 requires exactly one final 'end' line.")
    if end_lines and lines and lines[-1].strip() != 'end':
        raise ValueError("The single 'end' marker must appear only at the end of the script.")
    converted = []
    for line in lines:
        result = convert_line(line)
        if result is None:
            continue
        converted.append(result)
    return '\n'.join(converted)


def validate_path(path: Path) -> None:
    """Validate file path for security."""
    # Resolve to absolute path to prevent path traversal
    try:
        abs_path = path.resolve()
    except (OSError, RuntimeError) as e:
        raise ValueError(f"Invalid path: {e}")
    
    # Check if path exists and is a file
    if not abs_path.exists():
        raise ValueError(f"File not found: {abs_path}")
    if not abs_path.is_file():
        raise ValueError(f"Path is not a file: {abs_path}")
    
    # Check file extension
    if abs_path.suffix.lower() not in ALLOWED_EXTENSIONS:
        raise ValueError(f"File extension not allowed: {abs_path.suffix}. Allowed: {ALLOWED_EXTENSIONS}")
    
    # Check file size
    try:
        file_size = abs_path.stat().st_size
        if file_size > MAX_FILE_SIZE:
            raise ValueError(f"File too large: {file_size} bytes (max: {MAX_FILE_SIZE} bytes)")
        if file_size == 0:
            raise ValueError("File is empty")
    except OSError as e:
        raise ValueError(f"Cannot read file size: {e}")


def validate_source(source: str) -> None:
    """Validate source code for security."""
    if not source:
        raise ValueError("Source code is empty")
    
    lines = source.splitlines()
    if len(lines) > 100000:  # Prevent excessive line count
        raise ValueError(f"Too many lines: {len(lines)} (max: 100000)")
    
    for i, line in enumerate(lines, 1):
        if len(line) > MAX_LINE_LENGTH:
            raise ValueError(f"Line {i} too long: {len(line)} characters (max: {MAX_LINE_LENGTH})")


def convert_file(path: Path) -> str:
    """Convert NewPython file with security validation."""
    validate_path(path)
    try:
        text = path.read_text(encoding='utf-8')
    except UnicodeDecodeError:
        raise ValueError("File encoding must be UTF-8")
    except OSError as e:
        raise ValueError(f"Cannot read file: {e}")
    
    validate_source(text)
    return parse_newpython(text)


def main(argv: list[str] | None = None) -> int:
    argv = argv if argv is not None else sys.argv[1:]
    if not argv:
        print('Usage: python newpython_parser.py <source.newpy>')
        return 1
    source_path = Path(argv[0])
    try:
        result = convert_file(source_path)
        print(result)
        return 0
    except ValueError as e:
        print(f"Error: {e}", file=sys.stderr)
        return 1
    except Exception as e:
        print(f"Unexpected error: {e}", file=sys.stderr)
        return 1


if __name__ == '__main__':
    raise SystemExit(main())
