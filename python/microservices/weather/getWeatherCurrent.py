import requests
import json

url = "https://rapidapi.p.rapidapi.com/weather"

querystring = {"q":"Slough,uk","lat":"0","lon":"0","id":"2172797","lang":"null","units":"metric","mode":"xml, html"}

headers = {
    'x-rapidapi-key': "b15a25b44cmsh2606123013f2f9dp1a6645jsn39377a256bc2",
    'x-rapidapi-host': "community-open-weather-map.p.rapidapi.com"
    }

response = requests.request("GET", url, headers=headers, params=querystring)


print(response.text)

weather = json.loads(response.text)

readings = weather['main']
print(readings['temp'])
print(readings['pressure'])
print(readings['humidity'])