class ts_t:
    def __init__(self, type, timestamp, offset):
        self.type = type
        self.timestamp = timestamp
        self.offset = offset


class conf_tank_t:
    def __init__(self, diameter_value, diameter_unit, volume_value, volume_unit, height_value, height_unit):
        self.diameter_value = diameter_value
        self.diameter_unit = diameter_unit
        self.volume_value = volume_value
        self.volume_unit = volume_unit
        self.height_value = height_value
        self.height_unit = height_unit

class conf_sensor_t:
    def __init__(self, number, ip, role, type, height_value, height_unit):
        self.number = number
        self.ip = ip
        self.role = role
        self.type = type
        self.height_value = height_value
        self.height_unit = height_unit
        
class conf_fluid_t:
    def __init__(self, density_value, density_unit, gaspressure_value, gaspressure_unit):
        self.density_value = density_value
        self.density_unit = density_unit
        self.gaspressure_value = gaspressure_value
        self.gaspressure_unit = gaspressure_unit
        
class conf_cycle_t:
    def __init__(self, time_value, time_unit, frame_loss):
        self.time_value = time_value
        self.time_unit = time_unit
        self.frame_loss = frame_loss

class conf_t:
    def __init__(self, type, data):
        self.type = type
        self.data = data


class pressure_t:
    def __init__(self, type, value, unit, quality_code):
        self.type = type
        self.value = value
        self.unit = unit
        self.quality_code = quality_code


class level_t:
    def __init__(self, type, value, unit):
        self.type = type
        self.value = value
        self.unit = unit


class frame_t:
    def __init__(self, id, length, type, data):
        self.id = id
        self.length = length
        self.type = type
        self.data = data