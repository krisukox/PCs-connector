from fixtures import connection
from Connector import Connector

DUMMY_BYTES = chr(0) + chr(0)
KEY_VALUE = chr(1)
TRUE_VALUE = chr(15)

def to_key_msg(key):
    return bytes((KEY_VALUE + key + TRUE_VALUE + DUMMY_BYTES).encode())

KEY_E = 'E'
KEY_F = 'F'
KEY_H = 'H'

LINUX_KEY_E = chr(26)
LINUX_KEY_F = chr(41)
LINUX_KEY_H = chr(43)

MSG_E = to_key_msg(KEY_E)
MSG_F = to_key_msg(KEY_F)
MSG_H = to_key_msg(KEY_H)


def test_press_three_keys(connection):
    connection.perform_key_press(MSG_E, LINUX_KEY_E)
    connection.perform_key_press(MSG_F, LINUX_KEY_F)
    connection.perform_key_press(MSG_H, LINUX_KEY_H)

    connection.end_connection()


def test_stress_key_perform(connection):
    for x in range(0,100):
        connection.perform_key_press(MSG_E, LINUX_KEY_E)
        connection.perform_key_press(MSG_F, LINUX_KEY_F)
        connection.perform_key_press(MSG_H, LINUX_KEY_H)

    connection.end_connection()


def test_stress_key_spam(connection):
    for x in range(0,1000):
        connection.send_key_press(MSG_E)
        connection.send_key_press(MSG_F)
        connection.send_key_press(MSG_H)

    connection.end_connection()
