#include "Datastructure.h"
#include <iostream>
#include <fstream>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"
#include <string>
#pragma warning(disable:4996)
using namespace std;

HEADER_D* DataStructure::GetStart() {
	return this->Start;
}

// #1
DataStructure::DataStructure()
{
	Start = NULL;
}

// #2
DataStructure::DataStructure(char* pFilename)
{
	fstream File;
	File.open(pFilename, fstream::in | fstream::binary);
	if (!File.is_open()) {
		throw exception("Error opening file");
	}
	string result;
	while (std::getline(File, result))
	{
		ITEM1* item = new ITEM1;
		int n1 = result.find(' ', 0);
		int n2 = result.find(' ', n1 + 1);
		item->pID = new char[result.substr(0, n2).length() + 1];
		strcpy_s(item->pID, result.substr(0, n2).length() + 1, result.substr(0, n2).c_str());
		n1 = result.find(' ', n2 + 1);
		item->Code = stoul(result.substr(n2 + 1, n1 - n2), nullptr, 0);
		item->pNext = NULL;
		n2 = result.find('\n', n1 + 1);
		item->pTime = new char[result.substr(n1+1, n2 - n1).length() + 1];
		strcpy_s(item->pTime, result.substr(n1+1, n2 - n1).length() + 1, result.substr(n1+1, n2 - n1).c_str());
		*this += item;
	}
};

// #3
DataStructure::~DataStructure() {
	while (this->Start != NULL) {
		HEADER_D* dheader = new HEADER_D;
		HEADER_A* deleteheader = new HEADER_A;
		while (this->Start->pHeaderA != NULL) {
			deleteheader = this->Start->pHeaderA->pNext;
			delete this->Start->pHeaderA->pItems;
			delete this->Start->pHeaderA;
			this->Start->pHeaderA = deleteheader;
		}
		dheader = this->Start->pNext;
		delete this->Start;
		this->Start = dheader;
	}
	delete Start;
};

// #4
DataStructure::DataStructure(const DataStructure& Original)
{
	HEADER_D* p = Original.Start;
	while (p != NULL) {
		while (p != NULL) {
			HEADER_A* pHeaderA = p->pHeaderA; //Get the pointer to the linked-list of HeadersA
			while (pHeaderA != NULL) { //If it is not empty
				ITEM1* item = new ITEM1;
				ITEM1* pitem = (ITEM1*)pHeaderA->pItems;
				while (pitem != NULL) { //while list is not empty
					item->Code = pitem->Code;
					item->pID = pitem->pID;
					item->pTime = pitem->pTime;
					item->pNext = NULL;
					*this += item;
					pitem = pitem->pNext; // get the next item
				}
				pHeaderA = pHeaderA->pNext; //Get the pointer to the next HeaderA
			}
			p = p->pNext; //Get pointer to next HeaderD
		}
	}
};

// #5
int DataStructure::GetItemsNumber()
{
	int i = 0;
	HEADER_D* p = Start;
	while (p != NULL) {
		HEADER_A* pHeaderA = p->pHeaderA; //Get the pointer to the linked-list of HeadersA
		while (pHeaderA != NULL) { //If it is not empty
			ITEM1* item = (ITEM1*)pHeaderA->pItems; //copy the pItems into item
			while (item != NULL) { //while list is not empty
				i++; //for printing order
				item = item->pNext; // get the next item
			}
			pHeaderA = pHeaderA->pNext; //Get the pointer to the next HeaderA
		}
		p = p->pNext; //Get pointer to next HeaderD
	}
	return i;
};

// #6
ITEM1* DataStructure::GetItem(char* pID)
{
	char first;
	char second;
	char* spacefinder = strchr(pID, ' ');
	first = pID[0]; //assign first
	second = *(spacefinder + 1); //assign second
	HEADER_D* p = Start;
	if (p != NULL) {
		while (true) {
			if (p->cBegin == first) { // if the cBegin of headerD matches first
				HEADER_A* pHeaderA = p->pHeaderA; //start cycling through headerAs
				HEADER_A* pHeaderAStart = pHeaderA;
				while (true) {
					if (pHeaderA->cBegin == second) { // if the cBegin of headerA matches second
						ITEM1* item = (ITEM1*)pHeaderA->pItems; //copy the pItems into item
						while (true) {
							if (strcmp(item->pID, pID) == 0) { //if string of existing item and our new item match, return it
								return item;
							}
							if (item->pNext == NULL) { // if no strings matched
								return 0;
							}
							item = item->pNext; // get the next item
						}
					}
					if (pHeaderA->pNext == NULL) { //if no cBegin of headerA matches second
						return 0;
					}
					pHeaderA = pHeaderA->pNext; //Get the pointer to the next HeaderA
				}
			}
			if (p->pNext == NULL) { //if there is no such case of cBegins
				return 0;
			}
			p = p->pNext;
		}
	}
	else {
		return 0;
	}
};

