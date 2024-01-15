from xcom import constant as const
from xcom import serialization as serial
import socket

## Init

def udp_init():
    return socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

## Send

def udp_send(sock, dest, data):
    sock.sendto(data, (dest.strip('\0'), const.PORT))


def send_ts(sock, dest, type, timestamp, offset):
    frame = serial.init_ts(type, timestamp, offset)
    frame = serial.encode(frame)
    udp_send(sock, dest, frame)


def send_conf_tank(sock, dest, type, diameter_value, diameter_unit, volume_value, volume_unit, heigth_value, heigth_unit):
    if type == const.CONF_SEND_TANK:
        frame = serial.init_conf_send_tank(diameter_value, diameter_unit, volume_value, volume_unit, heigth_value, heigth_unit)
    elif type == const.CONF_GET_TANK:
        frame = serial.init_conf_get_tank() 
    frame = serial.encode(frame)
    udp_send(sock, dest, frame)
    
def send_conf_sensor(sock, dest, type, number, ip, role, heigth_value, heigth_unit):
    if type == const.CONF_SEND_SENSOR:
        frame = serial.init_conf_send_sensor(number, ip, role, heigth_value, heigth_unit)
    elif type == const.CONF_GET_SENSOR:
        frame = serial.init_conf_get_sensor()
    frame = serial.encode(frame)
    udp_send(sock, dest, frame)
    
def send_conf_fluid(sock, dest, type, density_value, density_unit, gaspressure_value, gaspressure_unit):
    if type == const.CONF_SEND_FLUID:
        frame = serial.init_conf_send_fluid(density_value, density_unit, gaspressure_value, gaspressure_unit)
    elif type == const.CONF_GET_FLUID:
        frame = serial.init_conf_get_fluid()
    frame = serial.encode(frame)
    udp_send(sock, dest, frame)
    
def send_conf_cycle(sock, dest, time_value, time_unit, frameLoss):
    frame = serial.init_conf_cycle(time_value, time_unit, frameLoss)
    frame = serial.encode(frame)
    udp_send(sock, dest, frame)
    
    
def send_pressure(sock, dest, type, value, unit, quality_code):
    if type == const.SEND_PRESSURE:
        frame = serial.init_pressure_send(value, unit, quality_code)
    elif type == const.GET_PRESSURE:
        frame = serial.init_pressure_get()
    frame = serial.encode(frame)
    udp_send(sock, dest, frame)
    
    
def send_level(sock, dest, type, value, unit):
    if type == const.SEND_LEVEL:
        frame = serial.init_level_send(value, unit)
    elif type == const.GET_LEVEL:
        frame = serial.init_level_get()
    frame = serial.encode(frame)
    udp_send(sock, dest, frame)
    
    
## Receive
    
def receive(sock, type, subtype):
    sock.settimeout(5)  # set timeout to 5 seconds
    try:
        data, _ = sock.recvfrom(1024)
    except socket.timeout:
        return None
    data = serial.decode(data)
    if data.type == type and data.data.type == subtype:
        return data
    else:
        return None
    
## Close

def udp_close(sock):
    sock.close()
    
## Rebind

def udp_reset(sock):
    udp_close(sock)
    sock = udp_init()
    return sock