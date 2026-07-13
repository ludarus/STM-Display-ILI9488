import can
import time

bus = can.interface.Bus(channel="can0", interface="socketcan")

# brightness fade
for i in range(256):
    msg = can.Message(arbitration_id=0x448, data=[i], is_extended_id=False)
    bus.send(msg)
    time.sleep(0.01)

bus.shutdown()
