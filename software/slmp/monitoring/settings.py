import json
from collections import namedtuple

with open('monitoring/2CAR/Connectors.json', 'r') as f:
    conns = json.load(f)['connectors']

AnalogConnector = namedtuple('AnalogConnector',
                                 ('value_type', 'type', 'name', 'threshold', 'register', 'description', 'direction', 'cashed'))
DiscreteConnector = namedtuple('DiscreteConnector',
                                   ('value_type', 'type', 'name', 'register', 'description', 'direction', 'cashed'))

with open('monitoring/2CAR/settings.json', 'r') as f:
    cash_frequency = json.load(f)['interval']