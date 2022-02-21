#include <iostream>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"
// IMPORTANT: follow the given order of *.h files: ICS0017DataSource.h must be
// the last
// Edvin Ess - Struct: 4 and Item: 1
void PrintStruct(ITEM1* item, int = 0);
HEADER_D* InsertItem(HEADER_D* p, char* = 0);
void FreeList(HEADER_D* p) {
	HEADER_D* tmp = (HEADER_D*)malloc(sizeof(HEADER_D));
	while (p != NULL) {
		while (p->pHeaderA != NULL) {
			tmp->pHeaderA = p->pHeaderA;
			p->pHeaderA = p->pHeaderA->pNext;
			free(tmp->pHeaderA);
		}
		tmp = p;
		p = p->pNext;
		free(tmp);
	}
}
void PrintStruct(ITEM1* item, int order) { //function for printing a single struct
	printf("%d)%s %lu %s\n", order, item->pID, item->Code, item->pTime);
}
char CheckFormat(char* pNewItemID) { //function for checking formatting
	if (strlen(pNewItemID) < 3) { //check the lenght of new ID
		printf("Formatting error! (Length of the new item ID is too short -%s)\n", pNewItemID);
		return 0;
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
			printf("Formatting error! (Detected an illegal character -%s)\n", pNewItemID);
			return 0;
		}
	}
	if (spacecounter == 0 || spacecounter > 1) { //if there are more than 1 space or 0 spaces, printing an error
		printf("Formatting error! (Must have 1 space -%s)\n", pNewItemID);
		return 0;
	}
	char* spacefinder = strchr(pNewItemID, ' '); //pointer to space of the word
	if ((pNewItemID[0] >= 'A' && pNewItemID[0] <= 'Z') && (*(spacefinder + 1) >= 'A' && *(spacefinder + 1) <= 'Z')) {// check if the first letters 
	//of the words are capital letters
		return *(spacefinder + 1);
	}
	else {
		printf("Formatting error! (Words do not begin with capital letters -%s)\n", pNewItemID);
		return 0;
	}
}
void PrintDataStructure(HEADER_D* pointer) { //function for printing data structure
	int i = 0;
	while (pointer != NULL) {
		HEADER_A* pHeaderA = pointer->pHeaderA; //Get the pointer to the linked-list of HeadersA
		while (pHeaderA != NULL) { //If it is not empty
			ITEM1* item = (ITEM1*)pHeaderA->pItems; //copy the pItems into item
			while (item != NULL) { //while list is not empty
				PrintStruct(item, (i + 1));
				i++; //for printing order
				item = item->pNext; // get the next item
			}
			pHeaderA = pHeaderA->pNext; //Get the pointer to the next HeaderA
		}
		pointer = pointer->pNext; //Get pointer to next HeaderD
	}
}
HEADER_D* InsertItem(HEADER_D* p, char* pNewItemID) { //function for inserting a new item
	ITEM1* newItem;
	char first; //The first letter of ID of object
	char second; // the first letter of the second word of ID cBegin
	HEADER_D* start = p;
	if (!pNewItemID || !*pNewItemID) { //If no pNewItemID is set
		ITEM1* pit1 = (ITEM1*)GetItem(1); //generate item
		newItem = pit1; //save item
		char* spacefinder = strchr(newItem->pID, ' ');
		first = newItem->pID[0]; //assign first
		second = *(spacefinder + 1); //assign second
	}
	else { //If pNewItemID is set, check for all the formatting requirements and set the item
		second = CheckFormat(pNewItemID); //check format and assign second if all is correct
		if (second == 0) {
			return start;
		}
		ITEM1* pit1 = (ITEM1*)GetItem(1, pNewItemID);
		newItem = pit1; //save item
		first = pNewItemID[0]; //assign first
	}
	while(true) {
		if (p->cBegin == first) { // if the cBegin of headerD matches first
			HEADER_A* pHeaderA = p->pHeaderA; //start cycling through headerAs
			HEADER_A* pHeaderAStart = pHeaderA;
			while (true) {
				if (pHeaderA->cBegin == second) { // if the cBegin of headerA matches second
					ITEM1* item = (ITEM1*)pHeaderA->pItems; //copy the pItems into item
					while (true) {
						if (strcmp(item->pID, newItem->pID) == 0){ //if string of existing item and our new item match, error
							printf("Error! (Such ID already exists - %s!)\n", newItem->pID);
							return start;
						}
						if (item->pNext == NULL) { // if no strings matched, create a new item at the end of the list
							newItem->pNext = item->pNext;
							item->pNext = newItem;
							item = newItem;
							return start; //return starting point of our initial headerD
						}
						item = item->pNext; // get the next item
					}
				}
				if (pHeaderA->pNext == NULL) { //if no cBegin of headerA matches second
					HEADER_A* missingA = (HEADER_A*)malloc(sizeof(HEADER_A)); // create new headerA
					missingA->cBegin = second; //assign its cBegin with value of second
					missingA->pItems = newItem; //pItems first value is the new item
					HEADER_A* pSave = (HEADER_A*)malloc(sizeof(HEADER_A)); //to save previous of a header
					while (true) {
						if (pHeaderAStart->cBegin > missingA->cBegin) { //if the current cBegin is bigger than first, put it behind cBegin
							if (p->pHeaderA != pHeaderAStart) { //if cBegin match that of the first cBegin in linked list
								pSave->pNext = missingA;
								missingA->pNext = pHeaderAStart;
							}
							else { // if it is not first
								missingA->pNext = pHeaderAStart;
								p->pHeaderA = missingA; // set the new member as first item in linked list
							}
							return start;
						}
						if (pHeaderAStart->pNext == NULL) { // if no letter was bigger than first, put it last in the list
							missingA->pNext = pHeaderAStart->pNext;
							pHeaderAStart->pNext = missingA;
							return start;
						}
						pSave = pHeaderAStart;
						pHeaderAStart = pHeaderAStart->pNext;
					}
				}
				pHeaderA = pHeaderA->pNext; //Get the pointer to the next HeaderA
			}
		}
		if (p->pNext == NULL) { //if there is no such case of cBegins then create one
			HEADER_D* missingD = (HEADER_D*)malloc(sizeof(HEADER_D));
			HEADER_A* missingA = (HEADER_A*)malloc(sizeof(HEADER_A));
			missingA->cBegin = second;
			missingA->pItems = newItem;
			missingA->pNext = NULL;
			missingD->pHeaderA = missingA;
			missingD->cBegin = first;
			while (true) {
				if (p->cBegin < missingD->cBegin) { //if the letter is first is later in the alpahabet than the current letter, place it last
					missingD->pPrior = p;
					missingD->pNext = p->pNext;
					p->pNext = missingD;
					return start;
				}
				if (p->pPrior == NULL) { //if first is the earliest letter, place it at start
					missingD->pNext = p;
					missingD->pPrior = NULL;
					return missingD;
				}
				p = p->pPrior;
			}
		}
		p = p->pNext;
	}
}
HEADER_D* RemoveItem(HEADER_D* p, char* pNewItemID) { //function for removing an item
	char first; //The first letter of ID of object
	char second; // the first letter of the second word of ID cBegin
	HEADER_D* start = p;
	second = CheckFormat(pNewItemID);
	if (second == 0) {
		return start;
	}
	first = pNewItemID[0]; //assign first
	while (true) {
		if (p->cBegin == first) { // if the cBegin of headerD matches first
			HEADER_A* pHeaderA = p->pHeaderA; //start cycling through headerAs
			HEADER_A* pHeaderAPrior = NULL;
			while (true) {
				if (pHeaderA->cBegin == second) { // if the cBegin of headerA matches second
					ITEM1* item = (ITEM1*)pHeaderA->pItems; //copy the pItems into item
					ITEM1* prioritem = (ITEM1*)malloc(sizeof(ITEM1));
					bool isempty = false;
					while (true) {
						if (strcmp(item->pID, pNewItemID) == 0) { //if string of existing item and our ID match
							if (pHeaderA->pItems == item) { //first item
								if (item->pNext != NULL) { //check if has a next item, list starts with next item
									pHeaderA->pItems = item->pNext;
									delete item;
								}
								else { //no next and previous = empty list
									isempty = true;
									delete item;
								}
							}
							else {
								if (item->pNext != NULL) { //has previous and next, reroute last item to next item directly
									prioritem->pNext = item->pNext;
									delete item;
								}
								else { //has no next item, assign prior->next null
									prioritem->pNext = NULL;
									delete item;
								}
							}
							if (isempty == true) { //if headerA empty, check headerD contents
								if (pHeaderAPrior == NULL) { //no previous Header
									if (pHeaderA->pNext != NULL) { //check if has a next Header, list starts with next item
										p->pHeaderA = pHeaderA->pNext;
										delete pHeaderA;
									}
									else { //no next and previous = empty list
										delete pHeaderA;
										if (p->pPrior == NULL) { //no previous HeaderD
											if (p->pNext != NULL) { //check if has a next Header, list starts with next HeaderD
												(p->pNext)->pPrior = NULL;
												start = p->pNext;
												delete p;
											}
											else { //no next and previous = empty list
												delete p;
											}
										}
										else {
											if (p->pNext != NULL) { //has previous and next, reroute last item to next item directly
												(p->pPrior)->pNext = p->pNext;
												(p->pNext)->pPrior = p->pPrior;
												delete p;
											}
											else { //has no next item, assign prior->next null
												(p->pPrior)->pNext = NULL;
												delete p;
											}
										}
									}
								}
								else {
									if (pHeaderA->pNext != NULL) { //has previous and next, reroute last item to next item directly
										pHeaderAPrior->pNext = pHeaderA->pNext;
										delete pHeaderA;
									}
									else { //has no next item, assign prior->next null
										pHeaderAPrior->pNext = NULL;
										delete pHeaderA;
									}
								}
							}
							return start;
						}
						if (item->pNext == NULL) { // if no strings matched, write an error and return struct
							printf("Error! (No Such ID exists - %s!)\n", pNewItemID);
							return start; //return starting point of our initial headerD
						}
						prioritem = item;
						item = item->pNext; // get the next item
					}
				}
				if (pHeaderA->pNext == NULL) { //if no cBegin of headerA matches second
					printf("Error! (No Such ID exists - %s!)\n", pNewItemID);
					return start; //return starting point of our initial headerD
				}
				pHeaderAPrior = pHeaderA;
				pHeaderA = pHeaderA->pNext; //Get the pointer to the next HeaderA
			}
		}
		if (p->pNext == NULL) { //if there is no such case of cBegins then create one
			printf("Error! (No Such ID exists - %s!)\n", pNewItemID);
			return start; //return starting point of our initial headerD
		}
		p = p->pNext;
	}
}
int main()
{
	HEADER_D* p = GetStruct4(1, 30); //construct the data structure
	PrintDataStructure(p);
	char testlist[][5] = { //adding test list
		"Z A",
		"Z Z",
		"Z K",
		"A Z",
		"A A",
		"A K",
		"G Z",
		"G A",
		"G K",
		"M A",
		"M Ba",
		"M Bb",
		"M Z",
	};
	printf("\nNew Data Structure:\n\n");
	for (int i = 0; i < 13; i++) { //inserting items
		p = InsertItem(p, testlist[i]);
	}
	PrintDataStructure(p);
	char faillist[][5] = { //names that are supposed to fail
		"M Ba",
		"Mba"
	};
	for (int i = 0; i < 2; i++) { //inserting error items
		printf("\nNew Data Structure:\n\n");
		p = InsertItem(p, faillist[i]);
	}
	printf("\nNew Data Structure with Remove:\n\n");
	for (int i = 0; i < 13; i++) { //inserting items
		p = RemoveItem(p, testlist[i]);
	}
	PrintDataStructure(p);
	for (int i = 0; i < 2; i++) { //inserting error items
		printf("\nNew Data Structure with Remove:\n\n");
		p = RemoveItem(p, faillist[i]);
	}
	printf("\nInsert Test to verify Remove works correctly:\n\n");
	for (int i = 0; i < 13; i++) { //inserting items
		p = InsertItem(p, testlist[i]);
	}
	PrintDataStructure(p);
	FreeList(p);
	PrintDataStructure(p);
	return 0;
}