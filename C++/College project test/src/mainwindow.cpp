#include "mainwindow.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#include "custom_table.h"
#include <vector>
#include <string>

// Static member initialization
Fl_Window* MainWindow::window = nullptr;
Database* MainWindow::db = nullptr;
User* MainWindow::currentUser = nullptr;
CustomTable* MainWindow::ordersTable = nullptr;
CustomTable* MainWindow::menuTable = nullptr;
CustomTable* MainWindow::inventoryTable = nullptr;
CustomTable* MainWindow::employeesTable = nullptr;
CustomTable* MainWindow::cartTable = nullptr;
Fl_Button* MainWindow::addOrderBtn = nullptr;
Fl_Button* MainWindow::addMenuItemBtn = nullptr;
Fl_Button* MainWindow::addInventoryItemBtn = nullptr;
Fl_Button* MainWindow::addEmployeeBtn = nullptr;
Fl_Button* MainWindow::addToCartBtn = nullptr;
Fl_Button* MainWindow::checkoutBtn = nullptr;
Fl_Button* MainWindow::logoutBtn = nullptr;
Fl_Button* MainWindow::backButton = nullptr;
Fl_Button* MainWindow::increaseQuantityBtn = nullptr;
Fl_Button* MainWindow::decreaseQuantityBtn = nullptr;
Fl_Button* MainWindow::addCategoryBtn = nullptr;
Fl_Button* MainWindow::editItemBtn = nullptr;
Fl_Button* MainWindow::deleteItemBtn = nullptr;
Fl_Input* MainWindow::itemNameInput = nullptr;
Fl_Input* MainWindow::itemPriceInput = nullptr;
Fl_Input* MainWindow::itemCategoryInput = nullptr;
Fl_Input* MainWindow::itemDescriptionInput = nullptr;
Fl_Button* MainWindow::chooseImageBtn = nullptr;
Fl_Button* MainWindow::saveItemBtn = nullptr;
Fl_Button* MainWindow::addCategoryBtn = nullptr;
Fl_Button* MainWindow::editCategoryBtn = nullptr;
Fl_Button* MainWindow::deleteCategoryBtn = nullptr;
Fl_Button* MainWindow::addItemBtn = nullptr;
void MainWindow::init() {
    window = new Fl_Window(1200, 800, "Cafe Management System");
    
    // Initialize database connection
    db = &Database::getInstance();
    if (!db->initialize()) {
        fl_message_title("Database Error");
        fl_alert("Failed to connect to the database. Please check your database settings.");
        delete window;
        exit(1);
    }
    
    // Initialize tables
    menuTable = new CustomTable(50, 50, 400, 300, "Menu Items");
    cartTable = new CustomTable(500, 50, 400, 300, "Cart");
    
    // Initialize buttons
    backButton = new Fl_Button(50, 10, 100, 30, "Back");
    backButton->callback(onBack);
    
    addToCartBtn = new Fl_Button(500, 360, 100, 30, "Add to Cart");
    addToCartBtn->callback(onAddToCart);
    
    increaseQuantityBtn = new Fl_Button(620, 360, 30, 30, "+");
    increaseQuantityBtn->callback(onIncreaseQuantity);
    
    decreaseQuantityBtn = new Fl_Button(580, 360, 30, 30, "-");
    decreaseQuantityBtn->callback(onDecreaseQuantity);
    
    checkoutBtn = new Fl_Button(500, 400, 100, 30, "Checkout");
    checkoutBtn->callback(onCheckout);
    
    // Admin controls
    addCategoryBtn = new Fl_Button(50, 10, 100, 30, "Add Category");
    addCategoryBtn->callback(onAddCategory);
    
    editCategoryBtn = new Fl_Button(160, 10, 100, 30, "Edit Category");
    editCategoryBtn->callback(onEditCategory);
    
    deleteCategoryBtn = new Fl_Button(270, 10, 100, 30, "Delete Category");
    deleteCategoryBtn->callback(onDeleteCategory);
    
    addItemBtn = new Fl_Button(380, 10, 100, 30, "Add Item");
    addItemBtn->callback(onAddItem);
    
    editItemBtn = new Fl_Button(490, 10, 100, 30, "Edit Item");
    editItemBtn->callback(onEditItem);
    
    deleteItemBtn = new Fl_Button(600, 10, 100, 30, "Delete Item");
    deleteItemBtn->callback(onDeleteItem);
    
    window->end();
    
    // Hide all tables initially
    hideAllTables();
    
    // Show authentication window
    AuthWindow::init();
    AuthWindow::show();
}

