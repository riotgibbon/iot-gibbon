# 

https://developers.meethue.com/develop/get-started-2/
https://192.168.0.14/debug/clip.html

	/api/newdeveloper

[
	{
		"error": {
			"type": 1,
			"address": "/",
			"description": "unauthorized user"
		}
	}
]


POST
/api
{"devicetype":"my_hue_app#raspberrypi"}


[
	{
		"success": {
			"username": "Zk16ZQhoxu1MHAJskpApN8i-y8xg0EfGULyBMHS7"
		}
	}
]


list lights


https://192.168.0.14/api/Zk16ZQhoxu1MHAJskpApN8i-y8xg0EfGULyBMHS7/lights

{
	"1": {
		"state": {
			"on": true,
			"bri": 254,
			"hue": 8597,
			"sat": 121,
			"effect": "none",
			"xy": [
				0.4452,
				0.4068
			],
			"ct": 343,
			"alert": "select",
			"colormode": "xy",
			"mode": "homeautomation",
			"reachable": true
		},
		"swupdate": {
			"state": "noupdates",
			"lastinstall": "2020-03-03T14:31:30"
		},
		"type": "Extended color light",
		"name": "door light",
		"modelid": "LCT015",
		"manufacturername": "Signify Netherlands B.V.",
		"productname": "Hue color lamp",
		"capabilities": {
			"certified": true,
			"control": {
				"mindimlevel": 1000,
				"maxlumen": 806,
				"colorgamuttype": "C",
				"colorgamut": [
					[
						0.6915,
						0.3083
					],
					[
						0.17,
						0.7
					],
					[
						0.1532,
						0.0475
					]
				],
				"ct": {
					"min": 153,
					"max": 500
				}
			},
			"streaming": {
				"renderer": true,
				"proxy": true
			}
		},
		"config": {
			"archetype": "sultanbulb",
			"function": "mixed",
			"direction": "omnidirectional",
			"startup": {
				"mode": "safety",
				"configured": true
			}
		},
		"uniqueid": "00:17:88:01:04:cf:55:b3-0b",
		"swversion": "1.50.2_r30933",
		"swconfigid": "772B0E5E",
		"productid": "Philips-LCT015-1-A19ECLv5"
	},
	"2": {
		"state": {
			"on": true,
			"bri": 254,
			"hue": 8597,
			"sat": 121,
			"effect": "none",
			"xy": [
				0.4452,
				0.4068
			],
			"ct": 343,
			"alert": "select",
			"colormode": "xy",
			"mode": "homeautomation",
			"reachable": true
		},
		"swupdate": {
			"state": "noupdates",
			"lastinstall": "2020-03-03T14:30:39"
		},
		"type": "Extended color light",
		"name": "window light",
		"modelid": "LCT015",
		"manufacturername": "Signify Netherlands B.V.",
		"productname": "Hue color lamp",
		"capabilities": {
			"certified": true,
			"control": {
				"mindimlevel": 1000,
				"maxlumen": 806,
				"colorgamuttype": "C",
				"colorgamut": [
					[
						0.6915,
						0.3083
					],
					[
						0.17,
						0.7
					],
					[
						0.1532,
						0.0475
					]
				],
				"ct": {
					"min": 153,
					"max": 500
				}
			},
			"streaming": {
				"renderer": true,
				"proxy": true
			}
		},
		"config": {
			"archetype": "sultanbulb",
			"function": "mixed",
			"direction": "omnidirectional",
			"startup": {
				"mode": "safety",
				"configured": true
			}
		},
		"uniqueid": "00:17:88:01:04:ff:49:f7-0b",
		"swversion": "1.50.2_r30933",
		"swconfigid": "772B0E5E",
		"productid": "Philips-LCT015-1-A19ECLv5"
	},
	"3": {
		"state": {
			"on": false,
			"bri": 203,
			"hue": 7676,
			"sat": 199,
			"effect": "none",
			"xy": [
				0.5016,
				0.4151
			],
			"ct": 443,
			"alert": "select",
			"colormode": "xy",
			"mode": "homeautomation",
			"reachable": true
		},
		"swupdate": {
			"state": "noupdates",
			"lastinstall": "2020-03-03T14:30:30"
		},
		"type": "Extended color light",
		"name": "Hue color lamp 3",
		"modelid": "LCT015",
		"manufacturername": "Signify Netherlands B.V.",
		"productname": "Hue color lamp",
		"capabilities": {
			"certified": true,
			"control": {
				"mindimlevel": 1000,
				"maxlumen": 806,
				"colorgamuttype": "C",
				"colorgamut": [
					[
						0.6915,
						0.3083
					],
					[
						0.17,
						0.7
					],
					[
						0.1532,
						0.0475
					]
				],
				"ct": {
					"min": 153,
					"max": 500
				}
			},
			"streaming": {
				"renderer": true,
				"proxy": true
			}
		},
		"config": {
			"archetype": "sultanbulb",
			"function": "mixed",
			"direction": "omnidirectional",
			"startup": {
				"mode": "safety",
				"configured": true
			}
		},
		"uniqueid": "00:17:88:01:04:cf:7c:96-0b",
		"swversion": "1.50.2_r30933",
		"swconfigid": "772B0E5E",
		"productid": "Philips-LCT015-1-A19ECLv5"
	},
	"4": {
		"state": {
			"on": false,
			"bri": 126,
			"alert": "select",
			"mode": "homeautomation",
			"reachable": true
		},
		"swupdate": {
			"state": "noupdates",
			"lastinstall": "2020-03-03T14:30:28"
		},
		"type": "Dimmable light",
		"name": "Hue white lamp 1",
		"modelid": "LWB010",
		"manufacturername": "Signify Netherlands B.V.",
		"productname": "Hue white lamp",
		"capabilities": {
			"certified": true,
			"control": {
				"mindimlevel": 2000,
				"maxlumen": 806
			},
			"streaming": {
				"renderer": false,
				"proxy": false
			}
		},
		"config": {
			"archetype": "classicbulb",
			"function": "functional",
			"direction": "omnidirectional",
			"startup": {
				"mode": "safety",
				"configured": true
			}
		},
		"uniqueid": "00:17:88:01:04:03:ad:50-0b",
		"swversion": "1.50.2_r30933",
		"swconfigid": "754CE4FC",
		"productid": "Philips-LWB010-1-A19DLv4"
	},
	"5": {
		"state": {
			"on": false,
			"bri": 126,
			"alert": "select",
			"mode": "homeautomation",
			"reachable": true
		},
		"swupdate": {
			"state": "noupdates",
			"lastinstall": "2020-03-03T14:30:25"
		},
		"type": "Dimmable light",
		"name": "Hue white lamp 2",
		"modelid": "LWB010",
		"manufacturername": "Signify Netherlands B.V.",
		"productname": "Hue white lamp",
		"capabilities": {
			"certified": true,
			"control": {
				"mindimlevel": 2000,
				"maxlumen": 806
			},
			"streaming": {
				"renderer": false,
				"proxy": false
			}
		},
		"config": {
			"archetype": "classicbulb",
			"function": "functional",
			"direction": "omnidirectional",
			"startup": {
				"mode": "safety",
				"configured": true
			}
		},
		"uniqueid": "00:17:88:01:04:c8:93:8e-0b",
		"swversion": "1.50.2_r30933",
		"swconfigid": "754CE4FC",
		"productid": "Philips-LWB010-1-A19DLv4"
	},
	"6": {
		"state": {
			"on": false,
			"bri": 144,
			"hue": 47104,
			"sat": 254,
			"effect": "none",
			"xy": [
				0.1532,
				0.0475
			],
			"ct": 153,
			"alert": "select",
			"colormode": "xy",
			"mode": "homeautomation",
			"reachable": true
		},
		"swupdate": {
			"state": "noupdates",
			"lastinstall": "2020-03-03T14:30:42"
		},
		"type": "Extended color light",
		"name": "bedroom light",
		"modelid": "LCT015",
		"manufacturername": "Signify Netherlands B.V.",
		"productname": "Hue color lamp",
		"capabilities": {
			"certified": true,
			"control": {
				"mindimlevel": 1000,
				"maxlumen": 806,
				"colorgamuttype": "C",
				"colorgamut": [
					[
						0.6915,
						0.3083
					],
					[
						0.17,
						0.7
					],
					[
						0.1532,
						0.0475
					]
				],
				"ct": {
					"min": 153,
					"max": 500
				}
			},
			"streaming": {
				"renderer": true,
				"proxy": true
			}
		},
		"config": {
			"archetype": "sultanbulb",
			"function": "mixed",
			"direction": "omnidirectional",
			"startup": {
				"mode": "safety",
				"configured": true
			}
		},
		"uniqueid": "00:17:88:01:04:3c:89:ba-0b",
		"swversion": "1.50.2_r30933",
		"swconfigid": "772B0E5E",
		"productid": "Philips-LCT015-1-A19ECLv5"
	}
}




