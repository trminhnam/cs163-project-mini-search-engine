#include "Engine.h"
#include "DataStructure.h"

int main() {

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