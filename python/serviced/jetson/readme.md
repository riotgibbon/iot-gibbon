# SystemD

https://learn.sparkfun.com/tutorials/how-to-run-a-raspberry-pi-program-on-startup/all#method-3-systemd

systemctl enable systemd-networkd.service systemd-networkd-wait-online.service

 sudo cp /home/toby/github/iot-gibbon/python/serviced/jetson/*.service /lib/systemd/system/

 sudo cp /home/toby/github/iot-gibbon/python/serviced/jetson/dump1090.service /lib/systemd/system/
sudo cp /home/toby/github/iot-gibbon/python/serviced/jetson/push1090Kafka.service /lib/systemd/system/


sudo systemctl disable machinebeat.service
sudo systemctl disable kibana.service

sudo systemctl daemon-reload

sudo systemctl enable machinebeat.service
sudo systemctl enable kibana.service
sudo systemctl enable dump1090.service
sudo systemctl enable push1090Kafka.service

sudo systemctl daemon-reload

restart

sudo systemctl restart machinebeat.service
sudo systemctl restart kibana.service
sudo systemctl restart dump1090.service
sudo systemctl restart push1090Kafka.service

sudo systemctl stop dump1090.service

status
sudo systemctl status machinebeat.service
sudo systemctl status kibana.service
sudo systemctl status dump1090.service
sudo systemctl status push1090Kafka.service