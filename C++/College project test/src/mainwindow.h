#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_File_Chooser.H>
#include <vector>
#include <string>
#include "database.h"
#include "custom_table.h"
#include "views/auth_window.h"

class MainWindow {
private:
    static Fl_Window* window;
    static Database* db;
    static User* currentUser;
    
    // Tables
    static CustomTable* ordersTable;
    static CustomTable* menuTable;
    static CustomTable* inventoryTable;
    static CustomTable* employeesTable;
    static CustomTable* cartTable;
    
    // Buttons
    static Fl_Button* backButton;
    static Fl_Button* addOrderBtn;
    static Fl_Button* addMenuItemBtn;
    static Fl_Button* addInventoryItemBtn;
    static Fl_Button* addEmployeeBtn;
    static Fl_Button* addToCartBtn;
    static Fl_Button* checkoutBtn;
    static Fl_Button* logoutBtn;
    static Fl_Button* increaseQuantityBtn;
    static Fl_Button* decreaseQuantityBtn;
    
    // Admin UI Elements
    static Fl_Button* addCategoryBtn;
    static Fl_Button* editItemBtn;
    static Fl_Button* deleteItemBtn;
    static Fl_Button* editCategoryBtn;
    static Fl_Button* deleteCategoryBtn;
    static Fl_Button* addItemBtn;
    static Fl_Input* itemNameInput;
    static Fl_Input* itemPriceInput;
    static Fl_Input* itemCategoryInput;
    static Fl_Input* itemDescriptionInput;
    static Fl_Button* chooseImageBtn;
    static Fl_Button* saveItemBtn;
    
    // Item management inputs
    static std::string selectedImagePath;
    
    // Callback functions
    static void onBack(Fl_Widget* w, void* v);
    static void onAddOrder(Fl_Widget* w, void* v);
    static void onAddMenuItem(Fl_Widget* w, void* v);
    static void onAddInventoryItem(Fl_Widget* w, void* v);
    static void onAddEmployee(Fl_Widget* w, void* v);
    static void onAddToCart(Fl_Widget* w, void* v);
    static void onCheckout(Fl_Widget* w, void* v);
    static void onLogout(Fl_Widget* w, void* v);
    static void onIncreaseQuantity(Fl_Widget* w, void* v);
    static void onDecreaseQuantity(Fl_Widget* w, void* v);
    static void onAddCategory(Fl_Widget* w, void* v);
    static void onEditItem(Fl_Widget* w, void* v);
    static void onDeleteItem(Fl_Widget* w, void* v);
    static void onEditCategory(Fl_Widget* w, void* v);
    static void onDeleteCategory(Fl_Widget* w, void* v);
    static void onChooseImage(Fl_Widget* w, void* v);
    static void onSaveItem(Fl_Widget* w, void* v);
    
    // Helper functions
    static void showAddItemDialog();
    static void showEditItemDialog(int itemId);
    static void updateCartDisplay();
    static void setupAdminView();
    static void setupCustomerView();
    
    // Table refresh function
    static void refreshTables();
    static void hideAllTables();
    static void showItemManagementDialog(bool isEdit = false);

public:
    static void init();
    static void show();
    static void showMessage(const char* title, const char* message);
    static void setCurrentUser(User* user);
    static User* getCurrentUser();
};

#endif // MAINWINDOW_H 