void MainWindow::show() {
    if (window) {
        window->show();
    }
}

void MainWindow::hideAllTables() {
    ordersTable->hide();
    menuTable->hide();
    inventoryTable->hide();
    employeesTable->hide();
    cartTable->hide();
    
    addOrderBtn->hide();
    addMenuItemBtn->hide();
    addInventoryItemBtn->hide();
    addEmployeeBtn->hide();
    addToCartBtn->hide();
    checkoutBtn->hide();
    logoutBtn->hide();
    backButton->hide();
    increaseQuantityBtn->hide();
    decreaseQuantityBtn->hide();
    addCategoryBtn->hide();
    editItemBtn->hide();
    deleteItemBtn->hide();
    itemNameInput->hide();
    itemPriceInput->hide();
    itemCategoryInput->hide();
    itemDescriptionInput->hide();
    chooseImageBtn->hide();
    saveItemBtn->hide();
}

void MainWindow::setupCustomerView() {
    hideAllTables();
    
    // Show customer-specific tables and buttons
    menuTable->show();
    cartTable->show();
    addToCartBtn->show();
    checkoutBtn->show();
    logoutBtn->show();
    backButton->show();
    
    // Refresh tables
    refreshTables();
}

void MainWindow::setupAdminView() {
    hideAllTables();
    
    // Show admin-specific tables and buttons
    ordersTable->show();
    menuTable->show();
    inventoryTable->show();
    employeesTable->show();
    
    addOrderBtn->show();
    addMenuItemBtn->show();
    addInventoryItemBtn->show();
    addEmployeeBtn->show();
    
    // Show admin controls
    addCategoryBtn->show();
    editCategoryBtn->show();
    deleteCategoryBtn->show();
    addItemBtn->show();
    editItemBtn->show();
    deleteItemBtn->show();
    
    logoutBtn->show();
    backButton->show();
    
    // Refresh tables
    refreshTables();
}

void MainWindow::setCurrentUser(User* user) {
    if (currentUser) {
        delete currentUser;
    }
    currentUser = user;
    
    if (currentUser) {
        if (currentUser->type == UserType::CUSTOMER) {
            setupCustomerView();
        } else {
            setupAdminView();
        }
    }
}

User* MainWindow::getCurrentUser() {
    return currentUser;
}

void MainWindow::refreshTables() {
    // Clear all tables
    ordersTable->clear();
    menuTable->clear();
    inventoryTable->clear();
    employeesTable->clear();
    cartTable->clear();
    
    if (currentUser) {
        if (currentUser->type == UserType::ADMIN) {
            // Refresh Orders Table
            std::vector<Order> orders = db->getOrders();
            ordersTable->resize_table(orders.size(), 5);
            int row = 0;
            for (const auto& order : orders) {
                ordersTable->set_value(row, 0, std::to_string(order.id).c_str());
                ordersTable->set_value(row, 1, std::to_string(order.userId).c_str());
                ordersTable->set_value(row, 2, std::to_string(order.totalAmount).c_str());
                ordersTable->set_value(row, 3, order.status.c_str());
                ordersTable->set_value(row, 4, order.createdAt.c_str());
                row++;
            }
            
            // Refresh Inventory Table
            std::vector<InventoryItem> inventoryItems = db->getInventoryItems();
            inventoryTable->resize_table(inventoryItems.size(), 4);
            row = 0;
            for (const auto& item : inventoryItems) {
                inventoryTable->set_value(row, 0, std::to_string(item.id).c_str());
                inventoryTable->set_value(row, 1, item.name.c_str());
                inventoryTable->set_value(row, 2, std::to_string(item.quantity).c_str());
                inventoryTable->set_value(row, 3, item.unit.c_str());
                row++;
            }
            
            // Refresh Employees Table
            std::vector<Employee> employees = db->getEmployees();
            employeesTable->resize_table(employees.size(), 4);
            row = 0;
            for (const auto& employee : employees) {
                employeesTable->set_value(row, 0, std::to_string(employee.id).c_str());
                employeesTable->set_value(row, 1, employee.name.c_str());
                employeesTable->set_value(row, 2, employee.role.c_str());
                employeesTable->set_value(row, 3, employee.contact.c_str());
                row++;
            }
        }
        
        // Refresh Menu Table (visible to both customers and admins)
        std::vector<MenuItem> menuItems = db->getMenuItems();
        menuTable->resize_table(menuItems.size(), 4);
        int row = 0;
        for (const auto& item : menuItems) {
            menuTable->set_value(row, 0, std::to_string(item.id).c_str());
            menuTable->set_value(row, 1, item.name.c_str());
            menuTable->set_value(row, 2, std::to_string(item.price).c_str());
            menuTable->set_value(row, 3, item.category.c_str());
            row++;
        }
    }
    
    // Redraw all tables
    ordersTable->redraw();
    menuTable->redraw();
    inventoryTable->redraw();
    employeesTable->redraw();
    cartTable->redraw();
}

