#include "ConsoleAndOthers.h"

string queryInput() {
	HANDLE hStdout;
	COORD inputCoord, printCoord;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

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
	while ((c = _getch()) && c!= '\r' && (int)c!=27) {
		//backspace is 8 and cannot input blank space at beginnning
		if (query.size() == 0 && (c == 8 || c == ' '))
			continue;
		//Cannot have two space together
		else if (query.size() > 0 && c == ' ' && query[query.size() - 1] == ' ')
			continue;
		//Using backspace 
		else if (c == 8 && query.size() > 0) {
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
			historyProcessing(query);
		SetConsoleCursorPosition(hStdout, inputCoord);
	}
	//ESC
	if (c == 27) query = "exit()";

	//After inputting, we clear console history suggestion
	clearHistoryConsole(hStdout, printCoord);

	//We set pointer back to the line after the input line
	inputCoord.Y++;
	SetConsoleCursorPosition(hStdout, inputCoord);
	
	//Add new raw query to history.txt for later searching
	if (query != "exit()" && query!=" ") {
		ofstream fout("history.txt", ios::app);
		if (fout.is_open()) {
			fout << query << endl;
			fout.close();
		}
	}
	return query;
}

void clearHistoryConsole(HANDLE& h, COORD& start) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	
	//Debugging
	//cout << endl;
	//cout << columns << endl;
	//cout << rows << endl;
	//cout << start.Y << endl;
	//system("pause");
	//--

	string tmp = "";
	for (int i = 0; i < columns; i++)
		tmp += " ";

	for (int i = start.Y; i < 13; i++) {
		cout << tmp;
	}
}

COORD GetConsoleCursorPosition(HANDLE hConsoleOutput)
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

void historyProcessing(string& rawQuery) {
	ifstream fin("history.txt");
	if (!fin.is_open()){
		cout << "Error... No history file.\n";
		return;
	}
	string line;
	int cnt = 0;
	while (getline(fin, line) && cnt<10) {

		if (line != rawQuery && line.find(rawQuery)==0) {
			if(cnt==0)
				cout << "History suggestion:\n";
			cout << ++cnt << ". " << line << endl;
		}

	}
	fin.close();
}