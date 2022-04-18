#ifndef AUTOMATON_UNDEFINED_H
#define AUTOMATON_UNDEFINED_H
#include <string>
#include <ctype.h>
#include <stack>
#include "Automata.h"
#include "Token.h"
using namespace std;

class Automaton_UNDEFINED : public Automata {
private:
	stack<int> apostropheIndexes;
public:
	Automaton_UNDEFINED(const string& tokenType = "UNDEFINED") {	// Constructor
		this->tokenType = tokenType;
	}

	~Automaton_UNDEFINED() {}										// Destructor

	int getReadCount(string& input) {								// Returns read count
		int readCount = 0;

		if (input[0] == '\'') {										// Reads unfinished strings
			apostropheIndexes.push(0);
			for (size_t i = 1; i < input.size(); i++) {
				if (input[i] == '\'') {
					if (apostropheIndexes.size() % 2 == 1) {
						if (i + 1 < input.size()) {
							if (input[i + 1] != '\'') {
								apostropheIndexes.pop();
								i = input.size();
							} else {
								i++;
							}
						}
					} else {
						apostropheIndexes.push(i);
					}
				}
			}

			if (!apostropheIndexes.empty()) {
				readCount = input.size();
			}
		} else if (input[0] == '#') {								// Reads unfinished multi-line comments
			if (input[1]) {
				if (input[1] == '|') {
					for (size_t i = 2; i < input.size(); i++) {
						if (input[i] == '|') {
							if (input[i + 1]) {
								if (input[i + 1] == '#') {
									i = input.size();
								}
							}
						} else if (i == input.size() - 1) {
							readCount = i + 1;
						}
					}
				}
			}
		}

		return readCount;
	}

	Token getToken(const string& value, const int& lineNum) const {
		return Token(tokenType, value, lineNum);
	}
};
#endif

