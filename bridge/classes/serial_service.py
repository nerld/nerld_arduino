from twisted.python import log
from twisted.application import service
from twisted.internet.serialport import SerialPort
from twisted.internet import reactor

from serial_protocol import SerialProtocol

class SerialService(service.Service):
  def startService(self):
    serialProtocol = SerialProtocol(True)
    self.serial = SerialPort(serialProtocol, '/dev/ttyATH0', reactor, baudrate=9600)