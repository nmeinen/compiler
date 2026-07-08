#include <iostream>
#include <stack>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

ifstream file;
ofstream fout;


//Operator precedence table using e as a character for empty slots 
// 						         ;    =    +     -    (    )    *    /   IF  THEN  ODD  ==   !=    >    <   >=   <=    {    }  CALL  WHILE DO
char stateTable[22][22] = {     {'e', '<', 'e', 'e', 'e', 'e', 'e', 'e', '>', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e'}, // ; State 0
								{'>', 'e', '<', '<', '<', 'e', '<', '<', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', '>', 'e', 'e', 'e'}, // = State 1
								{'>', 'e', '>', '>', '<', '>', '<', '<', 'e', '>', 'e', '>', '>', '>', '>', '>', '>', 'e', '>', 'e', 'e', '>'}, // + State 2
								{'>', 'e', '>', '>', '<', '>', '<', '<', 'e', '>', 'e', '>', '>', '>', '>', '>', '>', 'e', '>', 'e', 'e', '>'}, // - State 3
								{'e', 'e', '<', '<', '<', '=', '<', '<', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e'}, // ( State 4
								{'>', 'e', '>', '>', 'e', '>', '>', '>', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', '>'}, // ) State 5
								{'>', 'e', '>', '>', '<', '>', '>', '>', 'e', '>', 'e', '>', '>', '>', '>', '>', '>', 'e', '>', 'e', 'e', '>'}, // * State 6
								{'>', 'e', '>', '>', '<', '>', '>', '>', 'e', '>', 'e', '>', '>', '>', '>', '>', '>', 'e', '>', 'e', 'e', '>'}, // / State 7
								{'e', 'e', '<', '<', '<', 'e', '<', '<', 'e', '=', '<', '<', '<', '<', '<', '<', '<', 'e', 'e', 'e', 'e', 'e'}, // IF State 8
								{'>', '<', 'e', 'e', 'e', 'e', 'e', 'e', '<', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', '<', '>', '<', '<', 'e'}, // THEN State 9
								{'e', 'e', '<', '<', '<', 'e', '<', '<', 'e', '>', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', '>'}, // ODD State 10
								{'e', 'e', '<', '<', '<', 'e', '<', '<', 'e', '>', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', '>'}, // == State 11
								{'e', 'e', '<', '<', '<', 'e', '<', '<', 'e', '>', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', '>'}, // != State 12
								{'e', 'e', '<', '<', '<', 'e', '<', '<', 'e', '>', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', '>'}, // > State 13
								{'e', 'e', '<', '<', '<', 'e', '<', '<', 'e', '>', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', '>'}, // < State 14
								{'e', 'e', '<', '<', '<', 'e', '<', '<', 'e', '>', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', '>'}, // >= State 15
								{'e', 'e', '<', '<', '<', 'e', '<', '<', 'e', '>', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', '>'}, // <= State 16
								{'e', '<', 'e', 'e', 'e', 'e', 'e', 'e', '<', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', '<', '=', '<', '<', 'e'}, // { State 17
								{'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e'}, // } State 18
								{'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e'}, // CALL State 19 
								{'e', 'e', '<', '<', 'e', 'e', '<', '<', 'e', 'e', 'e', '<', '<', '<', '<', '<', '<', 'e', 'e', 'e', 'e', '='}, // WHILE State 20 
								{'e', '<', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', '<', '>', '<', '<', 'e'} };// DO State 21 


struct stkElement //Stack elements will have three fields based off token file from scanner
{
	string token; //Token type
	string tokenClass; //Token class
	char precedence; //Based off previous operator in stack
};

struct symbolTbl
{
	string symbol;
	string type;
	string value;
	string address;
	string segment;
};

//Instantiate stack to perform PDA
stack<stkElement> stk;

//Instantiate fix-up and startWhile stacks to store lables within IF and WHILE statements
stack<string> fixUp;
stack<string> startWhile;

vector<string> splitString(string input) //Function to seperate strings by space when reading in token file
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
void readSymbolTable(string fileName, string outputFile)
{
	string input;
	vector<symbolTbl> elements;

	file.open(fileName);
	fout.open(outputFile);


	getline(file, input); //Discard first line of symbol table

	while (getline(file, input))
	{
		symbolTbl element;
		istringstream iss(input);

		iss >> element.symbol >> element.type >> element.value >> element.address >> element.segment;
		elements.push_back(element);
	}

	for (int i = 0; i < elements.size(); i++)
	{
		if (elements[i].type == "$CONST")
		{
			fout << elements[i].symbol << "			DW		" << elements[i].value << endl;
		}
		else if (elements[i].type == "$NumLit")
		{
			fout << "Lit" << elements[i].symbol << "			DW		" << elements[i].value << endl;
		}
		else if (elements[i].address == "DS")
		{
			fout << elements[i].symbol << "			RESW		1" << endl;
		}
	}

	file.close();
	fout.close();
	file.clear();
	fout.clear();
}

int generateState(string st) //Reads in string to determine what input will be for precedence table
{
	if (st == ";")
		return 0;
	else if (st == "=")
		return 1;
	else if (st == "+")
		return 2;
	else if (st == "-")
		return 3;
	else if (st == "(")
		return 4;
	else if (st == ")")
		return 5;
	else if (st == "*")
		return 6;
	else if (st == "/")
		return 7;
	else if (st == "IF")
		return 8;
	else if (st == "THEN")
		return 9;
	else if (st == "ODD")
		return 10;
	else if (st == "==")
		return 11;
	else if (st == "!=")
		return 12;
	else if (st == ">")
		return 13;
	else if (st == "<")
		return 14;
	else if (st == ">=")
		return 15;
	else if (st == "<=")
		return 16;
	else if (st == "{")
		return 17;
	else if (st == "}")
		return 18;
	else if (st == "CALL")
		return 19;
	else if (st == "WHILE")
		return 20;
	else if (st == "DO")
		return 21;
	else if (st == "CONST" || st == "VAR") // Will signal when assignments are taking place (will not be needed for syntax analysis)
		return 22;
	else //Represents all non-terminals
		return 23;
}

void PrintStack(stack<stkElement> s)
{
	// If stack is empty
	if (s.empty())
		return;

	// Extract top of the stack
	stkElement x = s.top();

	// Pop the top element
	s.pop();

	// Print the current top
	// of the stack i.e., x
	cout << x.token << ' ';

	// Proceed to print
// remaining stack
	PrintStack(s);

	// Push the element back
	s.push(x);
}

void codeGeneration(int operand, string var1, string var2, string store) //Function to provide code geenration 
{

	switch (operand)
	{
	case 0: //Place W label at beginning of WHILE
		fout << var1 << ":" << endl;
		break;
	case 1: // =
		fout << "mov ax,[" << var2 << "]" << endl;
		fout << "mov [" << var1 << "],ax" << endl;
		break;
	case 2: // +
		fout << "mov ax,[" << var1 << "]" << endl;
		fout << "add ax,[" << var2 << "]" << endl;
		fout << "mov [" << store << "],ax" << endl;
		break;
	case 3: // -
		fout << "mov ax,[" << var1 << "]" << endl;
		fout << "sub ax,[" << var2 << "]" << endl;
		fout << "mov [" << store << "],ax" << endl;
		break;
	case 6: // *
		fout << "mov ax,[" << var1 << "]" << endl;
		fout << "mul [" << var2 << "]" << endl;
		fout << "mov [" << store << "],ax" << endl;
		break;
	case 7: // /
		fout << "mov dx,0" << endl;
		fout << "mov ax,[" << var1 << "]" << endl;
		fout << "mov bx,[" << var2 << "]" << endl;
		fout << "div bx" << endl;
		fout << "mov [" << store << "],ax" << endl;
		break;
	case 8: // IF
		fout << "nop" << endl;
		break;
	case 11: // ==
		fout << "mov ax,[" << var1 << "]" << endl;
		fout << "cmp ax,[" << var2 << "]" << endl;
		fout << "JNE " << store << endl;
		break;
	case 12: // !=
		fout << "mov ax,[" << var1 << "]" << endl;
		fout << "cmp ax,[" << var2 << "]" << endl;
		fout << "JE " << store << endl;
		break;
	case 13: // >
		fout << "mov ax,[" << var1 << "]" << endl;
		fout << "cmp ax,[" << var2 << "]" << endl;
		fout << "JLE " << store << endl;
		break;
	case 14: // <
		fout << "mov ax,[" << var1 << "]" << endl;
		fout << "cmp ax,[" << var2 << "]" << endl;
		fout << "JGE " << store << endl;
		break;
	case 15: // >=
		fout << "mov ax,[" << var1 << "]" << endl;
		fout << "cmp ax,[" << var2 << "]" << endl;
		fout << "JL " << store << endl;
		break;
	case 16: // <=
		fout << "mov ax,[" << var1 << "]" << endl;
		fout << "cmp ax,[" << var2 << "]" << endl;
		fout << "JG " << store << endl;
		break;
	case 20: // WHILE
		fout << "jmp " << var1 << endl;
		fout << "nop" << endl;
		break;
	}

}



void syntaxAnalysis(string fileName, string outputFile)
{
	int topOperator, inputState; //Will store top operator state to be found in state table
	string input; //File input
	vector<string> split; //Used to store input split by spaces
	stkElement element, tempElement; //Stores input as element to be evaluated by stack
	string var1, var2, store, labelVar; //Will store elements popped from stack to be fed into code generation function
	int operand; //Will store operator to be popped from stack
	string temp[7] = { "T1", "T2", "T3", "T4", "T5", "T6", "T7"}; //Stores temp variables to be used in stack
	string label[5] = { "L1", "L2", "L3", "L4", "L5" }; //Stores labels to be used in fix-up stack
	string whileLabel[5] = {"W1", "W2", "W3", "W4", "W5"}; //Stores labels to be used in while stack
	int tempCounter = 0; //Keeps track of which temp variables have been used
	int labelCounter = 0; //Keeps track of which lables have been used
	int whileLabelCounter = 0; //Keeps track of which while labels have been used

	file.open(fileName);
	fout.open(outputFile);

	//Read from token file and disregard elements until first left brace is encountered
	do
	{
		getline(file, input);
		split = splitString(input);
		element.token = split[0];
		element.tokenClass = split[1];
		inputState = generateState(element.token);
	} while (inputState != 17);
	element.precedence = 'F'; //First operator in stack, has no predecessing operator to have precedence with
	stk.push(element);
	topOperator = inputState; //Set first operator (left brace) as current top operator


	while (getline(file, input)) //Read through each line of file
	{
		split = splitString(input);
		element.token = split[0]; //Store token name
		element.tokenClass = split[1]; //Store token class
		inputState = generateState(element.token); //Calculate if input is an operator or non-terminal

		if (inputState == 20) //Need label at top of WHILE loop
		{
			labelVar = whileLabel[whileLabelCounter]; //Set variable to
			codeGeneration(0, labelVar, var2, store);
		}

		if (inputState == 22) //Signals assignment block beginning with CONST or VAR
		{
			while (inputState != 0) //Read through file until ; is encountered to signal end of assignments
			{
				getline(file, input);
				split = splitString(input);
				element.token = split[0]; //Store token name
				element.tokenClass = split[1]; //Store token class
				inputState = generateState(element.token); //Calculate if input is an operator or non-terminal
			}
		}

		else if (inputState == 23) //If element is a non-terminal
		{
			element.precedence = 'N'; //No precedence for non-terminal
			stk.push(element);
		}


		else //If element is an operator
		{
			element.precedence = stateTable[topOperator][inputState]; //Operator precedence to predecessor operator
			if (element.precedence == 'e') //Precedence doesn't exist between operators, programmer error
				cout << "Oops! It looks like this code has a syntax error :(" << endl;
			else if (element.precedence == '>') //Takes precedence found, pop stack and feed into code generation function
			{
				while (element.precedence == '>') //Loop for if multiple pops are needed
				{
					if (topOperator == 9) //Popping IF/THEN
					{
						stk.pop(); //Pop THEN
						operand = generateState(stk.top().token); //Set IF as operand
						var1 = fixUp.top(); //Set var1 to label at top of fix-up stack
						fixUp.pop(); //Pop label
						stk.pop(); //Pop IF
						topOperator = generateState(stk.top().token); //Set new top of stack as topOperator
					}
					else if (topOperator == 21) //Popping WHILE/DO
					{
						stk.pop(); //Pop DO
						operand = generateState(stk.top().token); //Set WHILE as operand
						var1 = startWhile.top(); //Set var1 to label at top of while stack
						startWhile.pop(); //Pop label
						stk.pop(); //Pop WHILE
						topOperator = generateState(stk.top().token); //Set new top of stack as topOperator
					}
					else {
						if (stk.top().tokenClass == "<integer>")
							var2 = "Lit" + stk.top().token;
						else
							var2 = stk.top().token; //Set second variable
						stk.pop();
						operand = generateState(stk.top().token); //Set operand
						stk.pop();
						if (stk.top().tokenClass == "<integer>")
							var1 = "Lit" + stk.top().token;
						else
							var1 = stk.top().token; //Set first variable
						stk.pop();
					}
					if (operand == 2 || operand == 3 || operand == 6 || operand == 7) //Will need temp variable for arithmatic operations
					{
						topOperator = generateState(stk.top().token); //Set next top operator
						tempElement.token = temp[tempCounter]; //Set up temp variable to be next top variable in stack
						tempElement.tokenClass = "Var(int)";
						tempElement.precedence = 'N';
						stk.push(tempElement); //Push temp variable into stack
						store = temp[tempCounter]; //Set temp variable to be plugged into code generation function
						tempCounter++; //Increment temp counter to prepare for next temp variable
					}
					else if (operand == 11 || operand == 12 || operand == 13 || operand == 14 || operand == 15 || operand == 16) //Will need label for compare operations
					{
						topOperator = generateState(stk.top().token);
						fixUp.push(label[labelCounter]); //Push label onto stack
						store = label[labelCounter]; //Set label as store value to be used in code generation function
						labelCounter++; //Increment label counter to prepare for next temp variable
						if (topOperator == 20) //WHILE/DO Statement
						{
							startWhile.push(whileLabel[whileLabelCounter]); //Push while label onto stack
							labelVar = whileLabel[whileLabelCounter]; //Set while label as store value to be used in code generation function
							whileLabelCounter++; //Increment label counter to prepare for next temp variable
						}
					}
					else
					{
						store = "-"; //No temp variable needed to store result
						topOperator = generateState(stk.top().token);
					}
					element.precedence = stateTable[topOperator][inputState]; //Recheck element's precedence to new top operator
					if (element.precedence != '>')//Push operator into stack if it does not take precedence
					{
						if (element.token == ")") //To pop parenthesis after expression inside has been evaluated
						{
							stkElement storeElement = stk.top(); //Store variable that stores evaluation
							stk.pop(); //Pop variable
							stk.pop(); //Pop (
							topOperator = generateState(stk.top().token); //Set topOperator predecessing operator
							stk.push(storeElement); //Push variable back into stack
						}
						if (element.token != ";" && element.token != ")")
						{
							stk.push(element);
							topOperator = inputState;
						}
					}

					codeGeneration(operand, var1, var2, store);
				}
			}

			else //Operand yields or is equal in precedence
			{
				if (element.token == "}" && topOperator == 17) //Check if brackets need to be popped
				{
					stk.pop(); // Pop {
					if (!stk.empty())
					{
						topOperator = generateState(stk.top().token); //If stack is not empty set up topOperator for next file read
					}
				}
				else
				{
					stk.push(element); //Push element into stack
					topOperator = generateState(stk.top().token); //Set as next top operator
				}
				
			}
		}
	}
	file.close();
	fout.close();
	file.clear();
	fout.clear();
}

int main()
{
	syntaxAnalysis("C:\\BurrisCompiler\\Scanner Output.txt", "C:\\BurrisCompiler\\Code Segment.txt");
	readSymbolTable("C:\\BurrisCompiler\\Symbol Table.txt", "C:\\BurrisCompiler\\Data Segment.txt");
}