Address	https://<bridge ip address>/api/1028d66426293e821ecfd9ef1a0731df/lights/1/state
Body	{"on":false}
Method	PUT


Address	https://192.168.0.14/api/Zk16ZQhoxu1MHAJskpApN8i-y8xg0EfGULyBMHS7/lights/2/state
Body	{"on":false}
Method	PUT


{"on":true, "sat":254, "bri":254,"hue":10000}



red
{"on":true, "sat":254, "bri":254,"hue":65535}

blue
{"on":true, "sat":254, "bri":254,"hue":45000}


control hue through average moisture


influx queries

select max(value) from mqtt_consumer where topic  =~ /soilmoisture/
> select max(value) from mqtt_consumer where topic  =~ /soilmoisture/
name: mqtt_consumer
time                max
----                ---
1605705023028466489 907



> select min(value), max(value), count(*) from mqtt_consumer where topic  =~ /soilmoisture/ group by "topic"
name: mqtt_consumer
tags: topic=home/tele/soilmoisture/livingroom/amaryllis
time min max count_value
---- --- --- -----------
0    819 839 143680

name: mqtt_consumer
tags: topic=home/tele/soilmoisture/livingroom/aralia
time min max count_value
---- --- --- -----------
0    0   841 143682

name: mqtt_consumer
tags: topic=home/tele/soilmoisture/livingroom/bonsai
time min max count_value
---- --- --- -----------
0    763 859 143680

