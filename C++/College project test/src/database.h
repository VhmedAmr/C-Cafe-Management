#ifndef DATABASE_H
#define DATABASE_H

#include <mysql.h>
#include <string>
#include <vector>
#include <map>
#include <memory>

// User types
enum class UserType {
    CUSTOMER,
    ADMIN
};

struct User {
    int id;
    std::string email;
    std::string password;
    std::string name;
    UserType type;
    std::string createdAt;
    bool isAdmin;
    
    User() : id(0), type(UserType::CUSTOMER), isAdmin(false) {}
};

struct MenuItem {
    int id;
    std::string name;
    double price;
    std::string category;
    std::string imagePath;
    std::string description;
    bool available;
    int quantity;
    
    MenuItem() : id(0), price(0.0), available(true), quantity(0) {}
};

struct Category {
    int id;
    std::string name;
    std::string createdAt;
    
    Category() : id(0) {}
};

struct OrderItem {
    int id;
    int orderId;
    int menuItemId;
    int quantity;
    double price;
    double subtotal;
};

struct Order {
    int id;
    int userId;
    std::string status;
    double totalAmount;
    std::string createdAt;
    std::vector<OrderItem> items;
};

struct InventoryItem {
    int id;
    std::string name;
    int quantity;
    double unitPrice;
    std::string lastUpdated;
    std::string unit;
};

struct Employee {
    int id;
    std::string name;
    std::string role;
    std::string contact;
    std::string hireDate;
};

class Database {
public:
    Database();
    ~Database();

    // Singleton pattern
    static Database& getInstance() {
        static Database instance;
        return instance;
    }

    bool initialize();
    bool createTables();

    // User Authentication
    bool registerUser(const std::string& email, const std::string& password, const std::string& name, UserType type);
    User* loginUser(const std::string& email, const std::string& password);
    bool changePassword(int userId, const std::string& newPassword);
    bool isEmailExists(const std::string& email);

    // Order Management
    bool addOrder(int userId, const std::vector<std::pair<int, int>>& items, double totalAmount);
    bool updateOrderStatus(int orderId, const std::string& status);
    std::vector<Order> getOrders(int userId = 0);
    Order* getOrder(int orderId);

    // Menu Management
    std::vector<MenuItem> getMenuItems(const std::string& category = "");
    MenuItem* getMenuItem(int id);
    bool addCategory(const std::string& name);
    bool editCategory(int id, const std::string& name);
    bool deleteCategory(int id);
    bool addItem(const std::string& name, double price, int quantity, 
                 const std::string& category, const std::string& imagePath);
    bool editItem(int id, const std::string& name, double price, int quantity, 
                  const std::string& category, const std::string& imagePath);
    bool deleteItem(int id);
    std::vector<Category> getCategories();

    // Inventory Management
    bool addInventoryItem(const std::string& name, int quantity, double unitPrice);
    bool updateInventoryItem(int itemId, int quantity);
    bool deleteInventoryItem(int itemId);
    std::vector<InventoryItem> getInventoryItems();

    // Employee Management
    bool addEmployee(const std::string& name, const std::string& role, const std::string& contact);
    bool updateEmployee(int employeeId, const std::string& name, const std::string& role, const std::string& contact);
    bool deleteEmployee(int employeeId);
    std::vector<Employee> getEmployees();

    // Reports
    std::vector<Order> getDailySales(const std::string& date);
    std::vector<std::pair<std::string, double>> getMonthlySales(int year, int month);
    std::vector<std::pair<std::string, int>> getInventoryReport();

    // Order related methods
    int createOrder(int userId, const std::vector<OrderItem>& items);

    // Admin Privileges
    bool hasAdminPrivileges(int userId);
    bool grantAdminPrivileges(int userId, bool canManageMenu, bool canManageInventory, bool canManageEmployees);

    // Cart Management
    bool addToCart(int menuItemId, int quantity);
    bool updateCartItemQuantity(int menuItemId, int quantity);
    bool removeFromCart(int menuItemId);
    std::vector<std::pair<MenuItem, int>> getCartItems();

private:
    MYSQL* conn;
    std::vector<MenuItem> cart;
    bool connectToDatabase();
    void createUsersTable();
    void createOrdersTable();
    void createMenuItemsTable();
    void createInventoryTable();
    void createEmployeesTable();
    void createOrderItemsTable();
    void createCategoriesTable();
    
    // Helper functions
    std::string escapeString(const std::string& str);
    MYSQL_RES* executeQuery(const std::string& query);
    bool executeUpdate(const std::string& query);
    std::string hashPassword(const std::string& password);
    bool verifyPassword(const std::string& password, const std::string& hashedPassword);
};

#endif // DATABASE_H 