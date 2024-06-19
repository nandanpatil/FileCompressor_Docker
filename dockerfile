# Use the official Node.js image
FROM node:14

# Install g++
RUN apt-get update && apt-get install -y g++

# Create and set the working directory
WORKDIR /usr/src/comptest

# Copy the rest of the application files
COPY . .

# Install dependencies
RUN npm install
RUN g++ main.cpp -o pro

# Expose the port the app runs on
EXPOSE 3000

# Command to run the app
CMD ["node","index.js"]
