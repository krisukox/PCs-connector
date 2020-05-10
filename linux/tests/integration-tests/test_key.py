from fixtures import connection
from Connector import Connector

COMMON_MSG_END1 = 'x'
COMMON_MSG_END2 = 'y'

DUMMY_BYTES = chr(0) + chr(0)

SCREEN_RESOLUTION = chr(32) + chr(10) + chr(10) + chr(10) + chr(10)
RECEIVED_SCREEN_RESOLUTION = chr(32)

KEY_BYTE = chr(1)
TRUE_VALUE = chr(15)

def to_screen_resolution_msg(width, height):
    screen_resolution_byte = (32).to_bytes(1, 'big')
    width_bytes = width.to_bytes(2, 'big')
    height_bytes = height.to_bytes(2, 'big')
    return screen_resolution_byte + width_bytes + height_bytes

def to_key_msg(key):
    return bytes((KEY_BYTE + key + TRUE_VALUE + DUMMY_BYTES).encode())

def test_three_keys(connection):
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

# """
# purpose: press three different keys in the row
# """
# def test_two_same_keys(setup_connection):
#     connector = setup_connection
#
#     key1 = 'H'
#     key2 = 'J'
#     key3 = 'K'
#
#     linux_key_code1 = chr(43)
#     linux_key_code2 = chr(44)
#     linux_key_code3 = chr(45)
#
#     msg1 = bytes((key1 + COMMON_MSG_END1 + DUMMY_BYTES).encode())
#     msg2 = bytes((key2 + COMMON_MSG_END2 + DUMMY_BYTES).encode())
#     msg3 = bytes((key3 + COMMON_MSG_END1 + DUMMY_BYTES).encode())
#
#     connector.perform_key_press(msg1, linux_key_code1)
#     connector.perform_key_press(msg2, linux_key_code2)
#     connector.perform_key_press(msg3, linux_key_code3)
#
#     connector.end_connection()
