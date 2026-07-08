#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

ifstream file;
ofstream fout;

int stateTable[31][17] = { {5, 3, 2, 7, 11, 14, 0, 17, 20, 23, 24, 25, 26, 27, 28, 29, 30}, //State 0
                          {1, 1, 1, 1, 1,   1, 1, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1}, //State 1
                          {2, 2, 2, 2, 2,   2, 2, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2}, //State 2
                          {4, 3, 4, 4, 4,   4, 4, 4,  4,  4,  4,  4,  4,  4,  4,  4,  4}, //State 3
                          {4, 4, 4, 4, 4,   4, 4, 4,  4,  4,  4,  4,  4,  4,  4,  4,  4}, //State 4
                          {5, 5, 6, 6, 6,   6, 6, 6,  6,  6,  6,  6,  6,  6,  6,  6,  6}, //State 5
                          {6, 6, 6, 6, 6,   6, 6, 6,  6,  6,  6,  6,  6,  6,  6,  6,  6}, //State 6
                          {10, 10, 8, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10}, //State 7
                          {8,  8,  9, 8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8}, //State 8
                          {8,  8,  8, 0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8}, //State 9
                          {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10}, //State 10
                          {12, 12, 12, 12, 13, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12}, //State 11
                          {12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12}, //State 12
                          {13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13}, //State 13
                          {15, 15, 15, 15, 16, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15}, //State 14
                          {15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15}, //State 15
                          {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, //State 16
                          {18, 18, 18, 18, 19, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18}, //State 17
                          {18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18}, //State 18
                          {19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19}, //State 19
                          {21, 21, 21, 21, 22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21}, //State 20
                          {21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21}, //State 21
                          {22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22}, //State 22
                          {23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23}, //State 23
                          {24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24}, //State 24
                          {25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25}, //State 25
                          {26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26}, //State 26
                          {27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27}, //State 27
                          {28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28}, //State 28
                          {29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29}, //State 29
                          {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30} }; //State 30

string reservedWords[10] = { "CONST", "IF", "VAR", "THEN", "PROCEDURE", "WHILE", "CALL", "DO", "ODD", "CLASS" };    

int symbolTableStates[12][10] = { {1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //State 0
                                 {0, 2, 0, 0, 0, 0, 0, 0, 0, 0}, //State 1
                                 {0, 0, 3, 0, 0, 0, 0, 0, 0, 0}, //State 2
                                 {0, 10, 0, 4, 0, 0, 0, 8, 0, 10}, //State 3
                                 {0, 5, 0, 0, 0, 0, 0, 0, 0, 0}, //State 4
                                 {0, 0, 0, 0, 6, 0, 0, 0, 0, 0}, //State 5
                                 {0, 0, 0, 0, 0, 7, 0, 0, 0, 0}, //State 6
                                 {0, 0, 0, 0, 0, 0, 3, 0, 4, 0}, //State 7
                                 {0, 9, 0, 0, 0, 0, 0, 0, 0, 0}, //State 8
                                 {0, 0, 0, 0, 0, 0, 3, 0, 8, 0}, //State 9
                                 {0, 10, 10, 10, 10, 11, 10, 10, 10, 10}, //State 10
                                 {0, 10, 10, 10, 10, 10, 10, 10, 10, 10} }; //State 11

vector<string> splitString(string input)
{
    stringstream ss(input);
    string s;
    vector<string> splitting;

    while (getline(ss, s, ' '))
    {
        splitting.push_back(s);
    }

    return splitting;
}


int charToToken(char ch) //Reads in character to determine what input it will be forf table
{
    if (ch >= 'a' && ch <= 'z') //Lowercase letter, return letter
        return 0;
    else if (ch >= 'A' && ch <= 'Z') //Uppercase letter, return letter
        return 0;
    else if (ch >= '0' && ch <= '9') //Digit, return digit
        return 1;
    else if (ch == '*') //return *
        return 2;
    else if (ch == '/') //return /
        return 3;
    else if (ch == '=') //return =
        return 4;
    else if (ch == '<') //return <
        return 5;
    else if (ch == ' ') //return whitespace
        return 6;
    else if (ch == '>') //return >
        return 7;
    else if (ch == '!') //return !
        return 8;
    else if (ch == '+') //return +
        return 9;
    else if (ch == '-') //return -
        return 10;
    else if (ch == ',') //return ,
        return 11;
    else if (ch == ';') //return ;
        return 12;
    else if (ch == '{') //return {
        return 13;
    else if (ch == '}') //return }
        return 14;
    else if (ch == '(') //return (
        return 15;
    else if (ch == ')') //return )
        return 16;
    else //illegal character
        return 20;
}

int stringToSymbol(string str)
{
    if (str == "$CLASS")
        return 0;
    else if (str == "<var>")
        return 1;
    else if (str == "$LB")
        return 2;
    else if (str == "$CONST")
        return 3;
    else if (str == "<assignment>")
        return 4;
    else if (str == "<integer>")
        return 5;
    else if (str == "<semi>")
        return 6;
    else if (str == "$VAR")
        return 7;
    else if (str == "<comma>")
        return 8;
    else
        return 9;
}

void processTokens(string fileName, string outputFile)
{
    char ch;
    int element;
    string str;
    int nextState = 0;
    bool finished = false;
    bool isReserved = false;
    char previousChar;
    string cap;

    file.open(fileName);
    fout.open(outputFile);

    while (file.get(ch)) //loop to read from file one character at a time
    {
        element = charToToken(ch); //Convert input character to proper number for stateTable
        finished = false;
        while (!finished && element != 20)
        {
            switch (nextState)
            {
            case 0:
                if (element == 6)
                {
                    finished = true;
                    break;
                }
                else
                {
                    nextState = stateTable[0][element];
                    break;
                }
            case 1:
                cout << "Error! Illegal character input :(\n";
                nextState = 0; //Reset states for next character
                finished = true;
                break;
            case 2:
                fout << "* <mop>\n";
                nextState = 0;
                finished = true;
                break;
            case 3:
                nextState = stateTable[3][element];
                if (nextState == 3)
                {
                    finished = true;
                    str += ch;
                }
                break;
            case 4:
                fout << str << " <integer>\n";
                nextState = 0;
                str.clear(); //Reset string to prepare for next integer or variable
                break;
            case 5:
                nextState = stateTable[5][element];
                if (nextState == 5)
                {
                    finished = true;
                    str += ch;
                }
                break;
            case 6:
                cap = str;
                for (int i = 0; i < str.length(); i++)
                {
                    cap[i] = toupper(str[i]); //Convert string to uppercase to check against reserved words
                }

                for (int i = 0; i < 10; i++)
                {
                    if (reservedWords[i] == cap) //Check if variable name is a reserved word
                        isReserved = true;
                }

                if (isReserved == true)
                {
                    fout << cap << " $" << cap << '\n';
                    isReserved = false;
                }

                else
                    fout << str << " <var>\n";
                nextState = 0;
                str.clear(); //Reset string to prepare for next integer or variable
                break;
            case 7:
                if (previousChar == '/')
                    nextState = stateTable[7][element];
                else
                    finished = true;
                break;
            case 8:
                if (previousChar == '/' && element == 2)
                {
                    finished = true;
                }
                else
                {
                    nextState = stateTable[8][element];
                    finished = true;
                }
                break;
            case 9:
                nextState = stateTable[9][element];
                finished = true;
                break;
            case 10:
                fout << "/ <mop>\n";
                nextState = 0;
                if (previousChar != '/')
                    finished = true;
                break;
            case 11:
                if (previousChar == '=')
                    nextState = stateTable[11][element];
                else
                    finished = true;
                break;
            case 12:
                fout << "= <assignment>\n";
                nextState = 0;
                break;
            case 13:
                fout << "== <relop>\n";
                nextState = 0;
                finished = true;
                break;
            case 14:
                if (previousChar == '<')
                    nextState = stateTable[14][element];
                else
                    finished = true;
                break;
            case 15:
                fout << "< <relop>\n";
                nextState = 0;
                break;
            case 16:
                fout << "<= <relop>\n";
                nextState = 0;
                finished = true;
                break;
            case 17:
                if(previousChar == '>')
                    nextState = stateTable[17][element];
                else
                    finished = true;
                break;
            case 18:
                fout << "> <relop>\n";
                nextState = 0;
                break;
            case 19:
                fout << ">= <relop>\n";
                nextState = 0;
                finished = true;
                break;
            case 20:
                if (previousChar == '!')
                    nextState = stateTable[20][element];
                else
                    finished = true;
                break;
            case 21:
                fout << "! <relop>\n";
                nextState = 0;
                break;
            case 22:
                fout << "!= <relop>\n";
                nextState = 0;
                finished = true;
                break;
            case 23:
                fout << "+ <addop>\n";
                nextState = 0;
                finished = true;
                break;
            case 24:
                fout << "- <addop>\n";
                nextState = 0;
                finished = true;
                break;
            case 25:
                fout << ", <comma>\n";
                nextState = 0;
                finished = true;
                break;
            case 26:
                fout << "; <semi>\n";
                nextState = 0;
                finished = true;
                break;
            case 27:
                fout << "{ $LB\n";
                nextState = 0;
                finished = true;
                break;
            case 28:
                fout << "} $RB\n";
                nextState = 0;
                finished = true;
                break;
            case 29:
                fout << "( $LP\n";
                nextState = 0;
                finished = true;
                break;
            case 30:
                fout << ") $RP\n";
                nextState = 0;
                finished = true;
                break;
            }
        }

        previousChar = ch;
    }

    file.close();
    fout.close();
    file.clear();
    fout.clear();
}

void createSymbolTable(string tokenInput, string tableOutput)
{
    string input;
    string token;
    string classType;
    int nextState = 0;
    bool finished = false;
    ofstream fout;
    int dataAddress = 0;
    int codeAddress = 0;
    vector<string> split;
    int element;


    file.open(tokenInput);
    fout.open(tableOutput);

    fout << "Symbol     Classification     Value     Address     Segment\n";

    while (getline(file, input))
    {
        split = splitString(input);
        token = split[0]; //Store token
        classType = split[1]; //Store class type
        finished = false;
        element = stringToSymbol(classType);

        while (!finished)
        {
            switch (nextState)
            {
            case 0:
                if (element == 0)
                {
                    nextState = symbolTableStates[0][element];
                }
                else
                    cout << "Error! Illegal program :(";
                finished = true;
                break;
            case 1:
                if (element == 1)
                {
                    nextState = symbolTableStates[1][element];
                    fout << token << "      <Program Name>                       " << codeAddress << "          CS\n";
                    codeAddress += 2;
                }
                else
                    cout << "Error! Illegal program :(";
                finished = true;
                break;
            case 2:
                if (element == 2)
                    nextState = symbolTableStates[2][element];
                else
                    cout << "Error! Illegal program :(";
                finished = true;
                break;
            case 3:
                nextState = symbolTableStates[3][element];
                finished = true;
                break;
            case 4:
                if (element == 1)
                {
                    fout << token << "          $CONST                ";
                    nextState = symbolTableStates[4][element];
                }
                else
                    cout << "Error! Illegal program :(";
                finished = true;
                break;
            case 5:
                if (element == 4)
                {
                    nextState = symbolTableStates[5][element];
                }
                else
                    cout << "Error! Illegal program :(";
                finished = true;
                break;
            case 6:
                if (element == 5)
                {
                    fout << token << "            " << dataAddress << "          DS\n";
                    dataAddress += 2;
                    nextState = symbolTableStates[6][element];
                }
                else
                    cout << "Error! Illegal program :(";
                finished = true;
                break;
            case 7:
                nextState = symbolTableStates[7][element];
                finished = true;
                break;
            case 8:
                if (element == 1)
                {
                    fout << token << "          <var>          ";
                    nextState = symbolTableStates[8][element];
                }
                else
                    cout << "Error! Illegal program :(";
                finished = true;
                break;
            case 9:
                fout << "                    " << dataAddress << "          DS\n";
                dataAddress += 2;
                nextState = symbolTableStates[9][element];
                finished = true;
                break;
            case 10:
                nextState = symbolTableStates[10][element];
                if (element != 5)
                    finished = true;
                break;
            case 11:
                fout << token << "         $NumLit               " << token << "          " << dataAddress << "          DS\n";
                dataAddress += 2;
                nextState = symbolTableStates[11][element];
                finished = true;
                break;
            }
        }

    }

    for (int i = 1; i < 8; i++)
    {
        fout << "T" << i << "      Var(int)                          " << dataAddress << "          DS\n";
        dataAddress += 2;
    }


    file.close();
    fout.close();
    file.clear();
    fout.clear();

}



int main()
{
    processTokens("C:\\BurrisCompiler\\Scanner Input.txt", "C:\\BurrisCompiler\\Scanner Output.txt");
    createSymbolTable("C:\\BurrisCompiler\\Scanner Output.txt", "C:\\BurrisCompiler\\Symbol Table.txt");
}
