//--------------------------//
							//
#include <iostream>			//
#include <fstream>			//
#include <sstream>			//
#include <string>			//
#include "Lexer.h"			//	LIBRARIES & HEADER FILES
#include "Parser.h"			//
#include "DatalogProgram.h"	//
#include "Interpreter.h"	//
using namespace std;		//
							//
//--------------------------//


//----------------------------------------------------------------------------------//
																					//
#define INPUT_FILE "in.txt"															//
#ifdef _MSC_VER																		//
#define _CRTDBG_MAP_ALLOC															//
#include <crtdbg.h>																	//	ADDITIONAL PREPROCESSOR DIRECTIVES
#define VS_MEM_CHECK _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	//
#else																				//
#define VS_MEM_CHECK																//
#endif																				//
																					//
//----------------------------------------------------------------------------------//


int main(int argc, char* argv[]) {
	//----------------------------------------------------------------------//
																			//
	// Enables memory leak check											//
	VS_MEM_CHECK															//
																			//
	// Verifies that two files are in argv (program and in file)			//
		if (argc < 2) {														//
			cerr << "Please provide name of input file";					//
			return 1;														//
		}																	//
																			//	I/O SETUP
		// Accesses the input file											//
	ifstream in(argv[1]);													//
																			//
	// Verifies that the input file was accessed							//
	if (!in) {																//
		cerr << "Unable to open " << argv[1] << " for input";				//
		return 2;															//
	}																		//
																			//
	//----------------------------------------------------------------------//


	string input = "";										// Declares and defines input string

	while (in.peek() != EOF) {								// Passes each character to input
		input += in.get();
	}

	Lexer lexer(input);										// Passes input to the Lexer

	Parser parser(lexer.getTokens());						// Passes tokens to the Parser

	if (!parser.isError()) {
		DatalogProgram datalogProgram(lexer.getTokens());	// Passes tokens to the DatalogProgram

		Interpreter interpreter(datalogProgram);			// Passes the DatalogProgram to the Interpreter

		cout << interpreter;								// Prints a summary of the Interpreter
	}

	// Returns 0
	return 0;
}