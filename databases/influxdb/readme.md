# Influx

## Installation 
https://pimylifeup.com/raspberry-pi-influxdb/

sudo apt update
sudo apt upgrade

wget -qO- https://repos.influxdata.com/influxdb.key | sudo apt-key add -

wget -qO- https://repos.influxdata.com/influxdb.key | sudo apt-key add -echo "deb https://repos.influxdata.com/debian buster stable" | sudo tee /etc/apt/sources.list.d/influxdb.list

sudo apt update


sudo apt install influxdb
sudo apt install influxdb-client


sudo systemctl unmask influxdb
sudo systemctl enable influxdb

sudo systemctl start influxdb
systemctl status influxdb

journalctl -u influxdb

## external hdd config

influxdb.conf-file:

https://community.home-assistant.io/t/influxdb-data-on-usb-disk/48177

```
  dir = "/mnt/d5899f34-4cea-448a-88a1-e889e262208d/influxdb/meta"
  dir = "/mnt/d5899f34-4cea-448a-88a1-e889e262208d/influxdb/data"
  wal-dir = "/mnt/d5899f34-4cea-448a-88a1-e889e262208d/influxdb/wal"
```

config location

/etc/influxdb/influxdb.conf

external dir:
/media/pi/201804


/media/pi/201804/pi/influxdb

[meta]
  # Where the metadata/raft database is stored
  # dir = "/var/lib/influxdb/meta"
  dir = "/media/pi/201804/pi/influxdb/meta"

[data]
  # The directory where the TSM storage engine stores TSM files.
  #  dir = "/var/lib/influxdb/data"
  dir = "/media/pi/201804/pi/influxdb/data"


  # The directory where the TSM storage engine stores WAL files.
  # wal-dir = "/var/lib/influxdb/wal"
  wal-dir = "/media/pi/201804/pi/influxdb/wal"


sudo chown -R influxdb:influxdb /media/pi/201804/pi/influxd

not workingls 

switched back to original and run

try stopping, copying the existing files to the HDD , and swapping the dirs - see what happens

new hard drive
`/dev/sda3       338G  2.5M  338G   1% /media/pi/PIBACK`

```
mkdir /media/pi/PIBACK/influxdb/
sudo cp -r /var/lib/influxdb/* /media/pi/PIBACK/influxdb/
```

stop, update config and restart ...

stop
`sudo systemctl stop influxdb`

config

sudo nano /etc/influxdb/influxdb.conf

[meta]
  # Where the metadata/raft database is stored
  # dir = "/media/pi/PIBACK//meta"
  dir = "/media/pi/PIBACK/influxdb/meta"

[data]
  # The directory where the TSM storage engine stores TSM files.
  #  dir = "/var/lib/influxdb/data"
  dir = "/media/pi/PIBACK/influxdb/data"


  # The directory where the TSM storage engine stores WAL files.
  # wal-dir = "/var/lib/influxdb/wal"
  wal-dir = "/media/pi/PIBACK/influxdb/wal"



try switching ownership of location

sudo chown -R influxdb:influxdb /media/pi/PIBACK/influxdb/

not having it


try reformatting

/dev/sda3 

https://raspberrytips.com/format-mount-usb-drive/#Format_or_reformat_the_entire_disk

sudo fdisk /dev/sda3 

sudo mkfs.ext4 /dev/sda2

new folder
/media/pi/67fea718-da7c-4d04-bf6e-0a67249ab3e1

`sudo mkdir /media/pi/67fea718-da7c-4d04-bf6e-0a67249ab3e1/influxdb`

change ownership

sudo chown -R influxdb:influxdb /media/pi/67fea718-da7c-4d04-bf6e-0a67249ab3e1/influxdb

