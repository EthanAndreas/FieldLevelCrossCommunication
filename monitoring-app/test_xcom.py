from xcom import constant as xcom_const
from xcom import network as xcom_network
from xcom import print as xcom_print

dest = "10.0.0.14"

sock = xcom_network.udp_init()

print("-------------UDP GET ON BOARD TEST-------------")

xcom_network.send_ts(sock, dest, xcom_const.INIT_TS, 108379192, 192)
print("Send ts: \033[92msuccess\033[0m")

xcom_network.send_conf_tank(sock, dest, xcom_const.CONF_GET_TANK, 0, 0, 0, 0, 0, 0)
print("Send tank info: \033[92msuccess\033[0m")
frame = xcom_network.receive(sock, xcom_const.MSG_CONF, xcom_const.CONF_SEND_TANK)
if frame == None:
    print("Get tank info: \033[91mfailed\033[0m")
else:
    print("Get tank info: \033[92msuccess\033[0m")

xcom_network.send_conf_sensor(sock, dest, xcom_const.CONF_GET_SENSOR, 0, 0, 0, 0, 0)
print("Send sensor info: \033[92msuccess\033[0m")
frame = xcom_network.receive(sock, xcom_const.MSG_CONF, xcom_const.CONF_SEND_SENSOR)
if frame == None:
    print("Get sensor info: \033[91mfailed\033[0m")
else:
    print("Get sensor info: \033[92msuccess\033[0m")
    
xcom_network.send_conf_fluid(sock, dest, xcom_const.CONF_GET_FLUID, 0, 0, 0, 0)
print("Send fluid info: \033[92msuccess\033[0m")
frame = xcom_network.receive(sock, xcom_const.MSG_CONF, xcom_const.CONF_SEND_FLUID)
if frame == None:
    print("Get fluid info: \033[91mfailed\033[0m")
else:
    print("Get fluid info: \033[92msuccess\033[0m")

xcom_network.send_conf_cycle(sock, dest, 45, xcom_const.TIME_UNIT_MILLISECOND, 12)
print("Send cycle: \033[92msuccess\033[0m")

xcom_network.send_pressure(sock, dest, xcom_const.GET_PRESSURE, 0, 0, 0)
print("Send pressure: \033[92msuccess\033[0m")
frame = xcom_network.receive(sock, xcom_const.MSG_PRESSURE, xcom_const.SEND_PRESSURE)
if frame == None:
    print("Get pressure: \033[91mfailed\033[0m")
else:
    print("Get pressure: \033[92msuccess\033[0m")
    
xcom_network.send_level(sock, dest, xcom_const.GET_LEVEL, 0, 0)
print("Send level: \033[92msuccess\033[0m")
frame = xcom_network.receive(sock, xcom_const.MSG_LEVEL, xcom_const.SEND_LEVEL)
if frame == None:
    print("Get level: \033[91mfailed\033[0m")
else:
    print("Get level: \033[92msuccess\033[0m")
    
xcom_network.udp_close(sock)