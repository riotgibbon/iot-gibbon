# Kafka


kafka-python 
pandas

## wurstmeister
ssh admin@192.168.0.46
cd /share/CACHEDEV1_DATA/data/github/iot-gibbon/aviation/kafka/wurstmeister/

 docker-compose up -d


docker-compose down



## Confluent

cd /share/CACHEDEV1_DATA/data/confluent

curl --silent --output docker-compose.yml \ 
  https://raw.githubusercontent.com/confluentinc/cp-all-in-one/7.3.1-post/cp-all-in-one/docker-compose.yml
q

## KSQL

https://ksqldb.io/quickstart.html

ksql http://192.168.0.46:8088





create table flights
(
    icao       varchar,
    altitude   integer,
    lat        numeric,
    lon        numeric,
    updated    TIMESTAMPTZ,
    flightdate varchar,
    speed      integer,
    callsign   varchar
)


CREATE STREAM riderLocations (profileId VARCHAR, latitude DOUBLE, longitude DOUBLE)
  WITH (kafka_topic='locations', value_format='json', partitions=1);


CREATE STREAM flights ( icao       varchar,
    altitude   integer,
    lat        DOUBLE,
    lon        DOUBLE,
    updated    TIMESTAMP,
    flightdate varchar,
    speed      integer,
    callsign   varchar)
  WITH (kafka_topic='flights_jetson', value_format='json', partitions=1);


  CREATE TABLE currentLocation AS
  SELECT profileId,
         LATEST_BY_OFFSET(latitude) AS la,
         LATEST_BY_OFFSET(longitude) AS lo
  FROM riderlocations
  GROUP BY profileId
  EMIT CHANGES;


  CREATE TABLE currentLocation AS
  SELECT icao,
         LATEST_BY_OFFSET(lat) AS lat,
         LATEST_BY_OFFSET(lon) AS lon,
         LATEST_BY_OFFSET(altitude) AS altitude
  FROM flights
  GROUP BY icao
  EMIT CHANGES;


  CREATE TABLE ridersNearMountainView AS
  SELECT ROUND(GEO_DISTANCE(la, lo, 37.4133, -122.1162), -1) AS distanceInMiles,
         COLLECT_LIST(profileId) AS riders,
         COUNT(*) AS count
  FROM currentLocation
  GROUP BY ROUND(GEO_DISTANCE(la, lo, 37.4133, -122.1162), -1);


  CREATE TABLE flightsNearSlough AS
  SELECT ROUND(GEO_DISTANCE(lat, lon, 51.503572,-0.597207), -1) AS distanceInMiles,
         COLLECT_LIST(icao) AS planes,
         COUNT(*) AS count
  FROM currentLocation
  GROUP BY ROUND(GEO_DISTANCE(lat, lon, 51.503572,-0.597207), -1);



SELECT * FROM flights
  WHERE GEO_DISTANCE(lat,lon, 51.503572,-0.597207) <= 5 EMIT CHANGES;

  SELECT * FROM flights
  WHERE altitude > 500 EMIT CHANGES;


not getting results back

try stream not linked to a valid topic

  CREATE STREAM flights_test ( icao       varchar,
    altitude   integer,
    lat        DOUBLE,
    lon        DOUBLE,
    updated    TIMESTAMP,
    flightdate varchar,
    speed      integer,
    callsign   varchar)
  WITH (kafka_topic='flights_jetson_test', value_format='json', partitions=1);


 Message        
----------------
 Stream created 
----------------


can create to non existent stream

has created, so does exist



ksql> describe flights extended;

Name                 : FLIGHTS
Type                 : STREAM
Timestamp field      : Not set - using <ROWTIME>
Key format           : KAFKA
Value format         : JSON
Kafka topic          : flights_jetson (partitions: 1, replication: 1)
Statement            : CREATE STREAM FLIGHTS (ICAO STRING, ALTITUDE INTEGER, LAT DOUBLE, LON DOUBLE, UPDATED TIMESTAMP, FLIGHTDATE STRING, SPEED INTEGER, CALLSIGN STRING) WITH (KAFKA_TOPIC='flights_jetson', KEY_FORMAT='KAFKA', PARTITIONS=1, VALUE_FORMAT='JSON');

 Field      | Type            
------------------------------
 ICAO       | VARCHAR(STRING) 
 ALTITUDE   | INTEGER         
 LAT        | DOUBLE          
 LON        | DOUBLE          
 UPDATED    | TIMESTAMP       
 FLIGHTDATE | VARCHAR(STRING) 
 SPEED      | INTEGER         
 CALLSIGN   | VARCHAR(STRING) 
------------------------------

Sources that have a DROP constraint on this source
--------------------------------------------------
CURRENTLOCATION

Queries that read from this STREAM
-----------------------------------
CTAS_CURRENTLOCATION_3 (RUNNING) : CREATE TABLE CURRENTLOCATION WITH (KAFKA_TOPIC='CURRENTLOCATION', PARTITIONS=1, REPLICAS=1) AS SELECT   FLIGHTS.ICAO ICAO,   LATEST_BY_OFFSET(FLIGHTS.LAT) LAT,   LATEST_BY_OFFSET(FLIGHTS.LON) LON,   LATEST_BY_OFFSET(FLIGHTS.ALTITUDE) ALTITUDE FROM FLIGHTS FLIGHTS GROUP BY FLIGHTS.ICAO EMIT CHANGES;

