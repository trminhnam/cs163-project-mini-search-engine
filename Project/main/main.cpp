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
	while (getline(cin, s)) {
		query = queryProcessing(s);
		for (int i = 0; i < query.size(); i++) {
			cout << "\"" << query[i] << "\"" << endl;
		}
	}
	//----------------------------------------------


	return 0;
}