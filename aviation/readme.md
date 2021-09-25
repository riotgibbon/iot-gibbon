# Aviation tracking



working 1090 for Jetson - 192.168.0.53
https://github.com/joergsteinkamp/dump1090

./dump1090 --net

./dump1090 --net --net-beast

http://192.168.0.53:8080/




config 
https://uk.flightaware.com/adsb/piaware/advanced_configuration

piaware-config
piaware-config receiver-host 192.168.0.53
piaware-config receiver-port 30005
piaware-config receiver-type relay

nc 192.168.0.53 30003
sudo systemctl restart piaware.service


tail /var/log/piaware.log

sudo journalctl --no-pager -u piaware
