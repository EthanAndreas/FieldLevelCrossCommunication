from xcom import constant as xcom_const
from xcom import network as xcom_network
from xcom import print as xcom_print
from utils import constant as const
import ping3
import socket
import struct

def test_ip_addr(ip_addr):
    """ Test the IP address
    Args:
        str : IP address of the sensor node
    Returns:
        int : -4 if IP address is empty string, -3 if IP address is invalid, -2 if IP address is unreachable or 0 if the IP address is valid
    """
    
    # Test IP address format 
    parts = ip_addr.split('.')
    if len(parts) != 4:
        return const.INVALID_IP_ADDR
    for part in parts:
        if not part.isdigit():
            return const.INVALID_IP_ADDR
        num = int(part)
        if num < 0 or num > 255:
            return const.INVALID_IP_ADDR
        
    try:
        socket.inet_aton(ip_addr)
    except socket.error:
        return const.INVALID_IP_ADDR

    # Check if the IP address is on the network
    response = ping3.ping(ip_addr, timeout=1)
    if response == False or response == None:
        return const.UNREACHABLE_IP_ADDR

    return const.SUCCESS


def ip_str_to_int(ip_addr):
    """Converts an IP address string to an integer
    Args:
        str : IP address
    Returns:
        int : IP address in integer format
    """
    
    ip_bytes = list(map(int, ip_addr.split('.')))
    if len(ip_bytes) != 4:
        raise ValueError('Invalid IP address')
    return (ip_bytes[0] << 24) | (ip_bytes[1] << 16) | (ip_bytes[2] << 8) | ip_bytes[3]


def unit_str_to_int(unit):
    """Convert a unit in string format to constant value from xcom
    Args:
        str : unit
    Returns:
        int : constant value from xcom
    """  
    
    # Pressure unit
    if unit == "Pa":
        return xcom_const.PRESSURE_UNIT_PASCAL
    elif unit == "bar":
        return xcom_const.PRESSURE_UNIT_BAR
    elif unit == "psi":
        return xcom_const.PRESSURE_UNIT_PSI
    
    # Length unit
    elif unit == "m":
        return xcom_const.LENGTH_UNIT_METER
    elif unit == "ft":
        return xcom_const.LENGTH_UNIT_FEET
    elif unit == "in":
        return xcom_const.LENGTH_UNIT_INCH
    
    # Volume unit
    elif unit == "m3":
        return xcom_const.VOLUME_UNIT_METER3
    elif unit == "L":
        return xcom_const.VOLUME_UNIT_LITER
    elif unit == "gal":
        return xcom_const.VOLUME_UNIT_GALLON
    
    # Density unit
    elif unit == "kg/m3":
        return xcom_const.DENSITY_UNIT_KGBYMETER3
    elif unit == "g/cm3":
        return xcom_const.DENSITY_UNIT_GBYCM3


def set_in_default_unit(value, unit):
    """ Set the value in the default unit"""
    
    # Pressure value (default unit is Pascal)
    if unit == xcom_const.PRESSURE_UNIT_PASCAL:
        return value
    elif unit == xcom_const.PRESSURE_UNIT_BAR:
        return value * 100000
    elif unit == xcom_const.PRESSURE_UNIT_PSI:
        return value * 6894.75729
    
    # Length value (default unit is meter)
    elif unit == xcom_const.LENGTH_UNIT_METER:
        return value
    elif unit == xcom_const.LENGTH_UNIT_FEET:
        return value * 0.3048
    elif unit == xcom_const.LENGTH_UNIT_INCH:
        return value * 0.0254
    
    # Volume value (default unit is meter3)
    elif unit == xcom_const.VOLUME_UNIT_METER3:
        return value
    elif unit == xcom_const.VOLUME_UNIT_LITER:
        return value * 0.001
    elif unit == xcom_const.VOLUME_UNIT_GALLON:
        return value * 0.00378541
    
    # Density value (default unit is kg/meter3)
    elif unit == xcom_const.DENSITY_UNIT_KGBYMETER3:
        return value
    elif unit == xcom_const.DENSITY_UNIT_GBYCM3:
        return value * 1000


