#include "menu_window.h"
#include <FL/fl_ask.H>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "../database.h"

// Static member initialization
Fl_Window* MenuWindow::window = nullptr;
Fl_Group* MenuWindow::menuGroup = nullptr;
Fl_Group* MenuWindow::receiptGroup = nullptr;
Fl_Text_Display* MenuWindow::receiptDisplay = nullptr;
Fl_Text_Buffer* MenuWindow::receiptBuffer = nullptr;
std::vector<OrderItem> MenuWindow::currentOrder;
double MenuWindow::totalAmount = 0.0;
User* MenuWindow::currentUser = nullptr;
Fl_Button* MenuWindow::backButton = nullptr;
Fl_Button* MenuWindow::addItemButton = nullptr;
Fl_Button* MenuWindow::editItemButton = nullptr;
Fl_Button* MenuWindow::deleteItemButton = nullptr;
Fl_Button* MenuWindow::addCategoryButton = nullptr;
Fl_Button* MenuWindow::editCategoryButton = nullptr;
Fl_Button* MenuWindow::deleteCategoryButton = nullptr;
Fl_Window* MenuWindow::itemDialog = nullptr;
Fl_Input* MenuWindow::nameInput = nullptr;
Fl_Input* MenuWindow::priceInput = nullptr;
Fl_Input* MenuWindow::categoryInput = nullptr;
Fl_Input* MenuWindow::descriptionInput = nullptr;
Fl_Button* MenuWindow::imageButton = nullptr;
std::string MenuWindow::selectedImagePath = "";

void MenuWindow::init() {
    // Create main window
    window = new Fl_Window(1200, 800, "Cafe Management System - Menu");
    
    // Create back button
    backButton = new Fl_Button(10, 10, 70, 30, "Back");
    backButton->callback(onBack);
    
    // Create receipt group (left side)
    receiptGroup = new Fl_Group(0, 50, 400, 750);
    receiptGroup->box(FL_DOWN_BOX);
    
    // Receipt header
    new Fl_Box(20, 60, 360, 30, "Receipt");
    
    // Receipt display
    receiptBuffer = new Fl_Text_Buffer();
    receiptDisplay = new Fl_Text_Display(20, 100, 360, 550);
    receiptDisplay->buffer(receiptBuffer);
    receiptDisplay->textfont(FL_COURIER);
    
    // Quantity control buttons
    Fl_Button* decreaseBtn = new Fl_Button(20, 670, 40, 40, "-");
    decreaseBtn->callback(onDecreaseQuantity);
    
    Fl_Button* increaseBtn = new Fl_Button(70, 670, 40, 40, "+");
    increaseBtn->callback(onIncreaseQuantity);
    
    // Send ticket and Pay buttons
    Fl_Button* sendButton = new Fl_Button(120, 670, 120, 40, "Send ticket");
    sendButton->callback(onSendTicket);
    
    Fl_Button* payButton = new Fl_Button(250, 670, 120, 40, "Pay");
    payButton->callback(onPay);
    
    receiptGroup->end();
    
    // Create menu group (right side)
    menuGroup = new Fl_Group(400, 50, 800, 750);
    menuGroup->box(FL_DOWN_BOX);
    
    menuGroup->end();  // End the group before adding buttons
    
    // Admin controls
    addItemButton = new Fl_Button(410, 10, 100, 30, "Add Item");
    addItemButton->callback(onAddItem);
    
    editItemButton = new Fl_Button(520, 10, 100, 30, "Edit Item");
    editItemButton->callback(onEditItem);
    
    deleteItemButton = new Fl_Button(630, 10, 100, 30, "Delete Item");
    deleteItemButton->callback(onDeleteItem);
    
    addCategoryButton = new Fl_Button(740, 10, 100, 30, "Add Category");
    addCategoryButton->callback(onAddCategory);
    
    editCategoryButton = new Fl_Button(850, 10, 100, 30, "Edit Category");
    editCategoryButton->callback(onEditCategory);
    
    deleteCategoryButton = new Fl_Button(960, 10, 100, 30, "Delete Category");
    deleteCategoryButton->callback(onDeleteCategory);
    
    menuGroup->begin();  // Begin the group again to add the buttons
    menuGroup->end();
    
    window->end();
    showMainMenu();
}

void MenuWindow::setCurrentUser(User* user) {
    currentUser = user;
    if (currentUser && currentUser->type == UserType::ADMIN) {
        setupAdminControls();
    } else {
        setupCustomerControls();
    }
    showMainMenu();
}

