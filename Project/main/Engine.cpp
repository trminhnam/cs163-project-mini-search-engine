#include "Engine.h"
#include "DataStructure.h"

//-----------------------------------------------------------


// Check correct char
char specialChar[35] = { '+',',',';',' ','\t',
						'\0','\f','\v','\n','\r',
						'!','{', '}','"','(', ')',
						'<','>','/','[',']','?','`',
						'=','|','~','@','%', '&',
						'^','_','\\','-', '.', '\'' };

bool isCorrectChar(char& c, string& s) {
	if (c == '-' && s == "")
		return true;
	else if (c == ':' && s == "intitle")
		return true;
	else if (c == '+' && s == "")
		return true;
	else if (c == '~' && s == "")
		return true;
	for (int i = 0; i < 35; i++) {
		if (c == specialChar[i])
			return false;
	}
	return true;
}

//--------------------------------------------------------------------


// Query processing
vector<string> inputQuery(string& input) {
	stringstream ss(input);
	string word, temp;
	vector<string> res;
	while (ss >> word) {
		temp.clear();
		int i = 0;
		//Minus operator
		if (word[0] == '-') {
			res.push_back("-");
			i++;
		}
		for (; i < word.length(); i++) {
			if (isCorrectChar(word[i], temp)) {
				if (word[i] >= 'A' && word[i] <= 'Z')
					word[i] += 32;
				temp += word[i];
				//intitle operator, include stopword operator, filetype: operator, ~ operator
				if (temp == "intitle:" || temp=="+" || temp=="filetype:" || temp=="~") {
					res.push_back(temp);
					temp.clear();
				}
			}
		}
		res.push_back(temp);
	}
	//Debugging
	//for(int i=0;i<res.size();i++)
		//cout << "\"" << res[i] << "\"" << endl;
	return res;
}

vector<int> AndOperator(vector<int>& res, node* keywordNode) {
	vector<int> tmp;
	if (keywordNode == nullptr)
		return res;

	int n = keywordNode->position.size();
	for (int i = 0; i < n; i++) {
		tmp.push_back(keywordNode->position[i].first);
	}
	res = getIntersection(res, tmp);
	return res;
}

vector<int> OrOperator(vector<int>& res, node* keywordNode) {
	vector<int> tmp;
	if (keywordNode == nullptr)
		return res;

	int n = keywordNode->position.size();
	for (int i = 0; i < n; i++) {
		tmp.push_back(keywordNode->position[i].first);
	}
	res = getUnion(res, tmp);
	return res;
}

//-------------------------------------------------------------------

// For AND operator
vector<int> getIntersection(vector<int>& a, vector<int>& b) {
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());
	vector<int> res(a.size() + b.size());

	vector<int>::iterator itr = set_intersection(a.begin(), a.end(), b.begin(), b.end(), res.begin());
	res.resize(itr - res.begin());
	return res;
}

//For OR operator
vector<int> getUnion(vector<int>& a, vector<int>& b) {
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());
	vector<int> res(a.size() + b.size());
	
	vector<int>::iterator itr = set_union(a.begin(), a.end(), b.begin(), b.end(), res.begin());
	res.resize(itr - res.begin());
	return res;
}



