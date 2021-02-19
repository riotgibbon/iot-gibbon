# MQTT

## Install

sudo apt install -y mosquitto mosquitto-clients


## Forwarding with brokers

broker config

/etc/mosquitto/mosquitto.conf 

nano /etc/mosquitto/mosquitto.conf 

pi to qnap

```
connection external-bridge
address 192.168.0.46
topic # out
```


qnap to pi

running in Docker

/usr/sbin/mosquitto -c /mosquitto/config/mosquitto.conf


/share/CACHEDEV1_DATA/data/mosquitto/config/mosquitto.conf


vi /mosquitto/config/mosquitto.conf

listener 8883 192.168.0.46

connection external-pi
address 192.168.0.63
topic # out

# qnap to jetson

connection external-jetson
address 192.168.0.71
topic # out
