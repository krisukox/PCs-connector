import pytest
from Connector import Connector

@pytest.fixture
def setup_connection():
    return Connector()
