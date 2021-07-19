#include "Engine.h"


//-----------------------------------------------------------
// Query processing
char specialChar[34] = { '+',',',';',' ',
					'\t','\0','!','{',
					'}','"','(',')','\f',
					'\v','\n','\r','=',
					'|','~','@','%', '&',
					'<','>','/','[',']','?',
					'`','^','_','\\','-', '.' };

bool isCorrectChar(char& c, string& s) {
	if (c == '-' && s == "")
		return true;
	else if (c == ':' && s == "intitle")
		return true;
	for (int i = 0; i < 34; i++) {
		if (c == specialChar[i])
			return false;
	}
	return true;
}

vector<string> queryProcessing(string& input) {
	stringstream ss(input);
	string word, temp;
	vector<string> res;
	while (ss >> word) {
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
				//inline operator
				if (temp == "inline:") {
					temp.clear();
					res.push_back("inline:");
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

