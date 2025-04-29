CREATE DATABASE IF NOT EXISTS cafe_management;
USE cafe_management;

-- Menu Items Table
CREATE TABLE IF NOT EXISTS menu_items (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL,
    price DECIMAL(10,2) NOT NULL,
    category VARCHAR(50),
    description TEXT,
    image_path VARCHAR(255),
    is_available BOOLEAN DEFAULT TRUE
);

-- Inventory Table
CREATE TABLE IF NOT EXISTS inventory (
    id INT PRIMARY KEY AUTO_INCREMENT,
    item_name VARCHAR(100) NOT NULL,
    quantity INT NOT NULL,
    unit VARCHAR(20),
    reorder_level INT,
    last_updated TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

-- Employees Table
CREATE TABLE IF NOT EXISTS employees (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL,
    position VARCHAR(50),
    contact_number VARCHAR(20),
    email VARCHAR(100),
    hire_date DATE,
    salary DECIMAL(10,2),
    is_active BOOLEAN DEFAULT TRUE
);

-- Orders Table
CREATE TABLE IF NOT EXISTS orders (
    id INT PRIMARY KEY AUTO_INCREMENT,
    customer_name VARCHAR(100),
    order_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    total_amount DECIMAL(10,2),
    status VARCHAR(20) DEFAULT 'pending',
    payment_status VARCHAR(20) DEFAULT 'unpaid'
);

-- Order Items Table
CREATE TABLE IF NOT EXISTS order_items (
    id INT PRIMARY KEY AUTO_INCREMENT,
    order_id INT,
    menu_item_id INT,
    quantity INT NOT NULL,
    price_at_time DECIMAL(10,2),
    FOREIGN KEY (order_id) REFERENCES orders(id),
    FOREIGN KEY (menu_item_id) REFERENCES menu_items(id)
);

-- Admin Privileges Table
CREATE TABLE IF NOT EXISTS admin_privileges (
    id INT PRIMARY KEY AUTO_INCREMENT,
    user_id INT NOT NULL,
    can_manage_menu BOOLEAN DEFAULT FALSE,
    can_manage_inventory BOOLEAN DEFAULT FALSE,
    can_manage_employees BOOLEAN DEFAULT FALSE,
    FOREIGN KEY (user_id) REFERENCES employees(id)
);

-- Insert sample data
INSERT INTO menu_items (name, price, category, description) VALUES
('Espresso', 3.50, 'Coffee', 'Strong Italian coffee'),
('Cappuccino', 4.50, 'Coffee', 'Espresso with steamed milk and foam'),
('Chocolate Cake', 5.00, 'Desserts', 'Rich chocolate cake'),
('Club Sandwich', 8.00, 'Sandwiches', 'Triple-decker with chicken, bacon, and vegetables');

INSERT INTO inventory (item_name, quantity, unit, reorder_level) VALUES
('Coffee Beans', 100, 'kg', 20),
('Milk', 50, 'liters', 10),
('Sugar', 30, 'kg', 5),
('Flour', 40, 'kg', 8);

INSERT INTO employees (name, position, contact_number, email, hire_date, salary) VALUES
('John Doe', 'Manager', '555-0101', 'john@cafe.com', '2023-01-01', 45000.00),
('Jane Smith', 'Barista', '555-0102', 'jane@cafe.com', '2023-02-15', 35000.00); 