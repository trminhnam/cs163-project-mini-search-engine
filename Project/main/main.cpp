#include "Engine.h"
#include "DataStructure.h"

int main() {
	//Load data into the Trie
	node *root = new node();
	node *rootSW = new node();
	node *rootSYM = new node();
	loadData(root, rootSW, rootSYM);

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
			cout << "\"" << query[i] << "\"" << endl;
		system("pause");

		querySearching(root, rootSW, rootSYM, query);
	}
	//----------------------------------------------
	delete root;
	delete rootSW;
	delete rootSYM;

	return 0;
}