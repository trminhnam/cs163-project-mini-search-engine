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

//Ham nay ngu lam :'(
//Chua viet xong
string correctQueryWord(string& s){
	//Lower the word
	transform(s.begin(), s.end(), s.begin(), ::tolower);


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

vector<int> querySearching(node *root, node *rootSW, node *rootSYM, vector<string>& query) {
	vector<int> ans;
	vector<int> notInc;
	vector<string> exactMatch;
	
	//Initialize res vector
	for (int i = 0; i < 11368; i++) ans.push_back(i);

	
	bool completeWord = false; //For query case "a and b"

	for (int i = 0; i < query.size(); i++) {
		// "a and b"
		if(query[i][0]=='\"' || completeWord){
			completeWord = true;
			//"a
			if(query[i][0]=='\"') // remove first character which is '\"'
				query[i] = query[i].substr(1, query[i].size());
			//b"
			else if(query[i][query[i].size()-1]=='\"'){
				completeWord = false;
				query[i].pop_back(); //remove final character which is '\"'
			}

			//Searching for the next words

			exactMatch.push_back(query[i]);
			if (!completeWord) {
				vector<int> curQuery = findExact(root, exactMatch);
				ans = getIntersection(ans, curQuery);
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
		}                 
		else if (query[i] == "~") { // Synonym
			i++;
			vector<string> sym = findSynonym(rootSYM, query[i]);
			for (int j = 0; j < sym.size(); j++) {
				query.push_back("OR");
				query.push_back(sym[j]);
			}			
		}
		// Normal query, including and query and money query
		// $200
		// handbag $200
		// range money query: $50$200
		else if (isRangeMoney(query[i])) {
			string s1, s2;
			for (int i1 = 1; ; i1++) {
				if (query[i][i1] == '$') {
					for (int i2 = i1 + 1; i2 < query[i].size(); i2++)
						s2 += query[i][i2];
					break;
				}
				else s1 += query[i][i1];
			}	

			int n1 = stoi(s1), n2 = stoi(s2);
			vector<int> curQuery;
			for (int num = n1; num < n2; num++) {
				string q = '$' + to_string(num);
				curQuery = OrOperator(curQuery, searchTrie(root, q));
			}
			ans = getIntersection(ans, curQuery);
		}
		else {
			ans = AndOperator(ans, searchTrie(root, query[i])); 
		}
		// 

	}
	displayTitle(ans);
	return ans;
}

// Display Data

void displayTitle(vector<int>& ans) {
	int pos = 0;
	while (1) {
		system("cls");
		cout << "There are a total of " << ans.size() << " results.\n\n";

		cout << "Showing results from " << pos << " to " << min(pos + 5, (int)ans.size()) << ".\n";
		for (int i = pos; i < min(pos + 5, (int)ans.size()); i++)
			cout << i << ". " << _title[ans[i]] << '\n';
		cout << "What would you like to do?\n";
		cout << "0: " << "Exit.\n";
		bool ok = false;
		if (pos) {
			cout << "1: " << "Go to previous page.\n";
			ok = true;
		}	
		for (int i = pos; i < min(pos + 5, (int)ans.size()); i++)
			cout << ok + i - pos + 1 << ": Access post " << _title[ans[i]] << ".\n";
		if (pos + 5 < (int)ans.size())
			cout << ok + pos + 5 - pos + 1 << ": Go to next page.\n";
		cout << "Please input your choice\n";
  		int r; cin >> r;
  		if (r == 0) {
  			system("cls");
  			getchar();
  			break;
  		}	
  		else if (pos && r == 1) {
  			system("cls");
  			pos -= 5;
  			continue;
  		}
  		else if (pos + 5 < (int)ans.size() && r == ok + pos + 5 - pos + 1) {
  			pos += 5;
  			continue;
  		}
		for (int i = pos; i < min(pos + 5, (int)ans.size()); i++)
			if (r == ok + i - pos + 1) {
				ifstream fIn; fIn.open(("Search-Engine-Data\\" + _title[ans[i]] + ".txt").c_str());
				displayFile(fIn);
				break;
			}
	}
}

void displayFile(ifstream &fIn) {
	system("cls");
	string s;
	while (getline(fIn, s))
		cout << s << '\n';
	system("pause");
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

vector<int> inTitle(node* root, string &s) {
	node *cur = searchTrie(root, s);
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
		bool ok = false;
		while (getline(fIn, s)) {
			istringstream iss(s);
			vector<string> curLine; string tmp;
			while (iss >> tmp) curLine.push_back(tmp);
			for (int id1 = 0; id1 + (int)exactMatch.size() - 1 < (int)curLine.size(); id1++) {
				bool cur_ok = false;
				for (int id2 = 0; id2 < (int)exactMatch.size(); id2++) {
					if (curLine[id1 + id2] != exactMatch[id2])
						cur_ok = false;
				}
				if (cur_ok) {
					ok = true;
					break;
				}
			}
			if (ok) break;								
		}
		if (ok) ans.push_back(i);
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