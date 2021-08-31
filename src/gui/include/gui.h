 /*! \file gui.h
\brief Header for a Graphical User Interface class.
\version 1.0
\date 07.03.2007
This defines a GUI class.
!*/
 
 
 #ifndef gui_h__
#define gui_h__

#ifdef WIN32
#include "Python/Python.h"
#include <windows.h>
#include "GL/gl.h"
#include "GL/glext.h"
#else
#include "Python.h"
#include "GL/gl.h"
#endif

#include "SDL/SDL.h"
#include "gui_items.h"

#ifndef BOGOFUNCTION
/*!\typedef bogoFunction
\brief Defines the type bogoFunction as a function pointer to a function returning an Integer withount input arguments.
*/
typedef int (*bogoFunction)();
#endif

class cGuiItems;
/*!
\class cGui
\brief Implementation of a Graphical User Interface
\Author Thomas Kueppers
\version 1.0
\attention The gui classes display facilities are currently hardwired for a 800x600 and 24 bit color display environment using an OpenGL rendering context. This context has to be set before the gui draws it's items. Or else undefined behaviour mihgth occur ;-)

 The gui class contain 0..n gui items. Each of it's gui items has to be a object of the class cGuiItems. Besides managing the gui items the gui class is also able to perform the drawing of the gui and the handling of user input. The gui class uses 
OpenGL for drawing and SDL for user interfacing.

If in loop mode the gui iterates through the following cycle:

	- Drawing
	  It calls the print method of each of it's gui item
	
	- it handles user input an checks, if the user activated one of the gui elements
	
	- if the user activated one of theses items controll is passed over to this item -> the edit 
	  method of this item is called. The actions of this method depend on the actual class
	  of the gui item. When the edit method is finished, controll is passed back to the gui loop.
	
	- If the edit function has called for the gui to quit the loop is finished. Else a new cycle beginns.
	
The gui class has several methods to manage gui items. <i>AddGuiItem</i> adds a new instance of a class derived from cGuiItems to the items controlled by the gui. <i>DeleteAllGuiItems</i> deletes all gui items controlled by the gui. <i>GetGuiItemIndexFromIdTag</i> returns the index of an gui item, that is 
registered with the gui and has the specified id tag. To display several of the gui item classes fonts are used. <i>loadFont</i> and <i>loadHighlightFont</i> are used to load bitmap fonts to display text. <i>InitBackground</i> is used to load an image to be used as background for the gui. <i>Quit</i> is used to tell the gui to terminate. After the termination of a Gui you have to delete the gui to completely free it's used memory. Before you do so, you might want to read certain data from the gui, especially the value of <i>nextFunction</i>. Certain gui items, e.g. buttons, have function pointer attached to them. if a button is clicked, the button stores it's function pointer in the variable and calls the <i>Quit</i> method, signalling, that the program using the gui class should go on wiht this function. In this case you have to save these function pointer, delete the gui to release memory and call the selected fucntion pointer from your source code.
!*/
class cGui{
 public:
 	/*!
 	Loop calls <i>Iterate</i> every 50 ms. until one of the gui items that are called by <i>Iterate</i> calls <i>Quit</i>.
 	!*/
 	int Loop();
 	
 	/*!
 	Perform one iteration of the gui cycle. The cycle contains drawing of the gui elements and handling user input since last iteration. if 
 	the user activates a gui item, the appropriate methods of this element are also called.
 	!*/
	int Iterate();
	
	/*!
	Initialize the background of the gui.
	@param fileName <b> const char * </b> Full Path of an image to use as Background. Must be an Jpeg, Png or Bmp image.
	!*/
	int InitBackground(const char * fileName);
	
	/*!Add a guiItem to the list of controlled gui items. Each element in this list is drawn and is accesible via user input
	@param newItem <b>cGuiItems</b> Object of class derived from cGuiItems.
	!*/
	int AddGuiItem(cGuiItems * newItem);
	
	/*!Signal the gui to stop running the function <i>Loop</i>. Mainly used inside the edit methods of the gui items controlled by the gui.
	!*/
	int Quit();
	
	/*!Load a font to use as standard display font. Uses loadBMPFont \see loadBMPFont and generateDisplay \see generateDisplay list to load the font.
	If no font is loaded certain gui items might not work correctly.
	@param fontName <b>char * </b> Complete path to the directory containig the font.
	!*/
	int loadFont(char * fontName);
	
	/*!Load a font to use as font for highlighted text. Uses loadBMPFont \see loadBMPFont and generateDisplay \see generateDisplay list to
	 load the font. If no font is loaded certain gui items might not work correctly.
	@param fontName <b>char * </b> Complete path to the directory containig the font.
	!*/
	int loadHighlightFont(char * fontName);
	
	/*!Calls <i>delete</i> on all gui items controlled by the gui
	!*/
	int DeleteAllGuiItems();
	
	/*!Each object of class guiItem or it's derived classes has string member, that is used as id tag. This functions returns the index of the first 
	element with the specified id tag in the guiItems array.
	@param searchTag <b>const char *</b> the id tag to search for
	@return the index of the gui item with the specified id tag in the guiItems array or -1 if searchTag is not found.
	!*/
	int GetGuiItemIndexFromIdTag(const char * searchTag);
	
	/// Constructor, oerloaded
	cGui(void);
	
	/*!Constructor, overloaded
	Loads the standard and the highlight font.
	@param stdFont <b>const char *</b> Complete path of the directory containing the selected bitmap font.
	@param highlightFont <b>const char *</b> Complete path of the directory containing the selected bitmap font.
	!*/
	cGui(const char * stdFont, const char * highlightedFont);
	
	///Destructor
	~cGui(void);
	
	///Dispaly List Index to the first display list of the standard font
	GLuint font;
	///Display List Index to the first display list of the highlight font
	GLuint fontHighlight;
	///If quit=1 the <i>loop</i> functions terminates
	int quit;
	///If editMode=1 the edit Method of the gui item in the editSlot is called.
	int editMode;
	///Active gui item. This is the gui item acitvated by the user.
	cGuiItems * editSlot;
	///Array conatining the gui items controlled by the gui.
	cGuiItems ** guiItems;
	///Size of the guiItems array
	int numOfGuiItems;
	///Index of the display List containing the background image
	GLuint background;
	///???
	SDL_Event * currentEvent;
	/*!Certain gui items (e.g buttons) can store the next function that should be called, after the gui terminates, before these gui
	 items therminate the gui.
	 !*/
	bogoFunction nextFunction;

};

#endif
