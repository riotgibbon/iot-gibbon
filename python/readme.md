# Raspberry Pi Garden

Projects and experiments




get most recent values

select last(value) from mqtt_consumer group by *;


get recent averages:

model
SELECT MEAN("longest-wait-time”) FROM “web” WHERE (“service” =~ /web1|app1/) AND $timeFilter GROUP BY time(1h), “service” fill(null)

SELECT MEAN(value) FROM mqtt_consumer   WHERE time > now() - 10s group by * 