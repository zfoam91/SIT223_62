# Use an official Ubuntu base image
FROM ubuntu:20.04

# Install necessary packages
RUN apt-get update && \
    apt-get install -y x11vnc xvfb libsfml-dev novnc && \
    apt-get clean

# Copy the compiled Minesweeper binary to the container
COPY minesweeper /app/minesweeper

# Set the display environment variable
ENV DISPLAY=:0

# Start the Xvfb server, set up VNC, and run the Minesweeper app
CMD Xvfb :0 -screen 0 1024x768x16 & \
    x11vnc -display :0 -forever -passwd simplepassword & \
    /app/minesweeper & \
    /usr/share/novnc/utils/launch.sh --vnc localhost:5900 --listen 8080
