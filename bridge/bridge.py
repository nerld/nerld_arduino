from twisted.python import log
from twisted.application import internet, service
from autobahn.twisted.websocket import WebSocketClientProtocol, \
    WebSocketClientFactory

from classes.serial_service import SerialService
from classes.nerld_web_socket_client_protocol import NerldWebSocketClientProtocol

# this will hold the services that combine to form the bridge
top_service = service.MultiService()

serialService = SerialService()
serialService.setName('SerialService')
serialService.setServiceParent(top_service)
serialService.setParent(top_service)

clientFactory = WebSocketClientFactory("ws://localhost:9000", debug=False)
clientFactory.protocol = NerldWebSocketClientProtocol()
clientFactory.protocol.setParent(top_service)
client_service = internet.TCPClient("192.168.0.18", 9000, clientFactory)
client_service.setName('ClientService')
client_service.setServiceParent(top_service)

# set the name of the application
application = service.Application("nerld_bridge")

# this hooks the collection we made to the application
top_service.setServiceParent(application)