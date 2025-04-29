#include "database.h"
#include <cstring>
#include <sstream>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <algorithm>

Database::Database() : conn(nullptr) {
}

Database::~Database() {
    if (conn) {
        mysql_close(conn);
    }
}

bool Database::initialize() {
    return connectToDatabase() && createTables();
}

bool Database::connectToDatabase() {
    conn = mysql_init(nullptr);
    if (!conn) {
        std::cerr << "Failed to initialize database connection" << std::endl;
        return false;
    }

    const char* host = "localhost";
    const char* user = "root";
    const char* pass = "2525";
    const char* dbname = "cafe_management";
    const int port = 3306;

    // First connect without specifying the database
    if (!mysql_real_connect(conn, host, user, pass, nullptr, port, nullptr, 0)) {
        std::cerr << "Failed to connect to database server: " << mysql_error(conn) << std::endl;
        return false;
    }

    // Then select the database
    if (mysql_select_db(conn, dbname) != 0) {
        std::cerr << "Failed to select database: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        conn = nullptr;
        return false;
    }

    return true;
}

void Database::createOrdersTable() {
    const char* query = "CREATE TABLE IF NOT EXISTS orders ("
        "id INT PRIMARY KEY AUTO_INCREMENT,"
        "user_id INT NOT NULL,"
        "status VARCHAR(20) NOT NULL DEFAULT 'pending',"
        "total_amount DECIMAL(10,2) NOT NULL,"
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
        "FOREIGN KEY (user_id) REFERENCES users(id)"
        ")";
    
    if (mysql_query(conn, query)) {
        std::cerr << "Error creating orders table: " << mysql_error(conn) << std::endl;
    }
}

void Database::createMenuItemsTable() {
    const char* query = "CREATE TABLE IF NOT EXISTS menu_items ("
        "id INT PRIMARY KEY AUTO_INCREMENT,"
        "name VARCHAR(100) NOT NULL,"
        "price DECIMAL(10,2) NOT NULL,"
        "category VARCHAR(50),"
        "description TEXT,"
        "image_path VARCHAR(255),"
        "is_available BOOLEAN DEFAULT TRUE"
        ")";
    
    if (mysql_query(conn, query)) {
        std::cerr << "Error creating menu_items table: " << mysql_error(conn) << std::endl;
    }
}

void Database::createInventoryTable() {
    const char* query = 
        "CREATE TABLE IF NOT EXISTS inventory ("
        "id INT PRIMARY KEY AUTO_INCREMENT,"
        "name VARCHAR(100) NOT NULL,"
        "quantity INT NOT NULL,"
        "unit_price DECIMAL(10,2) NOT NULL,"
        "last_updated TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ")";
    
    mysql_query(conn, query);
}

void Database::createEmployeesTable() {
    const char* query = 
        "CREATE TABLE IF NOT EXISTS employees ("
        "id INT PRIMARY KEY AUTO_INCREMENT,"
        "name VARCHAR(100) NOT NULL,"
        "role VARCHAR(50) NOT NULL,"
        "contact VARCHAR(100) NOT NULL,"
        "hire_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ")";
    
    mysql_query(conn, query);
}

void Database::createOrderItemsTable() {
    const char* query = "CREATE TABLE IF NOT EXISTS order_items ("
        "id INT PRIMARY KEY AUTO_INCREMENT,"
        "order_id INT NOT NULL,"
        "menu_item_id INT NOT NULL,"
        "quantity INT NOT NULL,"
        "price DECIMAL(10,2) NOT NULL,"
        "subtotal DECIMAL(10,2) NOT NULL,"
        "FOREIGN KEY (order_id) REFERENCES orders(id),"
        "FOREIGN KEY (menu_item_id) REFERENCES menu_items(id)"
        ")";
    
    if (mysql_query(conn, query)) {
        std::cerr << "Error creating order_items table: " << mysql_error(conn) << std::endl;
    }
}

void Database::createUsersTable() {
    const char* query = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INT AUTO_INCREMENT PRIMARY KEY,
            email VARCHAR(255) UNIQUE NOT NULL,
            password VARCHAR(255) NOT NULL,
            name VARCHAR(255) NOT NULL,
            type ENUM('CUSTOMER', 'ADMIN') NOT NULL,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    )";
    executeUpdate(query);
}

bool Database::createTables() {
    createOrdersTable();
    createMenuItemsTable();
    createInventoryTable();
    createEmployeesTable();
    createOrderItemsTable();
    createUsersTable();
    createCategoriesTable();
    return true;
}

