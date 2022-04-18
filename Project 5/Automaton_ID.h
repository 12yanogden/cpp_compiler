#ifndef AUTOMATON_ID_H
#define AUTOMATON_ID_H
#include <string>
#include <ctype.h>
#include "Automata.h"
#include "Token.h"
using namespace std;

class Automaton_ID : public Automata {
public:
	Automaton_ID(const string& tokenType = "ID") {						// Constructor
		this->tokenType = tokenType;
	}

	~Automaton_ID() {}													// Destructor

	int getReadCount(string& input) {									// Returns read count
		int readCount = 0;

		if (isalpha(input[0])) {
			for (size_t i = 1; i < input.size(); i++) {
				if (!isalnum(input[i])) {
					readCount = i;
					i = input.size();
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

