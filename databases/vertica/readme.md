# Vertica

https://hub.docker.com/r/dataplatform/docker-vertica/



docker run -p 5433:5433 -d -v /data/vertica/vertica_data:/home/dbadmin/docker dataplatform/docker-vertica

/share/CACHEDEV1_DATA/data/influx/

/share/CACHEDEV1_DATA/data/vertica/vertica_data

docker run -p 5433:5433 -d -v /share/CACHEDEV1_DATA/data/vertica/vertica_data:/home/dbadmin/docker dataplatform/docker-vertica

ssh admin@192.168.0.46


5433

192.168.0.46

Default DB Name - docker

Default User - dbadmin

Default Password (NO PASSWORD) -