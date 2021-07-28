#include "DataStructure.h"


//Trie functions

int _valChar(char c) {
	int cur = 0;
	if ('0' <= c && c <= '9') return c - '0';
	cur += 10;
	if ('a' <= c && c <= 'z') return c - 'a';
	cur += 26;
	if (c == '#') return cur;
	cur++;
	if (c == '$') return cur;

	return 0;
	// If reach this state, then c is lmao
}

void insertTrie(node*& root, string &s) {
	/*
		Insert a string into the trie
		root: Root node of the Trie
		string s: The word to be inserted
	*/
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
	node *cur = root;
	for (int i = 0; i < s.size(); i++) {
		char c = s[i]; int nxt = _valChar(c);
		if (!cur -> pNext[nxt]) cur -> pNext[nxt] = new node();
		cur = cur -> pNext[nxt];

		if (i == s.size() - 1) cur -> synonym.push_back(synonym);		
	}
}

node* searchTrie(node* root, string &s) {
	node *cur = root;	
	for (int i = 0; i < s.size(); i++) {
		char c=  s[i]; int nxt = _valChar(c);
		if (!cur -> pNext[nxt]) return nullptr;
		cur = cur -> pNext[nxt];

		if (i == s.size() - 1) return cur;
	}
}

// Nem ngu ngok has written the below lines
// use for seaching


void deleteTrie(node* root) {
	if (!root)
		return;
	for (int i = 0; i < 40; i++)
		deleteTrie(root->pNext[i]);
	delete root;
}
