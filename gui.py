from PyQt5.QtWidgets import QApplication, QMainWindow, QPushButton
from PyQt5 import uic, QtGui, QtCore, Qt
import sys
import subprocess
from PyQt5 import *
from PyQt5.QtCore import *
from PyQt5.Qt import *

class Create_user(QMainWindow):
    def __init__(self, other):
        super(Create_user, self).__init__()
        uic.loadUi('create_user.ui', self)
        self.reg.clicked.connect(self.create)
        self.other = other
        self.show()

    def create(self):
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


class CustomButton(QPushButton):
    def __init__(self):
        super(CustomButton, self).__init__()
        self.clicked_with = None

    def mousePressEvent(self, QMouseEvent):
        if QMouseEvent.button() == Qt.LeftButton:
            self.clicked_with = "Left"
        if QMouseEvent.button() == Qt.LeftButton and QMouseEvent.modifiers() == Qt.ShiftModifier:
            self.clicked_with = "Left+Shift"     
        QPushButton.mousePressEvent(self, QMouseEvent)



class Minefield(QMainWindow):
    def __init__(self):
        super(Minefield, self).__init__()
        uic.loadUi('field.ui', self)
        self.first = True
        for i in range(10):
            for j in range(10):
                button = CustomButton()
                button.setFixedSize(50, 50)
                button.clicked.connect(
                    lambda checked, button=button, i=i, j=j: self.interact(button, i, j)
                )
                self.gridLayout.addWidget(button, i, j)
        self.show()

    def interact(self, button, i, j):
        if self.first:
            res = subprocess.run(["out\\build\\x64-debug\\1.exe", "cre",
                                 str(i), str(j)], capture_output=True)
            rs = res.stdout.decode("utf-8").replace("\r", "")
            for k in rs.split('\n')[:-1]:
                x, y, val = list(map(int, k.split()))
                btn = self.gridLayout.itemAtPosition(x, y).widget()
                if val != 11 and val != 9:
                    btn.setText(str(val))
                btn.setEnabled(False)
            self.first = False
        else:
            res = subprocess.run(["out\\build\\x64-debug\\1.exe", "opn",
                                 str(i), str(j), button.clicked_with], capture_output=True)
            rs = res.stdout.decode("utf-8").replace("\r", "")
            print(rs)
            if len(rs.split('\n')) == 1:
                rs += "\n"
            for k in rs.split('\n')[:-1]:
                x, y, val = list(map(int, k.split()))
                btn = self.gridLayout.itemAtPosition(x, y).widget()
                if val != 11 and val != 9:
                    if val == 10:
                        btn.setStyleSheet('QPushButton {background-color: red}')
                        btn.setEnabled(False)
                        return
                    if val == 12:
                        btn.setText("M")
                        return
                    if val == 13:
                        btn.setText("")
                        return
                    btn.setText(str(val))
                else:
                    btn.setText("")
                btn.setEnabled(False)




class Login(QMainWindow):
    def __init__(self):
        super(Login, self).__init__()
        self.a = None
        self.b = None
        uic.loadUi('login.ui', self)
        self.createuser.clicked.connect(self.reg)
        self.enter.clicked.connect(self.ent)
        self.show()

    def reg(self):
        if self.a is None:
            self.a = Create_user(self)
        self.hide()
        self.a.show()

    def ent(self):
        if self.login.text() != "" and self.password.text() != "":
            res = subprocess.run(["out\\build\\x64-debug\\1.exe", "ent",
                                 self.login.text(), self.password.text()], capture_output=True)
            print(res.stdout.decode("utf-8"))
            if res.stdout.decode("utf-8") == "invalid login or password":
                self.res.setText("Invalid login or password")
            else:
                if self.b is None:
                    self.b = Minefield()
                self.hide()
                self.b.show()
        else:
            self.res.setText("Enter login and password")
            


  


if __name__ == '__main__':
    app = QApplication([])
    window = Login()
    app.exec()