void MenuWindow::setupAdminControls() {
    addItemButton->show();
    editItemButton->show();
    deleteItemButton->show();
    addCategoryButton->show();
    editCategoryButton->show();
    deleteCategoryButton->show();
}

void MenuWindow::setupCustomerControls() {
    addItemButton->hide();
    editItemButton->hide();
    deleteItemButton->hide();
    addCategoryButton->hide();
}

void MenuWindow::showMainMenu() {
    menuGroup->clear();
    menuGroup->begin();
    
    int x = 420;
    int y = 70;
    createCategoryButton("Salads", x, y, 180, 180);
    x += 200;
    createCategoryButton("Sandwiches", x, y, 180, 180);
    x += 200;
    createCategoryButton("Breakfasts", x, y, 180, 180);
    x += 200;
    createCategoryButton("Desserts", x, y, 180, 180);
    
    x = 420;
    y += 200;
    createCategoryButton("Coffee", x, y, 180, 180);
    x += 200;
    createCategoryButton("Specials", x, y, 180, 180);
    x += 200;
    createCategoryButton("Soft drinks", x, y, 180, 180);
    x += 200;
    createCategoryButton("Pizza menu", x, y, 180, 180);
    // Show admin controls if current user is admin
    if (currentUser && currentUser->type == UserType::ADMIN) {
        setupAdminControls();
    }
    menuGroup->end();
    menuGroup->redraw();
}

void MenuWindow::onBack(Fl_Widget* w, void* v) {
    showMainMenu();
}

void MenuWindow::onIncreaseQuantity(Fl_Widget* w, void* v) {
    if (!receiptDisplay->buffer()->selected()) {
        fl_alert("Please select an item from the receipt first.");
        return;
    }
    
    int start, end;
    receiptDisplay->buffer()->selection_position(&start, &end);
    std::string line = receiptDisplay->buffer()->line_text(receiptDisplay->buffer()->line_start(start));
    
    // Parse the line to get the item name
    std::string itemName = line.substr(0, 13);
    itemName = itemName.substr(0, itemName.find_last_not_of(" \t") + 1);
    
    // Find the item in the current order
    for (auto& item : currentOrder) {
        MenuItem* menuItem = Database::getInstance().getMenuItem(item.menuItemId);
        if (menuItem && menuItem->name == itemName) {
            item.quantity++;
            item.subtotal = item.quantity * item.price;
            delete menuItem;
            updateReceipt();
            return;
        }
        delete menuItem;
    }
}

void MenuWindow::onDecreaseQuantity(Fl_Widget* w, void* v) {
    if (!receiptDisplay->buffer()->selected()) {
        fl_alert("Please select an item from the receipt first.");
        return;
    }
    
    int start, end;
    receiptDisplay->buffer()->selection_position(&start, &end);
    std::string line = receiptDisplay->buffer()->line_text(receiptDisplay->buffer()->line_start(start));
    
    // Parse the line to get the item name
    std::string itemName = line.substr(0, 13);
    itemName = itemName.substr(0, itemName.find_last_not_of(" \t") + 1);
    
    // Find the item in the current order
    for (auto it = currentOrder.begin(); it != currentOrder.end(); ++it) {
        MenuItem* menuItem = Database::getInstance().getMenuItem(it->menuItemId);
        if (menuItem && menuItem->name == itemName) {
            if (it->quantity > 1) {
                it->quantity--;
                it->subtotal = it->quantity * it->price;
            } else {
                currentOrder.erase(it);
            }
            delete menuItem;
            updateReceipt();
            return;
        }
        delete menuItem;
    }
}

void MenuWindow::showItemDialog(bool isEdit) {
    itemDialog = new Fl_Window(400, 300, isEdit ? "Edit Item" : "Add Item");
    
    nameInput = new Fl_Input(120, 20, 200, 25, "Name:");
    priceInput = new Fl_Input(120, 55, 200, 25, "Price:");
    categoryInput = new Fl_Input(120, 90, 200, 25, "Category:");
    descriptionInput = new Fl_Input(120, 125, 200, 25, "Description:");
    
    imageButton = new Fl_Button(120, 160, 200, 25, "Choose Image");
    imageButton->callback(onChooseImage);
    
    Fl_Button* saveButton = new Fl_Button(120, 195, 200, 25, "Save");
    saveButton->callback(onSaveItem);
    
    if (isEdit) {
        // Get selected item details
        int start, end;
        receiptDisplay->buffer()->selection_position(&start, &end);
        std::string line = receiptDisplay->buffer()->line_text(receiptDisplay->buffer()->line_start(start));
        std::string itemName = line.substr(0, 13);
        itemName = itemName.substr(0, itemName.find_last_not_of(" \t") + 1);
        
        // Find the item in the database
        std::vector<MenuItem> items = Database::getInstance().getMenuItems();
        for (const auto& item : items) {
            if (item.name == itemName) {
                nameInput->value(item.name.c_str());
                priceInput->value(std::to_string(item.price).c_str());
                categoryInput->value(item.category.c_str());
                descriptionInput->value(item.description.c_str());
                selectedImagePath = item.imagePath;
                break;
            }
        }
    }
    
    itemDialog->end();
    itemDialog->set_modal();
    itemDialog->show();
}

