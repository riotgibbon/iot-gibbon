
# spark-submit --packages org.apache.spark:spark-sql-kafka-0-10_2.12:3.5.5 writeToCsv.py

import pyspark.sql.functions as F
from pyspark.sql.types import StructType, StructField, StringType, IntegerType, FloatType, TimestampType, DateType
from pyspark.sql import SparkSession
import os

spark =( SparkSession \
    .builder \
    .appName("Python Spark streaming example") \
    .getOrCreate())


kafkaBootstrapServers = "kafka:9092"
topic="flights"

basePath ="/output"

outputFolder = os.path.join(basePath, "spark_output_csv")

print("outputFolder:", outputFolder)

df =( spark \
  .readStream \
  .format("kafka") \
  .option("kafka.bootstrap.servers",kafkaBootstrapServers ) \
  .option("subscribe", topic) \
  .load()
)

schema =( StructType([    StructField("icao", StringType(), True),    StructField("GS", IntegerType(), True),    StructField("updated", TimestampType(), True),    StructField("flightDate", DateType(), True),    StructField("altitude", IntegerType(), True), StructField("lat", FloatType(), True),    StructField("lon", FloatType(), True)]))


jsonOptions={}
flights = (df \
    .select(F.from_json(F.col("value").cast("string"), schema, jsonOptions).alias("p")) \
    .select("p.icao","p.GS","p.updated","p.flightDate","p.altitude","p.lat", "p.lon" ) \
)
flights.printSchema() 


(flights \
    .filter("altitude>10000")
   .writeStream \
  .format("csv") \
  .option("startingOffsets", "earliest") \
  .option("path", os.path.join(outputFolder, "flights")) \
  .option("checkpointLocation",os.path.join( outputFolder,"checkpoint")) \
  .outputMode("append") \
  .start() \
  .awaitTermination()
  )