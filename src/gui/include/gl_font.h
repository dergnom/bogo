/*! \file gl_font.h
\brief Header for a OpenGL font API.
\author Thomas Kueppers
\version 1.0
\date 07.03.2007
\warning This version is NOT! thread safe. -> fileNameForChar(...) usage of static variables

gl_font is an implementation of a of a very limited font API for OpenGL. It uses bitmap fonts to display character strings
on the screen. The bitmap font is first loaded by <i>loadBMPFont</i>. This functions returns an array of pointers to SDL_Surfaces that
contain the images of the characters the font is able to display. The "font" has to be generated before it is loaded. This API uses the
to preprocessor definitions ASCII_SLICE and ASCII_START. ASCII_START is the ASCII code of is the first character the bitmap font contains.
ASCII_SLICE is the number of characters it contains, beginning with the first character specified by ASCII_START. <i>loadBMPFont</i> 
needs a directory that contains an image for every character in the font. After these font has been loaded into SDL_Surfaces, it can be
converted into OpenGL display lists via <i>generateDisplayLists</i>. It makes each character accesible via an OpenGL display list. <i>generateDisplayLists</i> generates ASCII_START + ASCII_SLICE lists and returns the index of the first (most likely empty) display list.
This way of display list arrangememt makes it possible, to calculate each display list index as it's  ASCII value plus the index of the first
display list. The function <i>writeDisplayList</i> uses these display lists, to write character strings to screen. 
!*/

#ifndef font_h__
#define font_h__



/// \brief defines, how many characters are in the font.
#define ASCII_SLICE 95
/// \brief defines the firs character in the font
#define ASCII_START 32



/*!
Returns the file name to store the characters bitmap image under. as file name the ascii code plus ".bmp" is used.
@param character <b>char</b> the character whose filename to return.
@returns <b>char *</b> the file name
!*/
char * fileNameForChar(char character);

/*!
Returns the file name for the specified character. the file name is the integer parameter plus ".bmp".
@param character <b>int</b> ascii code for the character whose filenam is to be generated.
@returns <b>char *</b> the file name
!*/
char * fileNameForChar(int character);

/*!
Loads a complete bitmap font. this fonts contains of one bitmap image for every character the font is able 
to display. The font begins at the character specified by ASCII_START and has ASCII_SLICE characters, where 
the character with the ASCII code ASCII_START is the first character.
@param name <b>const char  *</b> name of the directory where the images for the bitmap font are stored. name can 
be an abslute path (beginning with /) or a relative path to the present working directory of the application (beginning with ./).
@param array <b>SDL_Surface *</b> This array of pointers to SDL_Surraces is used to store each bitmap image for the bitmap
font. It must have one element for each character in the font. So the lenght of the array has to be equal to ASCII_SLICE.
The SDL_Surfaces are allocated by loadBMPFont. If not otherwise noticed these SDL_Surfaces have to bee 'free'ed manually.
@returns <b>int</b> 0 on success.
!*/
int loadBMPFont(const char * name,SDL_Surface * array[ASCII_SLICE]);

/*!
Generates an OpenGL display list for each character in the display font. The character display lists are ordered by the ASCII value. ASCII_START
empty display lists are generated followed by ASCII_SLICE lists containing the image data of the actually present characters. So the characters
can be referenced by there ASCCI code  plus the offset of the first display list. This offset is returned by this function.
@param fontArray <b>SDL_Surface *</b> This array has to contain ASCII_SLICE elements containing pointers to SDL_Surfaces containing images of each
character. This functions 'free's the memory allocated for each element of fontArray, so 'free' must not be called on them manually.
@return Index of the first generated display list. This list is the offset used to access the character of the bitmap font. 
!*/
GLuint generateDisplayList(SDL_Surface * fontArray[ASCII_SLICE]);

/*!
Write a string on the screen using OpenGL display lists containing bitmap characters \see generateDisplayList .
@param fontIndex <b>GLuint</b> Index to the first (normally empty, if ASCII_START > 0) display list containing  the characters of the bitmap font.
@param text <b>const char * </b> The text to write.
@param xPos <b>float</b> xPosition to start the writing.
@param yPos <b>float</b> yPosition to start the writing.
@param zPos <b>float</b> zPosition to start the writing.
!*/
int writeDisplayList(GLuint fontIndex,const char * text, float xPos, float yPos, float zPos);

/*!
Write a string on the screen using OpenGL display lists containing bitmap characters \see generateDisplayList .
A small blue triangle is placed in front of the first written character.
@param fontIndex <b>GLuint</b> Index to the first (normally empty, if ASCII_START > 0) display list containing  the characters of the bitmap font.
@param text <b>const char * </b> The text to write.
@param xPos <b>float</b> xPosition to start the writing.
@param yPos <b>float</b> yPosition to start the writing.
@param zPos <b>float</b> zPosition to start the writing.
!*/
int writeDisplayListTri(GLuint fontIndex,const char * text, float xPos, float yPos, float zPos);

#endif
