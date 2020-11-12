# InfluxDB python client

https://influxdb-python.readthedocs.io/en/latest/include-readme.html

pip3 install influxdb



>>> from influxdb import InfluxDBClient
>>> host='localhost'
>>> port=8086
>>> user=''
>>> password=''
>>> dbname = 'home'
client = InfluxDBClient(host, port, user, password, dbname)

query = "select last(value),* from mqtt_consumer group by *;"

>>> result = client.query(query)
>>> print("Result: {0}".format(result))
Result: ResultSet({'('mqtt_consumer', {'host': 'telegraf', 'topic': 'home/tele/blue/livingroom/desk'})': [{'time': '2020-11-12T15:47:20.801887Z', 'last': 20.0, 'value': 20.0}], '('mqtt_consumer', {'host': 'telegraf', 'topic': 'home/tele/clear/livingroom/desk'})': [{'time': '2020-11-12T15:47:20.802014Z', 'last': 54.0, 'value': 54.0}], '('mqtt_consumer', {'host': 'telegraf', 'topic': 'home/tele/colourTemp/livingroom/desk'})': [{'time': '2020-11-12T15:47:20.802036Z', 'last': 4548.991462072831, 'value': 4548.991462072831}], '('mqtt_consumer', {'host': 'telegraf', 'topic': 'home/tele/green/livingroom/desk'})': [{'time': '2020-11-12T15:47:20.801785Z', 'last': 40.0, 'value': 40.0}], '('mqtt_consumer', {'host': 'telegraf', 'topic': 'home/tele/humidity/Slough/openweather'})': [{'time': '2020-11-12T15:47:03.998568Z', 'last': 58.0, 'value': 58.0}], '('mqtt_consumer', {'host': 'telegraf', 'topic': 'home/tele/humidity/livingroom/desk'})': [{'time': '2020-11-12T15:47:19.879080Z', 'last': 51.394, 'value': 51.394}], '('mqtt_consumer', {'host': 'telegraf', 'topic': 'home/tele/humidity/livingroom/window'})': [{'time': '2020-11-12T15:47:15.839180Z', 'last': 65.44, 'value': 65.44}], '('mqtt_consumer', {'host': 'telegraf', 'topic': 'home/tele/humidityAdjusted/livingroom/window'})': [{'time': '2020-11-12T15:47:15.847448Z', 'last': 32.72, 'value': 32.72}], '('mqtt_consumer', {'host': 'telegraf', 'topic': 'home/tele/lightLux/livingroom/desk'})': [{'time': '2020-11-12T15:47:20.802065Z', 'last': 42.0034, 'value': 42.0034}], '('mqtt_consumer', {'topic': 'home/tele/lightReading/livingroom/window', 'host': 'telegraf'})': [{'time': '2020-11-12T15:47:15.893452Z', 'last': 1.0, 'value': 1.0}], '('mqtt_consumer', {'host': 'telegraf', 'topic': 'home/tele/lightVoltage/livingroom/window'})': [{'time': '2020-11-12T15:47:15.895420Z', 'last': 0.0048828125, 'value': 0.0048828125}], '('mqtt_consumer', {'topic': 'home/tele/pressure/Slough/openweather', 'host': 'telegraf'})': [{'time': '2020-11-12T15:47:03.998763Z', 'last': 1015.0, 'value': 1015.0}], '('mqtt_consumer', {'host': 'telegraf', 'topic': 'home/tele/pressure/livingroom/desk'})': [{'time': '2020-11-12T15:47:19.879921Z', 'last': 1013.69, 'value': 1013.69}], '('mqtt_consumer', {'host': 'telegraf', 'topic': 'home/tele/proximity/livingroom/desk'})': [{'time': '2020-11-12T15:47:20.797960Z', 'last': 195.0, 'value': 195.0}], '('mqtt_consumer', {'host': 'telegraf', 'topic': 'home/tele/red/livingroom/desk'})': [{'time': '2020-11-12T15:47:20.801702Z', 'last': 20.0, 'value': 20.0}], '('mqtt_consumer', {'host': 'telegraf', 'topic': 'home/tele/soilmoisture/livingroom/amaryllis'})': [{'time': '2020-11-12T15:47:15.922195Z', 'last': 829.0, 'value': 829.0}], '('mqtt_consumer', {'host': 'telegraf', 'topic': 'home/tele/soilmoisture/livingroom/aralia'})': [{'time': '2020-11-12T15:47:15.903470Z', 'last': 829.0, 'value': 829.0}], '('mqtt_consumer', {'host': 'telegraf', 'topic': 'home/tele/soilmoisture/livingroom/bonsai'})': [{'time': '2020-11-12T15:47:15.910939Z', 'last': 830.0, 'value': 830.0}], '('mqtt_consumer', {'host': 'telegraf', 'topic': 'home/tele/temperature/Slough/openweather'})': [{'time': '2020-11-12T15:47:03.997413Z', 'last': 12.12, 'value': 12.12}], '('mqtt_consumer', {'host': 'telegraf', 'topic': 'home/tele/temperature/livingroom/desk'})': [{'time': '2020-11-12T15:47:19.878566Z', 'last': 23.3, 'value': 23.3}], '('mqtt_consumer', {'host': 'telegraf', 'topic': 'home/tele/temperature/livingroom/window'})': [{'time': '2020-11-12T15:47:15.832934Z', 'last': 21.21, 'value': 21.21}], '('mqtt_consumer', {'host': 'telegraf', 'topic': 'home/tele/uvIndex/livingroom/window'})': [{'time': '2020-11-12T15:47:15.896640Z', 'last': 0.0, 'value': 0.0}]})

points = list(rs.get_points(measurement='cpu', tags={'host_name': 'influxdb.com'}))


points = list(result.get_points(measurement='mqtt_consumer', tags={'topic': 'home/tele/temperature/livingroom/desk'}))

>>> print(points)
[{'time': '2020-11-12T15:47:19.878566Z', 'last': 23.3, 'value': 23.3}]
>>> 