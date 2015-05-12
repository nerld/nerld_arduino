import json
from nerld_protocol import NerldProtocol

from twisted.python import log
from autobahn.twisted.websocket import WebSocketClientProtocol, \
    WebSocketClientFactory

class NerldWebSocketClientProtocol(WebSocketClientProtocol):

  # to access parent use: self.factory.parentService
  # to access serial use: self.factory.parentService.getServiceNamed('SerialService').serialProtocol

  def __init__(self):
    self.rigHash = "remote1337"

  def __call__(self):
    return self

  def onConnect(self, response):
    print("Server connected: {0}".format(response.peer))

  def onOpen(self):
    print("WebSocket connection open.")
    # register with the server
    registration = {"from": "rig", "command": "register", "rig_hash": self.rigHash}
    self.sendMessage(json.dumps(registration).encode('utf8'))

  def onMessage(self, payload, isBinary):

    print("Text message received: {0}".format(payload.decode('utf8')))

    # if NerldProtocol.validate_message_from_server(payload):
    #   self.sendToMaster(payload)

  def onClose(self, wasClean, code, reason):
    print("WebSocket connection closed: {0}".format(reason))

  def sendToMaster(self, message):
    self.factory.parentService.getServiceNamed('SerialService').serialProtocol.send_command_to_master(message)
