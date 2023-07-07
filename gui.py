from PyQt6.QtWidgets import QApplication, QMainWindow
from PyQt6 import uic, QtGui
import sys
import subprocess


class Create_user(QMainWindow):
    def __init__(self, other):
        super(Create_user, self).__init__()
        uic.loadUi('create_user.ui', self)
        self.reg.clicked.connect(self.try_to_create)
        self.other = other
        self.show()

    def try_to_create(self):
        if self.username.text() != "" and self.password.text() != "":
            res = subprocess.run(["out\\build\\x64-debug\\1.exe", "reg", self.username.text(), self.password.text()], capture_output=True)
            if res.stdout.decode("utf-8") == "user already exist":
                self.res.setText("This user already exist")
            else:
                print("success")
                self.close()
                self.other.show()
        else:
            self.res.setText("Enter login and password")



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
            self.a = Create_user(self)
        self.hide()
        self.a.show()
  


if __name__ == '__main__':
    app = QApplication([])
    window = Login()
    app.exec()

