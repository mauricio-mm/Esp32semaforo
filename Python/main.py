import sys
import threading
import paho.mqtt.client as mqtt
from PyQt5 import QtWidgets, QtGui, QtCore

# ---------------------- CONFIGURAÇÃO MQTT ----------------------
broker = "broker.emqx.io"
port = 1883
client_id = "py_iot"
topic = "lab318/semaphoro"

# ---------------------- SEMÁFORO ----------------------
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

# ---------------------- CLIENTE MQTT ----------------------
class MQTTClient(QtCore.QObject):
    nova_mensagem = QtCore.pyqtSignal(str)  # sinal para a GUI

    def __init__(self):
        super().__init__()
        self.client = mqtt.Client(client_id=client_id)
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message

    def start(self):
        self.client.connect(broker, port, 60)
        thread = threading.Thread(target=self.client.loop_forever)
        thread.daemon = True
        thread.start()

    def on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            print("Conectado ao broker MQTT!")
            client.subscribe(topic)
        else:
            print("Falha na conexão, código:", rc)

    def on_message(self, client, userdata, msg):
        mensagem = msg.payload.decode()
        print("Mensagem recebida:", mensagem)
        self.nova_mensagem.emit(mensagem)

# ---------------------- FUNÇÃO DE ATUALIZAÇÃO ----------------------
def atualizar_semaforo(msg):
    """
    Recebe uma mensagem no formato:
    { {1,0,0}, {0,0,1}, {1,0,0}, {1,0,0}, {1,0,0} }
    e atualiza os 5 semáforos
    """
    # Substitui {} por [] para virar lista Python
    msg = msg.replace("{", "[").replace("}", "]")
    try:
        estados = eval(msg)  # cuidado: só se confiar no broker!
        cores = ["red", "yellow", "green"]

        for i, estado in enumerate(estados):
            if i >= len(sinaleiras):
                continue
            if sum(estado) != 1:
                print(f"Estado inválido para S{i+1}: {estado}")
                continue
            cor = cores[estado.index(1)]
            sinaleiras[i].setLuz(cor)

    except Exception as e:
        print("Erro ao processar mensagem:", msg)
        print(e)

# ---------------------- INICIALIZAÇÃO GUI ----------------------
app = QtWidgets.QApplication(sys.argv)
janela = QtWidgets.QMainWindow()
janela.setWindowTitle("Semáforos MQTT")
janela.setGeometry(200, 200, 900, 400)

# Central widget e layout principal
central = QtWidgets.QWidget()
janela.setCentralWidget(central)
layout_principal = QtWidgets.QVBoxLayout(central)

h_layout = QtWidgets.QHBoxLayout()
layout_principal.addLayout(h_layout)

# Cria 5 semáforos
nomes = ["S1", "S2", "S3", "S4", "S5"]
sinaleiras = []

for nome in nomes:
    v_layout = QtWidgets.QVBoxLayout()
    v_layout.setAlignment(QtCore.Qt.AlignTop)

    semaforo = SemaforoWidget()
    sinaleiras.append(semaforo)

    label = QtWidgets.QLabel(nome)
    label.setAlignment(QtCore.Qt.AlignCenter)
    label.setSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)

    v_layout.addWidget(semaforo)
    v_layout.addWidget(label)
    h_layout.addLayout(v_layout)

# ---------------------- INICIALIZAÇÃO MQTT ----------------------
mqtt_client = MQTTClient()
mqtt_client.nova_mensagem.connect(atualizar_semaforo)
mqtt_client.start()

# ---------------------- EXECUÇÃO ----------------------
janela.show()
sys.exit(app.exec_())
