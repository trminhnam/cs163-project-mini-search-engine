#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <queue>
#include <stack>
#include <utility>

#include "DataStructure.h"

using namespace std;


//Load data

void loadData(node *root, node *rootSW, node *rootSYM);

// String processing

bool isCorrectChar(char& c, string& s);

// Query processing
vector<string> queryProcessing(string& input);
vector<int> querySearching(node *root, node *rootSW, node *rootSYM, vector<string>& query);

// Display
void displayTitle(vector<int>& ans);
void displayFile(ifstream& fIn);


vector<int> AndOperator(vector<int>& res, node* keywordNode);
vector<int> OrOperator(vector<int>& res, node* keywordNode);
vector<int> getIntersection(vector<int>& a, vector<int>& b);
vector<int> getUnion(vector<int>& a, vector<int>& b);
vector<int> inTitle(node* root, string &s);
vector<int> notInclude(node* keywordNode);


#endif // !_ENGINE_H_
