# Use a lightweight base image
FROM ubuntu:20.04

# Set non-interactive installation
ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    libsfml-dev \
    x11-apps \
    libsfml-graphics2.5 \
    libsfml-window2.5 \
    libsfml-system2.5 \
    && rm -rf /var/lib/apt/lists/*


# Set the working directory in the container
WORKDIR /app

# Copy the compiled Minesweeper binary and assets into the container
COPY tiles.png timer.png boom.png game.png ./
COPY ./minesweeper ./

ENV DISPLAY=:0

# Command to run the application
CMD ["./minesweeper"]
