# Couch

https://hub.docker.com/_/couchdb

user
sudo useradd -rs /bin/false couchdb

sudo chown couchdb:couchdb /share/CACHEDEV1_DATA/data/couchdb/*

/share/CACHEDEV1_DATA/data/couchdb/config/

cat /etc/passwd | grep couchdb
couchdb:x:996:96::/share/homes/couchdb:/bin/false


config
/opt/couchdb/etc

-v /home/couchdb/etc:/opt/couchdb/etc/local.d
-v /share/CACHEDEV1_DATA/data/couchdb:/opt/couchdb/etc/local.d

data
/opt/couchdb/data

-v /home/couchdb/data:/opt/couchdb/data 
-v /share/CACHEDEV1_DATA/data/couchdb/data:/opt/couchdb/data 


docker run -d -p 5984:5984 --user 996:96  --name my-couchdb -v /share/CACHEDEV1_DATA/data/couchdb:/opt/couchdb/etc/local.d -v /share/CACHEDEV1_DATA/data/couchdb/data:/opt/couchdb/data -e COUCHDB_USER=admin -e COUCHDB_PASSWORD=password -d couchdb

http://192.168.0.46:5984/_utils/    