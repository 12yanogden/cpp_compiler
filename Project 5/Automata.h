#ifndef AUTOMATA_H
#define AUTOMATA_H
#include <string>
#include "Token.h"
using namespace std;

class Automata {
protected:
	string tokenType;
public:
	Automata(const string& type = "NO TYPE") {									// Constructor
		this->tokenType = tokenType;
	}

	virtual ~Automata() {}														// Destructor

	virtual int getReadCount(string& input) = 0;								// Returns the read count

	virtual string getTokenType() const {										// Returns the tokenType
		return tokenType;
	}

	virtual Token getToken(const string& value, const int& lineNum) const = 0;	// Returns a token based on the data from the automaton
};
#endif

