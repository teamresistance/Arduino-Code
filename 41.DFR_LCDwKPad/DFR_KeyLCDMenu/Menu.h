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
    int getMenuMode();            //Returns Menu Mode
    void UpdateMenu(int keyVal)   //Update submenu pointer or action
  private:
    void setMenuMode(int mode);       //Sets Menu Mode
    void setMaxItems(int maxItems);   //Set the max menu items
    int _menuMode                     //Used to determine how to handle Sel & Esc
    int _subMenuPtr                   //Points to subMenu choice
    int _actMenuPtr
};

#endif
