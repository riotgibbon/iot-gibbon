import time
import json
from kafka import KafkaConsumer
from pandas import DataFrame
from datetime import datetime
import psycopg2

brokers='192.168.0.46:9092'
topic='flights_jetson'
sleep_time=300
offset='latest'

consumer = KafkaConsumer(bootstrap_servers=brokers, auto_offset_reset=offset,consumer_timeout_ms=1000)
consumer.subscribe([topic])

CONNECTION = "postgres://postgres:password@192.168.0.46:5432/postgres"

# {'icao': '406591', 'GS': 352, 'updated': '2023-01-06 11:19:42.593', 'flightDate': '2023-01-06', 'altitude': 14400, 
# 'lat': 51.18279, 'lon': -1.11764, 'callsign': 'BAW255__'}

while(True):

    with psycopg2.connect(CONNECTION) as conn:  
        cursor = conn.cursor()

        for message in consumer:
            # print(message)
            d=json.loads(message.value)
            altitude=d.get('altitude',-1)
            if altitude==None:
                altitude=-1
            # print(d['icao'])    
            sql =f"""insert into flights (icao, altitude, lat, lon, updated, flightdate, speed, callsign)
            values ('{d.get('icao')}', {altitude}, {d.get('lat',-1)}, {d.get('lon',-1)}, '{d.get('updated')}', '{time.strftime("%Y-%m-%d")}', {d.get('GS',0)}, '{d.get('callsign')}')"""
            print (sql)
            cursor.execute(sql)
            conn.commit()
        # df=DataFrame(d)