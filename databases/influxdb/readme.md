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