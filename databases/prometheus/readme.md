# Prometheus

## Docker


sudo docker run -p 9090:9090 prom/prometheus

remote volume
https://stackoverflow.com/questions/50009065/how-to-persist-data-in-prometheus-running-in-a-docker-container

/media/1f73286b-47a7-4ffa-9f0a-5c2a43c964a2/data/prometheus



toby@jet-gibbon:~$ sudo mkdir /media/1f73286b-47a7-4ffa-9f0a-5c2a43c964a2/data/prometheus
[sudo] password for toby: 
toby@jet-gibbon:~$ sudo chown 65534:65534  /media/1f73286b-47a7-4ffa-9f0a-5c2a43c964a2/data/prometheus

...
--volume a-new-volume:/prometheus \
...

sudo docker run -p 9090:9090 --volume /media/1f73286b-47a7-4ffa-9f0a-5c2a43c964a2/data/prometheus:/prometheus prom/prometheus


http://192.168.0.71:9090/

toby@jet-gibbon:~$ ls /media/1f73286b-47a7-4ffa-9f0a-5c2a43c964a2/data/prometheus/
chunks_head  lock  queries.active  wal

works

## MQTT

https://frederic-hemberger.de/notes/prometheus/monitoring-temperature-and-humidity-with-mqtt/
