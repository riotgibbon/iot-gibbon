# Neo4J

/share/CACHEDEV1_DATA/data/neo4j

user
sudo useradd -rs /bin/false neo4j

sudo chown neo4j:neo4j /share/CACHEDEV1_DATA/data/neo4j/*


cat /etc/passwd | grep neo4j

[~] # cat /etc/passwd | grep neo4j
neo4j:x:995:95::/share/homes/neo4j:/bin/false

https://neo4j.com/developer/docker/
https://neo4j.com/developer/docker-run-neo4j/


docker run \
    --name qnapneo4j \
    -p7474:7474 -p7687:7687 \
    --user 995:95  \
    -d \
    -v /share/CACHEDEV1_DATA/data/neo4j/data:/data \
    -v /share/CACHEDEV1_DATA/data/neo4j/logs:/logs \
    -v /share/CACHEDEV1_DATA/data/neo4j/import:/var/lib/neo4j/import \
    -v /share/CACHEDEV1_DATA/data/neo4j/plugins:/plugins \
    --env NEO4J_AUTH=neo4j/test \
    neo4j:latest
