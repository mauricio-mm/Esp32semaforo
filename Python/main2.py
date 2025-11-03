from PyQt5 import QtWidgets, uic, QtGui, QtCore
import sys

class SemaforoWidget(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setFixedSize(120, 300)
        self.luzes = ["red", "yellow", "green"]
        self.aceso = "red"

    def setLuz(self, cor):
        if cor in self.luzes:
            self.aceso = cor
            self.update()

    def paintEvent(self, event):
        painter = QtGui.QPainter(self)
        painter.setRenderHint(QtGui.QPainter.Antialiasing)

        rect = QtCore.QRect(20, 20, 80, 260)
        painter.setBrush(QtGui.QBrush(QtGui.QColor("black")))
        painter.drawRoundedRect(rect, 10, 10)

        cores = {"red": QtGui.QColor(255,0,0),
                 "yellow": QtGui.QColor(255,255,0),
                 "green": QtGui.QColor(0,255,0)}
        
        for i, cor in enumerate(self.luzes):
            y = 40 + i*80
            if self.aceso == cor:
                painter.setBrush(QtGui.QBrush(cores[cor]))
            else:
                painter.setBrush(QtGui.QBrush(QtGui.QColor(80,80,80)))
            painter.drawEllipse(30, y, 60, 60)

# Programa principal
app = QtWidgets.QApplication(sys.argv)
janela = QtWidgets.QMainWindow()
uic.loadUi("interface.ui", janela)

layout_principal = janela.centralwidget.layout()
if layout_principal is None:
    layout_principal = QtWidgets.QVBoxLayout(janela.centralwidget)
    janela.centralwidget.setLayout(layout_principal)

h_layout = QtWidgets.QHBoxLayout()

nomes = ["S1", "S2", "S3", "S4", "S5"]
sinaleiras = []

for nome in nomes:
    v_layout = QtWidgets.QVBoxLayout()
    v_layout.setAlignment(QtCore.Qt.AlignTop)  # força os widgets para o topo

    semaforo = SemaforoWidget()
    sinaleiras.append(semaforo)

    label = QtWidgets.QLabel(nome)
    label.setAlignment(QtCore.Qt.AlignCenter)

    # Evita que o label seja esticado
    label.setSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)

    v_layout.addWidget(semaforo)
    v_layout.addWidget(label)

    h_layout.addLayout(v_layout)

layout_principal.addLayout(h_layout)

janela.show()
sys.exit(app.exec_())
