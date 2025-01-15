CREATE TABLE sensores (
    id SERIAL PRIMARY KEY,
    sensor_id INT NOT NULL,
    humidity REAL NOT NULL,
    temperature REAL NOT NULL,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);