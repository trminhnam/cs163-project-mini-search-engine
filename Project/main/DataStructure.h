#ifndef _DATASTRUCTURE_H_
#define _DATASTRUCTURE_H_

#include <vector>

struct node {
	vector<int> title; // Which article has this keyword in title
	vector< pair<int, int> > position; // article and position of its
	vector<string> synonym; // If it's a synonym trie, this vector contains all the synonym of current keyword
	vector<node*> pNext; // '0' - > '9', 'a' -> 'z', '#', '$', '-'
	node() { // Constructor
		pNext.resize(40);
		for (int i = 0; i < 40; i++) pNext[i] = nullptr;
	}
};

#endif // !_DATASTRUCTURE_H_
