from twisted.python import log
from twisted.application import service
from twisted.internet.serialport import SerialPort
from twisted.internet import reactor

from serial_protocol import SerialProtocol

class MQTTService(service.Service):
  
  def __init__(self, parentService):
    self.parentService = parentService

  def startService(self):
    self.serialProtocol = SerialProtocol(self.parentService)
    self.serial = SerialPort(self.serialProtocol, '/dev/ttyATH0', reactor, baudrate=9600)