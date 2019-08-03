from fixtures import setup_connection

COMMON_MSG_END1 = 'x'
COMMON_MSG_END2 = 'y'

"""
purpose: press two different keys in the row
"""
def test_two_different_keys(setup_connection):
    connector = setup_connection

    key1 = 'e'
    key2 = 'f'

    msg1 = bytes((key1 + COMMON_MSG_END1).encode())
    msg2 = bytes((key2 + COMMON_MSG_END2).encode())

    connector.perform_key_press(msg1, key1)
    connector.perform_key_press(msg2, key2)

    connector.end_connection()

"""
purpose: press three different keys in the row
"""
def test_two_same_keys(setup_connection):
    connector = setup_connection

    key1 = 'd'
    key2 = 'd'

    msg1 = bytes((key1 + COMMON_MSG_END1).encode())
    msg2 = bytes((key2 + COMMON_MSG_END2).encode())

    connector.perform_key_press(msg1, key1)
    connector.perform_key_press(msg2, key2)

    connector.end_connection()
