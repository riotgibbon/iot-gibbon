# http://docs.streamlit.io/develop/tutorials/databases/mongodb

# streamlit_app.py

import streamlit as st
import pymongo

# Initialize connection.
# Uses st.cache_resource to only run once.
@st.cache_resource
def init_connection():
    return pymongo.MongoClient(**st.secrets["mongo"])

client = init_connection()

# Pull data from the collection.
# Uses st.cache_data to only rerun when the query changes or after 10 min.
@st.cache_data(ttl=5)
def get_data():
    db = client.test
    items = db.flights.find({}).sort("Timestamp", -1).limit(50)
    items = list(items)  # make hashable for st.cache_data
    return items

items = get_data()

st.map(items, size="altitude", zoom=3, use_container_width=True)
# Print results.
st.table(items)
# for item in items:
#     st.write(f"{item['name']} has a :{item['pet']}:")