DROP DATABASE IF EXISTS auctionDb; 
CREATE DATABASE auctionDb;
USE auctionDb;

-- Create the User table
CREATE TABLE user (
    user_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL,
    loggin BOOLEAN NOT NULL DEFAULT 0,
    room_id INT
);

-- Create the Room table
CREATE TABLE room (
    room_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    owner_id INT NOT NULL
);

-- Create the Item table
CREATE TABLE item (
    item_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    start_time DATETIME NOT NULL,
    end_time DATETIME NOT NULL,
    current_price DECIMAL(10, 2) NOT NULL DEFAULT 0.00,
    state ENUM('active', 'sold', 'expired') NOT NULL,
    buy_now_price DECIMAL(10, 2),
    owner_id INT NOT NULL,
    room_id INT NOT NULL
);

-- Create the Log table
CREATE TABLE log (
    log_id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL,
    item_id INT NOT NULL,
    room_id INT NOT NULL,
    bid_price DECIMAL(10, 2) NOT NULL,
    time DATETIME NOT NULL,
    status ENUM('success', 'fail') NOT NULL
);

-- Create the Room_Log table
CREATE TABLE room_log (
    room_log_id INT AUTO_INCREMENT PRIMARY KEY,
    item_id INT NOT NULL,
    room_id INT NOT NULL,
    status ENUM('pending', 'accepted', 'rejected') NOT NULL
);

-- Add Foreign Key Constraints
ALTER TABLE user
ADD CONSTRAINT fk_user_room FOREIGN KEY (room_id) REFERENCES room (room_id);

ALTER TABLE room
ADD CONSTRAINT fk_room_owner FOREIGN KEY (owner_id) REFERENCES user (user_id);

ALTER TABLE item
ADD CONSTRAINT fk_item_owner FOREIGN KEY (owner_id) REFERENCES user (user_id),
ADD CONSTRAINT fk_item_room FOREIGN KEY (room_id) REFERENCES room (room_id);

ALTER TABLE log
ADD CONSTRAINT fk_log_user FOREIGN KEY (user_id) REFERENCES user (user_id),
ADD CONSTRAINT fk_log_item FOREIGN KEY (item_id) REFERENCES item (item_id),
ADD CONSTRAINT fk_log_room FOREIGN KEY (room_id) REFERENCES room (room_id);

ALTER TABLE room_log
ADD CONSTRAINT fk_room_log_item FOREIGN KEY (item_id) REFERENCES item (item_id),
ADD CONSTRAINT fk_room_log_room FOREIGN KEY (room_id) REFERENCES room (room_id);

-- Test data on user
INSERT INTO user (name, password, room_id) VALUES
('minh', '123', NULL),
('hung', '123', NULL),
('quang', '123', NULL);