sudo cp -r /var/lib/influxdb/* /media/pi/67fea718-da7c-4d04-bf6e-0a67249ab3e1/influxdb/

update config

sudo nano /etc/influxdb/influxdb.conf


[meta]
  # Where the metadata/raft database is stored
  # dir = "/media/pi/PIBACK//meta"
  dir = "/media/pi/67fea718-da7c-4d04-bf6e-0a67249ab3e1/influxdb/meta"

[data]
  # The directory where the TSM storage engine stores TSM files.
  #  dir = "/var/lib/influxdb/data"
  dir = "/media/pi/67fea718-da7c-4d04-bf6e-0a67249ab3e1/influxdb/data"


  # The directory where the TSM storage engine stores WAL files.
  # wal-dir = "/var/lib/influxdb/wal"
  wal-dir = "/media/pi/67fea718-da7c-4d04-bf6e-0a67249ab3e1/influxdb/wal"


restart

sudo systemctl start influxdb
systemctl status influxdb

journalctl -u influxdb


## Starting


https://nwmichl.net/2020/07/14/telegraf-influxdb-grafana-on-raspberrypi-from-scratch/



journalctl -u telegraf



[global_tags]
[agent]
  interval = "10s"
  round_interval = true
  metric_batch_size = 1000
  metric_buffer_limit = 10000
  collection_jitter = "0s"
  flush_interval = "10s"
  flush_jitter = "0s"
  precision = ""
  debug = false
  quiet = false
  logfile = ""
  hostname = "telegraf"
  omit_hostname = false
[[outputs.influxdb]]
urls = ["http://localhost:8086"]
database = "home"
[[outputs.file]]
files = ["stdout"]
data_format = "json"
[[inputs.mqtt_consumer]]
servers = ["tcp://localhost:1883"]
topics = [
  "home/tele/#"
]



## Docker
https://ac20202021.medium.com/how-to-setup-influxdb-and-grafana-on-qnap-nas-a70a3ae91ae5

https://www.reddit.com/r/qnap/comments/ictt1g/qnap_influxdb_container/

docker run -it \
  -p 8086:8086 \
  -p 4242:4242 \
  --restart always \
  --name=influxdb \
  -v /share/CE_CACHEDEV1_DATA/Container/influxdb/config:/etc/influxdb \
  -v /share/CE_CACHEDEV1_DATA/Container/influxdb/db:/var/lib/influxdb \
  influxdb



[~] # ls /share/CACHEDEV1_DATA/data/influx/
config/  data/

data on /share/CACHEDEV1_DATA/data/influx/data/





sudo useradd -rs /bin/false influxdb
 sudo mkdir -p /etc/influxdb

build config file
  docker run --rm influxdb influxd config | sudo tee  /etc/influxdb/influxdb.conf &gt; /dev/null


cat /etc/influxdb/influxdb.conf

[data]
  dir = "/var/lib/influxdb/data"

update to 
/share/CACHEDEV1_DATA/data/influx/data

sudo chown influxdb:influxdb /var/lib/influxdb
sudo chown influxdb:influxdb /share/CACHEDEV1_DATA/data/influx/data

https://stackoverflow.com/questions/56596533/influxdb-move-only-one-database-of-many-from-one-server-instance-to-another


docker run -it \
  -p 8086:8086 \
  -p 4242:4242 \
  --restart always \
  --name=influxdb \
  -v /share/CE_CACHEDEV1_DATA/Container/influxdb/config:/etc/influxdb \
  -v /share/CE_CACHEDEV1_DATA/Container/influxdb/db:/var/lib/influxdb \
  influxdb

docker run -it \
  -p 8086:8086 \
  -p 4242:4242 \
  --restart always \
  --name=influxdb \
  -v /share/CACHEDEV1_DATA/data/influx/config:/etc/influxdb \
  -v /share/CACHEDEV1_DATA/data/influx/data:/var/lib/influxdb \
  influxdb




  creaa aa

backup
https://stackoverflow.com/questions/56596533/influxdb-move-only-one-database-of-many-from-one-server-instance-to-another


  influxd backup \
   -portable \
   -host 192.168.11.10:8088 \
   -database sensors \
   /var/lib/influxdb/export-sensors-01


 influxd backup \
   -portable \
   -host 192.168.0.63:8088 \
   -database home \
   /var/lib/influxdb/export-home-01


2021/01/31 13:23:09 backing up metastore to /var/lib/influxdb/export-home-01/meta.00
2021/01/31 13:23:09 Invalid metadata blob, ensure the metadata service is running (default port 8088)

on pi
  sudo nano /etc/influxdb/influxdb.conf
    bind-address = "127.0.0.1:8088"
  sudo service influxdb restart


2021/01/31 13:26:36 backing up metastore to /var/lib/influxdb/export-home-01/meta.00
2021/01/31 13:26:36 Download shard 0 failed dial tcp 192.168.0.63:8088: connect: connection refused.  Waiting 2s and retrying (0)...


https://stackoverflow.com/questions/42517853/force-influxdb-to-listen-on-localhost-only

bind-address = ":8088"



influxd restore \
   -portable \
   /var/lib/influxdb/export-home-01

  
  mapping config and folders


  docker run -d -p 8086:8086 --user 997:997 --name=influxdb \ 
-v /etc/influxdb/influxdb.conf:/etc/influxdb/influxdb.conf \ 
-v /var/lib/influxdb:/var/lib/influxdb \ 
influxdb \ 
-config /etc/influxdb/influxdb.conf


logging into container

 docker exec -it &lt;container_id&gt; /bin/bash

 b3b633253685        influxdb                          "/entrypoint.sh infl…"   19 hours ago        Up 17 minutes       0.0.0.0:4242->4242/tcp, 0.0.0.0:8086->8086/tcp   influxdb

 docker exec -it 'b3b633253685' /bin/bash


 not getting a config file, not obvious where data is

 delete old container

create new config

model
docker run --rm influxdb influxd config | sudo tee /etc/influxdb/influxdb.conf &gt; /dev/null

/share/CACHEDEV1_DATA/data/influx/config


  docker run --rm influxdb influxd config | sudo tee /share/CACHEDEV1_DATA/data/influx/config/influxdb.conf > /dev/null

nano /share/CACHEDEV1_DATA/data/influx/config/influxdb.conf 

check user
[~] # cat /etc/passwd | grep influxdb
influxdb:x:999:99::/share/homes/influxdb:/bin/false


docker run -d -p 8086:8086 --user 999:99 --name=influxdb \
-v /share/CACHEDEV1_DATA/data/influx/config/influxdb.conf:/etc/influxdb/influxdb.conf \
-v /share/CACHEDEV1_DATA/data/influx/data:/var/lib/influxdb \
influxdb \
-config /etc/influxdb/influxdb.conf


login and restore

d91e93b2ac7b 
 docker exec -it 'd91e93b2ac7b' /bin/bash

backed up data
/var/lib/influxdb/backups/

influxd restore \
   -portable \
   /var/lib/influxdb/backups/export-home-01/

  
2b18a87472e7
 docker exec -it '2b18a87472e7' /bin/bash


sudo chown influxdb:influxdb /share/CACHEDEV1_DATA/data/influx/data/backups/export-home-01/*

 influxd restore \
   -portable \
   /var/lib/influxdb/backups/export-home-01/



works
### Kapacitor

b
https://thenewstack.io/how-to-setup-influxdb-telegraf-and-grafana-on-docker-part-2/

add kapacitor

sudo useradd -rs /bin/false telegraf

/share/CACHEDEV1_DATA/data/influx/config/influxdb.conf

/share/CACHEDEV1_DATA/data/kapacitor/conf/

docker run --rm telegraf telegraf config | sudo tee /share/CACHEDEV1_DATA/data/kapacitor/conf/telegraf.conf > /dev/null

docker run --rm kapacitor kapacitord config | sudo tee /share/CACHEDEV1_DATA/data/kapacitor/conf/kapacitor.conf > /dev/null

sudo chown telegraf:telegraf /share/CACHEDEV1_DATA/data/kapacitor/conf/*

[[mqtt]]
  enabled = true
  name = "localhost"
  default = true
  url = "tcp://localhost:1883"


/share/CACHEDEV1_DATA/data/kapacitor/conf/kapacitor.conf

sudo useradd -rs /bin/false kapacitor

cat /etc/passwd | grep kapacitor
kapacitor:x:997:97::/share/homes/kapacitor:/bin/false


ps -ef | grep "kapacitor"


model
docker run -d -p 8086:8086 --user 999:99 --name=influxdb \
-v /share/CACHEDEV1_DATA/data/influx/config/influxdb.conf:/etc/influxdb/influxdb.conf \
-v /share/CACHEDEV1_DATA/data/influx/data:/var/lib/influxdb \
influxdb \
-config /etc/influxdb/influxdb.conf


docker run -d --user 999:99 --name=kapacitor \
-v /share/CACHEDEV1_DATA/data/kapacitor/conf/kapacitor.conf:/etc/kapacitor/kapacitor.conf \
kapacitor \
-config /etc/kapacitor/kapacitor.conf


does need to be Telegraf

[[outputs.influxdb]]
urls = ["http://localhost:8086"]
database = "home"
[[outputs.file]]
files = ["stdout"]
data_format = "json"
[[inputs.mqtt_consumer]]
servers = ["tcp://localhost:1883"]
topics = [
  "home/tele/#"
]


[[outputs.influxdb]]
urls = ["http://192.168.0.46:8086"]
database = "home"
[[outputs.file]]
files = ["stdout"]
data_format = "json"
[[inputs.mqtt_consumer]]
servers = ["tcp://192.168.0.46:1883"]
topics = [
  "home/tele/#"
]


cat /etc/passwd | grep telegraf

telegraf:x:998:98::/share/homes/telegraf:/bin/false
rm


docker run --rm telegraf telegraf config | sudo tee /share/CACHEDEV1_DATA/data/telegraf/conf/telegraf.conf > /dev/null

sudo chown telegraf:telegraf /share/CACHEDEV1_DATA/data/telegraf/conf/*


docker run -d  --user 998:98 --name=telegraf \
-v /share/CACHEDEV1_DATA/data/telegraf/conf/telegraf.conf:/etc/telegraf/telegraf.conf \
telegraf \
-config /etc/telegraf/telegraf.conf


2021-01-31T18:13:45Z E! [inputs.mqtt_consumer] Error in plugin: metric parse error: expected tag at 1:7: "1.4046"                           
2021-01-31T18:13:45Z E! [inputs.mqtt_consumer] Error in plugin: metric parse error: expected tag at 1:6: "80.38"                            
2021-01-31T18:13:45Z E! [inputs.mqtt_consumer] Error in plugin: metric parse error: expected tag at 1:6: "75.32" 


https://docs.influxdata.com/telegraf/v1.11/data_formats/input/value/

 data_type = "integer"

  data_type = "float"


works


restore again


 influxd backup \
   -portable \
   -host 192.168.0.63:8088 \
   -database home \
   /var/lib/influxdb/backups/export-home-03/


 influxd restore \
   -portable \
   /var/lib/influxdb/backups/export-home-03/


works





--network="host"

docker run -d --restart=always --net=container: --user 998:98 --name=telegraf \
-v /share/CACHEDEV1_DATA/data/telegraf/conf/:/etc/telegraf/ \
telegraf \
-config /etc/telegraf/telegraf.conf


sudo chown influxdb:influxdb /share/CACHEDEV1_DATA/data/influx/*

docker run --restart=always  -d -p 8086:8086  --user 999:99 --name=influxdb \
-v /share/CACHEDEV1_DATA/data/influx/config/influxdb.conf:/etc/influxdb/influxdb.conf \
-v /share/CACHEDEV1_DATA/data/influx/data:/var/lib/influxdb \
influxdb \
-config /etc/influxdb/influxdb.conf



### issues with influx permissions
restart influx from scratch

cat /etc/passwd | grep influxdb
influxdb:x:999:99::/share/homes/influxdb:/bin/false

sudo chown influxdb:influxdb /share/CACHEDEV1_DATA/data/influx/*

docker run   -d -p 8086:8086  --user 999:99 --name=influxdb \
-v /share/CACHEDEV1_DATA/data/influx/config/influxdb.conf:/etc/influxdb/influxdb.conf \
-v /share/CACHEDEV1_DATA/data/influx/data:/var/lib/influxdb \
influxdb \
-config /etc/influxdb/influxdb.conf


 influxd backup \
   -portable \
   -host 192.168.0.63:8088 \
   -database home \
   /var/lib/influxdb/backups/export-home-04/

 influxd restore \
   -portable \
   /var/lib/influxdb/backups/export-home-04/

