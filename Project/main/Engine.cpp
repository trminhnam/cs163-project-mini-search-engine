#include "Engine.h"
#include "DataStructure.h"


//-----------------------------------------------------------
vector<string> _title;

// Check correct char
const int N_speicalChar = 34;
char specialChar[N_speicalChar] = { '+',',',';','\t',
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
	else if (c == '#' && s == "")
		return true;
	for (int i = 0; i < N_speicalChar; i++) {
		if (c == specialChar[i])
			return false;
	}
	return true;
}

//--------------------------------------------------------------------


// Query processing

//Split the query input into vector of words (also operator)
vector<string> queryInputProcessing(string& input) {
	stringstream ss(input);
	string word, temp;
	vector<string> res;
	while (ss >> word) {
		if (word == "AND" || word == "OR"){
			res.push_back(word);
			continue;
		}
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

vector<int> querySearching(vector<string>& query) {
	vector<int> res;
	vector<int> notinclude;
	for (int i = 0; i < query.size(); i++) {
		
	}
}

// nam and long

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

// intitle: query

vector<int> inTitle(vector<int> &res, node* root, string &s) {
	node *cur = searchTrie(root, s);
	res = cur -> title;
	return res;	
}

// notinclude query

vector<int> notInclude(vector<int> &cur) {
	vector<int> res;
	sort(cur.begin(), cur.end());
	int pos = 0;
	for (int i = 0; i < _title.size(); i++) {
		if (pos < cur.size() && i == cur[pos]) pos++;
		else res.push_back(i);
	}
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

//-----------------------------------------------------------
bool isSpecialChar(char& c) {
	for (int i = 0; i < N_speicalChar; i++)
		if (c == specialChar[i])
			return true;
	return false;
}
// Load data into trie
void loadData(node *root, node *rootSW, node *rootSYM) {
	// Load text data into trie
	ifstream fTitle("Search-Engine-Data\\___index.txt");
	assert(fTitle.is_open());

	string title;
	while (getline(fTitle, title)) {
		_title.push_back(title);
		string tmp;
		for (int i = 0; i < title.size(); i++) {
			if (isSpecialChar(title[i]))
				continue;
			if ('A' <= title[i] && title[i] <= 'Z') tmp += char(title[i] - 'A' + 'a');
			else if (title[i] == ' '){
				insertTrie(root, tmp, _title.size() - 1); // insert title into the trie
				tmp.clear();
			}
			else tmp += title[i];
		}	
		insertTrie(root, tmp, _title.size() - 1); // insert title into the trie

		ifstream fData(title);
		string data;
		int cur = 0;
		while (fData >> data) {
			string tmp;
			for (int i = 0; i < data.size(); i++) {
				if (isSpecialChar(data[i])) continue;
				if ('A' <= data[i] && data[i] <= 'Z') tmp += char(data[i] - 'A' + 'a');
				else tmp += data[i];
			}	
			insertTrie(root, tmp, _title.size() - 1, cur++); // insert word into trie
		}
		fData.close();
	}
	fTitle.close();

	ifstream fStopword("stopword.txt");
	string stopword;
	while (fStopword >> stopword) {
		string tmp;
		for (int i = 0; i < tmp.size(); i++) {
			if (isSpecialChar(tmp[i])) continue;
			if ('A' <= stopword[i] && stopword[i] <= 'Z') tmp += char(stopword[i] - 'A' + 'a');
			else tmp += stopword[i];
		}	
		insertTrie(rootSW, tmp); // insert stopword into trie		
	}
	fStopword.close();

	ifstream fSynonym("synonym.txt");
	string synonym, curKey;
	while (getline(fSynonym, synonym)) {
		if (synonym[0] == '=') continue;
		if (synonym[0] == 'K') { // Key
			curKey.clear();
			for (int i = 5; i < synonym.size(); i++) {
				if ('A' <= synonym[i] && synonym[i] <= 'Z') curKey += char(synonym[i] - 'A' + 'a');
				else curKey += synonym[i];
			}
		}
		else if (synonym[0] == 'S') {
			string curSym;
			for (int i = 5; i < synonym.size(); i++) {
				if (synonym[i] == ' ') {
					insertTrie(rootSYM, curKey, curSym); // insert synonym of a word into trie				
					curSym.clear();
				}
				else {
					curSym += synonym[i];
				}	
			}
			insertTrie(rootSYM, curKey, curSym);
		}
	}

	fSynonym.close();
}