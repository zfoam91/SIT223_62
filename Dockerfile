# Use a lightweight base image
FROM ubuntu:20.04

# Set non-interactive installation
ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get install -y \
    libsfml-dev \
    x11-apps \
    xfce4 \
    xfce4-terminal \
    tightvncserver \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory in the container
WORKDIR /app

# Copy the compiled Minesweeper binary and assets into the container
COPY tiles.png timer.png boom.png game.png ./
COPY ./minesweeper ./

ENV USER=root
ENV HOME /root

# Set the display port for VNC
ENV DISPLAY :1

# Expose the VNC port
EXPOSE 5901

# Start the VNC server and run the Minesweeper application
CMD ["sh", "-c", "vncserver :1 -geometry 1024x768 -depth 24 && DISPLAY=:1 ./minesweeper"]