void Database::createCategoriesTable() {
    std::string query = "CREATE TABLE IF NOT EXISTS categories ("
                       "id INT AUTO_INCREMENT PRIMARY KEY,"
                       "name VARCHAR(100) NOT NULL UNIQUE,"
                       "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
                       ") ENGINE=InnoDB;";
    executeUpdate(query);
}

bool Database::addCategory(const std::string& name) {
    std::string query = "INSERT INTO categories (name, created_at) VALUES ('" + 
                       escapeString(name) + "', NOW())";
    return executeQuery(query);
}

bool Database::editCategory(int id, const std::string& name) {
    std::string query = "UPDATE categories SET name = '" + 
                       escapeString(name) + "' WHERE id = " + std::to_string(id);
    return executeQuery(query);
}

bool Database::deleteCategory(int id) {
    std::string query = "DELETE FROM categories WHERE id = " + std::to_string(id);
    return executeQuery(query);
}

bool Database::addItem(const std::string& name, double price, int quantity, 
                      const std::string& category, const std::string& imagePath) {
    std::string query = "INSERT INTO menu_items (name, price, quantity, category, image_path, available) VALUES ('" +
                       escapeString(name) + "', " + std::to_string(price) + ", " +
                       std::to_string(quantity) + ", '" + escapeString(category) +
                       "', '" + escapeString(imagePath) + "', 1)";
    return executeQuery(query);
}

bool Database::editItem(int id, const std::string& name, double price, int quantity, 
                       const std::string& category, const std::string& imagePath) {
    std::string query = "UPDATE menu_items SET name = '" + escapeString(name) +
                       "', price = " + std::to_string(price) +
                       ", quantity = " + std::to_string(quantity) +
                       ", category = '" + escapeString(category) +
                       "', image_path = '" + escapeString(imagePath) +
                       "' WHERE id = " + std::to_string(id);
    return executeQuery(query);
}

bool Database::deleteItem(int id) {
    std::string query = "DELETE FROM menu_items WHERE id = " + std::to_string(id);
    return executeQuery(query);
}

std::string Database::escapeString(const std::string& str) {
    char* escaped = new char[str.length() * 2 + 1];
    mysql_real_escape_string(conn, escaped, str.c_str(), str.length());
    std::string result(escaped);
    delete[] escaped;
    return result;
}

MYSQL_RES* Database::executeQuery(const std::string& query) {
    if (mysql_query(conn, query.c_str()) != 0) {
        return nullptr;
    }
    return mysql_store_result(conn);
}

bool Database::executeUpdate(const std::string& query) {
    return mysql_query(conn, query.c_str()) == 0;
}

bool Database::addOrder(int userId, const std::vector<std::pair<int, int>>& items, double totalAmount) {
    std::stringstream ss;
    ss << "INSERT INTO orders (user_id, total_amount) VALUES ("
       << userId << ", " << totalAmount << ")";
    
    if (!executeUpdate(ss.str())) {
        return false;
    }

    int orderId = mysql_insert_id(conn);

    for (const auto& item : items) {
        ss.str("");
        ss << "INSERT INTO order_items (order_id, menu_item_id, quantity, price) "
           << "SELECT " << orderId << ", " << item.first << ", " << item.second 
           << ", price FROM menu_items WHERE id = " << item.first;
        
        if (!executeUpdate(ss.str())) {
            return false;
        }
    }

    return true;
}

bool Database::updateOrderStatus(int orderId, const std::string& status) {
    std::string query = "UPDATE orders SET status = '" + status + "' WHERE id = " + std::to_string(orderId);
    
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Error updating order status: " << mysql_error(conn) << std::endl;
        return false;
    }
    
    return true;
}

