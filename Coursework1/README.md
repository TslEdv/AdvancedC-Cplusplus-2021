# Initial data

The initial data is located into a data structure consisting of arrays of pointers, headers and items.
Declarations of items as C / C++ structs are presented in file Items.h. 
There are 10 different types of items(ITEM1, ITEM2, …, ITEM10). 
Declarations of headers as C / C++ structs are presented in file Headers.h.
There are 5 different types of headers (HEADER_A, HEADER_B, HEADER_C, HEADER_D, HEADER_E).
The both files are stored in Instructor's stuff.
There are 5 different types of data structures (Struct1, Struct2, Struct3, Struct4, Struct5). 
To generate the initial data structure you have to use functions from ICS0017DataSource.dll. 
This DLL is implemented by instructor and stored in Instructor's stuff. 
It needs auxiliary file Colors.txt, created from https://en.m.wikipedia.org/wiki/Lists_of_colors.
To understand the building principles of our data structures analyse the examples on the following pages.
Let us emphasize that they are just examples: the actual presence and absence of items and headers is
determined by the work of item generator built into ICS0017DataSource.dll and is largerly occasional.
The DLL imports 6 public functions declared in file ICS0017DataSource.h (also stored in Instructor's stuff).
Five of them create data structure and return the pointer to it. The sixth function (GetItem()) constructs a
stand-alone item and returns the pointer to it. There is also a password-protected function for the
instructor. Comments explaining the usage of public functions are in ICS0017DataSource.h.

# Task 1

Write a C / C++ function that prints in the command prompt window the contents of all the items from
the current data structure.
Prototype (text printed in blue depends on the type of data structure, specify it yourself):
void PrintDataStructure(pointer_to_data_structure);
If the data structure is empty, print an error message.

# Task 2

Write a C / C++ function that inserts a new item into the current data structure.
Prototype (text printed in blue depends on the type of data structure, specify it yourself):
pointer_to_data_structure = InsertItem(pointer_to_data_structure, char *pNewItemID = 0);
The new item must be constructed by function GetItem() from ICS0017DataSource.dll. The user may set
the ID itself or set the pointer to it to zero. In the last case the ID is selected by GetItem().
You may freely select the position of new item in the linked list of items.
The return value is the pointer to the modified data structure.
The function must keep the current contents of data structure and throw an exception if:
• An item with same ID is already in the data structure.
• The ID presented by user does not follow the formatting rules.

# Task 3

Write a C / C++ function that inserts a new item into the current data structure.
Prototype (text printed in blue depends on the type of data structure, specify it yourself):
pointer_to_data_structure = RemoveItem(pointer_to_data_structure, char *pItemID);
The memory fields occupied by the item to be removed must be released (use operator delete). Do not
forget that if a header has lost all the items or other headers associated with it, this header itself must
also disappear.
The return value is the pointer to the modified data structure.
The function must keep the current contents of data structure and throw an exception if:
• Item with the specified ID does not exist.
• The ID presented by user does not follow the formatting rules.
