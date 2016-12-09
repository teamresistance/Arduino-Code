#ifndef Menu_h
#define Menu_h

#include "Arduino.h"

#define MAINMENU 0
#define SUBMENU 1
#define CONFMENU 2

#define NO_KEY 0
#define NEXT_KEY 1
#define SEL_KEY 2
#define UP_KEY 3
#define DN_KEY 4
#define ESC_KEY 5

class Menu
{
  public:	
    Menu();
    void SetMaxItems(int maxItems);
    void SetNames(String * p_names);
    int GetMenuMode();                //Returns Menu Mode, Main, Sub, Confirm
    int GetActPtr();                  //Returns Active Menu Pointer, 1 to max items
    String GetActName();
    int GetSubPtr();                  //Returns Sub Menu Pointer, 1 to max items
    String GetSubName();
    void UpdateMenu(int keyVal);      //Update submenu pointer or action
  private:
    int _maxItems;                    //Max number of items
    int _subMenuPtr;                  //Update Pointer to subMenu choice
    int _menuMode;                    //Used to determine how to handle Sel & Esc
    int _actMenuPtr;                  //Active Pointer to subMenu choice
    String * _p_names;            //Pointer to menu names
};

#endif
