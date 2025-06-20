
cd /Users/toby.evans/Documents/github/dexter/kafka/streamlit

create environment:
virtualenv .venv


activate:
. .venv/bin/activate

pip install -r setup/requirements.txt             



streamlit run app.py   




docker build -t kafkiz .
docker run --env-file .env   -p 8052:8052  pollution:latest
