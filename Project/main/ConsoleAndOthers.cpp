#include "ConsoleAndOthers.h"

string queryInput(HANDLE& hStdout) {
	COORD inputCoord, printCoord;
	
	string query = "";
	cout << ">> ";

	//Get current cursor position
	//inputCoord.X = wherex();
	//inputCoord.Y = wherey();
	inputCoord = GetConsoleCursorPosition(hStdout);
	printCoord.X = 0;
	printCoord.Y = inputCoord.Y + 2;

	//Debugging cursor
	//cout << inputCoord.X << " " << inputCoord.Y << endl;
	//system("pause");

	char c;
	bool isInHistory = false;
	while ((c = _getch()) && c!= '\r' && (int)c!=27) {
		//backspace is 8 and cannot input blank space at beginnning
		isInHistory = false;
		if (query.size() == 0 && (c == 8 || c == ' ')) {
			clearHistoryConsole(hStdout, inputCoord);
			SetConsoleCursorPosition(hStdout, inputCoord);
			continue;
		}
		//Cannot have two space together
		else if (query.size() > 0 && c == ' ' && query[query.size() - 1] == ' ')
			continue;
		//Using backspace 
		if (c == 8 && query.size() > 0) {
			query.pop_back();
			inputCoord.X--;
			SetConsoleCursorPosition(hStdout, inputCoord);
			cout << " ";
			SetConsoleCursorPosition(hStdout, inputCoord);
		}
		//Enter character
		else {
			query.push_back(c);
			cout << c;
			inputCoord.X++;
		}

		//Clear old console
		clearHistoryConsole(hStdout, printCoord);

		//Print History
		SetConsoleCursorPosition(hStdout, printCoord);
		if(query!="")
			historyProcessing(hStdout, query, isInHistory);

		//Set cursor back to input place
		SetConsoleCursorPosition(hStdout, inputCoord);
	}
	//ESC == exit()
	if (c == 27) query = "exit()";

	//After inputting, we clear console history suggestion
	clearHistoryConsole(hStdout, printCoord);

	//We set pointer back to the line after the input line
	inputCoord.Y++;
	inputCoord.X = 0;
	SetConsoleCursorPosition(hStdout, inputCoord);
	
	//Add new raw query to history.txt for later searching
	if (!isInHistory && query != "exit()" && query!=" " && query!="" && query!="help()" && query!="clearHistory()" && query!="viewHistory()") {
		ofstream fout("history.txt", ios::app);
		if (fout.is_open()) {
			fout << query << endl;
			fout.close();
		}
	}
	return query;
}

void clearHistoryConsole(HANDLE& h, COORD start) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;

	GetConsoleScreenBufferInfo(h, &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	
	//Debugging
	//cout << endl;
	//cout << columns << endl;
	//cout << rows << endl;
	//cout << start.Y << endl;
	//system("pause");

	string tmp = "";
	for (int i = 0; i < columns; i++)
		tmp += " ";
	
	for (int i = start.Y; i < 25; i++) {
		cout << tmp;
	}
}

COORD GetConsoleCursorPosition(HANDLE& hConsoleOutput)
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
	{
		return cbsi.dwCursorPosition;
	}
	else
	{
		// The function failed. Call GetLastError() for details.
		COORD invalid = { 0, 0 };
		return invalid;
	}
}

void historyProcessing(HANDLE& hStdout, string& rawQuery, bool& isInHistory) {
	ifstream fin("history.txt");
	if (!fin.is_open()){
		cout << "Error... No history file.\n";
		return;
	}
	string line;
	int num = 0;
	while (getline(fin, line) && num<10) {
		if(line==rawQuery){
			isInHistory = true;
			continue;
		}

		if (line.find(rawQuery)==0) {
			if(num==0)
				cout << "History suggestion:\n";
			cout << ++num << ". " << line << endl;
		}
	}
	fin.close();
}


//Further function
void displayHelp(HANDLE& h) {
	heading(h);

	ifstream fin("help.txt");
	if (!fin.is_open()) {
		cout << "Cannot load help data file.\n";
		system("pause");
		return;
	}
	int i = 1;
	string line;
	while (getline(fin, line)) {
		if (line == "") {
			cout << endl;
			continue;
		}
		else if (line[0] == 'I') {
			cout << '\t' << line << endl;
			continue;
		}
		cout << setw(3) << i++ << ". " << line << endl;
	}
	fin.close();
	system("pause");
}

void printHistory(HANDLE& h) {
	heading(h);

	ifstream fin("history.txt");
	if (!fin.is_open()) {
		cout << "Cannot load history data file.\n";
		system("pause");
		return;
	}
	cout << "History:\n";
	int i = 1;
	string line;
	while (getline(fin, line)) {
		if(line=="") {
			cout << endl;
			continue;
			continue;
		}
		cout << i++ << ". " << line << endl;
	}
	fin.close();
	system("pause");
}

void clearHistory(HANDLE& h) {
	heading(h);

	std::ofstream fout("history.txt");
	if (!fout.is_open()) {
		cout << "Cannot open history.txt to delete.\n";
		system("pause");
		return;
	}
	fout.open("history.txt", ios::out | ios::trunc);
	fout.close();
	cout << "Delete history successfully.\n";
	system("pause");
}
void loading() {

}