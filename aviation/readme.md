# Aviation tracking



working 1090 for Jetson - 192.168.0.53
https://github.com/joergsteinkamp/dump1090

./dump1090 --net

./dump1090 --net --net-beast

http://192.168.0.53:8080/




config 
https://uk.flightaware.com/adsb/piaware/advanced_configuration

piaware-config
piaware-config receiver-host 192.168.0.53
piaware-config receiver-port 30005
piaware-config receiver-type relay

nc 192.168.0.53 30003

sudo systemctl restart piaware.service


tail /var/log/piaware.log

sudo journalctl --no-pager -u piaware


## pyModeS

https://github.com/junzis/pyModeS


modeslive --source net --connect localhost 30002 raw
modeslive --source net --connect 192.168.0.53 30002 raw
modeslive --source net --connect 192.168.0.53 30005 beast


modeslive --source rtlsdr



## opensky-network
https://github.com/mikenye/docker-opensky-network/blob/master/README.md


docker run \
 -d \
 --rm \
 --name opensky \
 --network=ADSBNET \
 -e TZ="YOURTIMEZONE" \
 -e BEASTHOST=YOURBEASTHOST \
 -e LAT=YOURLATITUDE \
 -e LONG=YOURLONGITUDE \
 -e ALT=YOURALTITUDE \
 -e OPENSKY_USERNAME=YOUROPENSKYUSERNAME \
 -e OPENSKY_SERIAL=YOUROPENSKYSERIAL \
 mikenye/opensky-network



 docker run \
 -d \
 --rm \
 --name opensky \
 -e TZ="GMT" \
 -e BEASTHOST=localhost \
 -e LAT=51.50307 \
 -e LONG=-0.59729 \
 -e ALT=22 \
 -e OPENSKY_USERNAME=tobysevans \
 mikenye/opensky-network


 sudo docker run \
 -d \
 --restart always \
 --name opensky \
 -e TZ="GMT" \
 -e BEASTHOST=192.168.0.53 \
 -e LAT=51.50307 \
 -e LONG=-0.59729 \
 -e ALT=22 \
 -e OPENSKY_USERNAME=tobysevans \
 -e OPENSKY_SERIAL=-1408232414 \
 mikenye/opensky-network



sudo docker run \
    --rm \
    -it \
 -e BEASTHOST=192.168.0.53 \
 -e LAT=51.50307 \
 -e LONG=-0.59729 \
 -e ALT=22 \
 -e OPENSKY_USERNAME=tobysevans \
  -e OPENSKY_SERIAL=-1408232414 \

    mikenye/opensky-network




## Kafka
https://kafka-python.readthedocs.io/en/master/




 bin/kafka-console-consumer.sh --topic quickstart-events --from-beginning --bootstrap-server localhost:9092

  bin/kafka-console-consumer.sh --topic flights --from-beginning --bootstrap-server localhost:29092


  https://github.com/wurstmeister/kafka-docker

```
version: '2'
services:
  zookeeper:
    image: wurstmeister/zookeeper
    ports:
      - "2181:2181"
  kafka:
    build: .
    ports:
      - "9092:9092"
    environment:
      DOCKER_API_VERSION: 1.22
      KAFKA_ADVERTISED_HOST_NAME: 192.168.0.46
      KAFKA_ZOOKEEPER_CONNECT: zookeeper:2181
    volumes:
      - /var/run/docker.sock:/var/run/docker.sock
```


## dump1090

toby@jet-2gibbon:~/github/iot-gibbon/aviation$ python3 dump1090.py -h
usage: dump1090.py [-h] [-ah HOST] [-hp PORT] [-rt {raw,beast}] [-k KAFKAHOST]
                   [-kt KAFKATOPIC]

