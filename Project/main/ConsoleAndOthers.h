#ifndef _CONSOLEANDOTHERS_H_
#define _CONSOLEANDOTHERS_H_

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <time.h>
#include <iomanip>
using namespace std;

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

class Color {
public:
    Color(int desiredColor) {
        consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        color = desiredColor;
    }

    friend std::ostream& operator<<(std::ostream& ss, Color obj) {
        SetConsoleTextAttribute(obj.consoleHandle, obj.color);
        return ss;
    }
private:
    int color;
    HANDLE consoleHandle;
};

//Print history and get input query
string queryInput(HANDLE& hStdout);
void clearHistoryConsole(HANDLE& h, COORD start);
COORD GetConsoleCursorPosition(HANDLE& hConsoleOutput);
void historyProcessing(HANDLE& hStdout, string& rawQuery, bool& isInHistory);

//Further funcion
void displayHelp(HANDLE& h);
void printHistory(HANDLE& h);
void clearHistory(HANDLE& h);



//Heading
inline void heading(HANDLE& hStdout) {
	system("cls");
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns = 172, rows = 45;

	GetConsoleScreenBufferInfo(hStdout, &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;


	//cout << columns << " " << rows << endl;
	int mid = 2;

	string title = "MINI SEARCH ENGINE BY GROUP 6 - 20APCS1";

	//Fist make title
	string res = "|";
	for (int i = 0; i < columns / 2 - title.size() / 2 + 1; i++) {
		res += " ";
	}
	res += title;
	for (int i = res.size(); i < columns; i++) {
		if (i == columns - 1)
			res += "|";
		else
			res += " ";
	}


	//Then print out the limits and the title
	for (int i = 0; i < columns; i++)
		cout << "~";
	for (int i = 0; i < mid * 2 + 1; i++) {
		if (i == mid)
			cout << res;
		else
			for (int j = 0; j < columns; j++) {
				if (j == 0 || j == columns - 1)
					cout << "|";
				else
					cout << " ";
			}
	}
	for (int i = 0; i < columns; i++)
		cout << "~";

	cout << endl << endl << endl;
	//COORD cur = GetConsoleCursorPosition(hStdout);
	//SetConsoleCursorPosition(hStdout, cur);
}

#endif // !_CONSOLEANDOTHERS_H_
