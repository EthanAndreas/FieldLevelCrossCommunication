from xcom import constant as const
from xcom import protocol

def print_frame(frame):
    print(frame.id)
    print(frame.length)
    print(frame.type)
    
    if frame.type == const.MSG_TS:
        print(frame.data.type)
        print(frame.data.timestamp)
        print(frame.data.offset)
    
    elif frame.type == const.MSG_CONF:
        print(frame.data.type)
        
        if frame.data.type == const.CONF_SEND_TANK:
            print(frame.data.data.diameter_value)
            print(frame.data.data.diameter_unit)
            print(frame.data.data.volume_value)
            print(frame.data.data.volume_unit)
            print(frame.data.data.height_value)
            print(frame.data.data.height_unit)
            
        elif frame.data.type == const.CONF_SEND_SENSOR:
            print(frame.data.data.number)
            print(frame.data.data.ip)
            print(frame.data.data.role)
            print(frame.data.data.height_value)
            print(frame.data.data.height_unit)
            
        elif frame.data.type == const.CONF_SEND_FLUID:
            print(frame.data.data.density_value)
            print(frame.data.data.density_unit)
            print(frame.data.data.gaspressure_value)
            print(frame.data.data.gaspressure_unit)
            
        elif frame.data.type == const.CONF_CYCLE:
            print(frame.data.data.time_value)
            print(frame.data.data.time_unit)
            print(frame.data.data.frame_loss)
            
        elif frame.data.type in [const.CONF_GET_TANK, const.CONF_GET_SENSOR, const.CONF_GET_FLUID]:
            # allowed type
            pass
        
        else:
            print("Error: Unknown conf_type")
            
    elif frame.type == const.MSG_PRESSURE:
        print(frame.data.type)
        
        if frame.data.type == const.SEND_PRESSURE:
            print(frame.data.value)
            print(frame.data.unit)
            print(frame.data.quality_code)
            
        elif frame.data.type == const.GET_PRESSURE:
            # allowed type
            pass
        
        else:
            print("Error: Unknown pressure_type")
            
    elif frame.type == const.MSG_LEVEL:
        print(frame.data.type)
        
        if frame.data.type == const.SEND_LEVEL:
            print(frame.data.value)
            print(frame.data.unit)
            
        elif frame.data.type == const.GET_LEVEL:
            # allowed type
            pass
        
        else:
            print("Error: Unknown level_type")