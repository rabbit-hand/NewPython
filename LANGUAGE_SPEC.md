# NewPython v2 Language Specification

## Single Terminal `end`

NewPython v2 uses a single final `end` marker to close all nested blocks. The language relies on indentation for block structure, and the final `end` is required when the program contains one or more nested blocks.

### Rule

- Every file with block headers must end with exactly one `end` line.
- The `end` line must be the final non-empty line of the file.
- `end` is not repeated for each nested block.

## Block headers

Use the following block openings without a colon:

- `if condition`
- `elif condition`
- `else`
- `for target in iterable`
- `while condition`
- `def name params...`
- `class Name [Base...]`
- `try`
- `except Exception`
- `finally`
- `with expression`

Indentation defines the extent of each block.

## Expression syntax

### Bracket strings

- Use `[text]` to represent a Python string literal.
- Example: `print [Hello] name` becomes `print("Hello", name)`.

### Lambda expressions

- Use `lambda args body`.
- Example: `square = lambda x x * x` becomes `square = lambda x: x * x`.

### Simple function call sugar

- When a bare function name appears before one or more arguments, the parser converts it to a normal Python call.
- Example: `square 5` becomes `square(5)`.
- Example: `for i in range items` becomes `for i in range(items):`.

## Example program

```newpython
class Calculator
    def add self value
        return self.result + value
end
```

produces:

```python
class Calculator():
    def add(self, value):
        return self.result + value
```
