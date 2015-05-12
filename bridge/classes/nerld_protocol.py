class NerldProtocol:

  @classmethod
  def encode_message(cls, address, command, value):
    return "%s:%s:%s" % (address, command, value)

  @classmethod
  def decode_message_from_master(cls, msg):
    message = {'status': 'ok', 'rig_hash': 'remote1337'}
    try:
      message_args = msg.split(':')
      message['from'] = 'rig'
      message['address'] = int(message_args[0])
      message['command'] = int(message_args[1])
      message['value'] = message_args[2]
    except Exception, e:
      message = {'status': 'failed'}
    finally:
      return message

  @classmethod
  def validate_message_from_server(cls, msg):
    valid = True
    message = {'status': 'ok'}
    try:
      message_args = msg.split(':')
      message['address'] = int(message_args[0])
      message['command'] = int(message_args[1])
      message['value'] = message_args[2]
    except Exception, e:
      valid = False
    finally:
      return valid
