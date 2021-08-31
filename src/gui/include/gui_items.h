#ifndef gui_items_h__

#define gui_items_h__
#include "gui.h"

class cGui;

typedef int(*   bogoFunction )();


/*!\class cGuiItems
\author Thomas Kueppers
\version 1.0
\date 07.03.2007
\brief super class for all gui items

This class defines the basic functionality common to all gui items. This is basically attributes to specify thier geometry, a reference to their associated gui, a value to signal if the mouse is over the item and if the item is active aka in <i>editMode</i>. Each item has also an <i>idTag</i>. Each item has a method to <i>Print</i> the item. The method edit is called if the item is <i>InEditMode</i>. 
!*/

class cGuiItems {
public:
	
	///\brief x coordinate of the left border
	float leftBorder;
	///\brief x coordinate of the right border.
	float rightBorder;
	///\brief y coordinate of the buttom.
	float buttom;
	///\brief y coordinate of the top
	float top;
	///\brief mouseOver is = 1 if the mouse is over the item. This is set in the iterate method of cGui.
	float mouseOver;
	///\brief idTag to identify the item
	char idTag[256];
	///\brief pointer to the gui object this item is controlled by
	cGui * myGui;
	///\brief InEditMode = 1 if the user has activated this item. Set by the controlling gui.
	bool InEditMode(void);

	///\brief Display the gui item.
	virtual int Print(void);
	///\brief Implements the basic function of the item. Called, when InEditMode=1.
	virtual int Edit(void);
	///\brief Testfunction to check if polyphormismen works.
	virtual int testVirtual(int);


};

/*!\class cButton
\Author Thomas Kueppers
\Version 1.0
\Date 07.03.2007
\brief Implementation of a simple button

This class implements a Button. A function pointer is attached to this button an when the button is clicked an the edit function is called,
the nextFunction of the gui containing this button is set to this functionpointer. At the same time the button calls the Quit function of the gui.
!*/
class cButton : public cGuiItems{
public:
	///\brief Text displayed on the button.
	char label[256];
	///\brief Function that will be called, if the button is pressed. If no function should be called after the gui terminates set this to NULL.
	bogoFunction pressFunction;
	
	///\brief Display the button.
	virtual int Print(void);
	///\brief Terminates the gui and sets the gui's nextFunction to pressFunction.
	virtual int Edit(void);
	///\brief hTest function
	virtual int testVirtual(int);

	
};


/*class cQuitButton : public cGuiItems{
public:
	char label[256];
	
	virtual int Print(void);
	virtual int Edit(void);
	virtual int testVirtual(int);

};*/

/*!\class cTextLabel
\Author Thomas Kueppers
\Version 1.0
\Date 07.03.2007
\brief Implementation of a simple text label.

This class implements a simple text label, that displays a specified text. It's edit function does nothing.
!*/
class cTextLabel : public cGuiItems{
public:
	///\brief The displayed text.
	char  text[256];
	///\brief Displays the text label.
	int Print(void);
	///\brief The edit method of cTextLabel returns directly.
	int Edit(void);
	///\brief Textfunction.
	virtual int testVirtual(int); 
};

/*!\class cCharField
\Author Thomas Kueppers
\Version 1.0
\Date 07.03.2007
\brief Implementation of a character field.

This class implements a field where single characters can be entered.
!*/
class cCharField : public cGuiItems{
public:
	///\brief Keysym of the currently selected character.
	SDLKey * keysym;
	
	///\brief Displays the currently selected character.
	int Print(void);
	///\brief If the char field is activated it waits for the next keyboard input and stores it as new keysym, befor it returns.
	int Edit(void);
	///\brief testfunction
	int testVirtual(int); 
};

/*!\class cTextLineField
\Author Thomas Kueppers
\Version 1.0
\Date 07.03.2007
\brief Gui item to enter a line of text.
This class implements a gui item that stores a line of text, that can be entered and alterd by the user. If the item changes to edit mode the
text starts to blink and the user can chage it via the keyboard.

!*/
class cTextLineField : public cGuiItems{
public:
	///\brief Currently stored text.
	char * text;
	///\brief Maximal length of the stored text.
	unsigned int textLength;
	///\brief Constructor
	cTextLineField(void);
	///\brief Displays the current text. If the item is in edit mode the text blinks.
	int Print(void);
	///\brief If in edit mode the text can be changed via keyboard. Returns, when return is pressed.
	int Edit(void);
	///\brief Testfunction
	int testVirtual(int); 
	
private:
	///\brief Used to calculate the blinkintervalls
	unsigned int blinkCounter;
	///\brief Every 500ms the used font is changed, Print uses this font.
	GLuint currentBlinkFont;	
};

/*!\class cListDialogue
\Author Thomas Kueppers
\Version 1.0
\Date 07.03.2007
\brief Implements a List with options to choose from.

This class implements a Dialog, that contains of a List from which Options can be choosen. The Dialog consits of the display of the list and
to arrows, to change the selected list element. 

!*/
class cListDialogue : public cGuiItems{
public:
	///\brief Pointer to the a character string. The selected text will be copied to this character string.
	char * text;
	///\brief Pointer to the complete list of selections available in the gui item.
	char ** currentList;
	///\brief Length of the list.
	int currentListLength;
	///\brief How many row of the currentList shall be displayed at once.
	int rows;
	
	///\brief Constructor
	cListDialogue();
	///\brief Destructor
	~cListDialogue();
	///\Display the gui item
	int Print(void);
	///\brief If the user clicks on one of the arrows, the selection in the displayed list is shifted by one element.
	int Edit(void);
	///\brief Test function.
	int testVirtual(int);
	///\brief Set the geometry of the gui item. <i>calcSize()</i> is called, so that a integer number of rows can displayed in the table.
	int setPosition(float,float,float,float);
	///\brief Tells cListDialogue the size of the standard font used by the gui.
	/*! Calls calcSize, so that an integer number of rows can be displayed in the table!*/
	int setFontPixelHeight(int);
	///\brief Reset the list index so that the text stored in <i>text</i> is highlighted.
	int indexText();
	///\brief change the size, so that it looks nicer.
	int calcSize();
	
private:
	///\brief Length of the part of the <i>currentList</i> that is currently visible
	int displayListLength;
	///\brief The part of the List, that is currently visible.
	char displayList[40][40];
	///\brief The index locating the currently selected item in the list of visible items.
	int displayListIndex;
	///\brief The index locating the currently selected item in the list of all items.
	int currentListIndex;
	///\brief Height of the standard font of the gui in pixel.
	/*! Used to calculate the size of the dialog frame. If size of the highlight and the standard font of the gui differ differ greatly this might 
	lead to trouble.
	!*/
	float fontHeight;
};

#endif


