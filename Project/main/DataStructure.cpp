#include "DataStructure.h"


//Trie functions

int _valChar(char c) {
	int cur = 0;
	if ('0' <= c && c <= '9') return c - '0';
	cur += 10;
	if ('a' <= c && c <= 'z') return c - 'a';
	cur += 26;
	if ('a' == '#') return c;
	c++;
	if ('a' == '$') return c;

	// If reach this state, then c is lmao
	assert(c == 0);
}

void insertTrie(node*& root, string &s) {
	/*
		Insert a string into the trie
		root: Root node of the Trie
		string s: The word to be inserted
	*/
	if (!root) root = new node();
	node *cur = root;
	for (int i = 0; i < s.size(); i++) {
		char c = s[i]; int nxt = _valChar(c);
		if (!cur -> pNext[nxt]) cur -> pNext[nxt] = new node();
		cur = cur -> pNext[nxt];

		if (i == s.size() - 1)
			cur -> end = true;
	}
}

void insertTrie(node*& root, string &s, int curArticle) {
	/*
		Insert a string into the trie
		root: Root node of the Trie
		string s: The word to be inserted
		curARticle: The id of the current article that has string s in title
	*/
	if (!root) root = new node();
	node *cur = root;
	for (int i = 0; i < s.size(); i++) {
		char c = s[i]; int nxt = _valChar(c);
		if (!cur -> pNext[nxt]) cur -> pNext[nxt] = new node();
		cur = cur -> pNext[nxt];

		if (i == s.size() - 1) cur -> title.push_back(curArticle);		
	}
}

void insertTrie(node*& root, string &s, int curArticle, int posInTitle) {
	/*
		Insert a string into the trie
		root: Root node of the Trie
		string s: The word to be inserted
		posInTitle: Position of the string, in the current article
	*/
	if (!root) root = new node();
	node *cur = root;
	for (int i = 0; i < s.size(); i++) {
		char c = s[i]; int nxt = _valChar(c);
		if (!cur -> pNext[nxt]) cur -> pNext[nxt] = new node();
		cur = cur -> pNext[nxt];

		if (i == s.size() - 1) cur -> position.push_back(make_pair(curArticle, posInTitle));		
	}
}

void insertTrie(node*& root, string &s, string &synonym) {
	/*
		Insert a string into the trie
		root: Root node of the Trie
		string s: The word to be inserted
		string synonym: synonym of s
	*/
	if (!root) root = new node();
	node *cur = root;
	for (int i = 0; i < s.size(); i++) {
		char c = s[i]; int nxt = _valChar(c);
		if (!cur -> pNext[nxt]) cur -> pNext[nxt] = new node();
		cur = cur -> pNext[nxt];

		if (i == s.size() - 1) cur -> synonym.push_back(synonym);		
	}
}

bool searchTrie(node* root, string &s) {
	if (!root) root = new node();
	node *cur = root;
	for (int i = 0; i < s.size(); i++) {
		char c = s[i]; int nxt = _valChar(c);
		if (!cur -> pNext[nxt]) return false;
		cur = cur -> pNext[nxt];

		if (i == s.size() - 1) return true;		
	}
}

void deleteTrie(node* root) {
	if (!root)
		return;
	for (int i = 0; i < 40; i++)
		deleteTrie(root->pNext[i]);
	delete root;
}

void loadData(node *root, node *rootSW, node *rootSYM) {
	// Load text data into trie
	ifstream fTitle("Search-Engine-Data\\___index.txt");
	assert(fTitle.is_open());

	string title;
	while (getline(fTitle, title)) {
		_title.push_back(title);
		insertTrie(root, title, _title.size() - 1); // insert title into the trie

		ifstream fData(title);
		string data;
		int cur = 0;
		while (fData >> data)
			insertTrie(root, data, _title.size() - 1, cur++); // insert word into trie
		fData.close();
	}
	fTitle.close();

	ifstream fStopword("stopword.txt");
	string stopword;
	while (fStopword >> stopword)
		insertTrie(rootSW, stopword);		
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
					insertTrie(rootSYM, curKey, curSym);					
					curSym.clear();
				}
				else rootSYM += synonym[i];
			}
			insertTrie(rootSYM, curKey, curSym);
		}
	}
}