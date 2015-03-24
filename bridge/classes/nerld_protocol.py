class NerldProtocol:

  @classmethod
  def encode_message(cls, address, command, value):
    return "%s:%s:%s" % (address, command, value)

  @classmethod
  def decode_message(cls, msg):
    message_args = msg.split(':')
    print message_args
    return {'address': message_args[0], 'command': message_args[1], 'value': message_args[2]}