optional arguments:
  -h, --help            show this help message and exit
  -ah HOST, --host HOST
                        ADS-B 1090 host
  -hp PORT, --port PORT
                        ADS-B 1090 host port
  -rt {raw,beast}, --rawtype {raw,beast}
                        ADS-B 1090 raw type
  -k KAFKAHOST, --kafkaHost KAFKAHOST
                        Kafka bootstap host
  -kt KAFKATOPIC, --kafkaTopic KAFKATOPIC
                        Kafka topic

  python3 /home/toby/github/iot-gibbon/aviation/dump1090.py -ah localhost -k 192.168.0.46:9092 -kt flights_jetson



  ## Vertica integration

  from Kafka - https://docs.confluent.io/5.5.1/connect/kafka-connect-vertica/sink/index.html

  30 day connector


  from Vertica
  https://www.vertica.com/docs/10.0.x/HTML/Content/Authoring/KafkaIntegrationGuide/AutomaticallyCopyingDataFromKafka.htm#0

  run manual first
  https://www.vertica.com/docs/10.0.x/HTML/Content/Authoring/KafkaIntegrationGuide/ManuallyCopyingDataExample.htm?TocPath=Integrating%20with%20Apache%20Kafka|Manually%20Copying%20Data%20From%20Kafka|_____1

  use a Flex table - will dynamically create according to incoming data




COPY flights SOURCE KafkaSource(stream='flights_jetson|0|-2',
                                                                       brokers='192.168.0.46:9092',
                                                                       duration=interval '10000 milliseconds')
                                                    PARSER KafkaJSONParser()
                                                    REJECTED DATA AS TABLE public.flights_rejections
[2021-10-14 11:29:53] 107,786 rows affected in 10 s 247 ms


SELECT compute_flextable_keys('flights');

select * from flights_keys;

| key\_name | frequency | data\_type\_guess |
| :--- | :--- | :--- |
| GS | 107222 | Integer |
| altitude | 107264 | Integer |
| callsign | 101582 | Varchar\(20\) |
| icao | 107786 | Varchar\(20\) |
| updated | 107786 | Timestamp |
| lat | 107264 | Numeric\(10,6\) |
| lon | 107264 | Numeric\(9,6\) |



## Automated Kafka>Vertica ingest
https://www.vertica.com/docs/10.0.x/HTML/Content/Authoring/KafkaIntegrationGuide/AutomaticallyCopyingDataFromKafka.htm
https://www.vertica.com/docs/10.0.x/HTML/Content/Authoring/KafkaIntegrationGuide/SettingUpAScheduler.htm

config file - written to /home/dbadmin/docker/flights.conf

```
username=dbadmin
password=
dbhost=192.168.0.46
dbport=5433
config-schema=docker
```


CREATE RESOURCE POOL flights_pool MEMORYSIZE '10%' PLANNEDCONCURRENCY 1 QUEUETIMEOUT 0;


add to path
export PATH=/opt/vertica/packages/kafka/bin:$PATH

vkconfig scheduler --create --config-schema docker --operator dbadmin  --conf /home/dbadmin/docker/flights.conf
vkconfig cluster --create --cluster kafka_flights --hosts 192.168.0.46:9092 --conf /home/dbadmin/docker/flights.conf
vkconfig source --create --cluster kafka_flights --source flights_jetson --partitions 1 --conf /home/dbadmin/docker/flights.conf
vkconfig target --create --target-schema public --target-table flights --conf /home/dbadmin/docker/flights.conf
vkconfig load-spec --create --parser KafkaJSONParser --load-spec flights_load --conf /home/dbadmin/docker/flights.conf
vkconfig microbatch --create --microbatch flights --target-schema public --target-table flights \
           --add-source flights_jetson --add-source-cluster kafka_flights --load-spec flights_load \
           --conf /home/dbadmin/docker/flights.conf

  nohup vkconfig launch --conf /home/dbadmin/docker/flights.conf >/dev/null 2>&1 & 


  updated startup script : `/docker-entrypoint.sh`

  added vklconfig launch - everything else already setup. Starts automatically

