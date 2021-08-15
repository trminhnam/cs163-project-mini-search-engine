#include "Engine.h"
#include "DataStructure.h"
#include "ConsoleAndOthers.h"


int main() {
	//Set console full screen
	system("mode 650");

	//Another way to set console full screen
	system("mode con COLS=700");
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);

	//Load data into the Trie
	clock_t startTime = clock();
	cerr << "LOADING DATA...\n";
	cerr << "Have yourself a sip of coffee while waiting.\n";
	node *root = new node();
	node *rootSW = new node();
	node *rootSYM = new node();
	loadData(root, rootSW, rootSYM);
	cerr << "LOADING DONE!!\n";
	clock_t endTime = clock();
	cout << "Loading data time: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << " seconds.\n";

	system("pause");

	//This main function is for testing input query
	string s;
	vector<string> query;
	
	//Create a console for history, query input and also title
	HANDLE hStdout;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);


	while (1) {
		heading(hStdout);
		cout << "Please input what you'd like to search below: \n";
		cout << "If you use this program the first time, type \"help()\".\n";
		//getline(cin, s);
		
		s = queryInput(hStdout);

		if (s == "") 
			continue;
		else if (s == "exit()") 
			break;
		else if (s == "help()") {
			displayHelp(hStdout);
			continue;
		}
		else if (s == "viewHistory()") {
			printHistory(hStdout);
			continue;
		}
		else if (s == "clearHistory()") {
			clearHistory(hStdout);
			continue;
		}

		query = queryProcessing(s);

		//Debug for inputting query, will be deleted before submitting
		cout << "This output query after splitting is only for debugging, we will delete before submitting.\n";
		for (int i = 0; i < query.size(); i++)
			cout << "\"" << Color(11) << query[i] << Color(7) << "\"" << endl;
		cout << endl;
		//-------------

		system("pause");

		querySearching(root, rootSW, rootSYM, query, hStdout);
	}
	//----------------------------------------------
	delete root;
	delete rootSW;
	delete rootSYM;

	return 0;
}