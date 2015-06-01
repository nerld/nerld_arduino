from twisted.python import log
from twisted.application import internet, service
from autobahn.twisted.websocket import WebSocketClientProtocol, \
    WebSocketClientFactory

from classes.serial_service import SerialService
from classes.nerld_web_socket_client_protocol import NerldWebSocketClientProtocol

from classes.PyMQTT import MQTTListener, MQTTListenerFactory


import Queue

# this will hold the services that combine to form the bridge
top_service = service.MultiService()

serialService = SerialService(top_service)
serialService.setName('SerialService')
serialService.setServiceParent(top_service)

# clientFactory = WebSocketClientFactory("ws://localhost:8080")
# clientFactory.parentService = top_service
# clientFactory.protocol = NerldWebSocketClientProtocol()
# client_service = internet.TCPClient("192.168.0.10", 8080, clientFactory)
# client_service.factory = clientFactory
# client_service.setName('ClientService')
# client_service.setServiceParent(top_service)

mqttFactory = MQTTListenerFactory(top_service)
mqtt_service = internet.TCPClient("54.66.235.191", 1883, mqttFactory)
mqtt_service.factory = mqttFactory
mqtt_service.setName('MQTTService')
mqtt_service.setServiceParent(top_service)

# set the name of the application
application = service.Application("nerld_bridge")

# this hooks the collection we made to the application
top_service.setServiceParent(application)