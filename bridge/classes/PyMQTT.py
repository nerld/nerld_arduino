from twisted.python import log
import logging
# from twisted.application.service import Service
from twisted.internet import reactor
from twisted.internet.protocol import ReconnectingClientFactory
from twisted.internet.task import LoopingCall

from nerld_protocol import NerldProtocol
from MQTT import MQTTProtocol


import random
import sys

class MQTTListener(MQTTProtocol):
    pingPeriod = 60000

    def connectionMade(self):
        log.msg('MQTT Connected')
        self.clientId = "nerld-yun%i" % random.randint(1, 0xFFFF)
        self.connect(self.clientId, keepalive=self.pingPeriod)
        # TODO: make these constants configurable
        reactor.callLater(self.pingPeriod//1000, self.pingreq)
        reactor.callLater(5, self.processMessages)
            
    def pingrespReceived(self):
        log.msg('Ping received from MQ broker', logging.DEBUG)
        reactor.callLater(self.pingPeriod//1000, self.pingreq)

    def connackReceived(self, status):
        if status == 0:
            self.subscribe("rigs/some_hash/#") # SUBSCRIBE TOPIC HERE
        else:
            log.msg('Connecting to MQTT broker failed')
    
    def connectionLost(self, reason):
        log.err('CAUGHT In The ACT: connection LOST reason: %s' % (reason))
        
            
    def processMessages(self):
        reactor.callLater(5, self.processMessages)
            
    def publishReceived(self, topic, message, qos, dup, retain, messageId):
        # Received a publish on an output topic
        log.msg('RECV Topic: %s, Message: %s' % (topic, message ), logging.DEBUG)
        #mqttMessageBuffer.append((topic, message))

        # parse the topic and call the appropriate thing here
        try:
            topic_path = topic.split('/')

            if topic_path[2] == "slaves"
                address = topic_path[3]
                command = topic_path[4]
                value = message
                encodedMessage = NerldProtocol.encode_message(address, command, value)
                sendToMaster(encodeMessage)
            elif topic_path[2] == "health"
                # call health checkup
                pass
        except:
            pass

        # use the sendToMaster function

    def sendToMaster(self, message):
        self.factory.parentService.getServiceNamed('SerialService').serialProtocol.send_command_to_master(message)


class MQTTListenerFactory(ReconnectingClientFactory):
    #protocol = MQTTListener

    def __init__(self, service = None):
        self.service = service
        self.protocol = MQTTListener

    def publish(self, topic, message):
        # this is a HACK class needs to be instantiated before it could be used (happens in buildProtocol)
        if self.protocol != MQTTListener:
            #log.msg('SEND Topic: %s, Message: %s' % (topic, message ))
            self.protocol.publish(topic, message)

    def buildProtocol(self, addr):
        p = self.protocol()
        p.factory = self
        # HACK protocol class is exchanged by instance
        self.protocol = p
        log.msg("protocol build", logging.DEBUG)
        return p
    
    def clientConnectionLost(self, connector, reason):
        log.err('CAUGHT In The ACT: Lost connection.  Reason: %s' % (reason))
        self.protocol = MQTTListener
        ReconnectingClientFactory.clientConnectionLost(self, connector, reason)

    def clientConnectionFailed(self, connector, reason):
        log.err('CAUGHT In The ACT: Connection failed. Reason: %s' % (reason))
        # HACK failed instances where not tolerated, start over 
        self.protocol = MQTTListener
        
        ReconnectingClientFactory.clientConnectionFailed(self, connector, reason)
        
    def startedConnecting(self, connector):
        log.msg('reset reconnection delay', logging.DEBUG)
        self.resetDelay()

# def PostFiglet():
#     #log.msg('SEND Topic: %s, Message: %s' % (topic, message ))
#     x = random.random()
#     mqttFactory.publish('test/random', str(x)+' random')
#===============================================================================
#    mqttFactory.FigletPublish('tokudu/figlet', '''
# ... o   o  o-o  o-O-o o-O-o      o-o                       o  oself.protocol = MQTTListener
# ... |\ /| o   o   |     |        |                         |  |            
# ... | O | |   |   |     |       -O- o-o o-o     o-o  o  o -o- O--o o-o o-o  
# ... |   | o   O   |     |        |  | | |       |  | |  |  |  |  | | | |  |
# ... o   o  o-O\   o     o        o  o-o o       O-o  o--O  o  o  o o-o o  o 
# ...                                             |       |                  
# ...                                             o    o--o                   
# ...  o                  o         o 
# ...  |           o      |         | 
# ... -o- o   o   o  o-o -o- o-o  o-O 
# ...  |   \ / \ / |  \   |  |-' |  | 
# ...  o    o   o  | o-o  o  o-o  o-o 
# ''' )
#===============================================================================


# if __name__ == '__main__':
#     # TODO
#     import logging
#     logging.getLogger().setLevel(logging.INFO) 
#     mqttFactory = MQTTListenerFactory()
#     reactor.connectTCP("test.mosquitto.org", 1883, mqttFactory)
#     #reactor.listenTCP(1025, )
#     figlet = LoopingCall(PostFiglet)
#     figlet.start(5)
    
#     reactor.run()
