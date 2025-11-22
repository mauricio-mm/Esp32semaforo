import sys
import threading
import paho.mqtt.client as mqtt
from PyQt5 import QtWidgets, QtGui, QtCore

# ---------------------- CONFIGURAÇÃO MQTT ----------------------
broker = "broker.emqx.io"
port = 1883
client_id = "py_iot"
topic = "lab318/semaphoro"
topic_btn = "lab318/semaphoro_cb"

# ---------------------- SEMÁFORO ----------------------
class SemaforoWidget(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setFixedSize(120, 300)
        self.luzes = ["red", "yellow", "green"]
        self.aceso = None   # Nenhuma luz acesa (apagado)

    def setLuz(self, cor):
        self.aceso = cor
        self.update()

    def paintEvent(self, event):
        painter = QtGui.QPainter(self)
        painter.setRenderHint(QtGui.QPainter.Antialiasing)

        rect = QtCore.QRect(20, 20, 80, 260)
        painter.setBrush(QtGui.QBrush(QtGui.QColor("black")))
        painter.drawRoundedRect(rect, 10, 10)

        cores = {
            "red": QtGui.QColor(255,0,0),
            "yellow": QtGui.QColor(255,255,0),
            "green": QtGui.QColor(0,255,0)
        }

        for i, cor in enumerate(self.luzes):
            y = 40 + i * 80

            if self.aceso == cor:
                painter.setBrush(QtGui.QBrush(cores[cor]))
            else:
                painter.setBrush(QtGui.QBrush(QtGui.QColor(80,80,80)))  # apagado

            painter.drawEllipse(30, y, 60, 60)

# ---------------------- CLIENTE MQTT ----------------------
class MQTTClient(QtCore.QObject):
    nova_mensagem = QtCore.pyqtSignal(str)

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
            client.subscribe(topic_btn)
        else:
            print("Falha na conexão, código:", rc)

    def on_message(self, client, userdata, msg):
        mensagem = msg.payload.decode()
        topico = msg.topic

        print("Mensagem recebida:", mensagem, "| Tópico:", topico)

        if topico == topic_btn:
            print("Botão recebido:", mensagem)
            return
        
        if topico == topic:
            self.nova_mensagem.emit(mensagem)

# ---------------------- ATUALIZAÇÃO DAS SINALEIRAS ----------------------
def atualizar_semaforo(msg):

    msg = msg.replace("{", "[").replace("}", "]")

    try:
        estados = eval(msg)
        cores = ["red", "yellow", "green"]

        for i, estado in enumerate(estados):
            if i >= len(sinaleiras):
                continue

            # Apagado: [0,0,0]
            if estado == [0,0,0]:
                sinaleiras[i].setLuz(None)
                continue

            # Somente um LED pode estar ligado
            if sum(estado) != 1:
                print(f"Estado inválido para S{i+1}: {estado}")
                continue

            cor = cores[estado.index(1)]
            sinaleiras[i].setLuz(cor)

    except Exception as e:
        print("Erro ao processar mensagem:", msg)
        print(e)

# ---------------------- GUI ----------------------
app = QtWidgets.QApplication(sys.argv)
janela = QtWidgets.QMainWindow()
janela.setWindowTitle("Semáforos MQTT")
janela.setGeometry(200, 200, 900, 450)

central = QtWidgets.QWidget()
janela.setCentralWidget(central)
layout_principal = QtWidgets.QVBoxLayout(central)

h_layout = QtWidgets.QHBoxLayout()
layout_principal.addLayout(h_layout)

# 5 semáforos
nomes = ["S1", "S2", "S3", "S4", "S5"]
sinaleiras = []

for nome in nomes:
    v_layout = QtWidgets.QVBoxLayout()
    v_layout.setAlignment(QtCore.Qt.AlignTop)

    semaforo = SemaforoWidget()
    sinaleiras.append(semaforo)

    label = QtWidgets.QLabel(nome)
    label.setAlignment(QtCore.Qt.AlignCenter)

    v_layout.addWidget(semaforo)
    v_layout.addWidget(label)
    h_layout.addLayout(v_layout)

# ---------------------- BOTÃO ----------------------
estado_botao = 0

botao = QtWidgets.QPushButton("Manutenção: 0")
botao.setFixedHeight(60)
botao.setStyleSheet("font-size: 20px;")
layout_principal.addWidget(botao)

def clicar_botao():
    global estado_botao
    estado_botao = 1 - estado_botao
    botao.setText(f"Manutenção: {estado_botao}")
    mqtt_client.client.publish(topic_btn, str(estado_botao))
    print("Publicado:", estado_botao)

botao.clicked.connect(clicar_botao)

# ---------------------- MQTT ----------------------
mqtt_client = MQTTClient()
mqtt_client.nova_mensagem.connect(atualizar_semaforo)
mqtt_client.start()

# ---------------------- EXECUÇÃO ----------------------
janela.show()
sys.exit(app.exec_())
