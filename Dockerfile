# Use a lightweight base image
FROM ubuntu:20.04

# Set non-interactive installation
ENV DEBIAN_FRONTEND noninteractive

# Install SFML and X11 dependencies
RUN apt-get update && apt-get install -y \
    libsfml-graphics2.5 \
    libsfml-window2.5 \
    libsfml-system2.5 \
    firefox \
    libcanberra-gtk-module \
    libcanberra-gtk3-module \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory in the container
WORKDIR /app

# Copy the compiled Minesweeper binary and assets into the container
COPY tiles.png timer.png boom.png game.png ./
COPY ./minesweeper ./

# Make the binary executable
RUN chmod +x /app/minesweeper

# Command to run the application
CMD ["./minesweeper"]