// #7
void DataStructure::operator+=(ITEM1* newItem)
{
	char* spacefinder = strchr(newItem->pID, ' ');
	char first = newItem->pID[0]; //The first letter of ID of object
	char second = *(spacefinder + 1); // the first letter of the second word of ID cBegin
	HEADER_D* p = Start;
	if (p != NULL) {
		while (true) {
			if (p->cBegin == first) { // if the cBegin of headerD matches first
				HEADER_A* pHeaderA = p->pHeaderA; //start cycling through headerAs
				HEADER_A* pHeaderAStart = pHeaderA;
				while (true) {
					if (pHeaderA->cBegin == second) { // if the cBegin of headerA matches second
						ITEM1* item = (ITEM1*)pHeaderA->pItems; //copy the pItems into item
						while (true) {
							if (strcmp(item->pID, newItem->pID) == 0) { //if string of existing item and our new item match, error
								throw exception ("Error! (Such ID already exists!)\n");
							}
							if (item->pNext == NULL) { // if no strings matched, create a new item at the end of the list
								newItem->pNext = item->pNext;
								item->pNext = newItem;
								item = newItem;
								return; //return starting point of our initial headerD
							}
							item = item->pNext; // get the next item
						}
					}
					if (pHeaderA->pNext == NULL) { //if no cBegin of headerA matches second
						HEADER_A* missingA = new HEADER_A; // create new headerA
						missingA->cBegin = second; //assign its cBegin with value of second
						missingA->pItems = newItem; //pItems first value is the new item
						HEADER_A* pSave = new HEADER_A; //to save previous of a header
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
								return;
							}
							if (pHeaderAStart->pNext == NULL) { // if no letter was bigger than first, put it last in the list
								missingA->pNext = pHeaderAStart->pNext;
								pHeaderAStart->pNext = missingA;
								return;
							}
							pSave = pHeaderAStart;
							pHeaderAStart = pHeaderAStart->pNext;
						}
					}
					pHeaderA = pHeaderA->pNext; //Get the pointer to the next HeaderA
				}
			}
			if (p->pNext == NULL) { //if there is no such case of cBegins then create one
				HEADER_D* missingD = new HEADER_D;
				HEADER_A* missingA = new HEADER_A;
				missingA->cBegin = second;
				missingA->pItems = newItem;
				missingA->pNext = NULL;
				missingD->pHeaderA = missingA;
				missingD->cBegin = first;
				while (true) {
					if (p->cBegin < missingD->cBegin) { //if the letter is first is later in the alpahabet than the current letter, place it last
						missingD->pPrior = p;
						missingD->pNext = p->pNext;
						if (p->pNext != NULL) {
							p->pNext->pPrior = missingD;
						}
						p->pNext = missingD;
						return;
					}
					if (p->pPrior == NULL) { //if first is the earliest letter, place it at start
						missingD->pNext = p;
						missingD->pPrior = NULL;
						p->pPrior = missingD;
						Start = missingD;
						return;
					}
					p = p->pPrior;
				}
			}
			p = p->pNext;
		}
	}
	else {
		Start = new HEADER_D;
		Start->cBegin = first;
		Start->pNext = NULL;
		Start->pPrior = NULL;
		Start->pHeaderA = new HEADER_A;
		Start->pHeaderA->cBegin = second;
		Start->pHeaderA->pItems = newItem;
		Start->pHeaderA->pNext = NULL;
		return;
	}
};

// #8
void DataStructure::operator-=(char* pID)
{
	char first = pID[0]; //The first letter of ID of object
	char second = CheckFormat(pID); // the first letter of the second word of ID cBegin
	HEADER_D* p = Start;
	while (true) {
		if (p->cBegin == first) { // if the cBegin of headerD matches first
			HEADER_A* pHeaderA = p->pHeaderA; //start cycling through headerAs
			HEADER_A* pHeaderAPrior = NULL;
			while (true) {
				if (pHeaderA->cBegin == second) { // if the cBegin of headerA matches second
					ITEM1* item = (ITEM1*)pHeaderA->pItems; //copy the pItems into item
					ITEM1* prioritem = new ITEM1;
					bool isempty = false;
					while (true) {
						if (strcmp(item->pID, pID) == 0) { //if string of existing item and our ID match
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
												Start = p->pNext;
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
							return;
						}
						else if (item->pNext == NULL) { // if no strings matched, write an error and return struct
							throw exception("Error! (No Such ID exists)\n");
						}
						prioritem = item;
						item = item->pNext; // get the next item
					}
				}
				if (pHeaderA->pNext == NULL) { //if no cBegin of headerA matches second
					throw exception("Error! (No Such ID exists)\n");
				}
				pHeaderAPrior = pHeaderA;
				pHeaderA = pHeaderA->pNext; //Get the pointer to the next HeaderA
			}
		}
		if (p->pNext == NULL) { //if there is no such case of cBegins then create one
			throw exception("Error! (No Such ID exists)\n");
		}
		p = p->pNext;
	}
};

