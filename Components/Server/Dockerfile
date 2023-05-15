FROM gcc:latest as build

RUN apt-get update && apt-get install -y \
    cmake make qtbase5-dev

# Set the working directory inside the container
WORKDIR /build

# Copy the source code and any necessary files
COPY . /build

# Build your server (replace the commands with your actual build commands)
RUN cmake .
RUN make

# Stage 2: Final stage
FROM quay.io/pypa/manylinux2014_x86_64

# Set the working directory inside the container
WORKDIR /app

# Copy the built server from the previous stage
COPY --from=build /build/Server /app/server

# Expose the port your server listens on
EXPOSE 9999

# Set the entry point command to start your server
CMD ["./server"]