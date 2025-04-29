#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include "views/menu_window.h"
#include "views/auth_window.h"

int main(int argc, char *argv[]) {
    // Initialize the database
    if (!Database::getInstance().initialize()) {
        fl_alert("Failed to connect to the database. Please check your database settings.");
        return 1;
    }
    
    // Initialize the menu window
    MenuWindow::init();
    
    // Show the authentication window first
    AuthWindow::init();
    AuthWindow::show();
    
    // Run the FLTK event loop
    return Fl::run();
} 