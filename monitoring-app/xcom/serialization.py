from xcom import constant as const
from xcom import protocol
import struct
import socket


## Init

msg_id = 1

def init_ts(type, timestamp, offset):
    global msg_id
    frame = protocol.frame_t(msg_id, const.TS_SIZE, const.MSG_TS, protocol.ts_t(type, timestamp, offset))
    msg_id += 1
    return frame


def init_conf_send_tank(diameter_value, diameter_unit, volume_value, volume_unit, heigth_value, heigth_unit):
    global msg_id
    frame = protocol.frame_t(msg_id, const.CONF_SEND_TANK_SIZE, const.MSG_CONF, protocol.conf_t(const.CONF_SEND_TANK, protocol.conf_tank_t(diameter_value, diameter_unit, volume_value, volume_unit, heigth_value, heigth_unit)))
    msg_id += 1
    return frame

def init_conf_get_tank():
    global msg_id
    frame = protocol.frame_t(msg_id, const.CONF_GET_TANK_SIZE, const.MSG_CONF, protocol.conf_t(const.CONF_GET_TANK, None))
    msg_id += 1
    return frame

def init_conf_send_sensor(number, ip, role, type, heigth_value, heigth_unit):
    global msg_id
    frame = protocol.frame_t(msg_id, const.CONF_SEND_SENSOR_SIZE, const.MSG_CONF, protocol.conf_t(const.CONF_SEND_SENSOR, protocol.conf_sensor_t(number, ip, role, type, heigth_value, heigth_unit)))
    msg_id += 1
    return frame

def init_conf_get_sensor():
    global msg_id
    frame = protocol.frame_t(msg_id, const.CONF_GET_SENSOR_SIZE, const.MSG_CONF, protocol.conf_t(const.CONF_GET_SENSOR, None))
    msg_id += 1
    return frame

def init_conf_send_fluid(density, density_unit, gaspressure, gaspressure_unit):
    global msg_id
    frame = protocol.frame_t(msg_id, const.CONF_SEND_FLUID_SIZE, const.MSG_CONF, protocol.conf_t(const.CONF_SEND_FLUID, protocol.conf_fluid_t(density, density_unit, gaspressure, gaspressure_unit)))
    msg_id += 1
    return frame

def init_conf_get_fluid():
    global msg_id
    frame = protocol.frame_t(msg_id, const.CONF_GET_FLUID_SIZE, const.MSG_CONF, protocol.conf_t(const.CONF_GET_FLUID, None))
    msg_id += 1
    return frame

def init_conf_cycle(time_value, time_unit, frame_loss):
    global msg_id
    frame = protocol.frame_t(msg_id, const.CONF_CYCLE_SIZE, const.MSG_CONF, protocol.conf_t(const.CONF_CYCLE, protocol.conf_cycle_t(time_value, time_unit, frame_loss)))
    msg_id += 1
    return frame


def init_pressure_send(value, unit, quality_code):
    global msg_id
    frame = protocol.frame_t(msg_id, const.SEND_PRESSURE_SIZE, const.MSG_PRESSURE, protocol.pressure_t(const.SEND_PRESSURE, value, unit, quality_code))
    msg_id += 1
    return frame

def init_pressure_get():
    global msg_id
    frame = protocol.frame_t(msg_id, const.GET_PRESSURE_SIZE, const.MSG_PRESSURE, protocol.pressure_t(const.GET_PRESSURE, 0, 0, 0))
    msg_id += 1
    return frame


def init_level_send(value, unit):
    global msg_id
    frame = protocol.frame_t(msg_id, const.SEND_LEVEL_SIZE, const.MSG_LEVEL, protocol.level_t(const.SEND_LEVEL, value, unit))
    msg_id += 1
    return frame

def init_level_get():
    global msg_id
    frame = protocol.frame_t(msg_id, const.GET_LEVEL_SIZE, const.MSG_LEVEL, protocol.level_t(const.GET_LEVEL, 0, 0))
    msg_id += 1
    return frame


## Encode

def encode_ts(ts):
    return struct.pack('!BQI', ts.type, ts.timestamp, ts.offset)


def encode_conf_tank(tank):
    return struct.pack('!IBIBIB', tank.diameter_value, tank.diameter_unit, tank.volume_value, tank.volume_unit, tank.height_value, tank.height_unit)

def encode_conf_sensor(sensor):
    return struct.pack('!BIBBIB', sensor.number, sensor.ip, sensor.role, sensor.type, sensor.height_value, sensor.height_unit)

def encode_conf_fluid(fluid):
    return struct.pack('!IBIB', fluid.density_value, fluid.density_unit, fluid.gaspressure_value, fluid.gaspressure_unit)

def encode_conf_cycle(cycle):
    return struct.pack('!IBB', cycle.time_value, cycle.time_unit, cycle.frame_loss)