name: mqtt_consumer
tags: topic=home/tele/soilmoisture/livingroom/yucca
time min max count_value
---- --- --- -----------
0    0   907 31690





> select min(value), max(value), count(*) from mqtt_consumer where topic  =~ /soilmoisture/ group by time(1d) 
name: mqtt_consumer
time                min max count_value
----                --- --- -----------
1604880000000000000 827 859 5787
1604966400000000000 821 857 39526
1605052800000000000 816 844 42210
1605139200000000000 811 836 41421
1605225600000000000 818 834 42645
1605312000000000000 807 836 38397
1605398400000000000 0   839 42861
1605484800000000000 820 838 42831
1605571200000000000 821 841 39207
1605657600000000000 0   907 49303
1605744000000000000 3   836 50607
1605830400000000000 3   833 28055
> select min(value), max(value), count(*) from mqtt_consumer where topic  =~ /soilmoisture/ group by time(1d), "topic" 
name: mqtt_consumer
tags: topic=home/tele/soilmoisture/livingroom/amaryllis
time                min max count_value
----                --- --- -----------
1604880000000000000 835 839 1929
1604966400000000000 829 839 13175
1605052800000000000 827 836 14070
1605139200000000000 823 833 13807
1605225600000000000 825 834 14215
1605312000000000000 824 835 12799
1605398400000000000 820 832 14287
1605484800000000000 822 835 14277
1605571200000000000 824 835 13069
1605657600000000000 821 836 14157
1605744000000000000 820 834 11144
1605830400000000000 819 829 6782

name: mqtt_consumer
tags: topic=home/tele/soilmoisture/livingroom/aralia
time                min max count_value
----                --- --- -----------
1604880000000000000 827 837 1929
1604966400000000000 821 836 13176
1605052800000000000 821 835 14070
1605139200000000000 818 835 13807
1605225600000000000 818 834 14215
1605312000000000000 807 836 12799
1605398400000000000 0   839 14287
1605484800000000000 820 838 14277
1605571200000000000 821 841 13069
1605657600000000000 803 838 14157
1605744000000000000 816 836 11145
1605830400000000000 816 833 6782

name: mqtt_consumer
tags: topic=home/tele/soilmoisture/livingroom/bonsai
time                min max count_value
----                --- --- -----------
1604880000000000000 853 859 1929
1604966400000000000 839 857 13175
1605052800000000000 816 844 14070
1605139200000000000 811 836 13807
1605225600000000000 819 832 14215
1605312000000000000 816 831 12799
1605398400000000000 813 838 14287
1605484800000000000 822 838 14277
1605571200000000000 821 838 13069
1605657600000000000 816 839 14157
1605744000000000000 763 833 11144
1605830400000000000 786 831 6782

