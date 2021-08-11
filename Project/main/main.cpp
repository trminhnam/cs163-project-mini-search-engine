#include "Engine.h"
#include "DataStructure.h"

int main() {
	//Set console full screen
	system("mode 650");

	//Load data into the Trie
	cerr << "LOADING DATA...\n";
	cerr << "Have yourself a sip of coffee while waiting.\n";
	node *root = new node();
	node *rootSW = new node();
	node *rootSYM = new node();
	loadData(root, rootSW, rootSYM);
	cerr << "LOADING DONE!!\n";
	system("pause");
	system("cls");

	//This main function is for testing input query
	string s;
	vector<string> query;

	while (1) {
		cout << "Please input what you'd like to search below: \n";
		getline(cin, s);
		
		if (s == "") continue;
		else if (s == "exit()") break;

		query = queryProcessing(s);

		//Debug for inputting query, will be deleted before submitting
		for (int i = 0; i < query.size(); i++)
			cout << "\"" << Color(11) << query[i] << Color(7) << "\"" << endl;
		system("pause");

		querySearching(root, rootSW, rootSYM, query);
	}
	//----------------------------------------------
	delete root;
	delete rootSW;
	delete rootSYM;

	return 0;
}