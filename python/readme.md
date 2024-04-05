# Raspberry Pi Garden

Projects and experiments




get most recent values

select last(value) from mqtt_consumer group by *;


get recent averages:

model
SELECT MEAN("longest-wait-time”) FROM “web” WHERE (“service” =~ /web1|app1/) AND $timeFilter GROUP BY time(1h), “service” fill(null)

SELECT MEAN(value) FROM mqtt_consumer   WHERE time > now() - 10s group by * 




## Raspberry PI rebuild

rebuilt SD card
setup ssh https://itsfoss.com/ssh-into-raspberry/

interfaces
ssh
i2c

download project from Github
https://github.com/riotgibbon/iot-gibbon.git

mkdir github
git clone https://github.com/riotgibbon/iot-gibbon.git


upgrade Python
https://stackoverflow.com/questions/64718274/how-to-update-python-in-raspberry-pi
sudo apt-get update    
sudo apt-get install -y build-essential tk-dev libncurses5-dev libncursesw5-dev libreadline6-dev libdb5.3-dev libgdbm-dev libsqlite3-dev libssl-dev libbz2-dev libexpat1-dev liblzma-dev zlib1g-dev libffi-dev

bme680
sudo pip3 install adafruit-circuitpython-bme680