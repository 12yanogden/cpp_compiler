#ifndef AUTOMATON_COMMENT_H
#define AUTOMATON_COMMENT_H
#include <string>
#include <ctype.h>
#include "Automata.h"
#include "Token.h"
using namespace std;

class Automaton_COMMENT : public Automata {
public:
	Automaton_COMMENT(const string& tokenType = "COMMENT") {				// Constructor
		this->tokenType = tokenType;
	}

	~Automaton_COMMENT() {}													// Destructor

	int getReadCount(string& input) {										// Returns read count
		int readCount = 0;

		if (input[0] == '#') {
			if (input[1]) {
				if (input[1] == '|') {
					for (size_t i = 2; i < input.size(); i++) {				// Reads multi-line comments
						if (input[i] == '|') {
							if (i != input.size() - 1) {
								if (input[i + 1] == '#') {
									readCount = i + 2;
									i = input.size();
								}
							}
						}
					}
				} else {
					for (size_t i = 1; i < input.size(); i++) {				// Reads single-line comments
						if (input[i] == '\n') {
							readCount = i;
							i = input.size();
						}
					}
				}
			} else {
				readCount = 1;
			}
		}

		return readCount;
	}

	Token getToken(const string& value, const int& lineNum) const {			// Returns a token based on the data from the automaton
		return Token(tokenType, value, lineNum);
	}
};
#endif