// #9
DataStructure& DataStructure::operator=(const DataStructure& Right) {
	if (this == &Right) {
		return *this;
	}
	if (Start) {
		delete Start;
	}
	Start = NULL;
	HEADER_D* p = Right.Start;
	while (p != NULL) {
		while (p != NULL) {
			HEADER_A* pHeaderA = p->pHeaderA; //Get the pointer to the linked-list of HeadersA
			while (pHeaderA != NULL) { //If it is not empty
				ITEM1* item = new ITEM1;
				ITEM1* pitem = (ITEM1*)pHeaderA->pItems;
				while (pitem != NULL) { //while list is not empty
					item->Code = pitem->Code;
					item->pID = pitem->pID;
					item->pTime = pitem->pTime;
					item->pNext = NULL;
					*this += item;
					pitem = pitem->pNext; // get the next item
				}
				pHeaderA = pHeaderA->pNext; //Get the pointer to the next HeaderA
			}
			p = p->pNext; //Get pointer to next HeaderD
		}
	}
};
// #10
int DataStructure::operator==(DataStructure& Other)
{
	int pn = GetItemsNumber();
	int ppn = Other.GetItemsNumber();
	if (pn != ppn) {
		return 0;
	}
	else {
		int counter = 0;
		HEADER_D* p = Start;
		HEADER_D* pp = Other.Start;
		while (pp != NULL && p != NULL) {
			if (pp->cBegin == p->cBegin) {
				HEADER_A* ppa = pp->pHeaderA;
				HEADER_A* pa = p->pHeaderA;
				while (ppa != NULL && pa != NULL) {
					if (ppa->cBegin == pa->cBegin) {
						ITEM1* pitem = (ITEM1*)pa->pItems;
						ITEM1* ppitem = (ITEM1*)ppa->pItems;
						while (pitem != NULL && ppitem != NULL) {
							if (pitem->Code == ppitem->Code && strcmp(pitem->pID, ppitem->pID) == 0 && strcmp(pitem->pTime, ppitem->pTime) == 0) {
								counter++;
								pitem = pitem->pNext;
								ppitem = (ITEM1*)pp->pHeaderA->pItems;
							}
							ppitem = ppitem->pNext;
						}
						pa = pa->pNext;
						ppa = pp->pHeaderA;

					}
					ppa = ppa->pNext;
				}
				p = p->pNext;
				pp = Other.Start;
			}
			pp = pp->pNext;
		}
		if (counter == pn) {
			cout << counter << endl;
			return 1;
		}
		else {
			cout << counter << endl;
			return 0;
		}
	}
};

// #11
void DataStructure::Write(char* pFilename)
{
	HEADER_D* p = this->Start;
	string text;
	while (p != NULL) {
		HEADER_A* pHeaderA = p->pHeaderA; //Get the pointer to the linked-list of HeadersA
		while (pHeaderA != NULL) { //If it is not empty
			ITEM1* item = (ITEM1*)pHeaderA->pItems; //copy the pItems into item
			while (item != NULL) { //while list is not empty
				text += string(item->pID) + ' ' + to_string(item->Code) + ' ' + string(item->pTime) + '\n';
				item = item->pNext; // get the next item
			}
			pHeaderA = pHeaderA->pNext; //Get the pointer to the next HeaderA
		}
		p = p->pNext; //Get pointer to next HeaderD
	}
	char* arr = new char[text.length() + 2];
	strcpy_s(arr, text.length() + 1, text.c_str());
	fstream file;
	file.open(pFilename, fstream::out | fstream::binary);
	if (!file.is_open()) {
		throw exception("Error opening file");
	}
	file.write((char*)arr, (strlen(arr)) * sizeof(char));
	if (!file.good())
		throw exception("Error, failed to write!");
	file.close();
};

// #12
std::ostream& operator<<(std::ostream& ostr, const DataStructure& str)
{
	int i = 0;
	HEADER_D* p = str.Start;
	string text;
	while (p != NULL) {
		HEADER_A* pHeaderA = p->pHeaderA; //Get the pointer to the linked-list of HeadersA
		while (pHeaderA != NULL) { //If it is not empty
			ITEM1* item = (ITEM1*)pHeaderA->pItems; //copy the pItems into item
			while (item != NULL) { //while list is not empty
				text += to_string(i + 1) + ')' + item->pID + ' ' + to_string(item->Code) + ' ' + string(item->pTime) + "\n";
				i++; //for printing order
				item = item->pNext; // get the next item
			}
			pHeaderA = pHeaderA->pNext; //Get the pointer to the next HeaderA
		}
		p = p->pNext; //Get pointer to next HeaderD
	}
	if (i == 0) {
		throw exception("Data Structure is empty!");
	}
	return std::cout << text << endl;
};