// Helper functions
void MainWindow::showAddItemDialog() {
    itemNameInput->value("");
    itemPriceInput->value("");
    itemCategoryInput->value("");
    itemDescriptionInput->value("");
    selectedImagePath = "";
    saveItemBtn->label("Add Item");
    showItemManagementDialog(false);
}

void MainWindow::showEditItemDialog(int itemId) {
    std::vector<MenuItem> items = db->getMenuItems();
    for (const auto& item : items) {
        if (item.id == itemId) {
            itemNameInput->value(item.name.c_str());
            itemPriceInput->value(std::to_string(item.price).c_str());
            itemCategoryInput->value(item.category.c_str());
            itemDescriptionInput->value(item.description.c_str());
            selectedImagePath = item.imagePath;
            saveItemBtn->label("Update Item");
            showItemManagementDialog(true);
            break;
        }
    }
}

void MainWindow::updateCartDisplay() {
    cartTable->clear();
    double total = 0.0;
    for (const auto& item : db->getCartItems()) {
        cartTable->addRow({
            std::to_string(item.first.id),
            item.first.name,
            std::to_string(item.second),
            std::to_string(item.first.price * item.second)
        });
        total += item.first.price * item.second;
    }
    cartTable->addRow({ "", "", "Total:", std::to_string(total) });
}

// Callback implementations
void MainWindow::onAddOrder(Fl_Widget* w, void* v) {
    // Implementation for adding an order
}

void MainWindow::onAddMenuItem(Fl_Widget* w, void* v) {
    // Implementation for adding a menu item
}

void MainWindow::onAddInventoryItem(Fl_Widget* w, void* v) {
    // Implementation for adding an inventory item
}

void MainWindow::onAddEmployee(Fl_Widget* w, void* v) {
    // Implementation for adding an employee
}

void MainWindow::onAddToCart(Fl_Widget* w, void* v) {
    // Implementation for adding items to cart
}

void MainWindow::onCheckout(Fl_Widget* w, void* v) {
    // Implementation for checkout process
}

void MainWindow::onLogout(Fl_Widget* w, void* v) {
    if (currentUser) {
        delete currentUser;
        currentUser = nullptr;
    }
    hideAllTables();
    AuthWindow::show();
}

void MainWindow::onBack(Fl_Widget*, void*) {
    if (currentUser) {
        if (currentUser->type == UserType::CUSTOMER) {
            setupCustomerView();
        } else {
            setupAdminView();
        }
    }
}

void MainWindow::onIncreaseQuantity(Fl_Widget*, void*) {
    if (cartTable && cartTable->get_selected_row() >= 0) {
        int row = cartTable->get_selected_row();
        int menuItemId = std::stoi(cartTable->get_value(row, 0));
        int currentQuantity = std::stoi(cartTable->get_value(row, 3));
        db->updateCartItemQuantity(menuItemId, currentQuantity + 1);
        refreshTables();
    }
}

