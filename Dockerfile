# Use a lightweight base image
FROM ubuntu:20.04

# Set non-interactive installation
ENV DEBIAN_FRONTEND noninteractive

# Install SFML dependencies
RUN apt-get update && apt-get install -y \
    libsfml-graphics2.5 \
    libsfml-window2.5 \
    libsfml-system2.5 \
    && rm -rf /var/lib/apt/lists/*

# Create a user for running the app
RUN useradd -ms /bin/bash xterm
USER xterm

# Set the working directory in the container
WORKDIR /home/xterm

# Set the DISPLAY environment variable
ENV DISPLAY=:99

# Copy the compiled Minesweeper binary and assets into the container
COPY tiles.png timer.png boom.png game.png ./
COPY ./minesweeper ./

# Make the binary executable
RUN chmod +x /home/xterm/minesweeper

# Command to run the application
CMD ["./minesweeper"]
