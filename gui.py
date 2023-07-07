from PyQt6.QtWidgets import QApplication, QMainWindow
from PyQt6 import uic, QtGui
import sys
import subprocess


class Create_user(QMainWindow):
    def __init__(self):
        super(Create_user, self).__init__()
        uic.loadUi('create_user.ui', self)
        self.reg.clicked.connect(self.try_to_create)
        self.show()

    def try_to_create(self):
         subprocess.run(["out\\build\\x64-debug\\1.exe", "1", "2"])


class Minefield(QMainWindow):
    def __init__(self):
        super(Minefield, self).__init__()
        uic.loadUi('field.ui', self)
        self.show()


class Login(QMainWindow):
    def __init__(self):
        super(Login, self).__init__()
        self.a = None
        uic.loadUi('login.ui', self)
        self.createuser.clicked.connect(self.reg)
        self.show()

    def reg(self):
        if self.a is None:
            self.a = Create_user()
        self.close()
        self.a.show()


if __name__ == '__main__':
    app = QApplication([])
    window = Login()
    app.exec()

