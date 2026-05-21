from newpython_parser import parse_newpython


test_cases = [
    {
        'name': 'single end nested blocks',
        'source': '''if x > 0
    if y > 0
        print [ok]
end''',
        'expected': '''if x > 0:
    if y > 0:
        print("ok")''',
    },
    {
        'name': 'function definition and loop',
        'source': '''def process items
    for item in range items
        if item % 2 == 0
            print [even] item
end''',
        'expected': '''def process(items):
    for item in range(items):
        if item % 2 == 0:
            print("even", item)''',
    },
    {
        'name': 'class with one final end',
        'source': '''class Calculator
    def add self value
        return self.result + value
end''',
        'expected': '''class Calculator():
    def add(self, value):
        return self.result + value''',
    },
    {
        'name': 'lambda and bracket string',
        'source': '''square = lambda x x * x
print [result] square 5
end''',
        'expected': '''square = lambda x: x * x
print("result", square(5))''',
    },
]


def run_tests() -> int:
    passed = 0
    total = len(test_cases)
    for case in test_cases:
        actual = parse_newpython(case['source'])
        if actual == case['expected']:
            print(f"PASS: {case['name']}")
            passed += 1
        else:
            print(f"FAIL: {case['name']}")
            print('SOURCE:')
            print(case['source'])
            print('EXPECTED:')
            print(case['expected'])
            print('ACTUAL:')
            print(actual)
            print('-' * 40)
    print(f"\n{passed}/{total} tests passed.")
    return 0 if passed == total else 1


if __name__ == '__main__':
    raise SystemExit(run_tests())
