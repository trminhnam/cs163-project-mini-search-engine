#include "DataStructure.h"


//Trie functions




void deleteTrie(node* root) {
	if (!root)
		return;
	for (int i = 0; i < 40; i++)
		deleteTrie(root->pNext[i]);
	delete root;
}