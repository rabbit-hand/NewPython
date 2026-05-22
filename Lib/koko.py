import platform
import subprocess
import logging

logging.basicConfig(level=logging.INFO)

pyautogui = None


def _is_windows():
    return platform.system() == "Windows"


if _is_windows():
    try:
        import pyautogui
        pyautogui.FAILSAFE = True
    except ImportError:
        logging.error("pyautoguiがインストールされていません。'pip install pyautogui' を実行してください。")
        pyautogui = None


class Mouse:
    @staticmethod
    def click(x, y):
        try:
            if _is_windows():
                if pyautogui:
                    pyautogui.click(x, y)
                else:
                    logging.warning("pyautoguiが利用できません")
                    return
            else:
                subprocess.run(["xdotool", "mousemove", str(x), str(y), "click", "1"], check=True)
            logging.info(f"Clicked at ({x}, {y})")
        except Exception as e:
            logging.error(f"マウス操作に失敗しました: {e}")


class Keyboard:
    @staticmethod
    def type(text):
        try:
            if _is_windows():
                if pyautogui:
                    pyautogui.write(text)
                else:
                    logging.warning("pyautoguiが利用できません")
                    return
            else:
                subprocess.run(["xdotool", "type", text], check=True)
            logging.info(f"Typed: {text}")
        except Exception as e:
            logging.error(f"キーボード入力に失敗しました: {e}")


def run(cmd):
    try:
        return subprocess.check_output(cmd, shell=True, text=True)
    except subprocess.CalledProcessError as e:
        logging.error(f"コマンド実行失敗: {e}")
        return ""

