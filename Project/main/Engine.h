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
#include <string.h>
#include <Windows.h>

#include "DataStructure.h"
#include "ColorConsole.h"

using namespace std;


//Load data

bool isSpecialChar(char& c);
void loadData(node *root, node *rootSW, node *rootSYM);

// String processing

bool isCorrectChar(char& c, string& s);

// Query processing
vector<string> queryProcessing(string& input);
vector<int> querySearching(node *root, node *rootSW, node *rootSYM, vector<string>& query);

// Display
void displayTitle(vector<int>& ans, vector<string>& query);
void displayFile(ifstream& fIn);
void displayFile(ifstream& fIn, vector<string>& query); // With highlight

// Query searching

bool isRangeMoney(string& s);
vector<int> AndOperator(vector<int>& res, node* keywordNode);
vector<int> OrOperator(vector<int>& res, node* keywordNode);
vector<int> getIntersection(vector<int>& a, vector<int>& b);
vector<int> getUnion(vector<int>& a, vector<int>& b);
vector<int> inTitle(node* root, string &s);
vector<int> notInclude(node* keywordNode);
vector<string> findSynonym(node *rootSYM, string &s);
vector<int> findExact(node* root, vector<string> &exactMatch);

#endif // !_ENGINE_H_
