#ifndef LEXER_H
#define LEXER_H
#include <vector>
#include <string>
#include "Automata.h"
#include "Automaton_EXACT.h"
#include "Automaton_STRING.h"
#include "Automaton_ID.h"
#include "Automaton_COMMENT.h"
#include "Automaton_UNDEFINED.h"
#include "Token.h"
using namespace std;

class Lexer {
private:
	string input;
	vector<pair<int, int>> newLines;
	vector<Automata*> automata;
	vector<Token> tokens;
public:
	Lexer(const string& input = "") {																	// Constructor
		this->input = input;
		setAutomata();
		setTokens();
	}


	~Lexer() {																							// Destructor
		for (size_t i = 0; i < automata.size(); i++) {
			delete automata[i];
		}
	}


	void setAutomata() {																				// Populates the automata vector
		vector<pair<string, string>> exactTokenTypes = {
			{ "COMMA", "," },
			{ "PERIOD", "." },
			{ "COLON", ":" },
			{ "COLON_DASH", ":-" },
			{ "LEFT_PAREN", "(" },
			{ "RIGHT_PAREN", ")" },
			{ "Q_MARK", "?" },
			{ "MULTIPLY", "*" },
			{ "ADD", "+" },
			{ "SCHEMES", "Schemes" },
			{ "FACTS", "Facts" },
			{ "RULES", "Rules" },
			{ "QUERIES", "Queries" }
		};

		for (size_t i = 0; i < exactTokenTypes.size(); i++) {
			automata.push_back(new Automaton_EXACT(exactTokenTypes[i].first, exactTokenTypes[i].second));
		}

		automata.push_back(new Automaton_STRING());
		automata.push_back(new Automaton_ID());
		automata.push_back(new Automaton_COMMENT());
		automata.push_back(new Automaton_UNDEFINED());

	}


	void incrementLineNum(int& lineNum, const string& value) {											// Increments lineNum by the number of newlines found in the given value
		for (size_t i = 0; i < value.size(); i++) {
			if (value[i] == '\n') {
				lineNum++;
			}
		}
	}


	void setTokens() {																					// Populates the tokens vector
		int lineNum = 1;

		while (!input.empty()) {
			int maxRead = 0;
			Automata* maxAutomaton = nullptr;

			// Removes newlines and whitespace, increments lineNum
			while (input.substr(0, 1) == "/n" || isspace(input[0])) {
				incrementLineNum(lineNum, input.substr(0, 1));
				input.erase(0, 1);
			}

			if (!input.empty()) {
				// Sets maxRead and maxAutomaton
				for (size_t i = 0; i < automata.size(); i++) {
					int automatonRead = automata[i]->getReadCount(input);

					if (automatonRead > maxRead) {
						maxRead = automatonRead;
						maxAutomaton = automata[i];
					}
				}

				// Stores any unrecognized character as UNDEFINED and erases the character
				if (maxRead == 0) {
					tokens.push_back(automata.back()->getToken(input.substr(0, 1), lineNum));
					input.erase(0, 1);

					// Stores recognized characters as Tokens and erases the characters
				} else {
					tokens.push_back(maxAutomaton->getToken(input.substr(0, maxRead), lineNum));
					input.erase(0, maxRead);
				}

				// Increments lineNum
				incrementLineNum(lineNum, input.substr(0, maxRead));
			}
		}

		// Stores EOF token
		tokens.push_back(Token("EOF", "", lineNum));
	};


	vector<Token> getTokens() {																			// Returns the tokens vector without comments
		vector<Token> parserTokens;

		// Removes comments
		for (size_t i = 0; i < tokens.size(); i++) {
			if (tokens[i].getType() != "COMMENT") {
				parserTokens.push_back(tokens[i]);
			}
		}

		return parserTokens;
	}


	friend ostream& operator<< (ostream& out, Lexer& lexer) {											// Overloads the insertion operator to return a summary of the Lexer
		out << lexer.toString();
		return out;
	};

	string toString() {																					// Returns a summary of the Lexer
		stringstream out;

		for (size_t i = 0; i < tokens.size(); i++) {
			out << tokens[i] << endl;
		}

		out << "Total Tokens = " << tokens.size();

		return out.str();
	}
};
#endif
