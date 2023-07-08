from PyQt5.QtWidgets import QApplication, QMainWindow, QPushButton, QLabel
from PyQt5 import uic, QtGui, QtCore, Qt
import sys
import subprocess
from PyQt5 import *
from PyQt5.QtCore import *
from PyQt5.Qt import *
import time


class Create_user(QMainWindow):
    def __init__(self, other):
        super(Create_user, self).__init__()
        uic.loadUi('create_user.ui', self)
        self.reg.clicked.connect(self.create)
        self.other = other
        self.show()

    def create(self):
        if self.username.text() != "" and self.password.text() != "":
            res = subprocess.run(["out\\build\\x64-debug\\1.exe", "reg",
                                 self.username.text(), self.password.text()],
                                capture_output=True)
            if res.stdout.decode("utf-8") == "user already exist":
                self.res.setText("This user already exist")
            else:
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



class Records(QMainWindow):
    def __init__(self):
        super(Records, self).__init__()
        uic.loadUi('records.ui', self)
        for i in range(10):
            lb = QLabel()
            lb.setText(str(i + 1))
            lb1 = QLabel()
            lb1.setText("Not enough data")
            self.verticalLayout.addWidget(lb)
            self.verticalLayout_2.addWidget(lb1)

        self.show()

    def fill(self, data):
        data = list(filter(lambda x: x, data))
        for i in range(len(data)):
            try:
                self.verticalLayout_2.itemAt(i).widget().setText(data[i])
            except Exception:
                pass




class Minefield(QMainWindow):
    singleton: 'Minefield' = None
    def __init__(self):
        super(Minefield, self).__init__()
        uic.loadUi('field.ui', self)
        self.start = None
        self.res = None
        self.first = True
        self.rc = None
        for i in range(10):
            for j in range(10):
                button = CustomButton()
                button.setFixedSize(50, 50)
                button.clicked.connect(
                    lambda checked, button=button, i=i, j=j: self.interact(button, i, j)
                )
                self.gridLayout.addWidget(button, i, j)
        self.action.triggered.connect(self.rs)
        self.action_2.triggered.connect(self.relogin)
        self.action_3.triggered.connect(self.shrec)
        self.show()

    def rs(self):
        self.close()
        self.restart()

    def shrec(self):
        res = subprocess.run(["out\\build\\x64-debug\\1.exe", "rec", window.login.text()], capture_output=True)
        self.rc = Records()
        self.rc.fill(res.stdout.decode("utf-8").replace("\r", "").split('\n'))

    def relogin(self):
        self.close()
        window.show()
        
    @staticmethod
    def restart():
        Minefield.singleton = Minefield()
        

    def interact(self, button, i, j):
        if self.first:
            self.start = time.time()
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
                                 str(i), str(j), button.clicked_with],
                                capture_output=True)
            rs = res.stdout.decode("utf-8").replace("\r", "")
            if len(rs.split('\n')) == 1:
                rs += "\n"
            for k in rs.split('\n')[:-1]:
                try:
                    x, y, val = list(map(int, k.split()))
                except ValueError:
                    x, y, val = 0, 0, "You won"
                btn = self.gridLayout.itemAtPosition(x, y).widget()
                if val != 11 and val != 9:
                    if val == 10:
                        btn.setStyleSheet('QPushButton {background-color: red}')
                        for i in range(10):
                            for j in range(10):
                                bt = self.gridLayout.itemAtPosition(i, j).widget()
                                bt.setEnabled(False)
                        self.winOrNot.setText("You loose!")
                        return
                    if val == 12:
                        btn.setText("M")
                        return
                    if val == 13:
                        btn.setText("")
                        return
                    if val == "You won":
                        for i in range(10):
                            for j in range(10):
                                bt = self.gridLayout.itemAtPosition(i, j).widget()
                                bt.setEnabled(False)
                        self.winOrNot.setText("You won!")
                        self.res = time.time() - self.start
                        self.label.setText(f"Time: {self.res} s")
                        res = subprocess.run(["out\\build\\x64-debug\\1.exe", "wrr",
                                             window.login.text(), str(self.res)],
                                            capture_output=True)

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
                                 self.login.text(), self.password.text()],
                                capture_output=True)
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

