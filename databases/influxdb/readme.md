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