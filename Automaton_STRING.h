#ifndef AUTOMATON_STRING_H
#define AUTOMATON_STRING_H
#include <string>
#include "Automata.h"
#include "Token.h"
using namespace std;

class Automaton_STRING : public Automata {
public:
	Automaton_STRING(const string& tokenType = "STRING") {				// Constructor
		this->tokenType = tokenType;
	}

	~Automaton_STRING() {}												// Destructor

	int getReadCount(string& input) {									// Returns read count
		int readCount = 0;

		if (input[0] == '\'') {								// Reads beginning apostrophe
			for (size_t i = 1; i < input.size(); i++) {
				if (input[i] == '\'') {						// Reads ending apostrophe
					if (i != input.size() - 1) {			// Determines if at EOF
						if (input[i + 1] != '\'') {			// Determines if the next character is an apostrophe
							readCount = i + 1;
							i = input.size();
						} else {
							i++;
						}
					} else {
						readCount = i + 1;
					}
				}
			}
		}


		return readCount;
	}

	Token getToken(const string& value, const int& lineNum) const {		// Returns a token based on the data from the automaton
		return Token(tokenType, value, lineNum);
	}
};
#endif

