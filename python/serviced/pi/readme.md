# SystemD

https://learn.sparkfun.com/tutorials/how-to-run-a-raspberry-pi-program-on-startup/all#method-3-systemd

systemctl enable systemd-networkd.service systemd-networkd-wait-online.service

 sudo cp /home/pi/github/iot-gibbon/python/serviced/pi/*.service /lib/systemd/system/


sudo systemctl disable ifttt.service
sudo systemctl disable  pi_home.service
sudo systemctl disable  bme680_mqtt.service
sudo systemctl disable  weather.service
sudo systemctl disable apds9960_mqtt.service
sudo systemctl disable gp2y0a21yk0f_mqtt.service
sudo systemctl disable hue.service
sudo systemctl disable pmsa003i_mqtt.service

sudo systemctl daemon-reload

sudo systemctl enable ifttt.service
sudo systemctl enable pi_home.service
sudo systemctl enable bme680_mqtt.service
sudo systemctl enable weather.service
sudo systemctl enable apds9960_mqtt.service
sudo systemctl enable gp2y0a21yk0f_mqtt.service
sudo systemctl enable hue.service
sudo systemctl enable pmsa003i_mqtt.service



sudo systemctl restart ifttt.service
sudo systemctl restart pi_home.service
sudo systemctl restart bme680_mqtt.service
sudo systemctl restart weather.service
sudo systemctl restart apds9960_mqtt.service
sudo systemctl restart gp2y0a21yk0f_mqtt.service
sudo systemctl restart hue.service
sudo systemctl restart pmsa003i_mqtt.service


sudo systemctl status ifttt.service
sudo systemctl status pi_home.service
sudo systemctl status bme680_mqtt.service
sudo systemctl status weather.service
sudo systemctl status apds9960_mqtt.service
sudo systemctl status gp2y0a21yk0f_mqtt.service
sudo systemctl status hue.service
sudo systemctl status pmsa003i_mqtt.service