
# spark-submit --packages org.apache.spark:spark-sql-kafka-0-10_2.12:3.5.5,org.mongodb.spark:mongo-spark-connector_2.12:10.4.1 writeToMongodb.py

import pyspark.sql.functions as F
from pyspark.sql.types import StructType, StructField, StringType, IntegerType, FloatType, TimestampType, DateType
from pyspark.sql import SparkSession
import os


kafkaBootstrapServers = "kafka:9092"
topic="flights"

# kafkaBootstrapServers = "192.168.0.46:9092"
# topic="flights_jetson"


outputFolder = os.path.join(os.getcwd(), "spark_output_mongo")

mongoDbUri = "mongodb://mongodb:27017"
mongoDbDatabase = "test"
mongoDbCollection = "flights"

spark =( SparkSession \
    .builder \
    .appName("Python Spark streaming example - MongoDB") \
    .config("spark.mongodb.write.connection.uri", mongoDbUri) \
    .config("spark.mongodb.input.uri", f"{mongoDbUri}/{mongoDbDatabase}.{mongoDbCollection}") \
    .config("spark.mongodb.output.uri", f"{mongoDbUri}/{mongoDbDatabase}.{mongoDbCollection}") \
    .config("spark.mongodb.output.connecTimeoutMS", "10000") \
    .config("spark.mongodb.output.maxBatchSize", "1000") \
    .config("spark.mongodb.output.maxBatchSizeBytes", "1048576") \
    .getOrCreate())




print("Current Directory:", os.getcwd())

df =( spark \
  .readStream \
  .format("kafka") \
  .option("kafka.bootstrap.servers",kafkaBootstrapServers ) \
  .option("subscribe", topic) \
  .option("failOnDataLoss", 'false') \
  .load()
)

schema =( StructType([    StructField("icao", StringType(), True),    StructField("GS", IntegerType(), True),    StructField("updated", TimestampType(), True),    StructField("flightDate", DateType(), True),    StructField("altitude", IntegerType(), True), StructField("lat", FloatType(), True),    StructField("lon", FloatType(), True)]))


jsonOptions={}
flights = (df \
    .select(F.from_json(F.col("value").cast("string"), schema, jsonOptions).alias("p"), "Timestamp") \
    .select("p.icao","p.GS","p.updated","p.flightDate","p.altitude","p.lat", "p.lon", "Timestamp" ) \
)
flights.printSchema() 


(flights \
    .filter("altitude>30000")
   .writeStream \
  .format("mongodb") \
  .option("startingOffsets", "earliest") \
  .option("spark.mongodb.database", mongoDbDatabase) \
  .option("spark.mongodb.collection", mongoDbCollection) \
  .option("failOnDataLoss", 'false') \
  .option("checkpointLocation",os.path.join( outputFolder,"checkpoint")) \
  .outputMode("append") \
  .start() \
  .awaitTermination()
  )