void MenuWindow::onAddItem(Fl_Widget* w, void* v) {
    showItemDialog(false);
}

void MenuWindow::onEditItem(Fl_Widget* w, void* v) {
    if (!receiptDisplay->buffer()->selected()) {
        fl_alert("Please select an item to edit.");
        return;
    }
    showItemDialog(true);
}

void MenuWindow::onDeleteItem(Fl_Widget* w, void* v) {
    if (!receiptDisplay->buffer()->selected()) {
        fl_alert("Please select an item to delete.");
        return;
    }
    
    int start, end;
    receiptDisplay->buffer()->selection_position(&start, &end);
    std::string line = receiptDisplay->buffer()->line_text(receiptDisplay->buffer()->line_start(start));
    std::string itemName = line.substr(0, 13);
    itemName = itemName.substr(0, itemName.find_last_not_of(" \t") + 1);
    
    if (fl_choice("Are you sure you want to delete this item?", "No", "Yes", nullptr) == 1) {
        std::vector<MenuItem> items = Database::getInstance().getMenuItems();
        for (const auto& item : items) {
            if (item.name == itemName) {
                Database::getInstance().deleteItem(item.id);
                showMainMenu();
                break;
            }
        }
    }
}

void MenuWindow::onAddCategory(Fl_Widget* w, void* v) {
    const char* name = fl_input("Enter category name:");
    if (name && strlen(name) > 0) {
        Database::getInstance().addCategory(name);
        showMainMenu();
    }
}


void MenuWindow::onEditCategory(Fl_Widget* w, void* v) {
    const char* name = fl_input("Enter category name to edit:");
    if (name && strlen(name) > 0) {
        const char* newName = fl_input("Enter new category name:");
        if (newName && strlen(newName) > 0) {
            std::vector<Category> categories = Database::getInstance().getCategories();
            for (const auto& category : categories) {
                if (category.name == name) {
                    Database::getInstance().editCategory(category.id, newName);
                    showMainMenu();
                    break;
                }
            }
        }
    }
}

void MenuWindow::onDeleteCategory(Fl_Widget* w, void* v) {
    const char* name = fl_input("Enter category name to delete:");
    if (name && strlen(name) > 0) {
        if (fl_choice("Are you sure you want to delete this category?", "No", "Yes", nullptr) == 1) {
            std::vector<Category> categories = Database::getInstance().getCategories();
            for (const auto& category : categories) {
                if (category.name == name) {
                    Database::getInstance().deleteCategory(category.id);
                    showMainMenu();
                    break;
                }
            }
        }
    }
}


void MenuWindow::onChooseImage(Fl_Widget* w, void* v) {
    Fl_File_Chooser chooser(".", "Image Files (*.{jpg,jpeg,png})", Fl_File_Chooser::SINGLE, "Choose Image");
    chooser.show();
    while (chooser.shown()) {
        Fl::wait();
    }
    if (chooser.value()) {
        selectedImagePath = chooser.value();
        imageButton->label(fl_filename_name(selectedImagePath.c_str()));
    }
}

void MenuWindow::onSaveItem(Fl_Widget* w, void* v) {
    std::string name = nameInput->value();
    double price = std::stod(priceInput->value());
    std::string category = categoryInput->value();
    std::string description = descriptionInput->value();
    
    if (name.empty() || category.empty()) {
        fl_alert("Name and category are required.");
        return;
    }
    
    bool isEdit = false;
    int itemId = -1;
    
    // Check if we're editing an existing item
    std::vector<MenuItem> items = Database::getInstance().getMenuItems();
    for (const auto& item : items) {
        if (item.name == name) {
            isEdit = true;
            itemId = item.id;
            break;
        }
    }
    
    if (isEdit) {
        Database::getInstance().editItem(itemId, name, price, 0, category, selectedImagePath);  // Changed from updateMenuItem

    } else {
        Database::getInstance().addItem(name, price, 0, category, selectedImagePath);  // Changed from addMenuItem
    }
    
    itemDialog->hide();
    delete itemDialog;
    itemDialog = nullptr;
    showMainMenu();
}