std::vector<Order> Database::getOrders(int userId) {
    std::vector<Order> orders;
    std::string query = "SELECT id, user_id, status, total_amount, created_at FROM orders";
    
    if (userId > 0) {
        query += " WHERE user_id = " + std::to_string(userId);
    }
    
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Error getting orders: " << mysql_error(conn) << std::endl;
        return orders;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (result == nullptr) {
        return orders;
    }
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        Order order;
        order.id = std::stoi(row[0]);
        order.userId = std::stoi(row[1]);
        order.status = row[2];
        order.totalAmount = std::stod(row[3]);
        order.createdAt = row[4];
        
        // Get order items
        std::string itemsQuery = "SELECT menu_item_id, quantity, price, subtotal FROM order_items WHERE order_id = " + std::to_string(order.id);
        if (mysql_query(conn, itemsQuery.c_str())) {
            continue;
        }
        
        MYSQL_RES* itemsResult = mysql_store_result(conn);
        if (itemsResult == nullptr) {
            continue;
        }
        
        MYSQL_ROW itemRow;
        while ((itemRow = mysql_fetch_row(itemsResult))) {
            OrderItem item;
            item.menuItemId = std::stoi(itemRow[0]);
            item.quantity = std::stoi(itemRow[1]);
            item.price = std::stod(itemRow[2]);
            item.subtotal = std::stod(itemRow[3]);
            order.items.push_back(item);
        }
        
        mysql_free_result(itemsResult);
        orders.push_back(order);
    }
    
    mysql_free_result(result);
    return orders;
}

bool Database::addInventoryItem(const std::string& name, int quantity, double unitPrice) {
    std::stringstream ss;
    ss << "INSERT INTO inventory (name, quantity, unit_price) VALUES ('"
       << escapeString(name) << "', " << quantity << ", " << unitPrice << ")";
    return executeUpdate(ss.str());
}

bool Database::updateInventoryItem(int itemId, int quantity) {
    std::stringstream ss;
    ss << "UPDATE inventory SET quantity = " << quantity
       << ", last_updated = CURRENT_TIMESTAMP WHERE id = " << itemId;
    return executeUpdate(ss.str());
}

bool Database::deleteInventoryItem(int itemId) {
    std::stringstream ss;
    ss << "DELETE FROM inventory WHERE id = " << itemId;
    return executeUpdate(ss.str());
}

std::vector<InventoryItem> Database::getInventoryItems() {
    std::vector<InventoryItem> items;
    MYSQL_RES* result = executeQuery("SELECT * FROM inventory");
    
    if (result) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            InventoryItem item;
            item.id = std::stoi(row[0]);
            item.name = row[1];
            item.quantity = std::stoi(row[2]);
            item.unitPrice = std::stod(row[3]);
            item.lastUpdated = row[4];
            items.push_back(item);
        }
        mysql_free_result(result);
    }
    
    return items;
}

bool Database::addEmployee(const std::string& name, const std::string& role, const std::string& contact) {
    std::stringstream ss;
    ss << "INSERT INTO employees (name, role, contact) VALUES ('"
       << escapeString(name) << "', '" << escapeString(role) << "', '" << escapeString(contact) << "')";
    return executeUpdate(ss.str());
}

bool Database::updateEmployee(int employeeId, const std::string& name, const std::string& role, const std::string& contact) {
    std::stringstream ss;
    ss << "UPDATE employees SET name = '" << escapeString(name)
       << "', role = '" << escapeString(role)
       << "', contact = '" << escapeString(contact)
       << "' WHERE id = " << employeeId;
    return executeUpdate(ss.str());
}

bool Database::deleteEmployee(int employeeId) {
    std::stringstream ss;
    ss << "DELETE FROM employees WHERE id = " << employeeId;
    return executeUpdate(ss.str());
}

std::vector<Employee> Database::getEmployees() {
    std::vector<Employee> employees;
    MYSQL_RES* result = executeQuery("SELECT * FROM employees");
    
    if (result) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            Employee employee;
            employee.id = std::stoi(row[0]);
            employee.name = row[1];
            employee.role = row[2];
            employee.contact = row[3];
            employee.hireDate = row[4];
            employees.push_back(employee);
        }
        mysql_free_result(result);
    }
    
    return employees;
}

std::vector<Order> Database::getDailySales(const std::string& date) {
    std::stringstream ss;
    ss << "SELECT * FROM orders WHERE DATE(created_at) = '" << escapeString(date) << "'";
    
    std::vector<Order> orders;
    MYSQL_RES* result = executeQuery(ss.str());
    
    if (result) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            Order order;
            order.id = std::stoi(row[0]);
            order.userId = std::stoi(row[1]);
            order.totalAmount = std::stod(row[2]);
            order.status = row[3];
            order.createdAt = row[4];
            orders.push_back(order);
        }
        mysql_free_result(result);
    }
    
    return orders;
}

