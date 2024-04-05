-- https://www.vertica.com/docs/10.0.x/HTML/Content/Authoring/KafkaIntegrationGuide/ManuallyCopyingDataExample.htm
CREATE RESOURCE POOL flights_pool MEMORYSIZE '10%' PLANNEDCONCURRENCY 1 QUEUETIMEOUT 0;

DROP TABLE  flights;
CREATE FLEX TABLE flights();

 COPY flights SOURCE KafkaSource(stream='flights_jetson|0|-2',
                                    brokers='192.168.0.46:9092',
                                    duration=interval '10000 milliseconds')
                 PARSER KafkaJSONParser()
                 REJECTED DATA AS TABLE public.flights_rejections;

-- https://www.vertica.com/docs/10.0.x/HTML/Content/Authoring/KafkaIntegrationGuide/ManuallyCopyingDataExample.htm
-- CREATE RESOURCE POOL flights_pool MEMORYSIZE '10%' PLANNEDCONCURRENCY 1 QUEUETIMEOUT 0;

-- run on Vertica server
-- add to path
-- export PATH=/opt/vertica/packages/kafka/bin:$PATH

-- vkconfig scheduler --create --config-schema docker --operator dbadmin  --conf /home/dbadmin/docker/flights.conf
-- vkconfig cluster --create --cluster kafka_flights --hosts 192.168.0.46:9092 --conf /home/dbadmin/docker/flights.conf
-- vkconfig source --create --cluster kafka_flights --source flights_jetson --partitions 1 --conf /home/dbadmin/docker/flights.conf
-- vkconfig target --create --target-schema public --target-table flights --conf /home/dbadmin/docker/flights.conf
-- vkconfig load-spec --create --parser KafkaJSONParser --load-spec flights_load --conf /home/dbadmin/docker/flights.conf
-- vkconfig microbatch --create --microbatch flights --target-schema public --target-table flights \
--            --add-source flights_jetson --add-source-cluster kafka_flights --load-spec flights_load \
--            --conf /home/dbadmin/docker/flights.conf

--   nohup vkconfig launch --conf /home/dbadmin/docker/flights.conf >/dev/null 2>&1 & 

-- vkconfig scheduler --update  --frame-duration '00:01:00' --conf /home/dbadmin/docker/flights.conf
-- vkconfig scheduler --update  --frame-duration '00:00:10' --conf /home/dbadmin/docker/flights.conf

--   updated startup script : `/docker-entrypoint.sh`

--   added vklconfig launch - everything else already setup. Starts automatically




DROP TABLE  flights;
CREATE FLEX TABLE flights();


--manual copy
 COPY flights SOURCE KafkaSource(stream='flights_jetson|0|-2',
                                    brokers='192.168.0.46:9092',
                                    duration=interval '10000 milliseconds')
                 PARSER KafkaJSONParser()
                 REJECTED DATA AS TABLE public.flights_rejections;



--calcute flex fields
SELECT compute_flextable_keys('flights');
select * from flights_keys  ;
--materialise flex fields fpr performance
select materialize_flextable_columns('flights', 9);
SELECT * FROM v_catalog.materialize_flextable_columns_results WHERE table_schema = 'public' and table_name = 'flights';


select count(*) from flights;


select icao, callsign, GS, altitude, lat, lon, updated
from flights
where icao ='896457'
order by updated desc

;



select flightDate, direction, count(*) from (
                                                select icao,
                                                       callsign,
                                                       flightDate,
                                                       direction,
                                                       case when direction = 'takeoff' then min(updated) else max(updated) end as directionTime,
                                                       min(altitude::INT)                                                      as minAltitude
                                                from (
                                                         select *,
                                                                case when firstAltitude > lastAltitude then 'landing' else 'takeoff' END as direction

                                                         from (
                                                                  select icao,
                                                                         callsign,
                                                                         lat,
                                                                         lon,
                                                                         updated,
                                                                         altitude,
                                                                         coalesce(flightDate::DATE,  date(updated::TIMESTAMP)    )                                                                as flightDate,
--                                                                          flightDate::DATE,
                                                                         FIRST_VALUE(altitude::INT)
                                                                         OVER (PARTITION BY icao,callsign,date(updated::TIMESTAMP) ORDER BY updated::TIMESTAMP)      as firstAltitude,
                                                                         FIRST_VALUE(altitude::INT)
                                                                         OVER (PARTITION BY icao,callsign,date(updated::TIMESTAMP) ORDER BY updated::TIMESTAMP DESC) as lastAltitude
                                                                  from flights
                                                                  WHERE lat between 51.4 and 51.5
                                                                    and lon between -0.58 and -0.09
                                                                    and callsign is not null
                                                                    and altitude::INT between 0 and 8000
--                                                                     and icao ='407462'
                                                              ) as f
--                                                         WHere flightDate = CURRENT_DATE
                                                     ) as f2
                                                GROUP BY 1, 2, 3, 4
                                                order by directionTime DESC
                                            )f3
group by 1,2;




SELECT compute_flextable_keys('flights');

select * from flights_keys;
select count(*) from flights;


select icao, callsign, GS, altitude, lat, lon, updated
from flights
where icao ='4D21DC'
order by updated desc

;




CREATE VIEW heathrowMovements as
       SELECT icao,
           callsign,
           flightDate,
           direction,
           CASE WHEN direction = 'takeoff' THEN min(updated) ELSE max(updated) end as directionTime,
           min(altitude::INT) as minAltitude,
           max(altitude::INT) as maxAltitude,
           max(altitude::INT)  -min(altitude::INT) as diffAltitude
    FROM (
             SELECT *, CASE WHEN firstAltitude > lastAltitude THEN 'landing' ELSE 'takeoff' END AS direction
             FROM (
                      SELECT icao,
                             callsign,
                             lat,
                             lon,
                             updated,
                             altitude,
                             date(updated::TIMESTAMP) as flightDate,
                             FIRST_VALUE(altitude::INT)
                             OVER (PARTITION BY icao,callsign,date(updated::TIMESTAMP) ORDER BY updated::TIMESTAMP) as firstAltitude,
                             FIRST_VALUE(altitude::INT)
                             OVER (PARTITION BY icao,callsign,date(updated::TIMESTAMP) ORDER BY updated::TIMESTAMP DESC) as lastAltitude
                      FROM flights
                      WHERE (lat between 51.4 and 51.5
                        AND lon between -0.58 and -0.09)
                        AND callsign is not null
                        AND altitude::INT between 0 and 8000
                  ) as f
         ) as f2
    GROUP BY 1, 2, 3, 4
    HAVING max(altitude::INT)  -min(altitude::INT)  >0;

SELECT *
from heathrowMovements
WHERE flightDate= CURRENT_DATE
-- and icao ='A91B44'
ORDER BY directionTime DESC limit 50;


SELECT flightDate, direction, count(*) flights
FROM heathrowMovements
GROUP BY 1,2
ORDER BY 1 DESC ,2
;

SELECT flightDate,hour(directionTime), direction, count(*) flights
FROM heathrowMovements
WHERE flightDate in (CURRENT_DATE, CURRENT_DATE - INTERVAL '1 day')
GROUP BY 1,2,3
ORDER BY 1,2 DESC ,3
;

