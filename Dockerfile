FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    g++ \
    libsfml-dev

COPY . /app
WORKDIR /app

RUN g++ -O main.cpp lib.cpp -o minesweeper -lsfml-graphics -lsfml-window -lsfml-system

CMD ["./minesweeper"]