std::vector<std::pair<std::string, double>> Database::getMonthlySales(int year, int month) {
    std::stringstream ss;
    ss << "SELECT DATE(created_at) as date, SUM(total_amount) as total "
       << "FROM orders "
       << "WHERE YEAR(created_at) = " << year << " AND MONTH(created_at) = " << month
       << " GROUP BY DATE(created_at)";
    
    std::vector<std::pair<std::string, double>> sales;
    MYSQL_RES* result = executeQuery(ss.str());
    
    if (result) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            sales.emplace_back(row[0], std::stod(row[1]));
        }
        mysql_free_result(result);
    }
    
    return sales;
}

std::vector<std::pair<std::string, int>> Database::getInventoryReport() {
    const char* query = "SELECT name, quantity FROM inventory ORDER BY quantity ASC";
    std::vector<std::pair<std::string, int>> report;
    MYSQL_RES* result = executeQuery(query);
    
    if (result) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            report.emplace_back(row[0], std::stoi(row[1]));
        }
        mysql_free_result(result);
    }
    
    return report;
}

bool Database::registerUser(const std::string& email, const std::string& password, const std::string& name, UserType type) {
    if (isEmailExists(email)) {
        return false;
    }

    std::string hashedPassword = hashPassword(password);
    std::string typeStr = (type == UserType::CUSTOMER) ? "CUSTOMER" : "ADMIN";
    
    std::string query = "INSERT INTO users (email, password, name, type) VALUES ('" +
                       escapeString(email) + "', '" +
                       escapeString(hashedPassword) + "', '" +
                       escapeString(name) + "', '" +
                       typeStr + "')";
    
    return executeUpdate(query);
}

User* Database::loginUser(const std::string& email, const std::string& password) {
    std::string query = "SELECT * FROM users WHERE email = '" + escapeString(email) + "'";
    MYSQL_RES* result = executeQuery(query);
    
    if (!result) {
        return nullptr;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (!row) {
        mysql_free_result(result);
        return nullptr;
    }

    std::string hashedPassword = row[2];
    if (!verifyPassword(password, hashedPassword)) {
        mysql_free_result(result);
        return nullptr;
    }

    User* user = new User();
    user->id = std::stoi(row[0]);
    user->email = row[1];
    user->password = hashedPassword;
    user->name = row[3];
    user->type = (std::string(row[4]) == "CUSTOMER") ? UserType::CUSTOMER : UserType::ADMIN;
    user->isAdmin = (user->type == UserType::ADMIN);
    user->createdAt = row[5];

    mysql_free_result(result);
    return user;
}

bool Database::changePassword(int userId, const std::string& newPassword) {
    std::string hashedPassword = hashPassword(newPassword);
    std::string query = "UPDATE users SET password = '" + 
                       escapeString(hashedPassword) + 
                       "' WHERE id = " + std::to_string(userId);
    
    return executeUpdate(query);
}

bool Database::isEmailExists(const std::string& email) {
    std::string query = "SELECT COUNT(*) FROM users WHERE email = '" + escapeString(email) + "'";
    MYSQL_RES* result = executeQuery(query);
    
    if (!result) {
        return false;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    int count = std::stoi(row[0]);
    
    mysql_free_result(result);
    return count > 0;
}

std::string Database::hashPassword(const std::string& password) {
    // Simple hashing function (for demonstration purposes only)
    std::hash<std::string> hasher;
    size_t hash = hasher(password);
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hash;
    return ss.str();
}

bool Database::verifyPassword(const std::string& password, const std::string& hashedPassword) {
    return hashPassword(password) == hashedPassword;
}

int Database::createOrder(int userId, const std::vector<OrderItem>& items) {
    // Calculate total amount
    double totalAmount = 0;
    for (const auto& item : items) {
        totalAmount += item.subtotal;
    }
    
    // Create order
    std::string query = "INSERT INTO orders (user_id, total_amount) VALUES (" +
        std::to_string(userId) + ", " + std::to_string(totalAmount) + ")";
    
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Error creating order: " << mysql_error(conn) << std::endl;
        return 0;
    }
    
    int orderId = mysql_insert_id(conn);
    
    // Add order items
    for (const auto& item : items) {
        query = "INSERT INTO order_items (order_id, menu_item_id, quantity, price, subtotal) VALUES (" +
            std::to_string(orderId) + ", " +
            std::to_string(item.menuItemId) + ", " +
            std::to_string(item.quantity) + ", " +
            std::to_string(item.price) + ", " +
            std::to_string(item.subtotal) + ")";
        
        if (mysql_query(conn, query.c_str())) {
            std::cerr << "Error adding order item: " << mysql_error(conn) << std::endl;
        }
    }
    
    return orderId;
}

bool Database::hasAdminPrivileges(int userId) {
    std::string query = "SELECT can_manage_menu FROM admin_privileges WHERE user_id = ?";
    MYSQL_STMT* stmt = mysql_stmt_init(conn);
    if (!stmt) return false;

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0) {
        mysql_stmt_close(stmt);
        return false;
    }

    MYSQL_BIND bind[1];
    memset(bind, 0, sizeof(bind));
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = (void*)&userId;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        mysql_stmt_close(stmt);
        return false;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        mysql_stmt_close(stmt);
        return false;
    }

    bool hasPrivileges = false;
    MYSQL_BIND result[1];
    memset(result, 0, sizeof(result));
    result[0].buffer_type = MYSQL_TYPE_TINY;
    result[0].buffer = &hasPrivileges;

    if (mysql_stmt_bind_result(stmt, result) != 0) {
        mysql_stmt_close(stmt);
        return false;
    }

    bool success = (mysql_stmt_fetch(stmt) == 0);
    mysql_stmt_close(stmt);
    return success && hasPrivileges;
}

