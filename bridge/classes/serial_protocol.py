from twisted.python import log
from twisted.protocols.basic import LineReceiver

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