# Elasticsearch 

## Installation

https://www.elastic.co/guide/en/elasticsearch/reference/current/deb.html

sudo apt-get install apt-transport-https
echo "deb https://artifacts.elastic.co/packages/7.x/apt stable main" | sudo tee /etc/apt/sources.list.d/elastic-7.x.list

didn't work


https://www.digitalocean.com/community/tutorials/how-to-install-and-configure-elasticsearch-on-ubuntu-18-04

curl -fsSL https://artifacts.elastic.co/GPG-KEY-elasticsearch | sudo apt-key add -
echo "deb https://artifacts.elastic.co/packages/7.x/apt stable main" | sudo tee -a /etc/apt/sources.list.d/elastic-7.x.list
sudo apt update
sudo apt install elasticsearch



## Configure

use external disk for data

ensure is not FAT32 formatted
 check with `df -T`

 permissions need to be open for read/write from disk root
chown to `elasticsearch` user


/media/9EEC04FDEC04D187


mkdir /media/9EEC04FDEC04D187/data/elasticsearch


sudo nano /etc/elasticsearch/elasticsearch.yml

#path.data: /var/lib/elasticsearch
path.data: /media/9EEC04FDEC04D187/data/elasticsearch/data


#path.logs: /var/log/elasticsearch
path.logs: /media/9EEC04FDEC04D187/data/elasticsearch/log

new disk

/media/toby/toshiba

mkdir /media/toby/toshiba/data/elasticsearch

sudo nano /etc/elasticsearch/elasticsearch.yml

#path.data: /var/lib/elasticsearch
path.data: /media/toby/toshiba/data/elasticsearch/data

#path.logs: /var/log/elasticsearch
path.logs: /media/toby/toshiba/data/elasticsearch/log


sudo chown -R elasticsearch:elasticsearch /media/toby/toshiba/data/elasticsearch/data


/media/818ea40d-9b59-4cba-ab75-2b480d92a894

sudo nano /etc/elasticsearch/elasticsearch.yml

#path.data: /var/lib/elasticsearch
path.data: /media/818ea40d-9b59-4cba-ab75-2b480d92a894/data/elasticsearch/data

#path.logs: /var/log/elasticsearch
path.logs: /media/818ea40d-9b59-4cba-ab75-2b480d92a894/data/elasticsearch/log

sudo chown -R elasticsearch:elasticsearch /media/818ea40d-9b59-4cba-ab75-2b480d92a894/data/elasticsearch/



## Start/stop

sudo /bin/systemctl daemon-reload
sudo /bin/systemctl enable elasticsearch.service

sudo systemctl start elasticsearch.service
sudo systemctl stop elasticsearch.service
sudo systemctl restart elasticsearch.service

sudo systemctl status elasticsearch.service

toby@jet-gibbon:~$ curl -X GET 'http://localhost:9200'
{
  "name" : "jet-gibbon",
  "cluster_name" : "elasticsearch",
  "cluster_uuid" : "MSt5wgcdTpuqdhdVaKsV7A",
  "version" : {
    "number" : "7.10.0",
    "build_flavor" : "default",
    "build_type" : "deb",
    "build_hash" : "51e9d6f22758d0374a0f3f5c6e8f3a7997850f96",
    "build_date" : "2020-11-09T21:30:33.964949Z",
    "build_snapshot" : false,
    "lucene_version" : "8.7.0",
    "minimum_wire_compatibility_version" : "6.8.0",
    "minimum_index_compatibility_version" : "6.0.0-beta1"
  },
  "tagline" : "You Know, for Search"
}


192.168.0.72
curl -X GET 'http://192.168.0.72:9200'

not accessing

Mac  - 192.168.0.36

curl -X GET 'http://192.168.0.36:9200'

can't get it to run - not able to access from other machine

run basic webserver

sudo docker run -dit --name tecmint-web -p 8080:80 -v /home/user/website/:/usr/local/apache2/htdocs/ httpd:2.4

toby@jet-gibbon:~$ curl http://localhost:8080
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<html>
 <head>
  <title>Index of /</title>
 </head>
 <body>
<h1>Index of /</h1>
<ul></ul>
</body></html>


curl http://192.168.0.72:8080


can view from outside, Elastic issue


iscovery.type: single-node
transport.tcp.port: 9300
http.host: 0.0.0.0
http.port: 9200
transport.host: localhost
network.host: 0.0.0.0

## Logstash

read mqtt, write to elasticsearch


https://www.digitalocean.com/community/tutorials/how-to-install-elasticsearch-logstash-and-kibana-elastic-stack-on-ubuntu-18-04


sudo apt install logstash


https://github.com/elastic/Machinebeat

https://ela.st/machinebeat-linux

./machinebeat -e -c machinebeat.yml



toby@jet-gibbon:~/machinebeat$ cat modules.d/mqtt.yml
# Module: mqtt

- module: mqtt
  metricsets: ["topic"]
  enabled: true
  period: 1s
  host: "192.168.0.63:1883"
  topics: ["home/tele/#"]
  decode_payload: true
  #qos
  #user: ""
  #password: ""
  #ssl: true
  #CA: ""
  #clientCert: ""
  #clientKey: ""


need to set permissions to root

