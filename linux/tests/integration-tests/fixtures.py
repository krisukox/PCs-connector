import pytest
from Connector import Connector

@pytest.fixture
def connection():
    return Connector()
