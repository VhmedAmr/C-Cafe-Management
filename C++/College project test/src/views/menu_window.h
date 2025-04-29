#ifndef MENU_WINDOW_H
#define MENU_WINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_File_Chooser.H>
#include <vector>
#include <string>
#include "../database.h"

class MenuWindow {
public:
    static void init();
    static void show();
    static void hide();
    static bool isVisible();
    static void setCurrentUser(User* user);

private:
    static Fl_Window* window;
    static Fl_Group* menuGroup;
    static Fl_Group* receiptGroup;
    static Fl_Text_Display* receiptDisplay;
    static Fl_Text_Buffer* receiptBuffer;
    static std::vector<OrderItem> currentOrder;
    static double totalAmount;
    static User* currentUser;
    
    // Back button
    static Fl_Button* backButton;
    
    // Admin controls
    static Fl_Button* addItemButton;
    static Fl_Button* editItemButton;
    static Fl_Button* deleteItemButton;
    static Fl_Button* addCategoryButton;
    static Fl_Button* editCategoryButton;
    static Fl_Button* deleteCategoryButton;
    
    // Item management dialog
    static Fl_Window* itemDialog;
    static Fl_Input* nameInput;
    static Fl_Input* priceInput;
    static Fl_Input* categoryInput;
    static Fl_Input* descriptionInput;
    static Fl_Button* imageButton;
    static std::string selectedImagePath;
    
    // Callbacks
    static void onBack(Fl_Widget* w, void* v);
    static void onCategoryClick(Fl_Widget* w, void* v);
    static void onItemClick(Fl_Widget* w, void* v);
    static void onSendTicket(Fl_Widget* w, void* v);
    static void onPay(Fl_Widget* w, void* v);
    static void onIncreaseQuantity(Fl_Widget* w, void* v);
    static void onDecreaseQuantity(Fl_Widget* w, void* v);
    static void onAddItem(Fl_Widget* w, void* v);
    static void onEditItem(Fl_Widget* w, void* v);
    static void onDeleteItem(Fl_Widget* w, void* v);
    static void onAddCategory(Fl_Widget* w, void* v);
    static void onEditCategory(Fl_Widget* w, void* v);  // Add this line
    static void onDeleteCategory(Fl_Widget* w, void* v);  // Add this line
    static void onChooseImage(Fl_Widget* w, void* v);
    static void onSaveItem(Fl_Widget* w, void* v);
    
    // Helper functions
    static void createCategoryButton(const char* label, int x, int y, int w, int h);
    static void updateReceipt();
    static void clearOrder();
    static void showItemDialog(bool isEdit = false);
    static void setupAdminControls();
    static void setupCustomerControls();
    static void showMainMenu();
};

#endif // MENU_WINDOW_H 