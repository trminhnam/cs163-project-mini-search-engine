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

// Load data into trie
vector<string> _title;
void loadData(node *root, node *rootSW, node *rootSYM) {
	// Load text data into trie
	ifstream fTitle("Search-Engine-Data\\___index.txt");
	assert(fTitle.is_open());

	string title;
	while (getline(fTitle, title)) {
		_title.push_back(title);
		string tmp;
		for (int i = 0; i < title.size(); i++) {
			if ('a' <= title[i] && title[i] <= 'z' || title[i] == '$' || title[i] == '#' || title[i] == '\'') tmp += title[i];
			else {
				insertTrie(root, tmp, _title.size() - 1); // insert title into the trie
				tmp.clear();
			}
		}	
		insertTrie(root, tmp, _title.size() - 1); // insert title into the trie

		ifstream fData(title);
		string data;
		int cur = 0;
		while (fData >> data) {
			string tmp;
			for (int i = 0; i < data.size(); i++) if ('a' <= data[i] && data[i] <= 'z' || data[i] == '$' || data[i] == '#' || data[i] == '\'') tmp += data[i];
			insertTrie(root, data, _title.size() - 1, cur++); // insert word into trie
		}
		fData.close();
	}
	fTitle.close();

	ifstream fStopword("stopword.txt");
	string stopword;
	while (fStopword >> stopword)
		insertTrie(rootSW, stopword); // insert stopword into trie		
	fStopword.close();

	ifstream fSynonym("synonym.txt");
	string synonym, curKey;
	while (getline(fSynonym, synonym)) {
		if (synonym[0] == '=') continue;
		if (synonym[0] == 'K') { // Key
			curKey = synonym.substr(5, synonym.size() - 5);
		}
		else if (synonym[0] == 'S') {
			string curSym;
			for (int i = 5; i < synonym.size(); i++) {
				if (synonym[i] == ' ') {
					insertTrie(rootSYM, curKey, curSym); // insert synonym of a word into trie				
					curSym.clear();
				}
				else curSym += synonym[i];
			}
			insertTrie(rootSYM, curKey, curSym);
		}
	}

	fSynonym.close();
}