
cd /Users/toby.evans/Documents/github/iot-gibbon/python/streamlit

create environment:
virtualenv .venv


activate:
. .venv/bin/activate

pip install -r setup/requirements.txt             


cd pollution 
streamlit run app.py   



docker build -t pollution .
docker run --env-file .env   -p 8052:8052  pollution:latest
