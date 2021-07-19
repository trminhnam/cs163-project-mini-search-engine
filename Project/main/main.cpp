#include "Engine.h"
#include "DataStructure.h"

int main() {
	string s;
	vector<string> query;
	while (getline(cin, s)) {
		query = queryProcessing(s);
		for (int i = 0; i < query.size(); i++) {
			cout << "\"" << query[i] << "\"" << endl;
		}
	}

	return 0;
}