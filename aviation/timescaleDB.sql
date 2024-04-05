
drop table flights;
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
);

SELECT create_hypertable('flights','updated',  migrate_data => true );
CREATE INDEX ix_day_time ON flights (flightdate, updated DESC);
DROP INDEX ix_day_time ;

select count(*) from flights;
select flightdate,count(*) from flights group by 1 order by 1;


-- 51.503572,-0.597207


    select * from (
                      select icao,
                             altitude,
                             speed,
                             distance,
                             updated,
                             ROW_NUMBER() OVER (
                                 PARTITION BY icao
                                 ORDER BY updated desc) reading


                      from (
                               select st_distance(ST_MakePoint(lon, lat) ::geography,
                                                  ST_MakePoint(-0.597207, 51.503572) ::geography) distance,
                                      *
                               from flights
                               where altitude>0 and lat>0
--                                 and flightdate = CURRENT_DATE::TEXT
                                and updated > (CURRENT_TIMESTAMP - interval '1 minute')
                           ) f
                  )r
    where reading=1
    order by distance
    limit 1;

;



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
                        and updated > (CURRENT_TIMESTAMP - interval '1 day')
                  ) as f
         ) as f2
    GROUP BY 1, 2, 3, 4
    HAVING max(altitude::INT)  -min(altitude::INT)  >0
    ORDER BY directiontime desc;



