# IP address test return
SUCCESS = 0
FAILED_COMMAND = -2
UNREACHABLE_IP_ADDR = -3
INVALID_IP_ADDR = -4

# Request configuration 
FAILED_REQUEST_CONF = -2

# Request level
FAILED_REQUEST_LEVEL = -3

# Different number of sensors between the one retrieved by requesting configuration and the one entered by the user 
INVALID_NUM_SENSORS = -3

# Max number of sensors
MAX_NUM_ADDR = 3

# Error codes list
ERROR_CODES = [FAILED_COMMAND, UNREACHABLE_IP_ADDR, INVALID_IP_ADDR, FAILED_REQUEST_CONF, FAILED_REQUEST_LEVEL, INVALID_NUM_SENSORS]

# no existing variable for html program
NO_VALUE = -1