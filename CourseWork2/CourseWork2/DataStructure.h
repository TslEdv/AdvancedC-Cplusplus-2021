#pragma once
#include <iostream>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"
class DataStructure {
private:
	HEADER_D* Start;

	char CheckFormat(char* pNewItemID) { //function for checking formatting
		if (strlen(pNewItemID) < 3) { //check the lenght of new ID
			throw("Formatting error! (Length of the new item ID is too short -%s)\n", pNewItemID);
		}
		int spacecounter = 0;
		for (int i = 0; ; i++) { //count the number of spaces and check characters
			if (*(pNewItemID + i) == '\0') {
				break;
			}
			if (*(pNewItemID + i) == ' ') {
				spacecounter++;
			}
			if (isdigit(*(pNewItemID + i)) || ((!isalpha(*(pNewItemID + i))) && (*(pNewItemID + i) != ' ')) && (*(pNewItemID + i) != '-')) { //checks the input id for digits
				throw("Formatting error! (Detected an illegal character -%s)\n", pNewItemID);
			}
		}
		if (spacecounter == 0 || spacecounter > 1) { //if there are more than 1 space or 0 spaces, printing an error
			throw("Formatting error! (Must have 1 space -%s)\n", pNewItemID);
		}
		char* spacefinder = strchr(pNewItemID, ' '); //pointer to space of the word
		if ((pNewItemID[0] >= 'A' && pNewItemID[0] <= 'Z') && (*(spacefinder + 1) >= 'A' && *(spacefinder + 1) <= 'Z')) {// check if the first letters 
		//of the words are capital letters
			return *(spacefinder + 1);
		}
		else {
			throw("Formatting error! (Words do not begin with capital letters -%s)\n", pNewItemID);
		}
	}
public:
	HEADER_D* GetStart();

	// #1
	DataStructure();

	// #2
	DataStructure(char* pFilename);

	// #3
	~DataStructure();

	// #4
	DataStructure(const DataStructure& Original);

	// #5
	int GetItemsNumber();

	// #6
	ITEM1* GetItem(char* pID);

	// #7
	void operator+=(ITEM1* newItem);

	// #8
	void operator-=(char* pID);

	// #9
	DataStructure& operator=(const DataStructure& Right);

	// #10
	int operator==(DataStructure& Other);

	// #11
	void Write(char* pFilename);

	// #12
	friend std::ostream& operator<<(std::ostream& ostr, const DataStructure& str);
};

