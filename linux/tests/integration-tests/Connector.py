from socket import socket, AF_INET, SOCK_STREAM
from subprocess import Popen, PIPE
from time import sleep
from fcntl import fcntl, F_GETFL, F_SETFL
from os import O_NONBLOCK, path

class Connector:
    def __init__(self, tcp_ip='127.0.0.1', tcp_port=10000):
        self.__process = self.__run_process()
        self.__sender = self.__connect(tcp_ip, tcp_port)

    def __del__(self):
        self.end_connection()

    def __run_process(self):
        dirname = path.dirname(__file__)
        filename = path.join(dirname, '../../../build/linux/main_app/pcs_connector')
        process = Popen([filename, 'test'],
                        stdin=PIPE,
                        stdout=PIPE)
        flags = fcntl(process.stdout, F_GETFL)
        fcntl(process.stdout, F_SETFL, flags | O_NONBLOCK)
        sleep(0.1)
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

    def perform_key_press(self, msg, key):
        self.__sender.send(msg)
        out = self.__receive_and_decode()
        assert out == key

    def end_connection(self):
        self.__process.kill()