```
  toby@jet-gibbon:~/machinebeat$ sudo ./machinebeat -e -c machinebeat.yml
[sudo] password for toby: 
sudo: ./machinebeat: command not found
toby@jet-gibbon:~/machinebeat$ chmod +x m
machinebeat      machinebeat.yml  modules.d/       
toby@jet-gibbon:~/machinebeat$ chmod +x machinebeat
toby@jet-gibbon:~/machinebeat$ sudo ./machinebeat -e -c machinebeat.yml
Exiting: error loading config file: config file ("machinebeat.yml") must be owned by the user identifier (uid=0) or root
toby@jet-gibbon:~/machinebeat$ sudo chown root machinebeat.yml 
toby@jet-gibbon:~/machinebeat$ sudo ./machinebeat -e -c machinebeat.yml
Exiting: error loading config file: config file ("machinebeat.yml") can only be writable by the owner but the permissions are "-rw-rw-r--" (to fix the permissions use: 'chmod go-w /home/toby/machinebeat/machinebeat.yml')
toby@jet-gibbon:~/machinebeat$ sudo chmod go-w /home/toby/machinebeat/machinebeat.yml
toby@jet-gibbon:~/machinebeat$ sudo ./machinebeat -e -c machinebeat.yml
2020-11-27T13:36:02.589Z        INFO    instance/beat.go:647    Home path: [/home/toby/machinebeat] Config path: [/home/toby/machinebeat] Data path: [/home/toby/machinebeat/data] Logs path: [/home/toby/machinebeat/logs]
2020-11-27T13:36:02.599Z        INFO    instance/beat.go:655    Beat ID: d9e9a452-67a3-4a4c-b6af-4c68fb104487
2020-11-27T13:36:02.603Z        INFO    [seccomp]       seccomp/seccomp.go:124  Syscall filter successfully installed
2020-11-27T13:36:02.603Z        INFO    [beat]  instance/beat.go:983    Beat info       {"system_info": {"beat": {"path": {"config": "/home/toby/machinebeat", "data": "/home/toby/machinebeat/data", "home": "/home/toby/machinebeat", "logs": "/home/toby/machinebeat/logs"}, "type": "machinebeat", "uuid": "d9e9a452-67a3-4a4c-b6af-4c68fb104487"}}}
2020-11-27T13:36:02.604Z        INFO    [beat]  instance/beat.go:992    Build info      {"system_info": {"build": {"commit": "unknown", "libbeat": "7.8.1", "time": "0001-01-01T00:00:00.000Z", "version": "7.8.1"}}}
2020-11-27T13:36:02.604Z        INFO    [beat]  instance/beat.go:995    Go runtime info {"system_info": {"go": {"os":"linux","arch":"arm","max_procs":4,"version":"go1.15"}}}
2020-11-27T13:36:02.605Z        INFO    [beat]  instance/beat.go:999    Host info       {"system_info": {"host": {"architecture":"aarch64","boot_time":"2020-11-27T12:44:16Z","containerized":false,"name":"jet-gibbon","ip":["127.0.0.1/8","::1/128","192.168.0.72/24","fe80::924:af75:bbc1:c63c/64"],"kernel_version":"4.9.140-tegra","mac":["7e:10:2e:74:6b:62","00:04:4b:ec:60:ad","4c:1d:96:c7:d8:5f"],"os":{"family":"debian","platform":"ubuntu","name":"Ubuntu","version":"18.04.5 LTS (Bionic Beaver)","major":18,"minor":4,"patch":5,"codename":"bionic"},"timezone":"GMT","timezone_offset_sec":0,"id":"a3d9197b765643568af09eb2bd3e5ce7"}}}
2020-11-27T13:36:02.606Z        INFO    [beat]  instance/beat.go:1028   Process info    {"system_info": {"process": {"capabilities": {"inheritable":null,"permitted":["chown","dac_override","dac_read_search","fowner","fsetid","kill","setgid","setuid","setpcap","linux_immutable","net_bind_service","net_broadcast","net_admin","net_raw","ipc_lock","ipc_owner","sys_module","sys_rawio","sys_chroot","sys_ptrace","sys_pacct","sys_admin","sys_boot","sys_nice","sys_resource","sys_time","sys_tty_config","mknod","lease","audit_write","audit_control","setfcap","mac_override","mac_admin","syslog","wake_alarm","block_suspend","audit_read"],"effective":["chown","dac_override","dac_read_search","fowner","fsetid","kill","setgid","setuid","setpcap","linux_immutable","net_bind_service","net_broadcast","net_admin","net_raw","ipc_lock","ipc_owner","sys_module","sys_rawio","sys_chroot","sys_ptrace","sys_pacct","sys_admin","sys_boot","sys_nice","sys_resource","sys_time","sys_tty_config","mknod","lease","audit_write","audit_control","setfcap","mac_override","mac_admin","syslog","wake_alarm","block_suspend","audit_read"],"bounding":["chown","dac_override","dac_read_search","fowner","fsetid","kill","setgid","setuid","setpcap","linux_immutable","net_bind_service","net_broadcast","net_admin","net_raw","ipc_lock","ipc_owner","sys_module","sys_rawio","sys_chroot","sys_ptrace","sys_pacct","sys_admin","sys_boot","sys_nice","sys_resource","sys_time","sys_tty_config","mknod","lease","audit_write","audit_control","setfcap","mac_override","mac_admin","syslog","wake_alarm","block_suspend","audit_read"],"ambient":null}, "cwd": "/home/toby/machinebeat", "exe": "/home/toby/machinebeat/machinebeat", "name": "machinebeat", "pid": 11212, "ppid": 11211, "seccomp": {"mode":"filter"}, "start_time": "2020-11-27T13:36:02.240Z"}}}
2020-11-27T13:36:02.607Z        INFO    instance/beat.go:310    Setup Beat: machinebeat; Version: 7.8.1
2020-11-27T13:36:02.607Z        INFO    [index-management]      idxmgmt/std.go:184      Set output.elasticsearch.index to 'machinebeat-7.8.1' as ILM is enabled.
2020-11-27T13:36:02.607Z        INFO    eslegclient/connection.go:99    elasticsearch url: http://localhost:9200
2020-11-27T13:36:02.608Z        INFO    [publisher]     pipeline/module.go:113  Beat name: jet-gibbon
2020-11-27T13:36:02.608Z        INFO    [monitoring]    log/log.go:118  Starting metrics logging every 30s
2020-11-27T13:36:02.608Z        INFO    instance/beat.go:463    machinebeat start running.
2020-11-27T13:36:02.609Z        ERROR   cfgfile/reload.go:273   Error loading config from file '/home/toby/machinebeat/modules.d/mqtt.yml', error invalid config: config file ("/home/toby/machinebeat/modules.d/mqtt.yml") must be owned by the user identifier (uid=0) or root
2020-11-27T13:36:02.613Z        INFO    [monitoring]    log/log.go:153  Total non-zero metrics  {"monitoring": {"metrics": {"beat":{"cpu":{"system":{"ticks":20,"time":{"ms":28}},"total":{"ticks":60,"time":{"ms":76},"value":60},"user":{"ticks":40,"time":{"ms":48}}},"handles":{"limit":{"hard":1048576,"soft":1024},"open":13},"info":{"ephemeral_id":"99489e88-e888-4791-96d4-ecc33a036624","uptime":{"ms":52}},"memstats":{"gc_next":6056960,"memory_alloc":4120632,"memory_total":5888920,"rss":31449088},"runtime":{"goroutines":28}},"libbeat":{"config":{"module":{"running":0}},"output":{"type":"elasticsearch"},"pipeline":{"clients":0,"events":{"active":0}}},"system":{"cpu":{"cores":4},"load":{"1":0.36,"15":0.18,"5":0.19,"norm":{"1":0.09,"15":0.045,"5":0.0475}}}}}}
2020-11-27T13:36:02.613Z        INFO    [monitoring]    log/log.go:154  Uptime: 55.948828ms
2020-11-27T13:36:02.613Z        INFO    [monitoring]    log/log.go:131  Stopping metrics logging.
2020-11-27T13:36:02.613Z        INFO    instance/beat.go:469    machinebeat stopped.
2020-11-27T13:36:02.613Z        ERROR   instance/beat.go:958    Exiting: loading configs: 1 error: invalid config: config file ("/home/toby/machinebeat/modules.d/mqtt.yml") must be owned by the user identifier (uid=0) or root
Exiting: loading configs: 1 error: invalid config: config file ("/home/toby/machinebeat/modules.d/mqtt.yml") must be owned by the user identifier (uid=0) or root
toby@jet-gibbon:~/machinebeat$ sudo ./machinebeat -e -c modules.d/mqtt.yml
Exiting: error loading config file: config file ("modules.d/mqtt.yml") must be owned by the user identifier (uid=0) or root
toby@jet-gibbon:~/machinebeat$ sudo chown root modules.d/mqtt.yml
toby@jet-gibbon:~/machinebeat$ sudo ./machinebeat -e -c modules.d/mqtt.yml
Exiting: error loading config file: config file ("modules.d/mqtt.yml") can only be writable by the owner but the permissions are "-rw-rw-r--" (to fix the permissions use: 'chmod go-w /home/toby/machinebeat/modules.d/mqtt.yml')
toby@jet-gibbon:~/machinebeat$ sudo ./machinebeat -e -c machinebeat.yml
2020-11-27T13:36:41.796Z        INFO    instance/beat.go:647    Home path: [/home/toby/machinebeat] Config path: [/home/toby/machinebeat] Data path: [/home/toby/machinebeat/data] Logs path: [/home/toby/machinebeat/logs]
2020-11-27T13:36:41.796Z        INFO    instance/beat.go:655    Beat ID: d9e9a452-67a3-4a4c-b6af-4c68fb104487
2020-11-27T13:36:41.799Z        INFO    [seccomp]       seccomp/seccomp.go:124  Syscall filter successfully installed
2020-11-27T13:36:41.799Z        INFO    [beat]  instance/beat.go:983    Beat info       {"system_info": {"beat": {"path": {"config": "/home/toby/machinebeat", "data": "/home/toby/machinebeat/data", "home": "/home/toby/machinebeat", "logs": "/home/toby/machinebeat/logs"}, "type": "machinebeat", "uuid": "d9e9a452-67a3-4a4c-b6af-4c68fb104487"}}}
2020-11-27T13:36:41.799Z        INFO    [beat]  instance/beat.go:992    Build info      {"system_info": {"build": {"commit": "unknown", "libbeat": "7.8.1", "time": "0001-01-01T00:00:00.000Z", "version": "7.8.1"}}}
2020-11-27T13:36:41.799Z        INFO    [beat]  instance/beat.go:995    Go runtime info {"system_info": {"go": {"os":"linux","arch":"arm","max_procs":4,"version":"go1.15"}}}
2020-11-27T13:36:41.801Z        INFO    [beat]  instance/beat.go:999    Host info       {"system_info": {"host": {"architecture":"aarch64","boot_time":"2020-11-27T12:44:16Z","containerized":false,"name":"jet-gibbon","ip":["127.0.0.1/8","::1/128","192.168.0.72/24","fe80::924:af75:bbc1:c63c/64"],"kernel_version":"4.9.140-tegra","mac":["7e:10:2e:74:6b:62","00:04:4b:ec:60:ad","4c:1d:96:c7:d8:5f"],"os":{"family":"debian","platform":"ubuntu","name":"Ubuntu","version":"18.04.5 LTS (Bionic Beaver)","major":18,"minor":4,"patch":5,"codename":"bionic"},"timezone":"GMT","timezone_offset_sec":0,"id":"a3d9197b765643568af09eb2bd3e5ce7"}}}
2020-11-27T13:36:41.802Z        INFO    [beat]  instance/beat.go:1028   Process info    {"system_info": {"process": {"capabilities": {"inheritable":null,"permitted":["chown","dac_override","dac_read_search","fowner","fsetid","kill","setgid","setuid","setpcap","linux_immutable","net_bind_service","net_broadcast","net_admin","net_raw","ipc_lock","ipc_owner","sys_module","sys_rawio","sys_chroot","sys_ptrace","sys_pacct","sys_admin","sys_boot","sys_nice","sys_resource","sys_time","sys_tty_config","mknod","lease","audit_write","audit_control","setfcap","mac_override","mac_admin","syslog","wake_alarm","block_suspend","audit_read"],"effective":["chown","dac_override","dac_read_search","fowner","fsetid","kill","setgid","setuid","setpcap","linux_immutable","net_bind_service","net_broadcast","net_admin","net_raw","ipc_lock","ipc_owner","sys_module","sys_rawio","sys_chroot","sys_ptrace","sys_pacct","sys_admin","sys_boot","sys_nice","sys_resource","sys_time","sys_tty_config","mknod","lease","audit_write","audit_control","setfcap","mac_override","mac_admin","syslog","wake_alarm","block_suspend","audit_read"],"bounding":["chown","dac_override","dac_read_search","fowner","fsetid","kill","setgid","setuid","setpcap","linux_immutable","net_bind_service","net_broadcast","net_admin","net_raw","ipc_lock","ipc_owner","sys_module","sys_rawio","sys_chroot","sys_ptrace","sys_pacct","sys_admin","sys_boot","sys_nice","sys_resource","sys_time","sys_tty_config","mknod","lease","audit_write","audit_control","setfcap","mac_override","mac_admin","syslog","wake_alarm","block_suspend","audit_read"],"ambient":null}, "cwd": "/home/toby/machinebeat", "exe": "/home/toby/machinebeat/machinebeat", "name": "machinebeat", "pid": 11291, "ppid": 11290, "seccomp": {"mode":"filter"}, "start_time": "2020-11-27T13:36:41.440Z"}}}
2020-11-27T13:36:41.802Z        INFO    instance/beat.go:310    Setup Beat: machinebeat; Version: 7.8.1
2020-11-27T13:36:41.802Z        INFO    [index-management]      idxmgmt/std.go:184      Set output.elasticsearch.index to 'machinebeat-7.8.1' as ILM is enabled.
2020-11-27T13:36:41.803Z        INFO    eslegclient/connection.go:99    elasticsearch url: http://localhost:9200
2020-11-27T13:36:41.803Z        INFO    [publisher]     pipeline/module.go:113  Beat name: jet-gibbon
2020-11-27T13:36:41.804Z        INFO    [monitoring]    log/log.go:118  Starting metrics logging every 30s
2020-11-27T13:36:41.804Z        INFO    instance/beat.go:463    machinebeat start running.
2020-11-27T13:36:41.804Z        ERROR   cfgfile/reload.go:273   Error loading config from file '/home/toby/machinebeat/modules.d/mqtt.yml', error invalid config: config file ("/home/toby/machinebeat/modules.d/mqtt.yml") can only be writable by the owner but the permissions are "-rw-rw-r--" (to fix the permissions use: 'chmod go-w /home/toby/machinebeat/modules.d/mqtt.yml')
2020-11-27T13:36:41.808Z        INFO    [monitoring]    log/log.go:153  Total non-zero metrics  {"monitoring": {"metrics": {"beat":{"cpu":{"system":{"ticks":10,"time":{"ms":12}},"total":{"ticks":70,"time":{"ms":76},"value":70},"user":{"ticks":60,"time":{"ms":64}}},"handles":{"limit":{"hard":1048576,"soft":1024},"open":13},"info":{"ephemeral_id":"bdd6b408-0383-49c9-a450-ec759f74e6ae","uptime":{"ms":40}},"memstats":{"gc_next":6047616,"memory_alloc":4102800,"memory_total":5889136,"rss":32194560},"runtime":{"goroutines":28}},"libbeat":{"config":{"module":{"running":0}},"output":{"type":"elasticsearch"},"pipeline":{"clients":0,"events":{"active":0}}},"system":{"cpu":{"cores":4},"load":{"1":0.31,"15":0.18,"5":0.2,"norm":{"1":0.0775,"15":0.045,"5":0.05}}}}}}
2020-11-27T13:36:41.808Z        INFO    [monitoring]    log/log.go:154  Uptime: 44.885003ms
2020-11-27T13:36:41.809Z        INFO    [monitoring]    log/log.go:131  Stopping metrics logging.
2020-11-27T13:36:41.809Z        INFO    instance/beat.go:469    machinebeat stopped.
2020-11-27T13:36:41.809Z        ERROR   instance/beat.go:958    Exiting: loading configs: 1 error: invalid config: config file ("/home/toby/machinebeat/modules.d/mqtt.yml") can only be writable by the owner but the permissions are "-rw-rw-r--" (to fix the permissions use: 'chmod go-w /home/toby/machinebeat/modules.d/mqtt.yml')
Exiting: loading configs: 1 error: invalid config: config file ("/home/toby/machinebeat/modules.d/mqtt.yml") can only be writable by the owner but the permissions are "-rw-rw-r--" (to fix the permissions use: 'chmod go-w /home/toby/machinebeat/modules.d/mqtt.yml')
toby@jet-gibbon:~/machinebeat$ sudo chmod go-w /home/toby/machinebeat/modules.d/mqtt.yml
toby@jet-gibbon:~/machinebeat$ sudo ./machinebeat -e -c machinebeat.yml
2020-11-27T13:36:52.965Z        INFO    instance/beat.go:647    Home path: [/home/toby/machinebeat] Config path: [/home/toby/machinebeat] Data path: [/home/toby/machinebeat/data] Logs path: [/home/toby/machinebeat/logs]
2020-11-27T13:36:52.966Z        INFO    instance/beat.go:655    Beat ID: d9e9a452-67a3-4a4c-b6af-4c68fb104487
2020-11-27T13:36:52.968Z        INFO    [seccomp]       seccomp/seccomp.go:124  Syscall filter successfully installed
2020-11-27T13:36:52.969Z        INFO    [beat]  instance/beat.go:983    Beat info       {"system_info": {"beat": {"path": {"config": "/home/toby/machinebeat", "data": "/home/toby/machinebeat/data", "home": "/home/toby/machinebeat", "logs": "/home/toby/machinebeat/logs"}, "type": "machinebeat", "uuid": "d9e9a452-67a3-4a4c-b6af-4c68fb104487"}}}
2020-11-27T13:36:52.969Z        INFO    [beat]  instance/beat.go:992    Build info      {"system_info": {"build": {"commit": "unknown", "libbeat": "7.8.1", "time": "0001-01-01T00:00:00.000Z", "version": "7.8.1"}}}
2020-11-27T13:36:52.969Z        INFO    [beat]  instance/beat.go:995    Go runtime info {"system_info": {"go": {"os":"linux","arch":"arm","max_procs":4,"version":"go1.15"}}}
2020-11-27T13:36:52.970Z        INFO    [beat]  instance/beat.go:999    Host info       {"system_info": {"host": {"architecture":"aarch64","boot_time":"2020-11-27T12:44:16Z","containerized":false,"name":"jet-gibbon","ip":["127.0.0.1/8","::1/128","192.168.0.72/24","fe80::924:af75:bbc1:c63c/64"],"kernel_version":"4.9.140-tegra","mac":["7e:10:2e:74:6b:62","00:04:4b:ec:60:ad","4c:1d:96:c7:d8:5f"],"os":{"family":"debian","platform":"ubuntu","name":"Ubuntu","version":"18.04.5 LTS (Bionic Beaver)","major":18,"minor":4,"patch":5,"codename":"bionic"},"timezone":"GMT","timezone_offset_sec":0,"id":"a3d9197b765643568af09eb2bd3e5ce7"}}}
2020-11-27T13:36:52.971Z        INFO    [beat]  instance/beat.go:1028   Process info    {"system_info": {"process": {"capabilities": {"inheritable":null,"permitted":["chown","dac_override","dac_read_search","fowner","fsetid","kill","setgid","setuid","setpcap","linux_immutable","net_bind_service","net_broadcast","net_admin","net_raw","ipc_lock","ipc_owner","sys_module","sys_rawio","sys_chroot","sys_ptrace","sys_pacct","sys_admin","sys_boot","sys_nice","sys_resource","sys_time","sys_tty_config","mknod","lease","audit_write","audit_control","setfcap","mac_override","mac_admin","syslog","wake_alarm","block_suspend","audit_read"],"effective":["chown","dac_override","dac_read_search","fowner","fsetid","kill","setgid","setuid","setpcap","linux_immutable","net_bind_service","net_broadcast","net_admin","net_raw","ipc_lock","ipc_owner","sys_module","sys_rawio","sys_chroot","sys_ptrace","sys_pacct","sys_admin","sys_boot","sys_nice","sys_resource","sys_time","sys_tty_config","mknod","lease","audit_write","audit_control","setfcap","mac_override","mac_admin","syslog","wake_alarm","block_suspend","audit_read"],"bounding":["chown","dac_override","dac_read_search","fowner","fsetid","kill","setgid","setuid","setpcap","linux_immutable","net_bind_service","net_broadcast","net_admin","net_raw","ipc_lock","ipc_owner","sys_module","sys_rawio","sys_chroot","sys_ptrace","sys_pacct","sys_admin","sys_boot","sys_nice","sys_resource","sys_time","sys_tty_config","mknod","lease","audit_write","audit_control","setfcap","mac_override","mac_admin","syslog","wake_alarm","block_suspend","audit_read"],"ambient":null}, "cwd": "/home/toby/machinebeat", "exe": "/home/toby/machinebeat/machinebeat", "name": "machinebeat", "pid": 11313, "ppid": 11312, "seccomp": {"mode":"filter"}, "start_time": "2020-11-27T13:36:52.610Z"}}}
2020-11-27T13:36:52.971Z        INFO    instance/beat.go:310    Setup Beat: machinebeat; Version: 7.8.1
2020-11-27T13:36:52.972Z        INFO    [index-management]      idxmgmt/std.go:184      Set output.elasticsearch.index to 'machinebeat-7.8.1' as ILM is enabled.
2020-11-27T13:36:52.972Z        INFO    eslegclient/connection.go:99    elasticsearch url: http://localhost:9200
2020-11-27T13:36:52.973Z        INFO    [publisher]     pipeline/module.go:113  Beat name: jet-gibbon
2020-11-27T13:36:52.973Z        INFO    [monitoring]    log/log.go:118  Starting metrics logging every 30s
2020-11-27T13:36:52.973Z        INFO    instance/beat.go:463    machinebeat start running.
2020-11-27T13:36:52.974Z        WARN    [cfgwarn]       topic/mqtt.go:55        BETA: The MQTT metricset is beta.
2020-11-27T13:36:52.975Z        INFO    topic/client.go:72      [MQTT] Connect to broker URL: 192.168.0.63:1883
2020-11-27T13:36:54.218Z        INFO    topic/client.go:114     MQTT Client connected: true
2020-11-27T13:36:54.218Z        INFO    topic/client.go:196     Subscribe to home/tele/# with QoS 0
2020-11-27T13:36:54.221Z        INFO    cfgfile/reload.go:164   Config reloader started
2020-11-27T13:36:54.228Z        WARN    [cfgwarn]       topic/mqtt.go:55        BETA: The MQTT metricset is beta.
2020-11-27T13:36:54.230Z        INFO    topic/client.go:72      [MQTT] Connect to broker URL: 192.168.0.63:1883
2020-11-27T13:36:54.231Z        INFO    cfgfile/reload.go:224   Loading of config files completed.
2020-11-27T13:36:54.248Z        INFO    topic/client.go:128     Subscribed to configured topics
2020-11-27T13:36:55.970Z        INFO    [add_cloud_metadata]    add_cloud_metadata/add_cloud_metadata.go:89     add_cloud_metadata: hosting provider type not detected.
2020-11-27T13:36:56.977Z        INFO    [publisher_pipeline_output]     pipeline/output.go:144  Connecting to backoff(elasticsearch(http://localhost:9200))
2020-11-27T13:36:56.979Z        INFO    [publisher]     pipeline/retry.go:221   retryer: send unwait signal to consumer
2020-11-27T13:36:56.980Z        INFO    [publisher]     pipeline/retry.go:225     done
2020-11-27T13:36:57.047Z        INFO    [esclientleg]   eslegclient/connection.go:314   Attempting to connect to Elasticsearch version 7.10.0
2020-11-27T13:36:57.055Z        INFO    [esclientleg]   eslegclient/connection.go:314   Attempting to connect to Elasticsearch version 7.10.0
2020-11-27T13:36:57.162Z        INFO    [index-management]      idxmgmt/std.go:261      Auto ILM enable success.
2020-11-27T13:36:57.753Z        INFO    [index-management]      idxmgmt/std.go:274      ILM policy successfully loaded.
2020-11-27T13:36:57.754Z        INFO    [index-management]      idxmgmt/std.go:407      Set setup.template.name to '{machinebeat-7.8.1 {now/d}-000001}' as ILM is enabled.
2020-11-27T13:36:57.754Z        INFO    [index-management]      idxmgmt/std.go:412      Set setup.template.pattern to 'machinebeat-7.8.1-*' as ILM is enabled.
2020-11-27T13:36:57.754Z        INFO    [index-management]      idxmgmt/std.go:446      Set settings.index.lifecycle.rollover_alias in template to {machinebeat-7.8.1 {now/d}-000001} as ILM is enabled.
2020-11-27T13:36:57.754Z        INFO    [index-management]      idxmgmt/std.go:450      Set settings.index.lifecycle.name in template to {machinebeat {"policy":{"phases":{"hot":{"actions":{"rollover":{"max_age":"30d","max_size":"50gb"}}}}}}} as ILM is enabled.
2020-11-27T13:36:57.771Z        INFO    template/load.go:169    Existing template will be overwritten, as overwrite is enabled.
2020-11-27T13:36:57.771Z        INFO    template/load.go:109    Try loading template machinebeat-7.8.1 to Elasticsearch
2020-11-27T13:36:58.367Z        INFO    template/load.go:101    template with name 'machinebeat-7.8.1' loaded.
2020-11-27T13:36:58.367Z        INFO    [index-management]      idxmgmt/std.go:298      Loaded index template.
2020-11-27T13:37:00.466Z        INFO    [index-management]      idxmgmt/std.go:309      Write alias successfully generated.
2020-11-27T13:37:00.474Z        INFO    [publisher_pipeline_output]     pipeline/output.go:152  Connection to backoff(elasticsearch(http://localhost:9200)) established
2020-11-27T13:37:22.978Z        INFO    [monitoring]    log/log.go:145  Non-zero metrics in the last 30s        {"monitoring": {"metrics": {"beat":{"cpu":{"system":{"ticks":490,"time":{"ms":496}},"total":{"ticks":950,"time":{"ms":956},"value":950},"user":{"ticks":460,"time":{"ms":460}}},"handles":{"limit":{"hard":1048576,"soft":1024},"open":10},"info":{"ephemeral_id":"d3a5a128-3840-490c-9aa6-4d76421bd57b","uptime":{"ms":30044}},"memstats":{"gc_next":4545056,"memory_alloc":2653544,"memory_total":10506264,"rss":33816576},"runtime":{"goroutines":26}},"libbeat":{"config":{"module":{"running":0},"reloads":1,"scans":1},"output":{"events":{"acked":146,"batches":16,"total":146},"type":"elasticsearch"},"pipeline":{"clients":1,"events":{"active":0,"published":146,"retry":8,"total":146},"queue":{"acked":146}}},"metricbeat":{"mqtt":{"topic":{"events":146,"success":146}}},"system":{"cpu":{"cores":4},"load":{"1":0.47,"15":0.2,"5":0.25,"norm":{"1":0.1175,"15":0.05,"5":0.0625}}}}}}
2020-11-27T13:37:52.978Z        INFO    [monitoring]    log/log.go:145  Non-zero metrics in the last 30s        {"monitoring": {"metrics": {"beat":{"cpu":{"system":{"ticks":580,"time":{"ms":96}},"total":{"ticks":1180,"time":{"ms":244},"value":1180},"user":{"ticks":600,"time":{"ms":148}}},"handles":{"limit":{"hard":1048576,"soft":1024},"open":10},"info":{"ephemeral_id":"d3a5a128-3840-490c-9aa6-4d76421bd57b","uptime":{"ms":60045}},"memstats":{"gc_next":4740944,"memory_alloc":3706488,"memory_total":14797432,"rss":65536},"runtime":{"goroutines":26}},"libbeat":{"config":{"module":{"running":0}},"output":{"events":{"acked":152,"batches":18,"total":152}},"pipeline":{"clients":1,"events":{"active":0,"published":152,"total":152},"queue":{"acked":152}}},"metricbeat":{"mqtt":{"topic":{"events":152,"success":152}}},"system":{"load":{"1":0.45,"15":0.21,"5":0.27,"norm":{"1":0.1125,"15":0.0525,"5":0.0675}}}}}}
2020-11-27T13:38:22.978Z        INFO    [monitoring]    log/log.go:145  Non-zero metrics in the last 30s        {"monitoring": {"metrics": {"beat":{"cpu":{"system":{"ticks":660,"time":{"ms":72}},"total":{"ticks":1400,"time":{"ms":208},"value":1400},"user":{"ticks":740,"time":{"ms":136}}},"handles":{"limit":{"hard":1048576,"soft":1024},"open":10},"info":{"ephemeral_id":"d3a5a128-3840-490c-9aa6-4d76421bd57b","uptime":{"ms":90044}},"memstats":{"gc_next":5103984,"memory_alloc":2696416,"memory_total":18984784,"rss":126976},"runtime":{"goroutines":26}},"libbeat":{"config":{"module":{"running":0}},"output":{"events":{"acked":152,"batches":18,"total":152}},"pipeline":{"clients":1,"events":{"active":2,"published":154,"total":154},"queue":{"acked":152}}},"metricbeat":{"mqtt":{"topic":{"events":154,"success":154}}},"system":{"load":{"1":0.27,"15":0.2,"5":0.24,"norm":{"1":0.0675,"15":0.05,"5":0.06}}}}}}
2020-11-27T13:38:52.978Z        INFO    [monitoring]    log/log.go:145  Non-zero metrics in the last 30s        {"monitoring": {"metrics": {"beat":{"cpu":{"system":{"ticks":700,"time":{"ms":36}},"total":{"ticks":1590,"time":{"ms":184},"value":1590},"user":{"ticks":890,"time":{"ms":148}}},"handles":{"limit":{"hard":1048576,"soft":1024},"open":10},"info":{"ephemeral_id":"d3a5a128-3840-490c-9aa6-4d76421bd57b","uptime":{"ms":120043}},"memstats":{"gc_next":5379488,"memory_alloc":3391872,"memory_total":23493536,"rss":36864},"runtime":{"goroutines":26}},"libbeat":{"config":{"module":{"running":0}},"output":{"events":{"acked":151,"batches":20,"total":151}},"pipeline":{"clients":1,"events":{"active":5,"published":154,"total":154},"queue":{"acked":151}}},"metricbeat":{"mqtt":{"topic":{"events":154,"success":154}}},"system":{"load":{"1":0.22,"15":0.2,"5":0.23,"norm":{"1":0.055,"15":0.05,"5":0.0575}}}}}}
2020-11-27T13:39:22.978Z        INFO    [monitoring]    log/log.go:145  Non-zero metrics in the last 30s        {"monitoring": {"metrics": {"beat":{"cpu":{"system":{"ticks":780,"time":{"ms":84}},"total":{"ticks":1820,"time":{"ms":232},"value":1820},"user":{"ticks":1040,"time":{"ms":148}}},"handles":{"limit":{"hard":1048576,"soft":1024},"open":10},"info":{"ephemeral_id":"d3a5a128-3840-490c-9aa6-4d76421bd57b","uptime":{"ms":150043}},"memstats":{"gc_next":5586768,"memory_alloc":3739664,"memory_total":28070264,"rss":20480},"runtime":{"goroutines":26}},"libbeat":{"config":{"module":{"running":0}},"output":{"events":{"acked":159,"batches":20,"total":159}},"pipeline":{"clients":1,"events":{"active":0,"published":154,"total":154},"queue":{"acked":159}}},"metricbeat":{"mqtt":{"topic":{"events":154,"success":154}}},"system":{"load":{"1":0.13,"15":0.19,"5":0.21,"norm":{"1":0.0325,"15":0.0475,"5":0.0525}}}}}}
2020-11-27T13:39:52.978Z        INFO    [monitoring]    log/log.go:145  Non-zero metrics in the last 30s        {"monitoring": {"metrics": {"beat":{"cpu":{"system":{"ticks":840,"time":{"ms":56}},"total":{"ticks":2020,"time":{"ms":200},"value":2020},"user":{"ticks":1180,"time":{"ms":144}}},"handles":{"limit":{"hard":1048576,"soft":1024},"open":10},"info":{"ephemeral_id":"d3a5a128-3840-490c-9aa6-4d76421bd57b","uptime":{"ms":180044}},"memstats":{"gc_next":5865488,"memory_alloc":3538968,"memory_total":32174096,"rss":245760},"runtime":{"goroutines":26}},"libbeat":{"config":{"module":{"running":0}},"output":{"events":{"acked":147,"batches":17,"total":147}},"pipeline":{"clients":1,"events":{"active":4,"published":151,"total":151},"queue":{"acked":147}}},"metricbeat":{"mqtt":{"topic":{"events":151,"success":151}}},"system":{"load":{"1":0.08,"15":0.18,"5":0.19,"norm":{"1":0.02,"15":0.045,"5":0.0475}}}}}}
```


