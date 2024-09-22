# Use a lightweight base image
FROM ubuntu:20.04

# Install SFML dependencies
RUN apt-get update && apt-get install -y \
    libsfml-graphics2.5 \
    libsfml-window2.5 \
    libsfml-system2.5 \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory in the container
WORKDIR /app

# Copy the compiled Minesweeper binary into the container
COPY minesweeper /app/

# Make the binary executable
RUN chmod +x /app/minesweeper

# Expose the port the app runs on (if applicable)
# EXPOSE 80

# Command to run the application
CMD ["./minesweeper"]