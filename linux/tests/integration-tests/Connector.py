from socket import socket, AF_INET, SOCK_STREAM
from subprocess import Popen, PIPE
from time import sleep
from fcntl import fcntl, F_GETFL, F_SETFL
from os import O_NONBLOCK, path, getenv

class Connector:
    def __init__(self, screen_resolution_width, screen_resolution_height, tcp_ip='127.0.0.1', tcp_port=10555):
        self.__process = self.__run_process(screen_resolution_width, screen_resolution_height)
        self.__sender = self.__connect(tcp_ip, tcp_port)
        self.__sender.settimeout(15);

    def __del__(self):
        self.end_connection()

    def __run_process(self, screen_resolution_width, screen_resolution_height):
        dirname = path.dirname(__file__)
        CONNECTOR_BUILD_DIR = getenv('CONNECTOR_BUILD_DIR')
        if CONNECTOR_BUILD_DIR:
            filename = path.join(dirname, CONNECTOR_BUILD_DIR + '/linux/main_app/pcs_connector')
        else:
            filename = path.join(dirname, '../../../build/linux/main_app/pcs_connector')
        process = Popen([filename, 'test', str(screen_resolution_width), str(screen_resolution_height)],
                        stdin=PIPE,
                        stdout=PIPE)
        flags = fcntl(process.stdout, F_GETFL)
        fcntl(process.stdout, F_SETFL, flags | O_NONBLOCK)
        sleep(5)
        return process

    def __connect(self, tcp_ip, tcp_port):
        sender = socket(AF_INET, SOCK_STREAM)
        sender.connect((tcp_ip, tcp_port))
        return sender

    def __receive_and_decode(self):
        out = self.__process.stdout.read()
        while out is None:
            out = self.__process.stdout.read()
        out = out[0:1]
        return out.decode("utf-8")

    def __receive(self):
        out = self.__process.stdout.read()
        while out is None:
            out = self.__process.stdout.read()
        out = out[0:1]
        return out.decode("utf-8")

    def send_key_press(self, msg):
        self.__sender.send(msg)

    def perform_key_press(self, msg, key):
        self.__sender.send(msg)
        out = self.__receive_and_decode()
        assert out == key

    def exchange_screen_resolution(self, screen_resolution_msg, screen_resolution_received):
        self.__sender.send(screen_resolution_msg)
        received = self.__sender.recv(5)
        assert received == screen_resolution_received

    def end_connection(self):
        self.__process.kill()