For query topology and execution plan please run: EXPLAIN <QueryId>

Runtime statistics by host
-------------------------
 Host               | Metric                           | Value      | Last Message             
-----------------------------------------------------------------------------------------------
 ksqldb-server:8088 | consumer-failed-messages         |    6067010 | 2023-01-16T10:52:36.147Z 
 ksqldb-server:8088 | consumer-failed-messages-per-sec |         34 | 2023-01-16T10:52:36.147Z 
 ksqldb-server:8088 | consumer-messages-per-sec        |         34 | 2023-01-16T10:52:36.146Z 
 ksqldb-server:8088 | consumer-total-bytes             | 1029216374 | 2023-01-16T10:52:36.146Z 
 ksqldb-server:8088 | consumer-total-messages          |    6067010 | 2023-01-16T10:52:36.146Z 
-----------------------------------------------------------------------------------------------
(Statistics of the local KSQL server interaction with the Kafka topic flights_jetson)


messages failing

start small


CREATE STREAM flights ( icao       varchar,
    altitude   integer,
    lat        DOUBLE,
    lon        DOUBLE,
    updated    TIMESTAMP,
    flightdate varchar,
    speed      integer,
    callsign   varchar
    )
  WITH (kafka_topic='flights_jetson', value_format='json', partitions=1);



  CREATE STREAM flights ( icao       varchar
  )
  WITH (kafka_topic='flights_jetson', value_format='json', partitions=1);


  +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|ICAO                                                                                                                                                                                                     |
+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|4D00D8                                                                                                                                                                                                   |
|4CA707                                                                                                                                                                                                   |
|4CA8D8      


works

CREATE STREAM flights ( icao       varchar,
    altitude   integer,
    lat        DOUBLE,
    lon        DOUBLE,
    updated    TIMESTAMP,
    flightdate varchar,
    speed      integer,
    callsign   varchar
    )
  WITH (kafka_topic='flights_jetson', value_format='json', partitions=1);

select * from flights emit changes;


    CREATE STREAM flights ( icao       varchar,
    altitude   integer
    )
  WITH (kafka_topic='flights_jetson', value_format='json', partitions=1);


CREATE STREAM flights ( icao       varchar,
    altitude   integer,
    lat        DOUBLE,
    lon        DOUBLE
    )
  WITH (kafka_topic='flights_jetson', value_format='json', partitions=1);

  CREATE STREAM flights ( icao       varchar,
    altitude   integer,
    lat        DOUBLE,
    lon        DOUBLE,
    updated    TIMESTAMP
    )
  WITH (kafka_topic='flights_jetson', value_format='json', partitions=1);

  breaks on this



  2023-01-16 11:21:15.613000 +00:00

  easier to use RowTime for timestamp?

  https://docs.ksqldb.io/en/latest/reference/sql/time/



  CREATE STREAM flights ( icao       varchar,
    altitude   integer,
    lat        DOUBLE,
    lon        DOUBLE,
    updated    varchar,
    flightdate varchar,
    speed      integer,
    callsign   varchar
    )
  WITH (kafka_topic='flights_jetson', value_format='json', partitions=1);




  CREATE TABLE currentLocation AS
  SELECT icao,
         LATEST_BY_OFFSET(lat) AS lat,
         LATEST_BY_OFFSET(lon) AS lon,
         LATEST_BY_OFFSET(altitude) AS altitude
  FROM flights
  GROUP BY icao
  EMIT CHANGES;
 
  select * from currentLocation emit changes;


  CREATE TABLE flightsNearSlough AS
  SELECT ROUND(GEO_DISTANCE(lat, lon, 51.503572,-0.597207), -1) AS distanceInMiles,
         COLLECT_LIST(icao) AS planes,
         COUNT(*) AS count
  FROM currentLocation
  GROUP BY ROUND(GEO_DISTANCE(lat, lon, 51.503572,-0.597207), -1);


select * from flightsNearSlough emit changes;



  CREATE STREAM flights ( icao       varchar,
    altitude   integer,
    lat        DOUBLE,
    lon        DOUBLE,
    updated    varchar,
    flightdate varchar,
    gs      integer,
    callsign   varchar
    )
  WITH (kafka_topic='flights_jetson', value_format='json', partitions=1);



  CREATE TABLE currentLocation AS
  SELECT icao,
         LATEST_BY_OFFSET(lat) AS lat,
         LATEST_BY_OFFSET(lon) AS lon,
         LATEST_BY_OFFSET(altitude) AS altitude,
          LATEST_BY_OFFSET(gs) AS speed,
           LATEST_BY_OFFSET(callsign) AS callsign
  FROM flights
  GROUP BY icao
  EMIT CHANGES;
 
   CREATE TABLE flightsNearSlough AS
  SELECT ROUND(GEO_DISTANCE(lat, lon, 51.503572,-0.597207), -1) AS distanceInMiles,
    
         COLLECT_LIST(icao) AS planes,
         COUNT(*) AS count
  FROM currentLocation
  GROUP BY ROUND(GEO_DISTANCE(lat, lon, 51.503572,-0.597207), -1);

  select * from flightsNearSlough where distanceInMiles <20 and altitude <5000