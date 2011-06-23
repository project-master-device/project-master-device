import time, datetime

# Parse time string in "year-month-day hours:minutes:seconds.microseconds" format
# to tuple of parameters. 
def parse_time_string(string):
    format_str = "%Y-%m-%d %H:%M:%S"
    
    dt_str = str()
    mcs = 0

    if string.find('.') != -1:
        dt_str, other_str = string.split('.')
        mcs = int(other_str[:6])
    else:
        dt_str = string

    dt_structure = time.strptime(dt_str, format_str)

    res = []
    res.append(dt_structure.tm_year)
    res.append(dt_structure.tm_mon)
    res.append(dt_structure.tm_mday)
    res.append(dt_structure.tm_hour)
    res.append(dt_structure.tm_min)
    res.append(dt_structure.tm_sec)

    res.append(mcs)

    return tuple(res)

# Convert "year-month-day hours:minutes:seconds.microseconds" to build-in datetime object.
def datetime_from_string(string):
    return datetime.datetime(*parse_time_string(string))
