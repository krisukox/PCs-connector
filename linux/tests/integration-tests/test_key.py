from fixtures import connection
from Connector import Connector

DUMMY_BYTES = chr(0) + chr(0)

SCREEN_RESOLUTION_BYTE = (32).to_bytes(1, 'big')

KEY_VALUE = chr(1)
TRUE_VALUE = chr(15)

def to_screen_resolution_msg(width, height):
    width_bytes = width.to_bytes(2, 'big')
    height_bytes = height.to_bytes(2, 'big')
    return SCREEN_RESOLUTION_BYTE + width_bytes + height_bytes

def to_key_msg(key):
    return bytes((KEY_VALUE + key + TRUE_VALUE + DUMMY_BYTES).encode())


def test_press_three_keys(connection):
    key_E = 'E'
    key_F = 'F'
    key_H = 'H'

    linux_key_E = chr(26)
    linux_key_F = chr(41)
    linux_key_H = chr(43)

    msg_E = to_key_msg(key_E)
    msg_F = to_key_msg(key_F)
    msg_H = to_key_msg(key_H)

    screen_resolution_msg = to_screen_resolution_msg(2570, 2570)
    received_screen_resolution_msg = to_screen_resolution_msg(1200, 800)

    connection.exchange_screen_resolution(screen_resolution_msg, received_screen_resolution_msg)

    connection.perform_key_press(msg_E, linux_key_E)
    connection.perform_key_press(msg_F, linux_key_F)
    connection.perform_key_press(msg_H, linux_key_H)

    connection.end_connection()


def test_stress_key_perform(connection):
    key_E = 'E'
    key_F = 'F'
    key_H = 'H'

    linux_key_E = chr(26)
    linux_key_F = chr(41)
    linux_key_H = chr(43)

    msg_E = to_key_msg(key_E)
    msg_F = to_key_msg(key_F)
    msg_H = to_key_msg(key_H)

    screen_resolution_msg = to_screen_resolution_msg(2570, 2570)
    received_screen_resolution_msg = to_screen_resolution_msg(1200, 800)

    connection.exchange_screen_resolution(screen_resolution_msg, received_screen_resolution_msg)
    for x in range(0,100):
        connection.perform_key_press(msg_E, linux_key_E)
        connection.perform_key_press(msg_F, linux_key_F)
        connection.perform_key_press(msg_H, linux_key_H)

    connection.end_connection()


def test_stress_key_spam(connection):
    key_E = 'E'
    key_F = 'F'
    key_H = 'H'

    msg_E = to_key_msg(key_E)
    msg_F = to_key_msg(key_F)
    msg_H = to_key_msg(key_H)

    screen_resolution_msg = to_screen_resolution_msg(2570, 2570)
    received_screen_resolution_msg = to_screen_resolution_msg(1200, 800)

    connection.exchange_screen_resolution(screen_resolution_msg, received_screen_resolution_msg)
    for x in range(0,1000):
        connection.send_key_press(msg_E)
        connection.send_key_press(msg_F)
        connection.send_key_press(msg_H)

    connection.end_connection()
