import pyModeS as pms
from pyModeS.extra.tcpclient import TcpClient
from datetime import datetime
import json
from kafka import KafkaProducer


lat_ref, lon_ref = 51.50307, -0.59729
format_string = "%d/%m/%y %H:%M:%S.%f"

# define your custom class by extending the TcpClient
#   - implement your handle_messages() methods
class ADSBClient(TcpClient):

    flights = {}
    count=0
    producer=''

    def __init__(self, host, port, rawtype):
        super(ADSBClient, self).__init__(host, port, rawtype)
        self.producer = KafkaProducer(bootstrap_servers=['192.168.0.46:29092'], api_version=(0,11,5), value_serializer=lambda v: json.dumps(v).encode('utf-8'))
        # self.producer = KafkaProducer(bootstrap_servers=['localhost:29092'], api_version=(0,11,5))

    def handle_messages(self, messages):
        for msg, ts in messages:
            if len(msg) != 28:  # wrong data length
                continue
            self.count+=1
            updated = False
            df = pms.df(msg)

            if df != 17:  # not ADSB
                continue

            if pms.crc(msg) !=0:  # CRC fail
                continue

            icao = pms.adsb.icao(msg)
            default={'icao':icao}
            flight = self.flights.get(icao, default)


            tc = pms.adsb.typecode(msg)

            typecode = pms.adsb.typecode(msg)

            # callsign = pms.adsb.callsign(msg)

            # TODO: write you magic code here
            # print(ts, icao, tc, msg, typecode)
            if (typecode>0 and typecode <5):
                # print(f"icao {icao}, callsign: {pms.adsb.callsign(msg)}")
                flight['callsign']=pms.adsb.callsign(msg)
                updated = True
            if (typecode==19):
                speed, angle, verticalSpeed, speedType = pms.adsb.velocity(msg)
                # print(f"icao {icao},velocity: {pms.adsb.velocity(msg)}, ")
                flight[speedType] = speed
                # flight['speedType'] = speedType

                updated = True
            if (typecode >= 9 and typecode <= 18) or (typecode >= 20 and typecode <= 22):
                # print(f"icao {icao},altitude: {pms.adsb.altitude(msg)}")
                flight['altitude'] = pms.adsb.altitude(msg)
                lat, lon = pms.adsb.position_with_ref(msg, lat_ref, lon_ref)
                # print(f"icao {icao},lat: {lat},lon: {lon}")
                flight['lat'] = lat
                flight['lon'] = lon
                updated = True
            if (typecode >= 5 and typecode <= 8):
                lat, lon = pms.adsb.position_with_ref(msg, lat_ref, lon_ref)
                # print(f"icao {icao},lat: {lat},lon: {lon}")
                flight['lat'] = lat
                flight['lon'] = lon
                updated = True

            if updated:
                flight['updated']=datetime.utcnow().isoformat(sep=' ', timespec='milliseconds')
                self.flights[icao]=flight
                # print (flight)
                print (f"{self.count}: flights: {len(self.flights)}, {flight}")
                # self.producer.send('flights',  b'some_message_bytes')
                self.producer.send('flights', flight)
# run new client, change the host, port, and rawtype if needed
client = ADSBClient(host='localhost', port=30002, rawtype='raw')
client.run()
