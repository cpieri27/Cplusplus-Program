/*
 *	Grocery Tracking Program
 *
 *  Date: 12-11-2022
 *  Author: Chayne pieri
 */

#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName)
{
	char *procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();	
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("PrintMe","Test");
Output:
	Python will print on the screen:
		You sent me: Test
Return:
	100 is returned to the C++
*/
int callIntFunc(string proc, string param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}

/*
Description:
	Calling this function will prompt the user for one of 4 inputs and loop for validation.
	A valid input will assign that input to the referenced int
*/
void initialPrompt(int& input) {
	//Prompt for input
	cout << "1: Display overall word frequency" << endl;
	cout << "2: Find individual word frequency" << endl;
	cout << "3: Display Histogram" << endl;
	cout << "4: Exit" << endl;
	cout << "Enter your selection as a number 1, 2, 3, or 4." << endl;

	// input validation loop until only 1, 2, or 3 are entered
	while (!((cin >> input) && (input <= 4) && (input >= 1))) {
		cout << "Invalid input." << endl;
		cout << "Enter your selection as a number 1, 2, 3, or 4." << endl;

		cin.clear();
		cin.ignore();
	}
}

/*
Description:
	Calling this function will take in input information and output the frequency of each
	unique item.
*/
void overallFrequency() {
	cout << "Overall Item Frequency" << endl << endl;

	CallProcedure("OverallFrequency");
}

/*
Description:
	Calling this function will take in input information and output the frequency of a
	specified item.
*/
void individualFrequency() {
	string userItem;

	cout << "Individual Item Frequency" << endl << endl;
	
	// Get user item input
	cout << "Input item to look up frequency for: ";
	cin >> userItem;

	// Check file for requested item, return frequency of that item.
	cout << endl << userItem << " frequency: ";
	cout << callIntFunc("IndividualFrequency", userItem) << endl << endl;
}

/*
Description:
	Calling this function will take in input file and display a text-based histogram
	for all items.
*/
int overallHistogram() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	const int whiteTextBlackBackground = 15;
	const int stringWidth = 20;
	ifstream inFS;
	string fileName = "frequency.dat";
	string itemName;
	int itemFrequency;
	int i;


	cout << "Histogram" << endl << endl;

	// Change text blue
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);

	// Create file "frequency.dat" to read from
	CallProcedure("Histogram");

	// Try opening "frequency.dat"
	inFS.open(fileName);
	if (!inFS.is_open()) {
		cout << "Could not open file \"frequency.dat.\"" << endl;
		return 1; // 1 indicates error
	}

	// Print histogram headers
	cout << setw(stringWidth) << "Name";
	cout << " Frequency" << endl << endl;

	// Loop file to collect item name and frequency
	// Print each item and frequency in histogram
	inFS >> itemName;
	while (!inFS.fail()) {
		inFS >> itemFrequency;


		cout << setw(stringWidth) << itemName << " ";

		for (i = 0; i < itemFrequency; ++i) {
			cout << "*";
		}

		cout << endl;

		inFS >> itemName;
	}

	// Change text back to white
	SetConsoleTextAttribute(hConsole, whiteTextBlackBackground);
	cout << endl;

	// Close File
	inFS.close();

	return 0;
}

int main()
{
	int userChoice;

	// Loop program until 4 is input for exit
	do {
		// Ask user for input 1, 2, 3, or 4 with validation loop
		initialPrompt(userChoice);

		// Input handling from user's choice
		system("CLS");
		switch (userChoice) {
			case 1:
				// Print each word and it's frequency from the input file into a table on
				// the screen
				overallFrequency();
				system("pause");
				break;
			case 2:
				// Get input from user, look up input from file.  If input in file, print
				// to screen the input and its count
				individualFrequency();
				system("pause");
				break;
			case 3:
				// Create "frequency.dat" file using Python containing all unique items
				// from the input file, then read that created file and print a histogram
				// using C++
				overallHistogram();
				system("pause");
				break;
			case 4:
				// Exit program at end of loop because userChoice is 4
				break;
		}

		// Wait for keypress from user, then clear screen
		system("CLS");

	} while (userChoice != 4);

	// End program
	cout << "Exiting Program." << endl;

	return 0;
}