def send_pressure(ip_addr, pressure, unit):
    """ Send pressure values to the IP addresses
    Args:
        str : IP address of the sensor node
        int : pressure value
        int : unit
    Returns:
        int : -4 if IP address is empty string, -3 if IP address is invalid, -2 if IP address is unreachable, -1 if the command fails, or 0 if the command succeeds
    """
        
    # If the ip address is invalid, return the error code
    ret = test_ip_addr(ip_addr)
    if ret != const.SUCCESS:
        return ret
    
    sock = xcom_network.udp_init()
    
    # Request the pressure value thanks to get pressure packet type
    xcom_network.send_pressure(sock, ip_addr, xcom_const.SEND_PRESSURE, int(pressure), unit_str_to_int(unit), xcom_const.QUALITY_CODE_SIMULATE)

    xcom_network.udp_close(sock)

    return const.SUCCESS


def send_pressures(num_addr, ip_addr, pressure, unit):
    """ Send pressure values to the IP addresses
    Args:
        int : number of IP addresses
        list : list of IP addresses
        list : list of pressure values
        list : list of units
    Returns:
        int : -4 if IP address is empty string, -3 if IP address is invalid, -2 if IP address is unreachable, -1 if the command fails, or 0 if the command succeeds
    """
    
    for i in range(num_addr):
        # If the ip address is invalid, return the error code
        ret = test_ip_addr(ip_addr[i])
        if ret != const.SUCCESS:
            return ret
        
        sock = xcom_network.udp_init()
        
        # Send the pressure value thanks to send pressure packet type
        xcom_network.send_pressure(sock, ip_addr[i], xcom_const.SEND_PRESSURE, int(pressure[i]), unit_str_to_int(unit[i]), xcom_const.QUALITY_CODE_SIMULATE)
        
        xcom_network.udp_close(sock)
        
    return const.SUCCESS


def request_pressure(ip_addr):
    """ Request the data from the IP address
    Args:
        str : IP address of the sensor node
    Returns:
        int : -4 if IP address is empty string, -3 if IP address is invalid, -2 if IP address is unreachable, -1 if the command fails, or the result of the command
    """
    
    # If the ip address is invalid, return the error code
    ret = test_ip_addr(ip_addr)
    if ret != const.SUCCESS:
        return ret
    
    sock = xcom_network.udp_init()
    
    # Request the pressure value thanks to get pressure packet type
    xcom_network.send_pressure(sock, ip_addr, xcom_const.GET_PRESSURE, 0, 0, 0)
    frame = xcom_network.receive(sock, xcom_const.MSG_PRESSURE, xcom_const.SEND_PRESSURE)
    
    xcom_network.udp_close(sock)
    
    if frame == None:
        return const.FAILED_COMMAND
    
    return frame.data.value # transform to Pascal


def request_configuration(ip_addr):
    """ Request the configuration from the IP address
    Args:
        str : IP address of the sensor node
    Returns:
        int : -4 if IP address is empty string, -3 if IP address is invalid, -2 if IP address is unreachable, -1 if the command fails, or the result of the command
    """
    
    # If the ip address is invalid, return the error code
    ret = test_ip_addr(ip_addr)
    if ret != const.SUCCESS:
        return ret
    
    sock = xcom_network.udp_init()
    
    # Request the tank configuration thanks to get configuration packet type
    xcom_network.send_conf_tank(sock, ip_addr, xcom_const.CONF_GET_TANK, 0, 0, 0, 0, 0, 0)
    conf_tank = xcom_network.receive(sock, xcom_const.MSG_CONF, xcom_const.CONF_SEND_TANK)
    if conf_tank == None:
        xcom_network.udp_close(sock)
        return const.FAILED_COMMAND
    
    # Request the sensor configuration thanks to get configuration packet type
    conf_sensors = []
    for _ in range(xcom_const.MAX_SENSORS):
        xcom_network.send_conf_sensor(sock, ip_addr, xcom_const.CONF_GET_SENSOR, 0, 0, 0, 0, 0)
        conf_sensor = xcom_network.receive(sock, xcom_const.MSG_CONF, xcom_const.CONF_SEND_SENSOR)
        if conf_sensor != None:
            conf_sensors.append(conf_sensor)

    if len(conf_sensors) == 0 or len(conf_sensors) != xcom_const.MAX_SENSORS:
        xcom_network.udp_close(sock)
        return const.FAILED_COMMAND
    
    # Request the fluid configuration thanks to get configuration packet type
    xcom_network.send_conf_fluid(sock, ip_addr, xcom_const.CONF_GET_FLUID, 0, 0, 0, 0)
    conf_fluid = xcom_network.receive(sock, xcom_const.MSG_CONF, xcom_const.CONF_SEND_FLUID)
    if conf_fluid == None:
        xcom_network.udp_close(sock)
        return const.FAILED_COMMAND
    
    xcom_network.udp_close(sock)
    
    print("test 1")
    
    return [conf_tank, conf_sensors, conf_fluid]
 
        
