-- Create the "users" table to store user information
CREATE TABLE IF NOT EXISTS users (
    id SERIAL PRIMARY KEY,
    username VARCHAR(255) NOT NULL,
    password VARCHAR(255) NOT NULL
);

-- Create the "files" table to store file metadata
CREATE TABLE IF NOT EXISTS files  (
    file_id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    filename VARCHAR(255) NOT NULL,
    file_path TEXT NOT NULL,
    user_id INTEGER NOT NULL,
    upload_date TIMESTAMP NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users (id)
);
