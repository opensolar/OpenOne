
import matplotlib.pyplot as plt
import numpy as np
from datetime import date

def get_days_elapsed(d):
    delta = d - date(2020, 1, 1)
    return delta.days

def get_declination(day = 173):
    x = (360/365)*(day+11)
    deg = -23.45*np.cos(np.pi * x / 180.0)
    return np.pi * deg / 180.0


# days: number of days since january 1st (173 is the summer solstice)
# latitude: in degrees (montreal, canada is 45.5)
# hour: 0 to 23 (time of day, noon is 12)
def get_elevation(day = 173, latitude = 45.763969, hour = 12.0):
    declination = get_declination(day)

    lat_angle = np.pi * latitude / 180.0

    #hour angle (0 at solar noon, negative at sunrise, positive at sunset)
    # 360 deg per day so 15 deg per hour, if the earth is really round ;)
    
    hour_angle = (np.pi * 15.0 / 180.0 ) * (hour - 12.0)

    x = np.sin(declination) * np.sin(lat_angle)
    y = np.cos(declination) * np.cos(hour_angle) * np.cos(lat_angle)

    elevation = np.arcsin(x + y)
    
    return elevation
    
def get_azimuth(day = 173, latitude = 45.78, hour = 12.0):
    declination = get_declination(day)
    lat_angle = np.pi * latitude / 180.0
    hour_angle = (np.pi * 15.0 / 180.0 ) * (hour - 12.0)
    
    elevation = get_elevation(day = day, latitude = latitude, hour = hour)
    x = (np.sin(declination) * np.cos(lat_angle) - np.cos(declination) * np.cos(hour_angle) * np.sin(lat_angle)) / np.cos(elevation)

    if x < -1:
        x = -1
    elif x > 1:
        x = 1
    azimuth = np.arccos(x)
    
    
    if hour_angle <= 0:
        return azimuth
    else:
        return 2.0 * np.pi - azimuth
    
    
    

