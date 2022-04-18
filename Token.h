#ifndef TOKEN_H
#define TOKEN_H
using namespace std;

class Token {
private:
	string type;
	string value;
	int lineNum;
public:
	Token(const string& type = "", const string& value = "", const int& lineNum = 0) {		// Constructor
		this->type = type;
		this->value = value;
		this->lineNum = lineNum;
	}

	~Token() {}																				// Destructor


	const string getType() {																// Returns the type of the token
		return type;
	}


	const string getValue() {
		return value;
	}


	friend ostream& operator<< (ostream& out, Token& token) {								// Overloads the insertion operator to return a summary of the Token
		out << token.toString();
		return out;
	};


	const string toString() {																// Returns a summary of the Token
		return "(" + type + ",\"" + value + "\"," + to_string(lineNum) + ")";
	}
};
#endif