def request_level(conf):
    """ Request the level calculated by the main device
    Args:
        list : list of ip addresses
    Returns:
        int : -1 if the command fails, or the level in default unit
    """

    # Check if the configuration is valid
    if conf in const.ERROR_CODES:
        return const.FAILED_REQUEST_CONF
    
    # Retrieve the main device ip address
    mainIP = 0
    for i in range(len(conf[1])):
        if conf[1][i].data.data.type == xcom_const.TYPE_MAIN:
            mainIP = conf[1][i].data.data.ip
            break

    if mainIP == 0:
        return const.FAILED_REQUEST_CONF
    
    sock = xcom_network.udp_init()
    
    # Convert mainIP to string format
    mainIP_str = socket.inet_ntoa(struct.pack("!I",socket.htonl(mainIP)))

    # Request the level value thanks to get level packet type in broadcast because the level is calculated by the main device which the ip address is unknown
    xcom_network.send_level(sock, mainIP_str, xcom_const.GET_LEVEL, 0, 0)
    frame = xcom_network.receive(sock, xcom_const.MSG_LEVEL, xcom_const.SEND_LEVEL)
    if frame == None:
        xcom_network.udp_close(sock)
        return const.FAILED_REQUEST_LEVEL
    
    xcom_network.udp_close(sock)

    print("test 2")

    return set_in_default_unit(frame.data.value, frame.data.unit)
   
    
def calculate_level(num_addr, ip_addr, pressure, unit, conf):
    """ Calculate the level from the pressure value
    Args:
        list : list of ip addresses
        list : list of pressure values
        list : list of units
    Returns:
        int : -1 if the command fails, -2 if the number of sensors entered by the user is different from the number of sensors in the configuration, or the level in default unit
    """
    
    #  Check if the configuration is valid   
    if conf in const.ERROR_CODES:
        return const.FAILED_REQUEST_CONF
    
    print("test 3")
    
    # Retrieve the number of sensors
    for i in range(len(conf[1])):
        if conf[1][i].data.data.number != 0:
            if conf[1][i].data.data.number != num_addr:
                return const.INVALID_NUM_SENSORS  
    
    print("test 4")
    
    # Sort ip_addr and pressure array in the same order as the configuration sensors
    copy_ip_addr = ip_addr
    copy_pressure = pressure
    for i in range(num_addr):
        for j in range(num_addr):
            if conf[1][j].data.data.ip == ip_str_to_int(copy_ip_addr[i]):
                ip_addr[j] = copy_ip_addr[i]
                pressure[j] = copy_pressure[i]
                break
    
    print("test 5")
    
    xcom_print.print_frame(conf[0])
    xcom_print.print_frame(conf[1][0])
    xcom_print.print_frame(conf[1][1])
    xcom_print.print_frame(conf[1][2])
    xcom_print.print_frame(conf[2])
    
    
    # Calculate the level
    level = 0
    if num_addr == 1:
        # one sensor received and its role is high
        if conf[2].data.data.density_value != 0:
            level = (set_in_default_unit(int(pressure[0]), unit_str_to_int(unit[0])) - set_in_default_unit(conf[2].data.data.gaspressure_value, conf[2].data.data.gaspressure_unit)) / (set_in_default_unit(conf[2].data.data.density_value, conf[2].data.data.density_unit) * 9.81)
    
    elif num_addr == 2:
        # two sensors received and their roles are high and low
        if conf[2].data.data.density_value != 0:
            level = (set_in_default_unit(int(pressure[0]), unit_str_to_int(unit[0])) - set_in_default_unit(int(pressure[1]), unit_str_to_int(unit[1]))) / (set_in_default_unit(conf[2].data.data.density_value, conf[2].data.data.density_unit) * 9.81)
        
    elif num_addr == 3:
        # three sensors received and their roles are high, density and low
        density = 0
        if conf[1][1].data.data.height_value != 0:
            density = (set_in_default_unit(int(pressure[0]), unit_str_to_int(unit[0])) - set_in_default_unit(int(pressure[1]), unit_str_to_int(unit[1]))) / (set_in_default_unit(conf[1][1].data.data.height_value, conf[1][1].data.data.height_unit) * 9.81)
        if density != 0:
            level = (set_in_default_unit(int(pressure[0]), unit_str_to_int(unit[0])) - set_in_default_unit(int(pressure[2]), unit_str_to_int(unit[2]))) / (density * 9.81)
 
    return level