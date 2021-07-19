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
		Insert a into the trie rooted at trie
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
		Insert a into the trie rooted at trie
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
		Insert a into the trie rooted at trie
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
		Insert a into the trie rooted at trie
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