FROM postgres:latest

# Copy the SQL initialization script to the container
COPY init.sql /docker-entrypoint-initdb.d/

# Set the necessary environment variables
ENV POSTGRES_USER=myuser
ENV POSTGRES_PASSWORD=mypassword
ENV POSTGRES_DB=mydatabase

# Expose the PostgreSQL default port
EXPOSE 5432
