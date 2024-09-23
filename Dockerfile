FROM ubuntu:20.04
RUN apt-get update && apt-get install -y x11vnc xvfb libsfml-dev novnc
COPY minesweeper /app/minesweeper
COPY assets /app/assets
COPY novnc-index.html /index.html
ENV DISPLAY=:0
CMD Xvfb :0 -screen 0 1024x768x16 & \
    x11vnc -display :0 -forever -passwd ${VNC_PASSWORD} & \
    /app/minesweeper & \
    /usr/share/novnc/utils/launch.sh --vnc localhost:5900 --listen 8080