check contents

toby@jet-gibbon:~$ curl -X GET 'http://localhost:9200/_cat/indices/'
yellow open machinebeat-7.8.1-2020.11.27-000001 qXYeheF-SMOmnifdmASLvg 1 1 174 0 300.2kb 300.2kb
toby@jet-gibbon:~$ curl -X GET 'http://localhost:9200/_cat/indices/'
yellow open machinebeat-7.8.1-2020.11.27-000001 qXYeheF-SMOmnifdmASLvg 1 1 174 0 344.7kb 344.7kb


get results

toby@jet-gibbon:~$ curl -X GET 'http://localhost:9200/machinebeat-7.8.1-2020.11.27-000001/_search'
{"took":723,"timed_out":false,"_shards":{"total":1,"successful":1,"skipped":0,"failed":0},"hits":{"total":{"value":1888,"relation":"eq"},"max_score":1.0,"hits":[{"_index":"machinebeat-7.8.1-2020.11.27-000001","_type":"_doc","_id":"KaTrCXYBlL2Gekkr2jdb","_score":1.0,"_source":{"@timestamp":"2020-11-27T13:36:55.232Z","host":{"id":"a3d9197b765643568af09eb2bd3e5ce7","name":"jet-gibbon","containerized":false,"ip":["192.168.0.72","fe80::924:af75:bbc1:c63c"],"mac":["7e:10:2e:74:6b:62","00:04:4b:ec:60:ad","4c:1d:96:c7:d8:5f"],"hostname":"jet-gibbon","architecture":"aarch64","os":{"family":"debian","name":"Ubuntu","kernel":"4.9.140-tegra","codename":"bionic","platform":"ubuntu","version":"18.04.5 LTS (Bionic Beaver)"}},"agent":{"ephemeral_id":"d3a5a128-3840-490c-9aa6-4d76421bd57b","id":"d9e9a452-67a3-4a4c-b6af-4c68fb104487","name":"jet-gibbon","type":"machinebeat","version":"7.8.1","hostname":"jet-gibbon"},"ecs":{"version":"1.5.0"},"metricset":{"name":"topic","period":1000},"mqtt":{"message":{"content":"3","fields":{},"ID":0,"retained":true},"broker":"192.168.0.63:1883","isSystemTopic":false,"topic":"home/tele/soilmoisture/livingroom/yucca"},"service":{"type":"mqtt"},"event":{"duration":120269,"dataset":"mqtt.topic","module":"mqtt"}}},{"_index":"machinebeat-7.8.1-2020.11.27-000001","_type":"_doc","_id":"KqTrCXYBlL2Gekkr2jde","_score":1.0,"_source":{"@timestamp":"2020-11-27T13:36:55.232Z","event":{"dataset":"mqtt.topic","module":"mqtt","duration":819069},"metricset":{"name":"topic","period":1000},"mqtt":{"topic":"home/tele/soilmoisture/livingroom/yucca","message":{"retained":false,"content":"822","fields":{},"ID":0},"broker":"192.168.0.63:1883","isSystemTopic":false},"service":{"type":"mqtt"},"ecs":{"version":"1.5.0"},"host":{"mac":["7e:10:2e:74:6b:62","00:04:4b:ec:60:ad","4c:1d:96:c7:d8:5f"],"hostname":"jet-gibbon","architecture":"aarch64","name":"jet-gibbon","os":{"version":"18.04.5 LTS (Bionic Beaver)","family":"debian","name":"Ubuntu","kernel":"4.9.140-tegra","codename":"bionic","platform":"ubuntu"},"id":"a3d9197b765643568af09eb2bd3e5ce7","containerized":false,"ip":["192.168.0.72","fe80::924:af75:bbc1:c63c"]},"agent":{"ephemeral_id":"d3a5a128-3840-490c-9aa6-4d76421bd57b","id":"d9e9a452-67a3-4a4c-b6af-4c68fb104487","name":"jet-gibbon","type":"machinebeat","version":"7.8.1","hostname":"jet-gibbon"}}},{"_index":"machinebeat-7.8.1-2020.11.27-000001","_type":"_doc","_id":"K6TrCXYBlL2Gekkr2jdf","_score":1.0,"_source":{"@timestamp":"2020-11-27T13:36:55.232Z","ecs":{"version":"1.5.0"},"host":{"mac":["7e:10:2e:74:6b:62","00:04:4b:ec:60:ad","4c:1d:96:c7:d8:5f"],"hostname":"jet-gibbon","architecture":"aarch64","os":{"kernel":"4.9.140-tegra","codename":"bionic","platform":"ubuntu","version":"18.04.5 LTS (Bionic Beaver)","family":"debian","name":"Ubuntu"},"name":"jet-gibbon","id":"a3d9197b765643568af09eb2bd3e5ce7","containerized":false,"ip":["192.168.0.72","fe80::924:af75:bbc1:c63c"]},"event":{"dataset":"mqtt.topic","module":"mqtt","duration":1434374},"metricset":{"name":"topic","period":1000},"mqtt":{"isSystemTopic":false,"topic":"home/tele/temperature/livingroom/yucca","message":{"ID":0,"retained":false,"content":"14.62","fields":{}},"broker":"192.168.0.63:1883"},"service":{"type":"mqtt"},"agent":{"name":"jet-gibbon","type":"machinebeat","version":"7.8.1","hostname":"jet-gibbon","ephemeral_id":"d3a5a128-3840-490c-9aa6-4d76421bd57b","id":"d9e9a452-67a3-4a4c-b6af-4c68fb104487"}}},{"_index":"machinebeat-7.8.1-2020.11.27-000001","_type":"_doc","_id":"LKTrCXYBlL2Gekkr2jdf","_score":1.0,"_source":{"@timestamp":"2020-11-27T13:36:55.232Z","agent":{"type":"machinebeat","version":"7.8.1","hostname":"jet-gibbon","ephemeral_id":"d3a5a128-3840-490c-9aa6-4d76421bd57b","id":"d9e9a452-67a3-4a4c-b6af-4c68fb104487","name":"jet-gibbon"},"event":{"duration":741558945,"dataset":"mqtt.topic","module":"mqtt"},"metricset":{"period":1000,"name":"topic"},"mqtt":{"isSystemTopic":false,"topic":"home/tele/temperature/livingroom/aralia","message":{"ID":0,"retained":false,"content":"17.38","fields":{}},"broker":"192.168.0.63:1883"},"service":{"type":"mqtt"},"ecs":{"version":"1.5.0"},"host":{"ip":["192.168.0.72","fe80::924:af75:bbc1:c63c"],"mac":["7e:10:2e:74:6b:62","00:04:4b:ec:60:ad","4c:1d:96:c7:d8:5f"],"name":"jet-gibbon","hostname":"jet-gibbon","architecture":"aarch64","os":{"kernel":"4.9.140-tegra","codename":"bionic","platform":"ubuntu","version":"18.04.5 LTS (Bionic Beaver)","family":"debian","name":"Ubuntu"},"id":"a3d9197b765643568af09eb2bd3e5ce7","containerized":false}}},{"_index":"machinebeat-7.8.1-2020.11.27-000001","_type":"_doc","_id":"LaTrCXYBlL2Gekkr2jdf","_score":1.0,"_source":{"@timestamp":"2020-11-27T13:36:55.232Z","mqtt":{"isSystemTopic":false,"topic":"home/tele/present/livingroom/desk","message":{"content":"1","fields":{},"ID":0,"retained":false},"broker":"192.168.0.63:1883"},"service":{"type":"mqtt"},"event":{"module":"mqtt","duration":741682131,"dataset":"mqtt.topic"},"ecs":{"version":"1.5.0"},"host":{"architecture":"aarch64","os":{"family":"debian","name":"Ubuntu","kernel":"4.9.140-tegra","codename":"bionic","platform":"ubuntu","version":"18.04.5 LTS (Bionic Beaver)"},"id":"a3d9197b765643568af09eb2bd3e5ce7","containerized":false,"name":"jet-gibbon","ip":["192.168.0.72","fe80::924:af75:bbc1:c63c"],"mac":["7e:10:2e:74:6b:62","00:04:4b:ec:60:ad","4c:1d:96:c7:d8:5f"],"hostname":"jet-gibbon"},"agent":{"type":"machinebeat","version":"7.8.1","hostname":"jet-gibbon","ephemeral_id":"d3a5a128-3840-490c-9aa6-4d76421bd57b","id":"d9e9a452-67a3-4a4c-b6af-4c68fb104487","name":"jet-gibbon"},"metricset":{"name":"topic","period":1000}}},{"_index":"machinebeat-7.8.1-2020.11.27-000001","_type":"_doc","_id":"LqTrCXYBlL2Gekkr2jdf","_score":1.0,"_source":{"@timestamp":"2020-11-27T13:36:55.232Z","service":{"type":"mqtt"},"metricset":{"name":"topic","period":1000},"event":{"duration":741757918,"dataset":"mqtt.topic","module":"mqtt"},"mqtt":{"topic":"home/tele/proximityVoltage/livingroom/desk","message":{"fields":{},"ID":0,"retained":false,"content":"0.7043"},"broker":"192.168.0.63:1883","isSystemTopic":false},"ecs":{"version":"1.5.0"},"host":{"name":"jet-gibbon","mac":["7e:10:2e:74:6b:62","00:04:4b:ec:60:ad","4c:1d:96:c7:d8:5f"],"hostname":"jet-gibbon","architecture":"aarch64","os":{"name":"Ubuntu","kernel":"4.9.140-tegra","codename":"bionic","platform":"ubuntu","version":"18.04.5 LTS (Bionic Beaver)","family":"debian"},"id":"a3d9197b765643568af09eb2bd3e5ce7","containerized":false,"ip":["192.168.0.72","fe80::924:af75:bbc1:c63c"]},"agent":{"name":"jet-gibbon","type":"machinebeat","version":"7.8.1","hostname":"jet-gibbon","ephemeral_id":"d3a5a128-3840-490c-9aa6-4d76421bd57b","id":"d9e9a452-67a3-4a4c-b6af-4c68fb104487"}}},{"_index":"machinebeat-7.8.1-2020.11.27-000001","_type":"_doc","_id":"L6TrCXYBlL2Gekkr2jdf","_score":1.0,"_source":{"@timestamp":"2020-11-27T13:36:56.233Z","metricset":{"name":"topic","period":1000},"mqtt":{"isSystemTopic":false,"topic":"home/tele/present/livingroom/desk","message":{"ID":0,"retained":false,"content":"1","fields":{}},"broker":"192.168.0.63:1883"},"service":{"type":"mqtt"},"ecs":{"version":"1.5.0"},"host":{"containerized":false,"ip":["192.168.0.72","fe80::924:af75:bbc1:c63c"],"mac":["7e:10:2e:74:6b:62","00:04:4b:ec:60:ad","4c:1d:96:c7:d8:5f"],"hostname":"jet-gibbon","architecture":"aarch64","os":{"platform":"ubuntu","version":"18.04.5 LTS (Bionic Beaver)","family":"debian","name":"Ubuntu","kernel":"4.9.140-tegra","codename":"bionic"},"id":"a3d9197b765643568af09eb2bd3e5ce7","name":"jet-gibbon"},"agent":{"hostname":"jet-gibbon","ephemeral_id":"d3a5a128-3840-490c-9aa6-4d76421bd57b","id":"d9e9a452-67a3-4a4c-b6af-4c68fb104487","name":"jet-gibbon","type":"machinebeat","version":"7.8.1"},"event":{"dataset":"mqtt.topic","module":"mqtt","duration":709218}}},{"_index":"machinebeat-7.8.1-2020.11.27-000001","_type":"_doc","_id":"MKTrCXYBlL2Gekkr2jdf","_score":1.0,"_source":{"@timestamp":"2020-11-27T13:36:56.233Z","service":{"type":"mqtt"},"ecs":{"version":"1.5.0"},"host":{"containerized":false,"ip":["192.168.0.72","fe80::924:af75:bbc1:c63c"],"mac":["7e:10:2e:74:6b:62","00:04:4b:ec:60:ad","4c:1d:96:c7:d8:5f"],"hostname":"jet-gibbon","name":"jet-gibbon","architecture":"aarch64","os":{"family":"debian","name":"Ubuntu","kernel":"4.9.140-tegra","codename":"bionic","platform":"ubuntu","version":"18.04.5 LTS (Bionic Beaver)"},"id":"a3d9197b765643568af09eb2bd3e5ce7"},"agent":{"version":"7.8.1","hostname":"jet-gibbon","ephemeral_id":"d3a5a128-3840-490c-9aa6-4d76421bd57b","id":"d9e9a452-67a3-4a4c-b6af-4c68fb104487","name":"jet-gibbon","type":"machinebeat"},"event":{"dataset":"mqtt.topic","module":"mqtt","duration":1214046},"metricset":{"name":"topic","period":1000},"mqtt":{"broker":"192.168.0.63:1883","isSystemTopic":false,"topic":"home/tele/proximityVoltage/livingroom/desk","message":{"content":"0.7043","fields":{},"ID":0,"retained":false}}}},{"_index":"machinebeat-7.8.1-2020.11.27-000001","_type":"_doc","_id":"MqTrCXYBlL2Gekkr4Dcr","_score":1.0,"_source":{"@timestamp":"2020-11-27T13:36:57.232Z","ecs":{"version":"1.5.0"},"service":{"type":"mqtt"},"event":{"duration":13699,"dataset":"mqtt.topic","module":"mqtt"},"metricset":{"period":1000,"name":"topic"},"mqtt":{"topic":"home/tele/temperature/livingroom/window","message":{"content":"18.63","fields":{},"ID":0,"retained":false},"broker":"192.168.0.63:1883","isSystemTopic":false},"host":{"containerized":false,"ip":["192.168.0.72","fe80::924:af75:bbc1:c63c"],"mac":["7e:10:2e:74:6b:62","00:04:4b:ec:60:ad","4c:1d:96:c7:d8:5f"],"name":"jet-gibbon","hostname":"jet-gibbon","architecture":"aarch64","os":{"codename":"bionic","platform":"ubuntu","version":"18.04.5 LTS (Bionic Beaver)","family":"debian","name":"Ubuntu","kernel":"4.9.140-tegra"},"id":"a3d9197b765643568af09eb2bd3e5ce7"},"agent":{"type":"machinebeat","version":"7.8.1","hostname":"jet-gibbon","ephemeral_id":"d3a5a128-3840-490c-9aa6-4d76421bd57b","id":"d9e9a452-67a3-4a4c-b6af-4c68fb104487","name":"jet-gibbon"}}},{"_index":"machinebeat-7.8.1-2020.11.27-000001","_type":"_doc","_id":"M6TrCXYBlL2Gekkr4Dcr","_score":1.0,"_source":{"@timestamp":"2020-11-27T13:36:57.232Z","service":{"type":"mqtt"},"ecs":{"version":"1.5.0"},"host":{"architecture":"aarch64","os":{"codename":"bionic","platform":"ubuntu","version":"18.04.5 LTS (Bionic Beaver)","family":"debian","name":"Ubuntu","kernel":"4.9.140-tegra"},"id":"a3d9197b765643568af09eb2bd3e5ce7","containerized":false,"ip":["192.168.0.72","fe80::924:af75:bbc1:c63c"],"mac":["7e:10:2e:74:6b:62","00:04:4b:ec:60:ad","4c:1d:96:c7:d8:5f"],"name":"jet-gibbon","hostname":"jet-gibbon"},"agent":{"type":"machinebeat","version":"7.8.1","hostname":"jet-gibbon","ephemeral_id":"d3a5a128-3840-490c-9aa6-4d76421bd57b","id":"d9e9a452-67a3-4a4c-b6af-4c68fb104487","name":"jet-gibbon"},"event":{"dataset":"mqtt.topic","module":"mqtt","duration":142667},"metricset":{"period":1000,"name":"topic"},"mqtt":{"broker":"192.168.0.63:1883","isSystemTopic":false,"topic":"home/tele/humidity/livingroom/window","message":{"content":"74.64","fields":{},"ID":0,"retained":false}}}}]}}