void MainWindow::onDecreaseQuantity(Fl_Widget*, void*) {
    if (cartTable && cartTable->get_selected_row() >= 0) {
        int row = cartTable->get_selected_row();
        int menuItemId = std::stoi(cartTable->get_value(row, 0));
        int currentQuantity = std::stoi(cartTable->get_value(row, 3));
        if (currentQuantity > 1) {
            db->updateCartItemQuantity(menuItemId, currentQuantity - 1);
            refreshTables();
        } else {
            db->removeFromCart(menuItemId);
            refreshTables();
        }
    }
}

void MainWindow::showItemManagementDialog(bool isEdit) {
    Fl_Window* dialog = new Fl_Window(400, 500, isEdit ? "Edit Item" : "Add New Item");
    
    itemNameInput = new Fl_Input(100, 30, 250, 25, "Name:");
    itemPriceInput = new Fl_Input(100, 70, 250, 25, "Price:");
    itemCategoryInput = new Fl_Input(100, 110, 250, 25, "Category:");
    itemDescriptionInput = new Fl_Input(100, 150, 250, 25, "Description:");
    
    chooseImageBtn = new Fl_Button(100, 190, 250, 30, "Choose Image");
    chooseImageBtn->callback(onChooseImage);
    
    saveItemBtn = new Fl_Button(100, 230, 250, 30, "Save");
    saveItemBtn->callback(onSaveItem);
    
    if (isEdit && menuTable && menuTable->get_selected_row() >= 0) {
        int row = menuTable->get_selected_row();
        itemNameInput->value(menuTable->get_value(row, 1));
        itemPriceInput->value(menuTable->get_value(row, 2));
        itemCategoryInput->value(menuTable->get_value(row, 3));
        itemDescriptionInput->value(menuTable->get_value(row, 4));
    }
    
    dialog->end();
    dialog->show();
}

void MainWindow::onChooseImage(Fl_Widget*, void*) {
    Fl_File_Chooser chooser(".", "Images (*.{png,jpg,jpeg})", Fl_File_Chooser::SINGLE, "Choose Image");
    chooser.show();
    
    while(chooser.shown()) {
        Fl::wait();
    }
    
    if (chooser.value() != nullptr) {
        selectedImagePath = chooser.value();
    }
}

void MainWindow::onSaveItem(Fl_Widget*, void*) {
    if (!itemNameInput || !itemPriceInput || !itemCategoryInput || !itemDescriptionInput) {
        return;
    }
    
    std::string name = itemNameInput->value();
    double price = std::stod(itemPriceInput->value());
    std::string category = itemCategoryInput->value();
    std::string description = itemDescriptionInput->value();
    
    if (menuTable && menuTable->get_selected_row() >= 0) {
        // Edit existing item
        int itemId = std::stoi(menuTable->get_value(menuTable->get_selected_row(), 0));
        db->updateMenuItem(itemId, name, price, category, selectedImagePath, description);
    } else {
        // Add new item
        db->addMenuItem(name, price, category, description, selectedImagePath);
    }
    
    refreshTables();
    Fl_Window* dialog = itemNameInput->window();
    if (dialog) {
        dialog->hide();
        delete dialog;
    }
}

void MainWindow::onAddCategory(Fl_Widget* w, void* v) {
    Fl_Window* window = new Fl_Window(300, 200, "Add Category");
    Fl_Input* categoryName = new Fl_Input(100, 50, 200, 30, "Category Name:");
    Fl_Button* saveBtn = new Fl_Button(100, 120, 100, 30, "Save");
    Fl_Button* cancelBtn = new Fl_Button(210, 120, 100, 30, "Cancel");

    saveBtn->callback([](Fl_Widget* w, void* v) {
        Fl_Input* input = (Fl_Input*)v;
        std::string name = input->value();
        if (!name.empty()) {
            if (db->addCategory(name)) {
                fl_message("Category added successfully!");
                window->hide();
                refreshTables();
            } else {
                fl_message("Failed to add category!");
            }
        }
    }, categoryName);

    cancelBtn->callback([](Fl_Widget* w, void*) {
        window->hide();
    });

    window->end();
    window->show();
}

