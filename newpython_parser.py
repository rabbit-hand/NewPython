import re
import sys
from pathlib import Path

BLOCK_HEADERS = {
    'if', 'for', 'while', 'def', 'class', 'try', 'except', 'finally', 'with', 'elif', 'else'
}


def convert_bracket_strings(line: str) -> str:
    return re.sub(r"\[([^\]]*)\]", lambda m: f'"{m.group(1).replace('"', '\\"')}"', line)


def convert_lambda(line: str) -> str:
    match = re.search(r"\blambda\s+([A-Za-z_][A-Za-z0-9_]*(?:\s+[A-Za-z_][A-Za-z0-9_]*)*?)\s+(.+)", line)
    if not match:
        return line
    args = ", ".join(match.group(1).split())
    body = match.group(2).strip()
    return line[:match.start()] + f"lambda {args}: {body}"


def convert_print(line: str) -> str:
    indent = re.match(r"^\s*", line).group(0)
    stripped = line.strip()[len('print '):].strip()
    tokens = re.findall(r'"[^"]*"|\'[^\']*\'|\S+', stripped)
    converted = []
    i = 0
    while i < len(tokens):
        token = tokens[i]
        if re.match(r'^[A-Za-z_][A-Za-z0-9_\.]*$', token) and i + 1 < len(tokens) and re.match(r'^[A-Za-z0-9_\.]+$', tokens[i + 1]):
            args = [tokens[i + 1]]
            i += 2
            while i < len(tokens) and re.match(r'^[A-Za-z0-9_\.]+$', tokens[i]):
                args.append(tokens[i])
                i += 1
            converted.append(f"{token}({', '.join(args)})")
            continue
        converted.append(token)
        i += 1
    return indent + f"print({', '.join(converted)})"


def convert_def(line: str) -> str:
    match = re.match(r'^def\s+([A-Za-z_][A-Za-z0-9_]*)\s*(.*)$', line)
    if not match:
        return line
    name, params = match.groups()
    params = ', '.join(params.split()) if params else ''
    return f"def {name}({params}):"


def convert_class(line: str) -> str:
    match = re.match(r'^class\s+([A-Za-z_][A-Za-z0-9_]*)(?:\s+(.*))?$', line)
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
    if re.search(r'[+\-*/%<>=&|^!:,()\[\]{}]', expr):
        return expr
    tokens = expr.split()
    if len(tokens) <= 1:
        return expr
    target = tokens[0]
    if not re.match(r'^[A-Za-z_][A-Za-z0-9_\.]*$', target):
        return expr
    return f"{target}({', '.join(tokens[1:])})"


def convert_assignment_or_expression(line: str) -> str:
    indent = re.match(r"^\s*", line).group(0)
    stripped = line.strip()
    if stripped.startswith('return '):
        rest = stripped[len('return '):].strip()
        return indent + 'return ' + convert_simple_call(rest)
    if '=' in stripped and '==' not in stripped:
        left, right = stripped.split('=', 1)
        return indent + left.strip() + ' = ' + convert_simple_call(right.strip())
    if re.match(r'^[A-Za-z_][A-Za-z0-9_\.]*\s+[A-Za-z0-9_\.\s]+$', stripped):
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
    indent = re.match(r"^\s*", line).group(0)
    stripped = line.strip()
    if stripped.startswith('def '):
        return indent + convert_def(stripped)
    if stripped.startswith('class '):
        return indent + convert_class(stripped)
    if stripped in {'else', 'try', 'finally', 'except'} or any(stripped.startswith(keyword + ' ') for keyword in {'if', 'elif', 'while', 'for', 'with', 'except'}):
        return indent + convert_block_header(line)
    return convert_assignment_or_expression(line)


def parse_newpython(source: str) -> str:
    lines = source.splitlines()
    end_lines = [i for i, line in enumerate(lines) if line.strip() == 'end']
    if len(end_lines) > 1:
        raise ValueError("NewPython v2 requires exactly one final 'end' line.")
    if end_lines and end_lines[0] != max(i for i, line in enumerate(lines) if line.strip()):
        raise ValueError("The single 'end' marker must appear only at the end of the script.")
    converted = []
    for line in lines:
        result = convert_line(line)
        if result is None:
            continue
        converted.append(result)
    return '\n'.join(converted)


def convert_file(path: Path) -> str:
    text = path.read_text(encoding='utf-8')
    return parse_newpython(text)


def main(argv: list[str] | None = None) -> int:
    argv = argv if argv is not None else sys.argv[1:]
    if not argv:
        print('Usage: python newpython_parser.py <source.newpy>')
        return 1
    source_path = Path(argv[0])
    if not source_path.exists():
        print(f"File not found: {source_path}")
        return 1
    print(convert_file(source_path))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
