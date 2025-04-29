# Database connection parameters
$DB_HOST = "localhost"
$DB_PORT = "3306"
$DB_USER = "root"
$DB_PASS = ""  # Empty password
$DB_NAME = "cafe_management"

# Create database if it doesn't exist
Write-Host "Creating database if it doesn't exist..."
mysql -u $DB_USER -p --skip-ssl -e "CREATE DATABASE IF NOT EXISTS $DB_NAME;"

# Create tables and insert data
Write-Host "Creating tables and inserting data..."

# Menu Items Table
mysql -u $DB_USER -p --skip-ssl $DB_NAME -e "
CREATE TABLE IF NOT EXISTS menu_items (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL,
    price DECIMAL(10,2) NOT NULL,
    category VARCHAR(50),
    description TEXT,
    image_path VARCHAR(255),
    is_available BOOLEAN DEFAULT TRUE
);"

# Inventory Table
mysql -u $DB_USER -p --skip-ssl $DB_NAME -e "
CREATE TABLE IF NOT EXISTS inventory (
    id INT PRIMARY KEY AUTO_INCREMENT,
    item_name VARCHAR(100) NOT NULL,
    quantity INT NOT NULL,
    unit VARCHAR(20),
    reorder_level INT,
    last_updated TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);"

# Employees Table
mysql -u $DB_USER -p --skip-ssl $DB_NAME -e "
CREATE TABLE IF NOT EXISTS employees (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL,
    position VARCHAR(50),
    contact_number VARCHAR(20),
    email VARCHAR(100),
    hire_date DATE,
    salary DECIMAL(10,2),
    is_active BOOLEAN DEFAULT TRUE
);"

# Orders Table
mysql -u $DB_USER -p --skip-ssl $DB_NAME -e "
CREATE TABLE IF NOT EXISTS orders (
    id INT PRIMARY KEY AUTO_INCREMENT,
    customer_name VARCHAR(100),
    order_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    total_amount DECIMAL(10,2),
    status VARCHAR(20) DEFAULT 'pending',
    payment_status VARCHAR(20) DEFAULT 'unpaid'
);"

# Order Items Table
mysql -u $DB_USER -p --skip-ssl $DB_NAME -e "
CREATE TABLE IF NOT EXISTS order_items (
    id INT PRIMARY KEY AUTO_INCREMENT,
    order_id INT,
    menu_item_id INT,
    quantity INT NOT NULL,
    price_at_time DECIMAL(10,2),
    FOREIGN KEY (order_id) REFERENCES orders(id),
    FOREIGN KEY (menu_item_id) REFERENCES menu_items(id)
);"

# Admin Privileges Table
mysql -u $DB_USER -p --skip-ssl $DB_NAME -e "
CREATE TABLE IF NOT EXISTS admin_privileges (
    id INT PRIMARY KEY AUTO_INCREMENT,
    user_id INT NOT NULL,
    can_manage_menu BOOLEAN DEFAULT FALSE,
    can_manage_inventory BOOLEAN DEFAULT FALSE,
    can_manage_employees BOOLEAN DEFAULT FALSE,
    FOREIGN KEY (user_id) REFERENCES employees(id)
);"

# Insert sample data
Write-Host "Inserting sample data..."

# Menu Items
mysql -u $DB_USER -p --skip-ssl $DB_NAME -e "
INSERT INTO menu_items (name, price, category, description) VALUES
('Espresso', 3.50, 'Coffee', 'Strong Italian coffee'),
('Cappuccino', 4.50, 'Coffee', 'Espresso with steamed milk and foam'),
('Chocolate Cake', 5.00, 'Desserts', 'Rich chocolate cake'),
('Club Sandwich', 8.00, 'Sandwiches', 'Triple-decker with chicken, bacon, and vegetables');"

# Inventory
mysql -u $DB_USER -p --skip-ssl $DB_NAME -e "
INSERT INTO inventory (item_name, quantity, unit, reorder_level) VALUES
('Coffee Beans', 100, 'kg', 20),
('Milk', 50, 'liters', 10),
('Sugar', 30, 'kg', 5),
('Flour', 40, 'kg', 8);"

# Employees
mysql -u $DB_USER -p --skip-ssl $DB_NAME -e "
INSERT INTO employees (name, position, contact_number, email, hire_date, salary) VALUES
('John Doe', 'Manager', '555-0101', 'john@cafe.com', '2023-01-01', 45000.00),
('Jane Smith', 'Barista', '555-0102', 'jane@cafe.com', '2023-02-15', 35000.00);"

# Users Table (for authentication)
mysql -u $DB_USER -p --skip-ssl $DB_NAME -e "
CREATE TABLE IF NOT EXISTS users (
    id INT PRIMARY KEY AUTO_INCREMENT,
    email VARCHAR(255) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    name VARCHAR(255) NOT NULL,
    type ENUM('CUSTOMER', 'ADMIN') NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);"

# Insert sample users
mysql -u $DB_USER -p --skip-ssl $DB_NAME -e "
INSERT INTO users (email, password, name, type) VALUES
('admin@cafe.com', 'admin123', 'Admin User', 'ADMIN'),
('customer@cafe.com', 'customer123', 'Customer User', 'CUSTOMER');"

Write-Host "Database setup completed successfully!" 