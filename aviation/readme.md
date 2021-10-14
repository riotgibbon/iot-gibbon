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