running nicely, make into a startup service




{
                "_index": "machinebeat-7.8.1-2020.11.27-000001",
                "_type": "_doc",
                "_id": "M6TrCXYBlL2Gekkr4Dcr",
                "_score": 1.0,
                "_source": {
                    "@timestamp": "2020-11-27T13:36:57.232Z",
                    "service": {
                        "type": "mqtt"
                    },
                    "ecs": {
                        "version": "1.5.0"
                    },
                    "host": {
                        "architecture": "aarch64",
                        "os": {
                            "codename": "bionic",
                            "platform": "ubuntu",
                            "version": "18.04.5 LTS (Bionic Beaver)",
                            "family": "debian",
                            "name": "Ubuntu",
                            "kernel": "4.9.140-tegra"
                        },
                        "id": "a3d9197b765643568af09eb2bd3e5ce7",
                        "containerized": false,
                        "ip": [
                            "192.168.0.72",
                            "fe80::924:af75:bbc1:c63c"
                        ],
                        "mac": [
                            "7e:10:2e:74:6b:62",
                            "00:04:4b:ec:60:ad",
                            "4c:1d:96:c7:d8:5f"
                        ],
                        "name": "jet-gibbon",
                        "hostname": "jet-gibbon"
                    },
                    "agent": {
                        "type": "machinebeat",
                        "version": "7.8.1",
                        "hostname": "jet-gibbon",
                        "ephemeral_id": "d3a5a128-3840-490c-9aa6-4d76421bd57b",
                        "id": "d9e9a452-67a3-4a4c-b6af-4c68fb104487",
                        "name": "jet-gibbon"
                    },
                    "event": {
                        "dataset": "mqtt.topic",
                        "module": "mqtt",
                        "duration": 142667
                    },
                    "metricset": {
                        "period": 1000,
                        "name": "topic"
                    },
                    "mqtt": {
                        "broker": "192.168.0.63:1883",
                        "isSystemTopic": false,
                        "topic": "home/tele/humidity/livingroom/window",
                        "message": {
                            "content": "74.64",
                            "fields": {},
                            "ID": 0,
                            "retained": false
                        }
                    }
                }
            }




