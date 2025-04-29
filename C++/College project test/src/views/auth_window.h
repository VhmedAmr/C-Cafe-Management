#ifndef AUTH_WINDOW_H
#define AUTH_WINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Box.H>
#include <string>
#include "database.h"

class AuthWindow {
private:
    static Fl_Window* window;
    static Fl_Tabs* tabs;
    static Fl_Group* loginGroup;
    static Fl_Group* registerGroup;
    
    // Login inputs
    static Fl_Input* loginEmail;
    static Fl_Input* loginPassword;
    static Fl_Button* loginButton;
    
    // Register inputs
    static Fl_Input* registerName;
    static Fl_Input* registerEmail;
    static Fl_Input* registerPassword;
    static Fl_Input* registerConfirmPassword;
    static Fl_Choice* registerType;
    static Fl_Button* registerButton;
    
    // Callback functions
    static void onLogin(Fl_Widget* w, void* v);
    static void onRegister(Fl_Widget* w, void* v);
    static void onWindowClose(Fl_Widget* w, void* v);
    
    // Helper functions
    static void clearInputs();
    static void showError(const char* message);
    static void showSuccess(const char* message);

public:
    static void init();
    static void show();
    static void hide();
    static bool isVisible();
};

#endif // AUTH_WINDOW_H 