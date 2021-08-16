#include "Engine.h"
#include "DataStructure.h"
#include "ConsoleAndOthers.h"


//----------------------------------------------------------
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
	if (c == '-' && s == "") return true;
	else if (c == ':' && s == "intitle") return true;
	else if (c == '+' && s == "") return true;
	else if (c == '~' && s == "") return true;
	else if (c == '#' && s == "") return true;
	for (int i = 0; i < N_speicalChar; i++) 
	{
		if (c == specialChar[i])
			return false;
	}
	return true;
}

//--------------------------------------------------------------------


// Query processing


string correctQueryWord(string& s)
{
	//Lower the words
	transform(s.begin(), s.end(), s.begin(), ::tolower);

	return "";
}

// Check range money query
bool isRangeMoney(string& s) {
	int cnt = 0, n = s.length();
	for (int j = 0; j < n; j++) {
		if (s[j] == '$')
			cnt++;
		else if ((s[j] < '0' || s[j] > '9') && s[j] != '#')
			return false;
	}
	return cnt == 2;
}

//Split the query input into vector of words (also operator)
vector<string> queryProcessing(string& input) {
	stringstream ss(input);
	string word, temp;
	vector<string> res;
	while (ss >> word) {
		if (word == "AND" || word == "OR" ){
			res.push_back(word);
			continue;
		}
		else if(word[0]=='\"' || word[word.size()-1]=='\"'){
			//Lowercase the after word
			transform(word.begin(), word.end(), word.begin(), ::tolower);
			res.push_back(word);
			continue;
		}
		// "a
		// "a and b" ==>
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

vector<int> querySearching(node *root, node *rootSW, node *rootSYM, vector<string>& query, HANDLE& hStdout) {
	vector<int> ans;
	vector<int> notInc;
	vector<string> exactMatch;
	
	vector<string> wordToHighlight;	 //use this vector for output

	//Initialize res vector
	for (int i = 0; i < 11368; i++) ans.push_back(i);

	
	bool completeWord = false; //For query case "a and b"

	for (int i = 0; i < query.size(); i++) {
		// Wildcard char, nothing would change
		if (query[i] == "*") continue;

		// "a and b"
		if (query[i][0] == '\"' || completeWord) {
			completeWord = true;
			//"a
			if (query[i][0] == '\"') // remove first character which is '\"'
				query[i] = query[i].substr(1, query[i].size() - 1);
			//b"
			if (query[i][query[i].size() - 1] == '\"') {
				completeWord = false;
				query[i].pop_back(); //remove final character which is '\"'
			}

			//Searching for the next word

			exactMatch.push_back(query[i]);
			wordToHighlight.push_back(query[i]);

			if (!completeWord) {
				vector<int> curQuery = findExact(root, exactMatch);
				ans =  getIntersection(ans, curQuery);
				exactMatch.clear();
			}

			continue;
		}
		//Intitle query
		if (query[i] == "intitle:") {
			for (int j = i + 1; j < query.size(); j++) {
				vector<int> in = inTitle(root, query[j]);
				ans = getIntersection(ans, in);				
			}
			break;
		}
		//Filetype query 
		else if (query[i] == "filetype:") continue;
		//not include query
		else if (query[i] == "-") {
			i++;
			notInc = notInclude(searchTrie(root, query[i]));
			ans = getIntersection(ans, notInc);
		}
		else if (query[i] == "AND") continue;
		else if (query[i] == "OR") {
			i++;
			ans = OrOperator(ans, searchTrie(root, query[i]));

			//To hightlight the output
			wordToHighlight.push_back(query[i]);
		}                 
		else if (query[i] == "~") { // Synonym
			i++;
			vector<string> sym = findSynonym(rootSYM, query[i]);
			for (int j = 0; j < sym.size(); j++) {
				if (j)
					query.push_back("OR");
				query.push_back(sym[j]);

				wordToHighlight.push_back(sym[j]);
			}
			wordToHighlight.push_back(query[i]);
			
		}
		// range money query: $50$200
		else if (isRangeMoney(query[i])) {
			string s1, s2;
			for (int i1 = 1; ; i1++) {
				if (query[i][i1] == '$') {
					for (int i2 = i1 + 1; i2 < query[i].size(); i2++)
					{
						s2 += query[i][i2];
					}
					break;
				}
				else s1 += query[i][i1];
			}	

			int n1 = stoi(s1), n2 = stoi(s2);
			vector<int> curQuery;
			int prevSz = curQuery.size(), newSz;
			for (int num = n1; num < n2; num++) {
				string q = '$' + to_string(num);
				prevSz = curQuery.size();
				curQuery = OrOperator(curQuery, searchTrie(root, q));
				newSz = curQuery.size();
				if (newSz > prevSz) wordToHighlight.push_back(q);
			}
			ans = getIntersection(ans, curQuery);
		}
		// Normal query, including and query and money of query
		// handbag price  $200
		else {
			ans = AndOperator(ans, searchTrie(root, query[i])); 
			wordToHighlight.push_back(query[i]);
		}
		// 

	}

	sort(ans.begin(), ans.end());
	ans.resize(unique(ans.begin(), ans.end()) - ans.begin());	
	displayTitle(ans, wordToHighlight, hStdout);

	return ans;
}

// Display data


void displayTitle(vector<int>& answer, vector<string>& wordToHighlight, HANDLE& hStdout) {

	int pos = 0;
	while (1) {
		system("cls");
		heading(hStdout);
		cout << "There are a total of " << answer.size() << " results.\n\n";

		cout << "Showing results from " << pos << " to " << min(pos + 5, (int)answer.size()) << ".\n";
		for (int i = pos; i < min(pos + 5, (int)answer.size()); i++)
			cout << i << ". " << '\"' << _title[answer[i]] << '\"' << '\n';
		cout << '\n';
		cout << "What would you like to do?\n";
		cout << "0: " << "Exit.\n";
		bool ok = false;
		if (pos) {
			cout << "1: " << "Go to previous page.\n";
			ok = true;
		}
		for (int i = pos; i < min(pos + 5, (int)answer.size()); i++)
			cout << ok + i - pos + 1 << ": Access post " << '\"' << _title[answer[i]] << '\"' << ".\n";
		if (pos + 5 < (int)answer.size())
			cout << ok + pos + 5 - pos + 1 << ": Go to next page. \n";
		cout << '\n';
		cout << "Please input your choice: \n";
		int r; cin >> r;
		if (r == 0) {
			system("cls");
			heading(hStdout);
			getchar();
			break;
		}
		else if (pos && r == 1) {
			system("cls");
			heading(hStdout);
			pos -= 5;
			continue;
		}
		else if (pos + 5 < (int)answer.size() && r == ok + pos + 5 - pos + 1) {
			pos += 5;
			continue;
		}
		for (int i = pos; i < min(pos + 5, (int)answer.size()); i++)
			if (r == ok + i - pos + 1) {
				ifstream fIn; fIn.open(("Search-Engine-Data\\" + _title[answer[i]] + ".txt").c_str());

				displayFile(fIn, wordToHighlight, hStdout);

				break;
			}
	}
}

void displayFile(ifstream &fIn, HANDLE& hStdout) {
	system("cls");
	heading(hStdout);
	string s;
	while (getline(fIn, s))
		cout << s << '\n';
	fIn.close();
	system("pause");
}

void displayFile(ifstream& fIn, vector<string>& wordToHighlight, HANDLE& hStdout) {
//	for (int i = 0; i < wordToHighlight.size(); i++)
//		cout << wordToHighlight[i] << endl;
//	cout << endl;
//	system("cls");
	heading(hStdout);
	
	string s, tmp;
	while (getline(fIn, s)) {
		stringstream ss(s);
		while (ss >> tmp) {
			string __tmp;
			for (int i = 0; i < tmp.size(); i++) {
				if (isSpecialChar(tmp[i])) continue;
				if ('A' <= tmp[i] && tmp[i] <= 'Z') __tmp += char(tmp[i] - 'A' + 'a');
				else __tmp += tmp[i];
			}
			if (find(wordToHighlight.begin(), wordToHighlight.end(), __tmp) != wordToHighlight.end())
				cout << Color(11) << tmp << Color(7) << " ";
			else
				cout << tmp << " ";
		}
		cout << endl;
	}
	fIn.close();
	system("pause");
}

// nam and long

vector<int> AndOperator(vector<int>& res, node* keywordNode) {
	vector<int> tmp;
	if (keywordNode == nullptr)
		return {};

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

vector<int> inTitle(node* root, string &s) {
	node *cur = searchTrie(root, s);
	if (cur == nullptr)
		return {};
	vector<int> res = cur -> title;
	return res;	
}

// notinclude query

vector<int> notInclude(node *keywordNode) {
	vector<int> res;

	if (keywordNode == nullptr) {
		for (int i = 0; i < _title.size(); i++) res.push_back(i);
		return res;
	}

	vector<int> cur;
	int n = keywordNode->position.size();
	for (int i = 0; i < n; i++) {
		cur.push_back(keywordNode->position[i].first);
	}

	sort(cur.begin(), cur.end());
	cur.resize(unique(cur.begin(), cur.end()) - cur.begin());
	int pos = 0;
	for (int i = 0; i < _title.size(); i++) {
		if (pos < cur.size() && i == cur[pos]) pos++;
		else res.push_back(i);
	}
	return res;
}

vector<string> findSynonym(node *rootSYM, string &s) {
	node *cur = searchTrie(rootSYM, s);
	vector<string> res = cur -> synonym;
	
	return res;
}

// Find the list of title which have the exactMatch as a consecutive string
                  
vector<int> findExact(node *root, vector<string> &exactMatch) {
	vector<int> cur;
	for (int i = 0; i < _title.size(); i++)
		cur.push_back(i);
	for (int i = 0; i < exactMatch.size(); i++) {
		string s = exactMatch[i];
		cur = AndOperator(cur, searchTrie(root, s));
	}	
	
	vector<int> ans;
	for (int i = 0; i < cur.size(); i++) {
		ifstream fIn(("Search-Engine-Data\\" + _title[cur[i]] + ".txt").c_str());
		string s;
		vector<string> curLine;
		while (fIn >> s) {
			string tmp;
			for (int i = 0; i < s.size(); i++) {
				if (isSpecialChar(s[i])) continue;
				if ('A' <= s[i] && s[i] <= 'Z') tmp += char(s[i] - 'A' + 'a');
				else tmp += s[i];
			}
			curLine.push_back(tmp);
		}
		bool ok = false;
		for (int id1 = 0; id1 + (int)exactMatch.size() - 1 < (int)curLine.size(); id1++) {
			bool cur_ok = true;
			for (int id2 = 0; id2 < (int)exactMatch.size(); id2++) {
				if (curLine[id1 + id2] != exactMatch[id2])
					cur_ok = false;
			}
			if (cur_ok) {
				ok = true;
				break;
			}
		}
		if (ok) ans.push_back(cur[i]);
		fIn.close();
	}
	return ans;
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
		for (int i = 1; i <= 4; i++)
			title.pop_back();
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

		ifstream fData(("Search-Engine-Data\\" + title + ".txt").c_str());
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
		insertTrie(rootSW, tmp); 
		// insert stopword into trie		
	}
	fStopword.close();

	ifstream fSynonym("synonym.txt");
	string synonym, curKey;
	while (getline(fSynonym, synonym)) {
		if (synonym[0] == '=') continue;
		if (synonym[0] == 'K') { // Key
			curKey.clear();
			for (int i = 5; i < (int)synonym.size() - 1; i++) {
				if (isSpecialChar(synonym[i])) continue;
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
					if (isSpecialChar(synonym[i])) continue;
					if ('A' <= synonym[i] && synonym[i] <= 'Z') curSym += synonym[i];
					else curSym += synonym[i];
				}	
			}
		}
	}
	fSynonym.close();
}


//-------------------------
bool checkRangeMoney(string& s) {
	int t = 0, len = s.length();

	for (int j = 0; j < len; j++) 
	{
		if (s[j] == '$')
			t++;
		else if ((s[j] < '0' || s[j] > '9') && s[j] != '#')
			return false;
	}
	return (t == 2);
}


//===========================================================================
//Below is history handling

// Moving to ConsoleAndOthers.cpp

