
# spark-submit --packages org.apache.spark:spark-sql-kafka-0-10_2.12:3.5.5  --driver-class-path ./jars/postgresql-9.4.1207.jar writeToPostgres.py

# spark-submit --master spark://192.168.0.46:7077 --packages org.apache.spark:spark-sql-kafka-0-10_2.12:3.5.5  --driver-class-path ./jars/postgresql-9.4.1207.jar writeToPostgres.py

import pyspark.sql.functions as F
from pyspark.sql.types import StructType, StructField, StringType, IntegerType, FloatType, TimestampType, DateType
from pyspark.sql import SparkSession
import os


# kafkaBootstrapServers = "localhost:9092"
# topic="flights_fake"

kafkaBootstrapServers = "192.168.0.46:9092"
topic="flights_jetson"
outputFolder = os.path.join(os.getcwd(), "spark_output_mongo")

mongoDbUri = "mongodb://localhost:27017"
mongoDbDatabase = "test"
mongoDbCollection = "flights"

spark =( SparkSession \
    .builder \
    .appName("Python Spark streaming example - PostgreSQL") \
    .getOrCreate())


df =( spark \
  .readStream \
  .format("kafka") \
  .option("kafka.bootstrap.servers",kafkaBootstrapServers ) \
  .option("subscribe", topic) \
  .option("failOnDataLoss", 'false') \
  .load()
)

schema =( StructType([    StructField("icao", StringType(), True), StructField("callsign", StringType(), True),    StructField("GS", IntegerType(), True),    StructField("updated", TimestampType(), True),    StructField("flightDate", DateType(), True),    StructField("altitude", IntegerType(), True), StructField("lat", FloatType(), True),    StructField("lon", FloatType(), True)]))


jsonOptions={}
flights = (df \
    .select(F.from_json(F.col("value").cast("string"), schema, jsonOptions).alias("p"), "Timestamp") \
    .selectExpr("p.icao","p.altitude","p.lat", "p.lon","p.updated","p.flightDate", "p.GS as speed" , "p.callsign") \
)
flights.printSchema() 

def _write_streaming(
    df,
    epoch_id
) -> None:         

    (df.write \
        .mode('append') \
        .format("jdbc") \
        .option("url", f"jdbc:postgresql://localhost:5432/postgres") \
        .option("driver", "org.postgresql.Driver") \
        .option("dbtable", 'flights') \
        .option("user", 'timescaledb') \
        .option("password", 'password') \
        .save() )

(flights
    # .filter("altitude>30000")
    .writeStream \
    .foreachBatch(_write_streaming) \
    .start() \
    .awaitTermination()
)