NewPython v1 (codename: koko) — Unofficial AI-Assisted Fork
=========================================================

"NewPython v1 (koko)" is an experimental project designed to streamline the coding workflow through dialogues with AI, focusing on minimizing physical and cognitive friction during typing.

⚠️ IMPORTANT: Unofficial Prototype
----------------------------------
This project is **strictly unofficial**. It has absolutely no affiliation with the Python Software Foundation (PSF). Because it introduces a heavily modified syntax to optimize the typing flow, it does not maintain backward compatibility with standard Python.

🎯 Implemented & Confirmed Features
----------------------------------
* **Colonless Syntax:** Colons (:) at the end of ``if``, ``while``, ``def``, etc., have been removed to allow a smoother typing flow.
* **Explicit Block Terminator 'koko':** To provide absolute clarity for both humans and AI, the explicit keyword ``koko`` is used to define the end of a code block.
* **Parenthesis-Free Function Calls:** For simple single-argument calls, functions can be invoked by separating them with a space, such as ``math.sin 0``.
* **Bracket Strings [Strings]:** Simplifies string definitions by using brackets ``[ ]`` instead of traditional quotation marks.

💻 Code Samples (The "koko" Way)
----------------------------------
.. code-block:: python

   import math

   # Simple Condition (No colon, closed by koko)
   if True
       print [Hello World]
   koko

   # Function Definition & Clean Math Calls
   def calculate_sine x
       print math.sin x
   koko

   # Execution without redundant parentheses
   calculate_sine 0

⚠️ IMPORTANT: Unofficial Prototype
----------------------------------
This project is **strictly unofficial**. It has absolutely no affiliation with the Python Software Foundation (PSF). Because it introduces a heavily modified syntax to optimize the typing flow, it does not maintain backward compatibility with standard Python.

**DISCLAIMER OF WARRANTY**
--------------------------
This software is provided "as is", without warranty of any kind, express or implied, including but not limited to the warranties of merchantability, fitness for a particular purpose, and non-infringement. In no event shall the authors or copyright holders be liable for any claim, damages, or other liability, whether in an action of contract, tort, or otherwise, arising from, out of, or in connection with the software or the use or other dealings in the software. Use this software at your own risk.

=========================================================
🇯🇵 日本語説明書 (Japanese Description)
=========================================================

「NewPython v1 (koko)」は、AIとの対話を通じてプログラミングのワークフローを見直し、タイピングにおける物理的・精神的な「摩擦（ストレス）」を最小限に抑えることを目指した実験的プロジェクトです。

⚠️ 注意事項
------------
* **実験的プロジェクト**: 本リポジトリは開発途上のプロトタイプです。検証段階ですのでご注意ください。

💻 日本語サンプルコード
------------------------
NewPythonで実装された、シンプルで洗練された構文のサンプルです：

.. code-block:: python

   import math

   # シンプルな条件分岐（コロンなし、kokoで締める）
   if True
       print [Hello World]
   koko

   # 関数定義と計算（カッコを省略）
   def calculate_sine x
       print math.sin x
   koko

   # カッコを使わずに一発実行！
   calculate_sine 0

🛠️ 安全なインストール方法 (Linux / macOS)
-----------------------------------------------
既存の標準Python環境と衝突しないよう、NewPythonは独立した専用ディレクトリ（``/opt/koko``）に隔離してインストールし、独立したバイナリ「``koko``」を生成します。

.. code-block:: bash

   # 1. リポジトリをクローンして対象ブランチに切り替える
   git clone https://github.com/rabbit-hand/NewPython.git
   cd NewPython
   git checkout 3.12

   # 2. 専用のインストール先を指定して環境設定
   ./configure --prefix=/opt/koko --with-pydebug

   # 3. koko文法の自動生成とコンパイル
   make regen-grammar
   make -j

   # 4. 標準Pythonを上書きせずに安全にインストール
   sudo make install

   # 5. koko専用のショートカットコマンドを作成
   sudo ln -s /opt/koko/bin/koko /usr/local/bin/koko

🪟 安全なインストール方法 (Windows - WSL2経由)
------------------------------------------------------
NewPythonのコンパイルにはLinux環境が必要です。Windows環境では、WSL2（Windows Subsystem for Linux）を利用することで、既存の環境を汚さずに安全にビルド・実行できます。

**ビルド要件:**
- Linux環境 (WSL2推奨)
- build-essential (gcc, make, etc.)
- libssl-dev
- zlib1g-dev
- libbz2-dev
- libreadline-dev
- libsqlite3-dev
- Python 3.9+ (for pegen grammar regeneration)

1. **PowerShellを管理者として起動し、WSL2を有効化します:**
   .. code-block:: powershell

      PS> wsl --install

2. **起動したWSL2（Ubuntu）のターミナルで、ビルドに必要なツールを導入します:**
   .. code-block:: bash

      $ sudo apt update
      $ sudo apt install -y build-essential libssl-dev zlib1g-dev libbz2-dev libreadline-dev libsqlite3-dev python3 python3-pip

3. **Linux用のビルド手順を実行します:**
   上記の「Linux / macOS」セクションの手順を実行することで、``/opt/koko`` に安全に隔離された ``koko`` コマンドが完成します。

**注意:**
- Windowsネイティブ環境でのビルドはサポートされていません
- Visual StudioやMSBuildが必要なPCbuildは使用しません
- 必ずWSL2またはLinux環境を使用してください

本ソフトウェアは「現状のまま」提供され、明示的または黙示的を問わず、いかなる種類の保証もありません。これには、商品性、特定目的への適合性、および非侵害に関する保証が含まれますが、これらに限定されません。いかなる場合においても、著作者または著作権者は、契約、不法行為、その他を問わず、本ソフトウェアまたはその使用もしくはその他の取引から生じる、またはこれに関連するいかなる請求、損害、またはその他の責任についても責任を負いません。本ソフトウェアの使用は、自己責任で行ってください。
