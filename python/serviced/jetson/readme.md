# SystemD

https://learn.sparkfun.com/tutorials/how-to-run-a-raspberry-pi-program-on-startup/all#method-3-systemd

systemctl enable systemd-networkd.service systemd-networkd-wait-online.service

 sudo cp /home/toby/github/iot-gibbon/python/serviced/jetson/*.service /lib/systemd/system/


sudo systemctl disable machinebeat.service
sudo systemctl disable kibana.service

sudo systemctl daemon-reload

sudo systemctl enable machinebeat.service
sudo systemctl enable kibana.service

sudo systemctl daemon-reload

restart

sudo systemctl restart machinebeat.service
sudo systemctl restart kibana.service


status
sudo systemctl status machinebeat.service
sudo systemctl status kibana.service
