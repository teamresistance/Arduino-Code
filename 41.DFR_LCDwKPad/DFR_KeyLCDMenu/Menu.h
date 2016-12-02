#ifndef Menu_h
#define Menu_h

#include "Arduino.h"

#define MAINMENU 0
#define SUBMENU 1
#define CONFMENU 2

class Menu
{
  public:	
    Menu();
    int GetMenuMode();                //Returns Menu Mode, Main, Sub, Confirm
    int GetActMenu();                 //Returns Active Menu Pointer, 1 to max items
    void UpdateMenu(int keyVal)       //Update submenu pointer or action
  private:
    void setMenuMode(int mode);       //Sets Menu Mode
    void setMaxItems(int maxItems);   //Set the max menu items
    int _menuMode                     //Used to determine how to handle Sel & Esc
    int _subMenuPtr                   //Update Pointer to subMenu choice
    int _actMenuPtr                   //Active Pointer to subMenu choice
};

#endif
