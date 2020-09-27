
#  _____   _____  _______ __   _ _______  _____         _______  ______   _____  _____ 
# |     | |_____] |______ | \  | |______ |     | |      |_____| |_____/     |   |     |
# |_____| |       |______ |  \_| ______| |_____| |_____ |     | |    \_ . __|__ |_____|
#
#
# Author: Martin Lafleur (mlafleur@opensolar.io)
# Date: Sep 2020
# Project: OpenOne
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, version 3 of the License.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.
#    

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


# returns correction due to the orbital excentricity in h
# https://www.pveducation.org/pvcdrom/properties-of-sunlight/solar-time
# https://en.wikipedia.org/wiki/Analemma
def get_excentricity_correction(day):
    b = np.pi* (360 / 365) * (day -81) / 180.0
    eot = (9.87 * np.sin(2*b) - 7.53 * np.cos(b) - 1.5 * np.sin(b))/ 60.0
    return eot

dates = [
    date(2019,12,21),
    date(2020,1,20), 
    date(2020,2,20),
    date(2020,3,19),
    date(2020,4,20),
    date(2020,5,21),
    date(2020,6,20)
]

latitude = 45.78 #Saint-Jerome
longitude = -74.03 #Saint-Jerome
#longitude = -61.25 #Kegaska
#longitude = -89.28 #Thunderbay

meridian = -75
meridian_correction = (longitude - meridian) / 15.0 # in h

for d in dates:
    elapsed = get_days_elapsed(d)
    correction_factor  = meridian_correction + get_excentricity_correction(elapsed)
    
    elevations = []
    azimuths = []
    for h in range(6,19):

    
        elevation_deg = 180.0 * get_elevation(hour = h - correction_factor, day = elapsed, latitude = latitude) / np.pi
        elevations.append(elevation_deg)
        
        azimuth_deg = 180.0 * get_azimuth(hour = h - correction_factor, day = elapsed, latitude = latitude) / np.pi

        azimuths.append(azimuth_deg)
        #if h == 12:
        #    plt.plot(azimuth_deg, elevation_deg, marker = "*")
           
    plt.plot(azimuths, elevations, label = "elevation for " + str(d), marker = ".")
    
    

for h in range(6,19):
    elevations = []
    azimuths = []
    for d in range(365):
        correction_factor  = meridian_correction + get_excentricity_correction(d)
        elevation_deg = 180.0 * get_elevation(hour = h - correction_factor, day = d) / np.pi
        elevations.append(elevation_deg)
        
        azimuth_deg = 180.0 * get_azimuth(hour = h - correction_factor, day = d) / np.pi
        azimuths.append(azimuth_deg)

    if h == 6 or h ==18:
        plt.plot(azimuths, elevations, label = "time:" + str(h) + " h")   
    else:
        plt.plot(azimuths, elevations)


plt.xlabel('azimuth angle (deg)')
plt.ylabel('elevation angle (deg)')
plt.title('longitude = ' + str(longitude) + ' latitude = ' + str(latitude))
plt.grid(True)
plt.legend()
plt.show()












#Analemma

elevations = []
azimuths = []
h = 13

dates = [
    date(2019,12,21),
    date(2020,1,20), 
    date(2020,2,20),
    date(2020,3,19),
    date(2020,4,20),
    date(2020,5,21),
    date(2020,6,20),
    date(2020,7,20),
    date(2020,8,20),
    date(2020,9,20),
    date(2020,10,20),
    date(2020,11,20)
    
]

for d in range(0,365, 5):
    #elapsed = get_days_elapsed(d)
    elapsed = d
    correction_factor  = meridian_correction + get_excentricity_correction(d)
    elevation_deg = 180.0 * get_elevation(hour = h - correction_factor, day = elapsed) / np.pi
    elevations.append(elevation_deg)
    
    azimuth_deg = 180.0 * get_azimuth(hour = h - correction_factor, day = elapsed) / np.pi
    azimuths.append(azimuth_deg)

    if len(elevations) >= 2:
        #plt.plot(azimuths[-2:], elevations[-2:], label = "elevation ending " + str(d), marker = ".")
        plt.plot(azimuths[-2:], elevations[-2:], marker = ".")        

plt.xlabel('azimuth angle (deg)')
plt.ylabel('elevation angle (deg)')
plt.title('Analemma longitude = ' + str(longitude) + ' latitude = ' + str(latitude) + ' time = ' + str(h))
plt.grid(True)
plt.legend()
plt.show()