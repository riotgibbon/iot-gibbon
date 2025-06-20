from pyspark.sql import SparkSession
import sys
host =sys.argv[1]

print(f"host: {host}")

spark =( SparkSession \
    .builder \
    .appName("mongoReader") \
    .config("spark.mongodb.read.connection.uri", f"mongodb://{host}/dubai.routes") \
    .getOrCreate()
)

df = spark.read.format("mongodb").load()

df.printSchema()
df.show()

df_colours = df.groupBy("route_color").count().orderBy("count", ascending=True)
df_colours.show()

