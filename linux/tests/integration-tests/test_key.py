from fixtures import setup_connection
from Connector import Connector

COMMON_MSG_END1 = 'x'
COMMON_MSG_END2 = 'y'

DUMMY_BYTES = chr(0) + chr(0)

SCREEN_RESOLUTION = chr(32) + chr(10) + chr(10) + chr(10) + chr(10)

KEY_BYTE = chr(1)
TRUE_VALUE = chr(15)

# SC

# """
# purpose: press two different keys in the row
# """
def test_two_different_keys(setup_connection):
    connector = setup_connection

    key1 = 'E'
    key2 = 'F'

    linux_key_code1 = chr(26)
    linux_key_code2 = chr(41)

    msg1 = bytes((KEY_BYTE + key1 + TRUE_VALUE + DUMMY_BYTES).encode())
    msg2 = bytes((KEY_BYTE + key2 + TRUE_VALUE + DUMMY_BYTES).encode())

    screen_resolution_msg = bytes((SCREEN_RESOLUTION).encode())

    connector.exchange_screen_resolution(screen_resolution_msg)

    connector.perform_key_press(msg1, linux_key_code1)
    connector.perform_key_press(msg2, linux_key_code2)
    #
    connector.end_connection()

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
