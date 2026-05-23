# NewPython v2 - Single `end` Terminator

**⚠️ DISCLAIMER: This is an UNOFFICIAL and EXPERIMENTAL project. Use at your own risk.**

**Last Updated: May 23, 2026**

---

## 🚨 Important Notice

**This project is NOT affiliated with or endorsed by the Python Software Foundation.**

NewPython v2 is an experimental programming language variant that modifies Python's syntax. It is currently in active development and should be considered unstable. Do not use this for production systems, critical infrastructure, or any mission-critical applications.

### Risks and Limitations

- **Experimental Status**: This project is under active development and may contain bugs, security vulnerabilities, or breaking changes.
- **No Official Support**: There is no official support or warranty provided.
- **Compatibility Issues**: Code written in NewPython may not be compatible with standard Python interpreters.
- **Security**: While we have implemented security measures, this experimental project has not undergone comprehensive security audits.
- **Data Loss**: Using experimental software may result in data loss or corruption.

---

## 📋 Table of Contents

- [What is NewPython v2?](#what-is-newpython-v2)
- [Key Features](#key-features)
- [Download and Installation](#download-and-installation)
- [Usage](#usage)
- [Syntax Guide](#syntax-guide)
- [Examples](#examples)
- [Testing](#testing)
- [Documentation in Other Languages](#documentation-in-other-languages)

---

## What is NewPython v2?

NewPython v2 introduces a simplified block syntax that uses indentation to structure code and requires exactly one final `end` marker at the end of the script. This experimental language aims to provide an alternative syntax for Python-like programming.

---

## Key Features

- **Single `end` Terminator**: Close all nested blocks with one `end` statement
- **No Colons**: Block headers don't require colons (`:`)
- **Bracket Strings**: Use `[text]` for string literals
- **Simplified Lambda**: `lambda x x * x` instead of `lambda x: x * x`
- **Function Call Sugar**: Implicit parentheses for simple calls

---

## Download and Installation

### Prerequisites

- Python 3.8 or higher
- Git (for cloning the repository)

### Download Options

#### Option 1: Clone from GitHub

```bash
git clone https://github.com/rabbit-hand/NewPython.git
cd NewPython
```

#### Option 2: Download ZIP File

1. Visit https://github.com/rabbit-hand/NewPython
2. Click the green "Code" button
3. Select "Download ZIP"
4. Extract the downloaded file
5. Navigate to the extracted directory

### Installation

No installation required! The parser is a standalone Python script. Simply ensure you have Python 3.8+ installed.

---

## Usage

### Basic Usage

Convert a NewPython file to standard Python:

```bash
python newpython_parser.py sample.newpy
```

### Running the Output

Save the output to a file and run it with Python:

```bash
python newpython_parser.py sample.newpy > output.py
python output.py
```

---

## Syntax Guide

### Key Rules

- **No colons** at the end of block headers
- Use normal **indentation** to express nested blocks
- Close every multi-line program with a single **`end`** line
- The parser removes the final `end` and produces valid Python

### Supported Block Headers

- `if`, `elif`, `else`
- `for`, `while`
- `def`, `class`
- `try`, `except`, `finally`
- `with`

### Expression Syntax

#### Bracket Strings

Use `[text]` to represent a Python string literal:

```newpython
print [Hello] name
```

converts to:

```python
print("Hello", name)
```

#### Lambda Expressions

Use `lambda args body`:

```newpython
square = lambda x x * x
```

converts to:

```python
square = lambda x: x * x
```

#### Simple Function Call Sugar

When a bare function name appears before arguments, the parser converts it to a normal Python call:

```newpython
square 5
```

converts to:

```python
square(5)
```

```newpython
for item in range items
```

converts to:

```python
for item in range(items):
```

#### Flat Style Without Indentation

NewPython v2 also supports a flat writing style without indentation.

In this style, block statements can be written continuously without leading spaces. The parser interprets the structure of the program and closes all open blocks with the final `end`.

> **Note:** This style is supported as an experimental shorthand, but indentation is still recommended for readability.

```newpython
def process items
for item in range items
if item % 2 == 0
print [even] item
end
```

converts to:

```python
def process(items):
    for item in range(items):
        if item % 2 == 0:
            print("even", item)
```

---

## Examples

### Example 1: Nested If Statements

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

### Example 2: Function with Loop

```newpython
def process items
    for item in range items
        if item % 2 == 0
            print [even] item
end
```

converts to:

```python
def process(items):
    for item in range(items):
        if item % 2 == 0:
            print("even", item)
```

### Example 3: Class Definition

```newpython
class Calculator
    def add self value
        return self.result + value
end
```

converts to:

```python
class Calculator():
    def add(self, value):
        return self.result + value
```

### Example 4: Lambda and Function Call

```newpython
square = lambda x x * x
print [result] square 5
end
```

converts to:

```python
square = lambda x: x * x
print("result", square(5))
```

### Example 5: Flat Style Without Indentation

```newpython
def process items
for item in range items
if item % 2 == 0
print [even] item
end
```

converts to:

```python
def process(items):
    for item in range(items):
        if item % 2 == 0:
            print("even", item)
```

---

## Testing

Run the test suite to verify the parser works correctly:

```bash
python test_parser.py
```

Expected output:

```text
PASS: single end nested blocks
PASS: function definition and loop
PASS: class with one final end
PASS: lambda and bracket string

4/4 tests passed.
```

---

## Documentation in Other Languages

- **[README_EN.md](README_EN.md)** - Comprehensive English documentation
- **[README_JA.md](README_JA.md)** - 日本語ドキュメント (Japanese documentation)
- **[LANGUAGE_SPEC.md](LANGUAGE_SPEC.md)** - Detailed language specification

---

## Sample Code

Sample NewPython files are provided in the `examples/` directory:

- `hello.newpy` - Basic hello world example
- `calculator.newpy` - Simple calculator class
- `loops.newpy` - Various loop examples
- `functions.newpy` - Function definition examples
- `lambda_example.newpy` - Lambda expression examples

---

## License

This project is provided as-is for educational and experimental purposes. See the [LICENSE](LICENSE) file for details.

---

## Support

As this is an unofficial experimental project, there is no official support. However, you can:

- Report issues on GitHub Issues
- Check existing documentation
- Experiment with the code at your own risk

---

## Acknowledgments

This project is inspired by Python but is not affiliated with the Python Software Foundation. Python is a trademark of the Python Software Foundation.

---

**Remember: This is experimental software. Use responsibly and at your own risk.**