toby@jet-gibbon:~$ curl -X GET 'http://localhost:9200/machinebeat-7.8.1-2020.11.27-000001/_count'{"count":9167,"_shards":{"total":1,"successful":1,"skipped":0,"failed":0}}

curl -X GET 'http://localhost:9200/machinebeat-7.8.18*/_count'

target installation directory

https://askubuntu.com/questions/6897/where-to-install-programs

/usr/local

create sudo /usr/local/services

/usr/local

 sudo cp -r  machinebeat /usr/local/services/



sudo /usr/local/services/machinebeat/machinebeat -e -c machinebeat.yml


create as a service:

/usr/local/services/machinebeat/machinebeat -e -c /usr/local/services/machinebeat/machinebeat.yml


## ports for elastic

see if port range is blocked


sudo systemctl start elasticsearch.service
sudo systemctl stop elasticsearch.service
sudo systemctl restart elasticsearch.service
sudo systemctl status elasticsearch.service

sudo docker run -dit --name not-elastic -p 9200:80 -v /home/user/website/:/usr/local/apache2/htdocs/ httpd:2.4

curl http://localhost:9200/docker.html

```
toby@jet-gibbon:~/machinebeat$ curl http://localhost:9200/docker.html
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<html>
 <head>
  <title>Docker demo</title>
 </head>
 <body>
<h1>Hello from Docker/</h1>
<ul></ul>
</body></html>
```

http://192.168.0.72:9200/docker.html


works externally

http://192.168.0.72:9200/

now works locally and from laptop


## Kibana

sudo apt install kibana


Elastic:
discovery.type: single-node
transport.tcp.port: 9300
http.host: 0.0.0.0
http.port: 9200
transport.host: localhost
network.host: 0.0.0.0

discovery.type: single-node
http.host: 0.0.0.0
transport.host: localhost
network.host: 0.0.0.0







cd kibana-7.10.0-linux-aarch64/
bin/kibana

kibana-7.10.0-linux-aarch64/bin/kibana

shift and run as a service


192.168.0.72

## Index Pattern in Kibana
MqttValue
return Float.parseFloat(doc['mqtt.message.content.keyword'].value)

