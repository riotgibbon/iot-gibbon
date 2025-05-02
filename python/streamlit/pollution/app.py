import streamlit as st
import pandas as pd
from datetime import datetime, timedelta
from urllib.error import URLError

from streamlit_date_picker import date_range_picker, date_picker, PickerType
import pydeck as pdk
import os

st.write("""
# Pollution Data
Hello *world!*
""")


# conn = st.connection("postgresql", type="sql")
conn = os.getenv('JDBC')

st.subheader('Date Range Picker')
st.markdown("### 1.Time Range Picker")
default_start, default_end = datetime.now() - timedelta(minutes=90), datetime.now()
refresh_value = timedelta(minutes=90)
date_range_string = date_range_picker(picker_type=PickerType.time,
                                      start=default_start, end=default_end,
                                      key='time_range_picker',
                                      refresh_button={'is_show': True, 'button_name': 'Refresh Last 90 Minutes',
                                                      'refresh_value': refresh_value})
if date_range_string:
    start, end = date_range_string
    st.write(f"Time Range Picker [{start}, {end}]")




# Perform query.
query =f"""
select to_char(((date_trunc('seconds', (tsz - timestamptz 'epoch') / 60) * 60 + timestamptz 'epoch') at time zone 'UTC'),'yyyy-mm-dd HH24:MI')   as ts,
avg(lat) as lat,
avg(lon) as lon,
st_point(avg(lon), avg(lat)) as geom,
concat('POINT (', cast(avg(lon) as varchar), ' ', cast(avg(lat) as varchar), ')') as wkt,
max(pm1) as max_pm1,
max(pm25) as max_pm25,
max(pm10) as max_pm10,
max(carbonmonoxide) as max_carbonmonoxide
from (select ts at TIME ZONE 'UTC' at time zone 'Europe/London' as tsz, * from pollution) t
where tsz BETWEEN '{start}' and '{end}'
group by 1
order by 1  
"""

df = conn.query(query)
print(query)


st.line_chart(df, x="ts",y=["max_pm1","max_pm25", "max_pm10"])

radius = st.slider("Radius", 0, 50, 200)
elevation_scale = st.slider("Elevation Scale", 0, 100, 4)
elevation_range = st.slider("Elevation Range", 0, 100, [0, 30])
extruded = st.checkbox("Extruded", True)

@st.cache_data
def from_data_file(filename):
    url = (
        "https://raw.githubusercontent.com/streamlit/"
        "example-data/master/hello/v1/%s" % filename
    )
    return pd.read_json(url)
tooltip = {
    "html": "{ts}, pm1: <b>{max_pm1}</b> µg/m3<br/>pm2.5: <b>{max_pm25}</b> µg/m3  pm10: <b>{max_pm10}</b> µg/m3<br/>Carbon Monoxide: <b>{max_carbonmonoxide}</b>",
    "style": {"background": "grey", "color": "white", "font-family": '"Helvetica Neue", Arial', "z-index": "10000"},
}
try:
    ALL_LAYERS = {
        "pm1": pdk.Layer(
            "ColumnLayer",
            data=df,
            get_position=["lon", "lat"],
            radius=radius,
            elevation_scale= elevation_scale,  
            get_elevation="max_pm1",
            elevation_range=elevation_range,
            get_fill_color=["max_pm1", 140],
            extruded=extruded,
            pickable=True,
    auto_highlight=True,
        ),
        "pm2.5": pdk.Layer(
            "ColumnLayer",
            data=df,
            get_position=["lon", "lat"],
            radius=radius,
            elevation_scale=   elevation_scale,  
            get_elevation="max_pm25",
            elevation_range=    elevation_range,
            get_fill_color=[255,255,178],
            extruded=extruded,
            pickable=True,
            auto_highlight=True,
        ),
        "pm10": pdk.Layer(
            "ColumnLayer",
            data=df,
            get_position=["lon", "lat"],
            radius=200,
            elevation_scale=    elevation_scale,  
            get_elevation="max_pm10",
            elevation_range=    elevation_range,
            get_fill_color=[189,0,38],
            extruded=extruded,
            pickable=True,
    auto_highlight=True,
        ),


        # ,
        # "Bart Stop Exits": pdk.Layer(
        #     "ScatterplotLayer",
        #     data=from_data_file("bart_stop_stats.json"),
        #     get_position=["lon", "lat"],
        #     get_color=[200, 30, 0, 160],
        #     get_radius="[exits]",
        #     radius_scale=0.05,
        # ),
        # "Bart Stop Names": pdk.Layer(
        #     "TextLayer",
        #     data=from_data_file("bart_stop_stats.json"),
        #     get_position=["lon", "lat"],
        #     get_text="name",
        #     get_color=[0, 0, 0, 200],
        #     get_size=10,
        #     get_alignment_baseline="'bottom'",
        # ),
        # "Outbound Flow": pdk.Layer(
        #     "ArcLayer",
        #     data=from_data_file("bart_path_stats.json"),
        #     get_source_position=["lon", "lat"],
        #     get_target_position=["lon2", "lat2"],
        #     get_source_color=[200, 30, 0, 160],
        #     get_target_color=[200, 30, 0, 160],
        #     auto_highlight=True,
        #     width_scale=0.0001,
        #     get_width="outbound",
        #     width_min_pixels=3,
        #     width_max_pixels=30,
        # ),
    }
    st.sidebar.markdown("### Map Layers")
    selected_layers = [
        layer
        for layer_name, layer in ALL_LAYERS.items()
        if st.sidebar.checkbox(layer_name, True)
    ]
    if selected_layers:
        st.pydeck_chart(
            pdk.Deck(
                map_style=None,
                initial_view_state={
                    "latitude": 51.49255,
                    "longitude": -0.632748,
                    "zoom": 11,
                    "pitch": 50,
                },
                tooltip=tooltip,
                layers=selected_layers,
            )
        )
    else:
        st.error("Please choose at least one layer above.")
except URLError as e:
    st.error(
        """
        **This demo requires internet access.**
        Connection error: %s
    """
        % e.reason
    )

    # BESJZT