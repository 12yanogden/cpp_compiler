#ifndef AUTOMATON_EXACT_H
#define AUTOMATON_EXACT_H
#include <string>
#include "Automata.h"
#include "Token.h"
using namespace std;

class Automaton_EXACT : public Automata {
private:
	string compareValue;
public:
	Automaton_EXACT(const string& tokenType = "NO TYPE", const string& compareValue = "") {		// Constructor
		this->tokenType = tokenType;
		this->compareValue = compareValue;
	}

	~Automaton_EXACT() {}																		// Destructor

	int getReadCount(string& input) {															// Returns read count
		int readCount = 0;
		string value = "";

		for (size_t i = 0; i < input.size(); i++) {
			if (input[i] == compareValue[i]) {
				value += input[i];
			} else {
				i = input.size();
			}
		}

		if (value == compareValue) {
			readCount = value.size();
		}

		return readCount;
	}

	Token getToken(const string& value, const int& lineNum) const {								// Returns a token based on the data from the automaton
		return Token(tokenType, value, lineNum);
	}
};
#endif

