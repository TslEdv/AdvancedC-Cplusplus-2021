#include <iostream>
#include <fstream>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"
#include "DataStructure.h"
#include <string>
using namespace std;
// Edvin Ess - Struct: 4 and Item: 1
int main()
{
	DataStructure DS; // Test case #1
	try {
		for (int i = 0; i < 10; i++) { // Test case #2
			ITEM1* item = (ITEM1*)GetItem(1);
			DS += item;
		}
		std::cout << DS << std::endl; // Test case #3
		std::cout << "Number of Items: " << DS.GetItemsNumber() << std::endl; // Test case #4
		char addlist[][11] = { 
			"Dark Khaki",
			"X X",
		};
		char remlist[][14] = { //removing test list
			"Banana Mania",
			"Persian Green",
			"Vegas Gold",
		};
		for (int i = 0; i < 2; i++) { // Test case #5 & #6
			ITEM1* item = DS.GetItem(addlist[i]);
			if (item != 0) {
				std::cout << item->pID << endl;
			}
			else {
				std::cout << "Item was not found" << endl << endl;
			}
		}
		DataStructure copy = DS; // Test case #7
		std::cout << copy << std::endl << std::endl;
		std::cout << (DS == copy) << endl; // Bonus case - compare Initial and fresh copy
		for (int i = 0; i < 3; i++) { // Test case #8
			DS -= remlist[i];
		}
		std::cout << DS << std::endl;
		std::cout << copy << std::endl << std::endl;
		std::cout << (DS == copy) << endl; // Test case #9
		char path[] = "C:\\Users\\User\\Desktop\\testdoc.bin";
		DS.Write(path); // Test case #10
		DataStructure binary = path; // Test case #11
		std::cout << binary << std::endl;
		std::cout << DS << std::endl;
		std::cout << (DS == binary) << endl;
		binary = copy; // Test case #12
		std::cout << binary << std::endl;
		copy.~DataStructure();
		std::cout << copy << std::endl << std::endl;
	}
	catch (const exception &msg) {
		cerr << msg.what() << endl;
	}
    return 0;
}
