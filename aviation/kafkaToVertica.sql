-- https://www.vertica.com/docs/10.0.x/HTML/Content/Authoring/KafkaIntegrationGuide/ManuallyCopyingDataExample.htm
CREATE RESOURCE POOL flights_pool MEMORYSIZE '10%' PLANNEDCONCURRENCY 1 QUEUETIMEOUT 0;

DROP TABLE  flights;
CREATE FLEX TABLE flights();
 COPY flights SOURCE KafkaSource(stream='flights_jetson|0|-2',
                                    brokers='192.168.0.46:9092',
                                    duration=interval '10000 milliseconds')
                 PARSER KafkaJSONParser()
                 REJECTED DATA AS TABLE public.flights_rejections;

SELECT compute_flextable_keys('flights');
select * from flights_keys;
select icao, callsign, GS, altitude, lat, lon, updated
from flights
where icao ='406A9F'
order by updated desc

;

-- 4C01E0 landing
-- 4076CD takeoff

select count(*) from flights;

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
                                                                         date(updated::TIMESTAMP)                                                                    as flightDate,
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
                                                     ) as f2
                                                GROUP BY 1, 2, 3, 4
                                                order by directionTime DESC
                                            )f3
group by 1,2;

select * from (
                  select 51.45759 as lat, -0.10284 as lon
              ) source
 WHERE lat between 51.4 and 51.5
and lon between -0.58 and -0.09
;


-- NW  51.479900,-0.518647
-- SE 51.457754,-0.374273

select date_part('DAY', updated::TIMESTAMP) as day, date_part('HOUR', updated::TIMESTAMP) as hour,
       count (distinct callsign) as flights, count(*) as messages
from flights
WHERE
      lat between 51.457754 and 51.479900
and lon between  -0.518647 and -0.374273
GROUP BY 1,2
 order by 1,2




