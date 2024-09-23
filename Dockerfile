# Use a lightweight base image
FROM ubuntu:20.04

# Install SFML dependencies
RUN apt-get update && apt-get install -y \
    x11-apps \
    libsfml-dev \
    && rm -rf /var/lib/apt/lists/*


# Replace 1000 with your user / group id
RUN export uid=1000 gid=1000 && \
    mkdir -p /home/developer && \
    echo "developer:x:${uid}:${gid}:Developer,,,:/home/developer:/bin/bash" >> /etc/passwd && \
    echo "developer:x:${uid}:" >> /etc/group && \
    echo "developer ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/developer && \
    chmod 0440 /etc/sudoers.d/developer && \
    chown ${uid}:${gid} -R /home/developer

USER developer
ENV HOME /home/developer

# Set the working directory in the container
WORKDIR /app

# Copy the compiled Minesweeper binary and assets into the container
COPY tiles.png timer.png boom.png game.png ./
COPY ./minesweeper ./

# Make the binary executable
RUN chmod +x /app/minesweeper

# Expose the port the app runs on (if applicable)
# EXPOSE 80

# Set the DISPLAY environment variable
ENV DISPLAY=:0

# Command to run the application
CMD ["./minesweeper"]