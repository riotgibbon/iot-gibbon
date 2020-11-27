# SystemD

https://learn.sparkfun.com/tutorials/how-to-run-a-raspberry-pi-program-on-startup/all#method-3-systemd

systemctl enable systemd-networkd.service systemd-networkd-wait-online.service

 sudo cp /home/pi/github/iot-gibbon/python/serviced/jetson/*.service /lib/systemd/system/


sudo systemctl disable machinebeat.service

sudo systemctl daemon-reload

sudo systemctl machinebeat ifttt.service


restart

sudo systemctl restart machinebeat.service


status
sudo systemctl status machinebeat.service