name: mqtt_consumer
tags: topic=home/tele/soilmoisture/livingroom/yucca
time                min max count_value
----                --- --- -----------
1605657600000000000 0   907 6832
1605744000000000000 3   834 17174
1605830400000000000 3   831 7722






> select min(value), max(value), mean(value), count(*) from mqtt_consumer where topic  =~ /soilmoisture/ group by time(1d), "topic" 
name: mqtt_consumer
tags: topic=home/tele/soilmoisture/livingroom/amaryllis
time                min max mean              count_value
----                --- --- ----              -----------
1604880000000000000 835 839 837.5707620528772 1929
1604966400000000000 829 839 834.0735483870968 13175
1605052800000000000 827 836 831.9231698649609 14070
1605139200000000000 823 833 829.9571956254074 13807
1605225600000000000 825 834 829.4460077383046 14215
1605312000000000000 824 835 829.4580826627081 12799
1605398400000000000 820 832 829.0445159935606 14287
1605484800000000000 822 835 828.2684737689991 14277
1605571200000000000 824 835 829.1179126176448 13069
1605657600000000000 821 836 829.0701419792329 14157
1605744000000000000 820 834 827.152727925341  11144
1605830400000000000 819 829 825.2336572438163 6792

name: mqtt_consumer
tags: topic=home/tele/soilmoisture/livingroom/aralia
time                min max mean              count_value
----                --- --- ----              -----------
1604880000000000000 827 837 831.7667185069985 1929
1604966400000000000 821 836 831.1419247115969 13176
1605052800000000000 821 835 830.3656716417911 14070
1605139200000000000 818 835 828.3000651843267 13807
1605225600000000000 818 834 827.0423496306719 14215
1605312000000000000 807 836 827.6431752480662 12799
1605398400000000000 0   839 817.8952194302512 14287
1605484800000000000 820 838 830.2141906563004 14277
1605571200000000000 821 841 830.7886601882317 13069
1605657600000000000 803 838 826.214381578018  14157
1605744000000000000 816 836 827.1020188425302 11145
1605830400000000000 816 833 825.4440518256773 6792

name: mqtt_consumer
tags: topic=home/tele/soilmoisture/livingroom/bonsai
time                min max mean              count_value
----                --- --- ----              -----------
1604880000000000000 853 859 856.3286677034733 1929
1604966400000000000 839 857 848.0382542694497 13175
1605052800000000000 816 844 835.955223880597  14070
1605139200000000000 811 836 829.5211849062069 13807
1605225600000000000 819 832 827.75863524446   14215
1605312000000000000 816 831 825.2479881240722 12799
1605398400000000000 813 838 825.684818366347  14287
1605484800000000000 822 838 832.8265742102683 14277
1605571200000000000 821 838 830.4826689111638 13069
1605657600000000000 816 839 830.4233947870312 14157
1605744000000000000 763 833 819.9213029432879 11144
1605830400000000000 786 831 815.9386042402826 6792

name: mqtt_consumer
tags: topic=home/tele/soilmoisture/livingroom/yucca
time                min max mean              count_value
----                --- --- ----              -----------
1605657600000000000 0   907 817.4259367681499 6832
1605744000000000000 3   834 821.9319319902178 17174
1605830400000000000 3   831 820.0293509180243 7734




average of all plants

> select min(value), max(value), mean(value), count(*) from mqtt_consumer where topic  =~ /soilmoisture/ group by time(1d)
name: mqtt_consumer
time                min max mean              count_value
----                --- --- ----              -----------
1604880000000000000 827 859 841.888716087783  5787
1604966400000000000 821 857 837.7510752416131 39526
1605052800000000000 816 844 832.748021795783  42210
1605139200000000000 811 836 829.2594819053137 41421
1605225600000000000 818 834 828.0823308711455 42645
1605312000000000000 807 836 827.4497486782822 38397
1605398400000000000 0   839 824.2081845967197 42861
1605484800000000000 820 838 830.4364128785226 42831
1605571200000000000 821 841 830.1297472390135 39207
1605657600000000000 0   907 827.025150599355  49303
1605744000000000000 3   836 823.7774220957575 50607
1605830400000000000 3   833 821.6073443320915 28185




make range between 810 and 850

40 


red
{"on":true, "sat":254, "bri":254,"hue":65535}

blue
{"on":true, "sat":254, "bri":254,"hue":45000}

20,535

call is 20,000

each point 500

