# NewPython v2 - 単一の `end` ターミネータ

**⚠️ 免責事項: これは非公式かつ実験的なプロジェクトです。自己責任で使用してください。**

**最終更新日: 2026年5月23日**

---

## 🚨 重要な注意事項

**このプロジェクトはPython Software Foundationによって承認または支援されていません。**

NewPython v2はPythonの構文を変更する実験的なプログラミング言語バリアントです。現在アクティブに開発中であり、不安定とみなすべきです。本番システム、重要なインフラストラクチャ、またはミッションクリティカルなアプリケーションには使用しないでください。

### リスクと制限事項

- **実験的ステータス**: このプロジェクトはアクティブに開発中であり、バグ、セキュリティ脆弱性、または破壊的な変更が含まれている可能性があります。
- **公式サポートなし**: 公式サポートまたは保証は提供されません。
- **互換性の問題**: NewPythonで記述されたコードは標準のPythonインタープリタと互換性がない可能性があります。
- **セキュリティ**: セキュリティ対策を実装していますが、この実験的なプロジェクトは包括的なセキュリティ監査を受けていません。
- **データ損失**: 実験的なソフトウェアの使用により、データの損失や破損が発生する可能性があります。

---

## 📋 目次

- [NewPython v2とは](#newpython-v2とは)
- [主な機能](#主な機能)
- [ダウンロードとインストール](#ダウンロードとインストール)
- [使用方法](#使用方法)
- [構文ガイド](#構文ガイド)
- [例](#例)
- [テスト](#テスト)
- [貢献](#貢献)
- [ライセンス](#ライセンス)

---

## NewPython v2とは

NewPython v2は、インデントを使用してコードを構造化し、スクリプトの最後に正確に1つの最終的な `end` マーカーを必要とする簡略化されたブロック構文を導入します。この実験的な言語は、Pythonのようなプログラミングのための代替構文を提供することを目的としています。

---

## 主な機能

- **単一の `end` ターミネータ**: 1つの `end` ステートメントですべてのネストされたブロックを閉じる
- **コロンなし**: ブロックヘッダーにコロン（`:`）を必要としない
- **ブラケット文字列**: 文字列リテラルに `[text]` を使用
- **簡略化されたラムダ**: `lambda x: x * x` の代わりに `lambda x x * x`
- **関数呼び出しシュガー**: シンプルな呼び出しの暗黙的な括弧

---

## ダウンロードとインストール

### 前提条件

- Python 3.8以上
- Git（リポジトリのクローン用）

### ダウンロードオプション

#### オプション1: GitHubからクローン

```bash
git clone https://github.com/rabbit-hand/NewPython.git
cd NewPython
```

#### オプション2: ZIPファイルをダウンロード

1. https://github.com/rabbit-hand/NewPython にアクセス
2. 緑色の「Code」ボタンをクリック
3. 「Download ZIP」を選択
4. ダウンロードしたファイルを展開
5. 展開したディレクトリに移動

### インストール

インストールは不要です！パーサーはスタンドアロンのPythonスクリプトです。Python 3.8以上がインストールされていることを確認してください。

---

## 使用方法

### 基本的な使用方法

NewPythonファイルを標準Pythonに変換：

```bash
python newpython_parser.py sample.newpy
```

### 出力の実行

出力をファイルに保存してPythonで実行：

```bash
python newpython_parser.py sample.newpy > output.py
python output.py
```

---

## 構文ガイド

### 基本ルール

- ブロックヘッダーの最後に**コロンなし**
- ネストされたブロックを表現するために通常の**インデント**を使用
- すべての複数行プログラムを単一の**`end`** 行で閉じる
- パーサーは最後の `end` を削除し、有効なPythonを生成

### サポートされているブロックヘッダー

- `if`、`elif`、`else`
- `for`、`while`
- `def`、`class`
- `try`、`except`、`finally`
- `with`

### 式の構文

#### ブラケット文字列

Pythonの文字列リテラルを表すために `[text]` を使用：

```newpython
print [Hello] name
```

は以下に変換されます：

```python
print("Hello", name)
```

#### ラムダ式

`lambda args body` を使用：

```newpython
square = lambda x x * x
```

は以下に変換されます：

```python
square = lambda x: x * x
```

#### シンプルな関数呼び出しシュガー

裸の関数名が引数の前に現れる場合、パーサーはそれを通常のPython呼び出しに変換します：

```newpython
square 5
```

は以下に変換されます：

```python
square(5)
```

```newpython
for item in range items
```

は以下に変換されます：

```python
for item in range(items):
```

---

## 例

### 例1: ネストされたifステートメント

```newpython
if x > 0
    if y > 0
        print [ok]
end
```

は以下に変換されます：

```python
if x > 0:
    if y > 0:
        print("ok")
```

### 例2: ループ付き関数

```newpython
def process items
    for item in range items
        if item % 2 == 0
            print [even] item
end
```

は以下に変換されます：

```python
def process(items):
    for item in range(items):
        if item % 2 == 0:
            print("even", item)
```

### 例3: クラス定義

```newpython
class Calculator
    def add self value
        return self.result + value
end
```

は以下に変換されます：

```python
class Calculator():
    def add(self, value):
        return self.result + value
```

### 例4: ラムダと関数呼び出し

```newpython
square = lambda x x * x
print [result] square 5
end
```

は以下に変換されます：

```python
square = lambda x: x * x
print("result", square(5))
```

---

## テスト

テストスイートを実行してパーサーが正しく動作することを確認：

```bash
python test_parser.py
```

期待される出力：
```
PASS: single end nested blocks
PASS: function definition and loop
PASS: class with one final end
PASS: lambda and bracket string

4/4 tests passed.
```

---

## 貢献

これは実験的なプロジェクトです。貢献は歓迎しますが、以下の点を理解してください：

1. これは公式のPythonプロジェクトではありません
2. 機能はいつでも追加または削除される可能性があります
3. 破壊的な変更が予想されます
4. 安定性または互換性の保証はありません

貢献をご希望の場合：

1. リポジトリをフォーク
2. 機能ブランチを作成
3. 変更を加える
4. 新しい機能のテストを追加
5. プルリクエストを送信

---

## サンプルコード

サンプルNewPythonファイルは `examples/` ディレクトリに提供されています：

- `hello.newpy` - 基本的なハローワールドの例
- `calculator.newpy` - シンプルな計算機クラス
- `loops.newpy` - 様々なループの例
- `functions.newpy` - 関数定義の例

---

## ドキュメント

- [言語仕様](LANGUAGE_SPEC.md) - 詳細な言語仕様
- [README_EN.md](README_EN.md) - 英語ドキュメント

---

## ライセンス

このプロジェクトは教育および実験目的でそのまま提供されています。詳細については[LICENSE](LICENSE)ファイルを参照してください。

---

## サポート

これは非公式の実験的なプロジェクトであるため、公式サポートはありません。ただし、以下のことができます：

- GitHub Issuesで問題を報告
- 既存のドキュメントを確認
- 自己責任でコードを試す

---

## 謝辞

このプロジェクトはPythonに触発されていますが、Python Software Foundationとは提携していません。PythonはPython Software Foundationの商標です。

---

**注意: これは実験的なソフトウェアです。責任を持って、自己責任で使用してください。**
