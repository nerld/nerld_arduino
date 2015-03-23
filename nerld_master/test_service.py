from twisted.application import internet, service
from twisted.internet.protocol import ServerFactory, Protocol
from twisted.internet.serialport import SerialPort
from twisted.python import log

from twisted.internet.serialport import SerialPort
from twisted.protocols.basic import LineReceiver
from twisted.internet import reactor

from autobahn.twisted.websocket import WebSocketClientProtocol, \
    WebSocketClientFactory

import urllib
import urllib2
import json

class SerialProtocol(LineReceiver):
  """
  MCU serial communication protocol.
  """

  def __init__(self, debug = True):
    self.debug = debug

  def connectionMade(self):
    print('Serial port connected.')

  def lineReceived(self, line):
    if self.debug:
      print("Serial RX: {0}".format(line))

class SerialService(service.Service):
  def startService(self):
    serialProtocol = SerialProtocol(True)
    self.serial = SerialPort(serialProtocol, '/dev/ttyATH0', reactor, baudrate=9600)

class NerldWebSocketClientProtocol(WebSocketClientProtocol):

  def onConnect(self, response):
    print("Server connected: {0}".format(response.peer))

  def onOpen(self):
    print("WebSocket connection open.")

    def hello():
      self.sendMessage(u"Hello, world!".encode('utf8'))
      self.sendMessage(b"\x00\x01\x03\x04", isBinary=True)
      self.factory.reactor.callLater(1, hello)

    # start sending messages every second ..
    hello()

  def onMessage(self, payload, isBinary):
    if isBinary:
      print("Binary message received: {0} bytes".format(len(payload)))
    else:
      print("Text message received: {0}".format(payload.decode('utf8')))

  def onClose(self, wasClean, code, reason):
    print("WebSocket connection closed: {0}".format(reason))

# configuration parameters
port = 10000
iface = 'localhost'

# this will hold the services that combine to form the poetry server
top_service = service.MultiService()

serialService = SerialService()
serialService.setServiceParent(top_service)

clientFactory = WebSocketClientFactory("ws://localhost:9000", debug=False)
clientFactory.protocol = NerldWebSocketClientProtocol
client_service = internet.TCPClient("192.168.0.18", 9000, clientFactory)
client_service.setServiceParent(top_service)

# this variable has to be named 'application'
application = service.Application("fastpoetry")

# this hooks the collection we made to the application
top_service.setServiceParent(application)