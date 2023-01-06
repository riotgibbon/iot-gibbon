import time
import psycopg2
import psycopg2.extras
from phue import Bridge  # https://github.com/studioimaginaire/phue
import traceback

sleepSeconds=5

CONNECTION = "postgres://postgres:password@192.168.0.46:5432/postgres"

hueHigh = 65535
hueLow = 0
lightId=8
host = '192.168.0.11'
transitionTime=50
b = Bridge(host)

# If the app is not registered and the button is not pressed, press the button and call connect() (this only needs to be run a single time)
b.connect()

# Get the bridge state (This returns the full dictionary that you can explore)
b.get_api()

def mapRange( x,  in_min,  in_max,  out_min,  out_max):
  return ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)

with psycopg2.connect(CONNECTION) as conn:  
    cursor = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)
    while True:
        try:
            # print("getting data")
            query="""
            select * from (
                  select icao,
                         altitude,
                         speed,
                         distance,
                         ts,
                         updated,
                         ROW_NUMBER() OVER (
                             PARTITION BY icao
                             ORDER BY ts desc) reading,
                        first_value
                            (ts) over(order by ts desc) as LAST_VALUE
                  from (
                           select st_distance(ST_MakePoint(lon, lat) ::geography,
                                              ST_MakePoint(-0.597207, 51.503572) ::geography) distance,
                                  TO_TIMESTAMP(updated, 'YYYY-MM-DD HH24:MI:SS.MS')           ts,
                                  *
                           from flights

                           where altitude>0 and lat>0
                            and flightdate = CURRENT_DATE::TEXT
                       ) f

              )r
where reading=1
and ts>last_value - interval '2 minute'
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
            print(f"icao: {icao} , altitude : {altitude}, speed: {speed}, distance : {distance}")

            hueMappedValue =int(mapRange (altitude, 500, 15000, hueLow, hueHigh))
            sat =int(mapRange (speed, 0, 600, 100,254))
            bri =int(mapRange (10000- distance, 0, 10000, 100,254))
            command =  {'transitiontime' : transitionTime,  'hue':  hueMappedValue, 'sat':sat, 'bri': bri}
            print(command)
            b.set_light(lightId,command)
        except Exception as error:
            traceback.print_exc() 

        time.sleep(sleepSeconds)