void MainWindow::onEditCategory(Fl_Widget* w, void* v) {
    if (menuTable && menuTable->get_selected_row() >= 0) {
        int categoryId = std::stoi(menuTable->get_value(menuTable->get_selected_row(), 0));
        std::string currentName = menuTable->get_value(menuTable->get_selected_row(), 1);
        
        Fl_Window* window = new Fl_Window(300, 200, "Edit Category");
        Fl_Input* categoryName = new Fl_Input(100, 50, 200, 30, "Category Name:");
        categoryName->value(currentName.c_str());
        
        Fl_Button* saveBtn = new Fl_Button(100, 120, 100, 30, "Save");
        Fl_Button* cancelBtn = new Fl_Button(210, 120, 100, 30, "Cancel");

        saveBtn->callback([categoryId](Fl_Widget* w, void* v) {
            Fl_Input* input = (Fl_Input*)v;
            std::string name = input->value();
            if (!name.empty()) {
                if (db->editCategory(categoryId, name)) {
                    fl_message("Category updated successfully!");
                    window->hide();
                    refreshTables();
                } else {
                    fl_message("Failed to update category!");
                }
            }
        }, categoryName);

        cancelBtn->callback([](Fl_Widget* w, void*) {
            window->hide();
        });

        window->end();
        window->show();
    } else {
        fl_alert("Please select a category to edit");
    }
}

void MainWindow::onDeleteCategory(Fl_Widget* w, void* v) {
    if (menuTable && menuTable->get_selected_row() >= 0) {
        int categoryId = std::stoi(menuTable->get_value(menuTable->get_selected_row(), 0));
        if (fl_choice("Are you sure you want to delete this category?", "No", "Yes", 0) == 1) {
            if (db->deleteCategory(categoryId)) {
                fl_message("Category deleted successfully!");
                refreshTables();
            } else {
                fl_message("Failed to delete category!");
            }
        }
    } else {
        fl_alert("Please select a category to delete");
    }
}

void MainWindow::onAddItem(Fl_Widget* w, void* v) {
    Fl_Window* window = new Fl_Window(400, 300, "Add Item");
    Fl_Input* itemName = new Fl_Input(100, 50, 200, 30, "Item Name:");
    Fl_Input* itemPrice = new Fl_Input(100, 90, 200, 30, "Price:");
    itemPrice->value("0.00");
    Fl_Input* itemQuantity = new Fl_Input(100, 130, 200, 30, "Quantity:");
    itemQuantity->value("0");
    Fl_Input* itemCategory = new Fl_Input(100, 170, 200, 30, "Category:");
    Fl_Button* chooseImageBtn = new Fl_Button(100, 210, 100, 30, "Choose Image");
    Fl_Button* saveBtn = new Fl_Button(100, 270, 100, 30, "Save");
    Fl_Button* cancelBtn = new Fl_Button(210, 270, 100, 30, "Cancel");

    chooseImageBtn->callback(onChooseImage);
    saveBtn->callback([](Fl_Widget* w, void* v) {
        Fl_Window* win = (Fl_Window*)v;
        Fl_Input* name = (Fl_Input*)win->child(1);
        Fl_Input* price = (Fl_Input*)win->child(3);
        Fl_Input* quantity = (Fl_Input*)win->child(5);
        Fl_Input* category = (Fl_Input*)win->child(7);
        
        std::string imageName = ""; // You'll need to implement image selection
        if (db->addItem(name->value(), std::stod(price->value()), std::stoi(quantity->value()),
                       category->value(), imageName)) {
            fl_message("Item added successfully!");
            win->hide();
            refreshTables();
        } else {
            fl_message("Failed to add item!");
        }
    }, window);

    cancelBtn->callback([](Fl_Widget* w, void*) {
        window->hide();
    });

    window->end();
    window->show();
}

void MainWindow::onEditItem(Fl_Widget* w, void* v) {
    // Similar implementation to onAddItem but with edit functionality
}

void MainWindow::onDeleteItem(Fl_Widget* w, void* v) {
    // Similar implementation to existing onDeleteItem
}

void MainWindow::showMessage(const char* title, const char* message) {
    fl_message_title(title);
    fl_message("%s", message);
} 