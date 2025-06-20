# kafka tools

This is a demonstration of Spark reading Kafka streams, transformin the input and writing to MongoDB. 

Pre-requisites
* PySpark - `pip install pyspark`
* Docker/Rancher for containers

in `docker-compose.yml` set the following environment variable to the local IP address, eg:

```yaml
KAFKA_ADVERTISED_HOST_NAME: 192.168.64.1
```
finding the local IP address on MacOS:
ipconfig getifaddr en0



Run local kafka/Mongo containers:

`docker compose up -d`

In `dexter/kafka/datagen/.env`, also set the local IP address:

`export KAFKA_BROKERS=192.168.64.1:9092`

Run the data simulator (remove `--dry-run` to actually send data):

```bash

docker run \
  --rm -it \
  -v ${PWD}/datagen/.env:/app/.env \
  -v ${PWD}/datagen/tests/flights.json:/app/blah.json \
      materialize/datagen -s blah.json -n 1000 -w 500 --dry-run
```

Launch the Kafka/Spark/MongoDb environment"



run the Spark processor:

container spark:
`docker exec -it kafka-spark-1    spark-submit  --packages org.apache.spark:spark-sql-kafka-0-10_2.12:3.5.5,org.mongodb.spark:mongo-spark-connector_2.12:10.4.1 /scripts/writeToMongodb.py`


Streamlit visualisation

cd streamlit

if running for the first time, create environment:
```
virtualenv .venv
pip install -r setup/requirements.txt
```

activate environment:
```
. .venv/bin/activate
```

run app:
```
streamlit run app.py
```


## KPI


`docker exec -it kafka-spark-1    spark-submit  --packages org.mongodb.spark:mongo-spark-connector_2.12:10.4.1 /scripts/readRoutes.py mongodb`

`spark-submit  --packages org.mongodb.spark:mongo-spark-connector_2.12:10.4.1 ./scripts/readRoutes.py 127.0.0.1`



## Faker

https://github.com/MaterializeInc/datagen

Generates test data for Kafka topics.

get docker image

```bash
docker pull materialize/datagen
```
generate data

```bash         

docker run \
  --rm -it \
  -v ${PWD}/.env:/app/.env \
  -v ${PWD}/tests/schema.json:/app/blah.json \
      materialize/datagen -s blah.json -n 1 --dry-run
```



docker run \
  --rm -it \
  -v ${PWD}/datagen/.env:/app/.env \
  -v ${PWD}/datagen/tests/ecommerce.json:/app/blah.json \
      materialize/datagen -s blah.json -n 1 --dry-run


docker run \
  --rm -it \
  -v ${PWD}/datagen/.env:/app/.env \
  -v ${PWD}/datagen/tests/flights.json:/app/blah.json \
      materialize/datagen -s blah.json -n 1000 -w 500 --dry-run







## troubleshooting



sudo launchctl unload /Library/LaunchDaemons/com.cisco.secureclient.vpnagentd.plist 
sudo launchctl load /Library/LaunchDaemons/com.cisco.secureclient.vpnagentd.plist 



docker inspect kafka-mongodb-1 | grep IPAddress