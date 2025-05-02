
FROM python:3.10

COPY pollution .

RUN  pip3  install -r requirements.txt

ENV STREAMLIT_SERVER_PORT=8052

ENTRYPOINT ["streamlit", "run", "app.py","--logger.level=debug", "--server.address", "0.0.0.0"]
