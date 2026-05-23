NewPython v2 - Single end Terminator

⚠️ DISCLAIMER: This is an UNOFFICIAL and EXPERIMENTAL project. Use at your own risk.

Last Updated: May 24, 2026

---

🚨 Important Notice

This project is NOT affiliated with or endorsed by the Python Software Foundation.

NewPython v2 is an experimental programming language variant that modifies Python's syntax. It is currently in active development and should be considered unstable. Do not use this for production systems, critical infrastructure, or any mission-critical applications.

Risks and Limitations

- Experimental Status: This project is under active development and may contain bugs, security vulnerabilities, or breaking changes.
- No Official Support: There is no official support or warranty provided.
- Compatibility Issues: Code written in NewPython may not be compatible with standard Python interpreters.
- Security: While we have implemented security measures, this experimental project has not undergone comprehensive security audits.
- Data Loss: Using experimental software may result in data loss or corruption.

---

📋 Table of Contents

- [What is NewPython v2?](#what-is-newpython-v2)
- [Key Features](#key-features)
- [Download and Installation](#download-and-installation)
- [Usage](#usage)
- [Syntax Guide](#syntax-guide)
- [Examples](#examples)
- [Testing](#testing)
- [Documentation in Other Languages](#documentation-in-other-languages)
- [Sample Code](#sample-code)
- [License](#license)
- [Support](#support)
- [Acknowledgments](#acknowledgments)

---

What is NewPython v2?

NewPython v2 introduces a simplified block syntax that uses indentation to structure code and requires exactly one final end marker at the end of the script. This experimental language aims to provide an alternative syntax for Python-like programming.

In addition to the standard indentation-based style, NewPython v2 also supports a flat writing style without indentation for certain patterns. This allows users to write compact examples while still relying on the parser to interpret nested block structure and close all open blocks with the final end.

---

Key Features

- Single end Terminator: Close all nested blocks with one end statement
- No Colons: Block headers don't require colons (:)
- Bracket Strings: Use [text] for string literals
- Simplified Lambda: lambda x x * x instead of lambda x: x * x
- Function Call Sugar: Implicit parentheses for simple calls
- Optional Flat Style: Write some examples without indentation and let the parser infer structure

---

Download and Installation

Prerequisites

- Python 3.8 or higher
- Git (for cloning the repository)

Download Options

Option 1: Clone from GitHub

``bash
git clone https://github.com/rabbit-hand/NewPython.git
cd NewPython
`

Option 2: Download ZIP File

1. Visit https://github.com/rabbit-hand/NewPython
2. Click the green "Code" button
3. Select "Download ZIP"
4. Extract the downloaded file
5. Navigate to the extracted directory

Installation

No installation required. The parser is a standalone Python script. Simply ensure you have Python 3.8 or higher installed.

---

Usage

Basic Usage

Convert a NewPython file to standard Python:

`bash
python newpython_parser.py sample.newpy
`

Running the Output

Save the output to a file and run it with Python:

`bash
python newpython_parser.py sample.newpy > output.py
python output.py
`

---

Syntax Guide

Key Rules

- No colons at the end of block headers
- Use normal indentation to express nested blocks
- Close every multi-line program with a single end line
- The parser removes the final end and produces valid Python

Supported Block Headers

The following block headers are supported:

- if
- elif
- else
- for
- while
- def
- class
- try
- except
- finally
- with

Expression Syntax

Bracket Strings

Use [text] to represent a Python string literal:

`newpython
print [Hello] name
`

This converts to:

`python
print("Hello", name)
`

Lambda Expressions

Use lambda args body:

`newpython
square = lambda x x * x
`

This converts to:

`python
square = lambda x: x * x
`

Simple Function Call Sugar

When a bare function name appears before arguments, the parser converts it to a normal Python call:

`newpython
square 5
`

This converts to:

`python
square(5)
`

Another example:

`newpython
for item in range items
`

This converts to:

`python
for item in range(items):
`

Flat Style Without Indentation

NewPython v2 also supports a flat writing style without indentation.

In this style, block statements can be written continuously without leading spaces. The parser interprets the structure of the program and closes all open blocks with the final end.

This style is useful for short examples, compact demonstrations, and parser experiments. However, because indentation normally makes nested logic easier to read, the indentation-based style is still recommended for most real code.

Example:

`newpython
def process items
for item in range items
if item % 2 == 0
print [even] item
end
`

This converts to:

`python
def process(items):
    for item in range(items):
        if item % 2 == 0:
            print("even", item)
`

Recommendation on Style

Use the indentation-based style when:

- Writing larger programs
- Working with nested logic
- Collaborating with other developers
- Prioritizing readability and maintainability

Use the flat style when:

- Writing very short examples
- Demonstrating syntax quickly
- Testing parser behavior
- Creating compact experimental snippets

---

Examples

Example 1: Nested If Statements

`newpython
if x > 0
    if y > 0
        print [ok]
end
`

This converts to:

`python
if x > 0:
    if y > 0:
        print("ok")
`

Example 2: Function with Loop

`newpython
def process items
    for item in range items
        if item % 2 == 0
            print [even] item
end
`

This converts to:

`python
def process(items):
    for item in range(items):
        if item % 2 == 0:
            print("even", item)
`

Example 3: Class Definition

`newpython
class Calculator
    def add self value
        return self.result + value
end
`

This converts to:

`python
class Calculator():
    def add(self, value):
        return self.result + value
`

Example 4: Lambda and Function Call

`newpython
square = lambda x x * x
print [result] square 5
end
`

This converts to:

`python
square = lambda x: x * x
print("result", square(5))
`

Example 5: Flat Style Without Indentation

`newpython
def process items
for item in range items
if item % 2 == 0
print [even] item
end
`

This converts to:

`python
def process(items):
    for item in range(items):
        if item % 2 == 0:
            print("even", item)
`

This example demonstrates that NewPython v2 can also accept a flat style without indentation in certain cases. The parser infers the nested structure and produces standard Python output. Even so, indentation is still recommended for clarity in non-trivial programs.

---

Testing

Run the test suite to verify that the parser works correctly:

`bash
python test_parser.py
`

Expected output:

`text
PASS: single end nested blocks
PASS: function definition and loop
PASS: class with one final end
PASS: lambda and bracket string

4/4 tests passed.
`

---

Documentation in Other Languages

- [README_EN.md](README_EN.md) - Comprehensive English documentation
- [README_JA.md](README_JA.md) - 日本語ドキュメント (Japanese documentation)
- [LANGUAGE_SPEC.md](LANGUAGE_SPEC.md) - Detailed language specification

---

Sample Code

Sample NewPython files are provided in the examples/ directory:

- hello.newpy - Basic hello world example
- calculator.newpy - Simple calculator class
- loops.newpy - Various loop examples
- functions.newpy - Function definition examples
- lambda_example.newpy` - Lambda expression examples

---

License

This project is provided as-is for educational and experimental purposes. See the [LICENSE](LICENSE) file for details.

---

Support

As this is an unofficial experimental project, there is no official support. However, you can:

- Report issues on GitHub Issues
- Check existing documentation
- Experiment with the code at your own risk

---

Acknowledgments

This project is inspired by Python but is not affiliated with the Python Software Foundation. Python is a trademark of the Python Software Foundation.

---

Remember: This is experimental software. Use responsibly and at your own risk.
