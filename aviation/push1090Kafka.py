import pyModeS as pms
from pyModeS.extra.tcpclient import TcpClient
from datetime import datetime, timedelta
import json
from kafka import KafkaProducer
import argparse
import sys
import cachetools


lat_ref, lon_ref = 51.50307, -0.59729
format_string = "%d/%m/%y %H:%M:%S.%f"

# define your custom class by extending the TcpClient
#   - implement your handle_messages() methods∂
class ADSBClient(TcpClient):

    flights = cachetools.TTLCache(maxsize=256, ttl=20*60)
    count=0
    producer=''
    args = {}


    def __init__(self, args):
        super(ADSBClient, self).__init__(args.host, args.port, args.rawtype)
        self.args = args
        self.producer = KafkaProducer(bootstrap_servers=[args.kafkaHost], api_version=(0,11,5), value_serializer=lambda v: json.dumps(v).encode('utf-8'))


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
                flight['updated']=datetime.now().isoformat(sep=' ', timespec='milliseconds')
                flight['flightDate']=datetime.today().strftime('%Y-%m-%d')
                self.flights.__setitem__(icao,flight)
                print (f"{self.count}: flights: {len(self.flights)}, {flight}")
                self.producer.send(self.args.kafkaTopic, flight)

def check_positive(value):
    ivalue = int(value)
    if ivalue <= 0:
        raise argparse.ArgumentTypeError("%s is an invalid positive int value" % value)
    return ivalue


def parse_args(args):
    parser = argparse.ArgumentParser()
    localhost = 'localhost'
    parser.add_argument("-ah", "--host", help="ADS-B 1090 host", default=localhost)
    parser.add_argument("-hp", "--port", help="ADS-B 1090 host port", type=check_positive, default=30002)
    parser.add_argument("-rt", "--rawtype", help="ADS-B 1090 raw type", choices =['raw','beast'], default='raw')
    parser.add_argument("-k", "--kafkaHost", help="Kafka bootstap host",  default='localhost:9092')
    parser.add_argument("-kt", "--kafkaTopic", help="Kafka topic",  default='flights')


    return parser.parse_args(args)

if __name__ == '__main__':
    parsed_args = parse_args(sys.argv[1:])
    print(parsed_args)
    # run new client, change the host, port, and rawtype if needed
    client = ADSBClient(parsed_args)
    # client = ADSBClient(host='localhost', port=30002, rawtype='raw')
    client.run()



        # loop = asyncio.get_event_loop()
        # loop.run_until_complete(main(sys.argv))
    # sys.exit(main(sys.argv))