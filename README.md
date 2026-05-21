# NewPython v2 — Single `end` Terminator

NewPython v2 introduces a simplified block syntax that uses indentation to structure code and requires exactly one final `end` marker at the end of the script.

## Key rules

- No colons (`:`) at the end of block headers.
- Use normal indentation to express nested blocks.
- Close every multi-line program with a single `end` line.
- The parser removes that final `end` and produces valid Python.

## Supported block headers

- `if`, `elif`, `else`
- `for`, `while`
- `def`, `class`
- `try`, `except`, `finally`
- `with`

## Example

```newpython
if x > 0
    if y > 0
        print [ok]
end
```

converts to:

```python
if x > 0:
    if y > 0:
        print("ok")
```

## Syntax highlights

- `print [Hello] name` becomes `print("Hello", name)`.
- `lambda x x * x` becomes `lambda x: x * x`.
- `for item in range items` becomes `for item in range(items):`.
- `def greet name` becomes `def greet(name):`.

## Usage

```bash
python newpython_parser.py sample.newpy
```

## Test

```bash
python test_parser.py
```
