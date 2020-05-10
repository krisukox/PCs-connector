import pytest
from Connector import Connector

SCREEN_RESOLUTION_BYTE = (32).to_bytes(1, 'big')

def to_screen_resolution_msg(width, height):
    width_bytes = width.to_bytes(2, 'big')
    height_bytes = height.to_bytes(2, 'big')
    return SCREEN_RESOLUTION_BYTE + width_bytes + height_bytes


@pytest.fixture
def connection():
    received_resolution_width = 1200
    received_resolution_height = 800

    connector = Connector(received_resolution_width, received_resolution_height)
    dummy_screen_resolution_msg = to_screen_resolution_msg(2570, 2570)
    received_screen_resolution_msg = to_screen_resolution_msg(received_resolution_width, received_resolution_height)

    connector.exchange_screen_resolution(dummy_screen_resolution_msg, received_screen_resolution_msg)
    return connector
