import paho.mqtt.client as mqtt
import time
import psycopg2
import psycopg2.extras
from phue import Bridge  # https://github.com/studioimaginaire/phue
import traceback
import json




sleepSeconds=2
transitionTime=sleepSeconds*10

CONNECTION = "postgres://postgres:password@192.168.0.46:5432/postgres"

hueHigh = 65535
hueLow = 0
lightId=8
host = '192.168.0.11'

b = Bridge(host)

# If the app is not registered and the button is not pressed, press the button and call connect() (this only needs to be run a single time)
b.connect()

# Get the bridge state (This returns the full dictionary that you can explore)
b.get_api()

def getMqttClient():
    client = mqtt.Client()
    while (True):
        try:
            client.connect("192.168.0.46", 1883, 60)
            break
        except Exception:
            print ("error connecting, pausing")
            traceback.print_exc()
            time.sleep(5)
    return client

client = getMqttClient()   

def mapRange( x,  in_min,  in_max,  out_min,  out_max):
    x= max(in_min, x)
    x= min(in_max, x)
        
    return ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)


while True:
    try:
        with psycopg2.connect(CONNECTION) as conn:  
            cursor = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)
        # print("getting data")
        query="""
    select * from (
                      select icao,
                             altitude,
                             speed,
                             distance,
--                              ts,
                             updated,
                             ROW_NUMBER() OVER (
                                 PARTITION BY icao
                                 ORDER BY updated desc) reading


                      from (
                               select st_distance(ST_MakePoint(lon, lat) ::geography,
                                                  ST_MakePoint(-0.597207, 51.503572) ::geography) distance,
                                      *
                               from flights
                               where altitude>0 and lat>0
                                and flightdate = CURRENT_DATE::TEXT
                                and updated > (CURRENT_TIMESTAMP - interval '2 minute')
                           ) f
                  )r
    where reading=1
    order by distance
    limit 1
        """
        cursor.execute(query)
        row = cursor.fetchone()
        # print (row)
        icao=row[0]
        altitude=row[1]
        speed=row[2]
        distance=row[3]
        updated=row[4]
        planeData={'icao': icao , 'altitude' : altitude, 'speed': speed, 'distance' : distance, 'updated': str(updated)}
        print(planeData)
        maxDistance = 75000
        maxAltitude=40000

        hueMappedValue =int(mapRange (altitude, 500, 40000, hueLow, hueHigh))
        sat =int(mapRange (speed, 0, 600, 50,254))

        bri =int(mapRange (maxDistance - distance, 0, maxDistance, 25,200))
        command =  {'transitiontime' : transitionTime,  'hue':  hueMappedValue, 'sat':sat, 'bri': bri}
        print(command)
        b.set_light(lightId,command)
        body={}
        body['hue']=command #json.dumps(lightInfo)
        
        body['plane']=  planeData

        jsonMsg = json.dumps(body)
        print(jsonMsg)
        client.publish('home/cmd/hue/plane', jsonMsg)

    except Exception as error:
        client.publish('home/cmd/hue/plane', f'could not publish: {error}')
        traceback.print_exc() 

    time.sleep(sleepSeconds)