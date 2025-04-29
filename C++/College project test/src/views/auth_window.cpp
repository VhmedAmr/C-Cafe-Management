#include "auth_window.h"
#include <FL/fl_ask.H>
#include <FL/Fl_Secret_Input.H>
#include "../mainwindow.h"
#include "menu_window.h"

// Static member initialization
Fl_Window* AuthWindow::window = nullptr;
Fl_Tabs* AuthWindow::tabs = nullptr;
Fl_Group* AuthWindow::loginGroup = nullptr;
Fl_Group* AuthWindow::registerGroup = nullptr;
Fl_Input* AuthWindow::loginEmail = nullptr;
Fl_Input* AuthWindow::loginPassword = nullptr;
Fl_Button* AuthWindow::loginButton = nullptr;
Fl_Input* AuthWindow::registerName = nullptr;
Fl_Input* AuthWindow::registerEmail = nullptr;
Fl_Input* AuthWindow::registerPassword = nullptr;
Fl_Input* AuthWindow::registerConfirmPassword = nullptr;
Fl_Choice* AuthWindow::registerType = nullptr;
Fl_Button* AuthWindow::registerButton = nullptr;

void AuthWindow::init() {
    // Create main window
    window = new Fl_Window(400, 400, "Cafe Management System - Login");
    window->callback(onWindowClose);
    
    // Create tabs
    tabs = new Fl_Tabs(10, 10, 380, 380);
    
    // Login tab
    loginGroup = new Fl_Group(10, 35, 380, 355, "Login");
    loginGroup->begin();
    
    // Login inputs
    new Fl_Box(20, 45, 360, 20, "Email:");
    loginEmail = new Fl_Input(20, 65, 360, 25);
    
    new Fl_Box(20, 95, 360, 20, "Password:");
    loginPassword = new Fl_Secret_Input(20, 115, 360, 25);
    
    loginButton = new Fl_Button(150, 160, 100, 30, "Login");
    loginButton->callback(onLogin);
    
    loginGroup->end();
    
    // Register tab
    registerGroup = new Fl_Group(10, 35, 380, 355, "Register");
    registerGroup->begin();
    
    // Register inputs
    new Fl_Box(20, 45, 360, 20, "Name:");
    registerName = new Fl_Input(20, 65, 360, 25);
    
    new Fl_Box(20, 95, 360, 20, "Email:");
    registerEmail = new Fl_Input(20, 115, 360, 25);
    
    new Fl_Box(20, 145, 360, 20, "Password:");
    registerPassword = new Fl_Secret_Input(20, 165, 360, 25);
    
    new Fl_Box(20, 195, 360, 20, "Confirm Password:");
    registerConfirmPassword = new Fl_Secret_Input(20, 215, 360, 25);
    
    new Fl_Box(20, 245, 360, 20, "Account Type:");
    registerType = new Fl_Choice(20, 265, 360, 25);
    registerType->add("Customer");
    registerType->add("Admin");
    registerType->value(0);
    
    registerButton = new Fl_Button(150, 310, 100, 30, "Register");
    registerButton->callback(onRegister);
    
    registerGroup->end();
    
    // Add tabs to window
    tabs->add(loginGroup);
    tabs->add(registerGroup);
    tabs->end();
    
    window->end();
}

void AuthWindow::show() {
    if (window) {
        window->show();
    }
}

void AuthWindow::hide() {
    if (window) {
        window->hide();
    }
}

bool AuthWindow::isVisible() {
    return window && window->visible();
}

void AuthWindow::onLogin(Fl_Widget* w, void* v) {
    std::string email = loginEmail->value();
    std::string password = loginPassword->value();
    
    if (email.empty() || password.empty()) {
        showError("Please fill in all fields");
        return;
    }
    
    User* user = Database::getInstance().loginUser(email, password);
    if (user) {
        showSuccess("Login successful!");
        hide();
        // Initialize and show the menu window
        MenuWindow::init();
        MenuWindow::setCurrentUser(user);
        MenuWindow::show();
    } else {
        showError("Invalid email or password");
    }
}

void AuthWindow::onRegister(Fl_Widget* w, void* v) {
    std::string name = registerName->value();
    std::string email = registerEmail->value();
    std::string password = registerPassword->value();
    std::string confirmPassword = registerConfirmPassword->value();
    UserType type = registerType->value() == 0 ? UserType::CUSTOMER : UserType::ADMIN;
    
    if (name.empty() || email.empty() || password.empty() || confirmPassword.empty()) {
        showError("Please fill in all fields");
        return;
    }
    
    if (password != confirmPassword) {
        showError("Passwords do not match");
        return;
    }
    
    if (Database::getInstance().registerUser(email, password, name, type)) {
        showSuccess("Registration successful! Please login.");
        clearInputs();
        tabs->value(loginGroup);
    } else {
        showError("Registration failed. Email may already be in use.");
    }
}

void AuthWindow::onWindowClose(Fl_Widget* w, void* v) {
    hide();
}

void AuthWindow::clearInputs() {
    loginEmail->value("");
    loginPassword->value("");
    registerName->value("");
    registerEmail->value("");
    registerPassword->value("");
    registerConfirmPassword->value("");
    registerType->value(0);
}

void AuthWindow::showError(const char* message) {
    fl_message_title("Error");
    fl_alert("%s", message);
}

void AuthWindow::showSuccess(const char* message) {
    fl_message_title("Success");
    fl_message("%s", message);
} 