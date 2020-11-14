# SystemD

https://learn.sparkfun.com/tutorials/how-to-run-a-raspberry-pi-program-on-startup/all#method-3-systemd

systemctl enable systemd-networkd.service systemd-networkd-wait-online.service

 sudo cp *.service /lib/systemd/system/


sudo systemctl disable ifttt.service
sudo systemctl disable  pi_home.service
sudo systemctl disable  bme680_mqtt.service
sudo systemctl disable  weather.service
sudo systemctl disable apds9960_mqtt.service
sudo systemctl disable gp2y0a21yk0f_mqtt.service
sudo systemctl daemon-reload

sudo systemctl enable ifttt.service
sudo systemctl enable pi_home.service
sudo systemctl enable bme680_mqtt.service
sudo systemctl enable weather.service
sudo systemctl enable apds9960_mqtt.service
sudo systemctl enable gp2y0a21yk0f_mqtt.service
