#ifndef _DATASTRUCTURE_H_
#define _DATASTRUCTURE_H_

#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>
#include <string>

using namespace std;

struct node {
	vector<int> title;                    // Which article has this keyword in title
	vector< pair<int, int> > position;    // article and position of its
	vector<string> synonym;               // If it's a synonym trie, this vector contains all the synonym of current keyword
	vector<node*> pNext;                  // '0' - > '9', 'a' -> 'z', '#', '$'
	bool end;
	node() { // Constructor
		pNext.resize(40);
		for (int i = 0; i < 40; i++) pNext[i] = nullptr;
		end = false;
	}
	~node() {
		for (int i = 0; i < 40; i++)
			delete pNext[i];
		title.clear();
		position.clear();
		synonym.clear();
		pNext.clear();
	}
};

//Trie functions

int _valChar(char c); 
void insertTrie(node*& root, string &s);                                    // for stopword trie
void insertTrie(node*& root, string &s, int curArticle);                    // for word in title of article
void insertTrie(node*& root, string &s, int curArticle, int posInTitle);    // for word in article
void insertTrie(node*& root, string &s, string& synonym);                   // for synonym trie
node* searchTrie(node* root, string &s); 
void deleteTrie(node* root);


//


#endif // !_DATASTRUCTURE_H_
