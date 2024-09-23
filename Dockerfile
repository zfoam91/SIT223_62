# Use a lightweight base image
FROM nginx:alpine

# Set the working directory
WORKDIR /usr/share/nginx/html

# Copy the Minesweeper zip file and index.html into the container
COPY minesweeper.zip ./
COPY index.html ./

# Expose port 80
EXPOSE 80

# Use Nginx to serve the application
CMD ["nginx", "-g", "daemon off;"]