def encode_conf(data):
    if data.type == const.CONF_SEND_TANK:
        return struct.pack('!B', data.type) + encode_conf_tank(data.data)
    
    elif data.type == const.CONF_GET_TANK:
        return struct.pack('!B', data.type)
    
    elif data.type == const.CONF_SEND_SENSOR:
        return struct.pack('!B', data.type) + encode_conf_sensor(data.data)
    
    elif data.type == const.CONF_GET_SENSOR:
        return struct.pack('!B', data.type)
    
    elif data.type == const.CONF_SEND_FLUID:
        return struct.pack('!B', data.type) + encode_conf_fluid(data.data)
    
    elif data.type == const.CONF_GET_FLUID:
        return struct.pack('!B', data.type)
    
    elif data.type == const.CONF_CYCLE:
        return struct.pack('!B', data.type) + encode_conf_cycle(data.data)


def encode_pressure(pressure): 
    if pressure.type == const.SEND_PRESSURE:
        return struct.pack('!BIBB', pressure.type, pressure.value, pressure.unit, pressure.quality_code)
    
    elif pressure.type == const.GET_PRESSURE:
        return struct.pack('!B', pressure.type)
    

def encode_level(level):
    if level.type == const.SEND_LEVEL:
        return struct.pack('!BIB', level.type, level.value, level.unit)
    
    elif level.type == const.GET_LEVEL:
        return struct.pack('!B', level.type)
    

def encode(frame):
    
    if frame.type == const.MSG_TS:
        return struct.pack('!QIB', frame.id, frame.length, frame.type) + encode_ts(frame.data)
    
    elif frame.type == const.MSG_CONF:
        return struct.pack('!QIB', frame.id, frame.length, frame.type) + encode_conf(frame.data)
    
    elif frame.type == const.MSG_PRESSURE:
        return struct.pack('!QIB', frame.id, frame.length, frame.type) + encode_pressure(frame.data)
    
    elif frame.type == const.MSG_LEVEL:
        return struct.pack('!QIB', frame.id, frame.length, frame.type) + encode_level(frame.data)


## Decode


def decode_ts(data):
    type, timestamp, offset = struct.unpack('!BQI', data)
    return protocol.ts_t(type, timestamp, offset)


def decode_conf_tank(data):
    diameter_value, diameter_unit, volume_value, volume_unit, height_value, height_unit = struct.unpack('!IBIBIB', data)
    return protocol.conf_tank_t(diameter_value, diameter_unit, volume_value, volume_unit, height_value, height_unit)

def decode_conf_sensor(data):
    number, ip, role, type, height_value, height_unit = struct.unpack('!BIBBIB', data)
    return protocol.conf_sensor_t(number, ip, role, type, height_value, height_unit)

def decode_conf_fluid(data):
    density_value, density_unit, gaspressure_value, gaspressure_unit = struct.unpack('!IBIB', data)
    return protocol.conf_fluid_t(density_value, density_unit, gaspressure_value, gaspressure_unit)

def decode_conf_cycle(data):
    time_value, time_unit, frame_loss = struct.unpack('!IBB', data)
    return protocol.conf_cycle_t(time_value, time_unit, frame_loss)

def decode_conf(data):
    conf_type = data[0]
    config_data = data[1:]
    
    if conf_type == const.CONF_SEND_TANK:
        return protocol.conf_t(conf_type, decode_conf_tank(config_data))
    
    elif conf_type == const.CONF_GET_TANK:
        return protocol.conf_t(conf_type, None)
    
    elif conf_type == const.CONF_SEND_SENSOR:
        return protocol.conf_t(conf_type, decode_conf_sensor(config_data))
    
    elif conf_type == const.CONF_GET_SENSOR:
        return protocol.conf_t(conf_type, None)
    
    elif conf_type == const.CONF_SEND_FLUID:
        return protocol.conf_t(conf_type, decode_conf_fluid(config_data))
    
    elif conf_type == const.CONF_GET_FLUID:
        return protocol.conf_t(conf_type, None)
    
    elif conf_type == const.CONF_CYCLE:
        return protocol.conf_t(conf_type, decode_conf_cycle(config_data))
    
    else:
        print("Error: Unknown conf_type")
        return None


def decode_pressure(data):
    type = data[0]
    
    if type == const.SEND_PRESSURE:
        value, unit, quality_code = struct.unpack('!IBB', data[1:])
        return protocol.pressure_t(type, value, unit, quality_code)
    
    elif type == const.GET_PRESSURE:
        return protocol.pressure_t(type, 0, 0, 0)
    
    else:
        print("Error: Unknown type")
        return None


def decode_level(data):
    type = data[0]
    
    if type == const.SEND_LEVEL:
        value, unit = struct.unpack('!IB', data[1:])
        return protocol.level_t(type, value, unit)
    
    elif type == const.GET_LEVEL:
        return protocol.level_t(type, 0, 0)
    
    else:
        print("Error: Unknown type")
        return None
    

def decode(payload):
    id, length, frame_type = struct.unpack('!QIB', payload[:13])
    data = payload[13:]
    
    if frame_type == const.MSG_TS:
        ts = decode_ts(data)
        return protocol.frame_t(id, length, frame_type, ts)
    
    elif frame_type == const.MSG_CONF:
        conf = decode_conf(data)
        return protocol.frame_t(id, length, frame_type, conf)
    
    elif frame_type == const.MSG_PRESSURE:
        pressure = decode_pressure(data)
        return protocol.frame_t(id, length, frame_type, pressure)
    
    elif frame_type == const.MSG_LEVEL:
        level = decode_level(data)
        return protocol.frame_t(id, length, frame_type, level)