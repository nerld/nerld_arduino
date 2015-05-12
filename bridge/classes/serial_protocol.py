import json
from nerld_protocol import NerldProtocol

from twisted.python import log
from twisted.protocols.basic import LineReceiver

class SerialProtocol(LineReceiver):
  """
  MCU serial communication protocol.
  """
  
  def __init__(self, parentService, debug = True):
    self.debug = debug
    self.parentService = parentService

  def connectionMade(self):
    print('Serial port connected.')

  def lineReceived(self, line):
    if self.debug:
      print("Serial RX: {0}".format(line))

    self.dispatcher(NerldProtocol.decode_message_from_master(line))

  def dispatcher(self, params):

    print params

    if params['status'] != 'ok':
      print 'Invalid command was passed.'
    else:
      # self.send_command_server(params)
      pass
    return

  def publish_to_server(self, topic, message):
    mqttFactory = self.parentService.getServiceNamed('MQTTService').factory
    mqttFactory.publish(topic, message)
    return

  def send_command_to_master(self, params):
    self.transport.write(params)
    return