void MenuWindow::createCategoryButton(const char* label, int x, int y, int w, int h) {
    Fl_Button* btn = new Fl_Button(x, y, w, h, label);
    btn->callback(onCategoryClick);
}

void MenuWindow::onCategoryClick(Fl_Widget* w, void* v) {
    // Clear existing menu items
    menuGroup->clear();
    menuGroup->begin();
    
    std::string category = w->label();
    int x = 420;
    int y = 20;
    int count = 0;
    
    // Get menu items from database
    std::vector<MenuItem> items = Database::getInstance().getMenuItems(category);
    
    for (const auto& item : items) {
        if (item.available) {
            Fl_Button* btn = new Fl_Button(x, y, 180, 180);
            std::stringstream ss;
            ss << item.name << "\n$" << std::fixed << std::setprecision(2) << item.price;
            btn->copy_label(ss.str().c_str());
            
            // Store pointer to menu item for callback
            MenuItem* itemPtr = new MenuItem(item);
            btn->callback(onItemClick, (void*)itemPtr);
            
            count++;
            if (count % 4 == 0) {
                x = 420;
                y += 200;
            } else {
                x += 200;
            }
        }
    }
    
    menuGroup->end();
    menuGroup->redraw();
}

void MenuWindow::onItemClick(Fl_Widget* w, void* v) {
    const MenuItem* menuItem = static_cast<const MenuItem*>(v);
    
    // Check if item already exists in order
    auto it = std::find_if(currentOrder.begin(), currentOrder.end(),
        [menuItem](const OrderItem& orderItem) { return orderItem.menuItemId == menuItem->id; });
    
    if (it != currentOrder.end()) {
        // Increment quantity
        it->quantity++;
        it->subtotal = it->quantity * it->price;
    } else {
        // Add new item
        OrderItem newItem;
        newItem.menuItemId = menuItem->id;
        newItem.quantity = 1;
        newItem.price = menuItem->price;
        newItem.subtotal = menuItem->price;
        currentOrder.push_back(newItem);
    }
    
    updateReceipt();
}

void MenuWindow::updateReceipt() {
    std::stringstream ss;
    ss << "Name          Qty  Price  Subtotal\n";
    ss << "--------------------------------\n";
    
    totalAmount = 0.0;
    for (const auto& item : currentOrder) {
        // Get menu item details
        MenuItem* menuItem = Database::getInstance().getMenuItem(item.menuItemId);
        if (menuItem) {
            ss << std::left << std::setw(14) << menuItem->name.substr(0, 13)
               << std::right << std::setw(3) << item.quantity
               << std::setw(7) << std::fixed << std::setprecision(2) << item.price
               << std::setw(9) << item.subtotal << "\n";
            totalAmount += item.subtotal;
            delete menuItem;
        }
    }
    
    ss << "--------------------------------\n";
    ss << "Total: $" << std::fixed << std::setprecision(2) << totalAmount;
    
    receiptBuffer->text(ss.str().c_str());
}

void MenuWindow::onSendTicket(Fl_Widget* w, void* v) {
    if (currentOrder.empty()) {
        fl_alert("Please add items to your order first.");
        return;
    }
    
    // Create order in database
    int orderId = Database::getInstance().createOrder(currentUser->id, currentOrder);
    if (orderId > 0) {
        fl_message("Order #%d has been sent to the kitchen!", orderId);
        clearOrder();
    } else {
        fl_alert("Failed to create order. Please try again.");
    }
}

void MenuWindow::onPay(Fl_Widget* w, void* v) {
    if (currentOrder.empty()) {
        fl_alert("Please add items to your order first.");
        return;
    }
    
    std::stringstream ss;
    ss << "Total amount to pay: $" << std::fixed << std::setprecision(2) << totalAmount;
    if (fl_choice("%s\n\nProceed with payment?", "No", "Yes", nullptr, ss.str().c_str()) == 1) {
        // Create order and mark as paid
        int orderId = Database::getInstance().createOrder(currentUser->id, currentOrder);
        if (orderId > 0) {
            Database::getInstance().updateOrderStatus(orderId, "paid");
            fl_message("Payment successful! Order #%d has been processed.", orderId);
            clearOrder();
        } else {
            fl_alert("Failed to process payment. Please try again.");
        }
    }
}

void MenuWindow::clearOrder() {
    currentOrder.clear();
    totalAmount = 0.0;
    updateReceipt();
}

void MenuWindow::show() {
    if (window) {
        window->show();
    }
}

void MenuWindow::hide() {
    if (window) {
        window->hide();
    }
}

bool MenuWindow::isVisible() {
    return window && window->visible();
} 