bool Database::addToCart(int menuItemId, int quantity) {
    MenuItem* item = getMenuItem(menuItemId);
    if (!item) return false;

    // Check if item already exists in cart
    for (auto& cartItem : cart) {
        if (cartItem.id == menuItemId) {
            cartItem.quantity += quantity;
            return true;
        }
    }

    // Add new item to cart
    cart.push_back(*item);
    cart.back().quantity = quantity;
    return true;
}

bool Database::updateCartItemQuantity(int menuItemId, int quantity) {
    for (auto& item : cart) {
        if (item.id == menuItemId) {
            if (quantity <= 0) {
                cart.erase(std::remove_if(cart.begin(), cart.end(),
                    [menuItemId](const MenuItem& item) { return item.id == menuItemId; }),
                    cart.end());
            } else {
                item.quantity = quantity;
            }
            return true;
        }
    }
    return false;
}

std::vector<std::pair<MenuItem, int>> Database::getCartItems() {
    std::vector<std::pair<MenuItem, int>> result;
    for (const auto& item : cart) {
        result.push_back({item, item.quantity});
    }
    return result;
}

std::vector<Category> Database::getCategories() {
    std::vector<Category> categories;
    std::string query = "SELECT id, name, created_at FROM categories ORDER BY name";
    
    MYSQL_RES* result = executeQuery(query);
    if (result) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            Category category;
            category.id = std::stoi(row[0]);
            category.name = row[1];
            category.createdAt = row[2];
            categories.push_back(category);
        }
        mysql_free_result(result);
    }
    
    return categories;
}

MenuItem* Database::getMenuItem(int id) {
    std::string query = "SELECT id, name, price, category, description, image_path, is_available "
                       "FROM menu_items WHERE id = " + std::to_string(id);
    
    MYSQL_RES* result = executeQuery(query);
    if (!result) {
        return nullptr;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    if (!row) {
        mysql_free_result(result);
        return nullptr;
    }
    
    MenuItem* item = new MenuItem();
    item->id = std::stoi(row[0]);
    item->name = row[1];
    item->price = std::stod(row[2]);
    item->category = row[3];
    item->description = row[4] ? row[4] : "";
    item->imagePath = row[5] ? row[5] : "";
    item->available = row[6] ? (std::string(row[6]) == "1") : true;
    
    mysql_free_result(result);
    return item;
}

std::vector<MenuItem> Database::getMenuItems(const std::string& category) {
    std::vector<MenuItem> items;
    std::string query = "SELECT id, name, price, category, description, image_path, is_available "
                       "FROM menu_items";
    
    if (!category.empty()) {
        query += " WHERE category = '" + escapeString(category) + "'";
    }
    
    MYSQL_RES* result = executeQuery(query);
    if (!result) {
        return items;
    }
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        MenuItem item;
        item.id = std::stoi(row[0]);
        item.name = row[1];
        item.price = std::stod(row[2]);
        item.category = row[3];
        item.description = row[4] ? row[4] : "";
        item.imagePath = row[5] ? row[5] : "";
        item.available = row[6] ? (std::string(row[6]) == "1") : true;
        items.push_back(item);
    }
    
    mysql_free_result(result);
    return items;
} 