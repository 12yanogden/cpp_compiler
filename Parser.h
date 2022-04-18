#ifndef Parser_H
#define Parser_H
#include <vector>
#include <string>
#include "Token.h"
using namespace std;

class Parser {
private:
	Token errorToken;
public:
	Parser(vector<Token> tokens) {												// Constructor
		try {
			parseAll(tokens);
		} catch (Token errorToken) {
			this->errorToken = errorToken;
		}
	}


	~Parser() {}																// Destructor


	bool isError() {															// Returns if an error token has been selected
		return (errorToken.getType() != "");
	}


	void verify(vector<Token>& tokens, string type) {
		if (tokens.front().getType() != type) {
			throw tokens.front();
		}

		tokens.erase(tokens.begin());
	}


	const void parseComma(vector<Token>& tokens) {
		if (tokens.front().getType() == "COMMA") {
			verify(tokens, "COMMA");
		}
	}

	const void parseUndefined(vector<Token>& tokens) {
		for (size_t i = 0; i < tokens.size(); i++) {
			if (tokens[i].getType() == "UNDEFINED") {
				throw tokens[i];
			}
		}
	}


	void parseAll(vector<Token>& tokens) {
		enum types { SCHEMES, FACTS, RULES, QUERIES };
		pair<vector<string>, string> grammar = { { "SCHEMES", "FACTS", "RULES", "QUERIES" }, "COLON" };

		parseUndefined(tokens);

		for (size_t i = 0; i < grammar.first.size(); i++) {
			verify(tokens, grammar.first[i]);
			verify(tokens, grammar.second);

			switch (i) {
			case SCHEMES:
				parseSchemes(tokens);
				break;
			case FACTS:
				parseFacts(tokens);
				break;
			case RULES:
				parseRules(tokens);
				break;
			case QUERIES:
				parseQueries(tokens);
				break;
			}
		}
	}


	void parseSchemes(vector<Token>& tokens) {
		do {
			parseScheme(tokens);
		} while (tokens.front().getType() != "FACTS");
	}


	void parseScheme(vector<Token>& tokens) {
		vector<string> grammar = { "ID", "LEFT_PAREN",/*ID(s),*/ "RIGHT_PAREN" };
		for (size_t i = 0; i < grammar.size(); i++) {

			if (i == 2) {
				do {
					verify(tokens, "ID");
					parseComma(tokens);
				} while (tokens.front().getType() == "ID");
			}

			verify(tokens, grammar[i]);
		}
	}


	void parseFacts(vector<Token>& tokens) {
		while (tokens.front().getType() != "RULES") {
			parseFact(tokens);
		}
	}


	void parseFact(vector<Token>& tokens) {
		vector<string> grammar = { "ID", "LEFT_PAREN",/*STRING(s),*/ "RIGHT_PAREN", "PERIOD" };
		for (size_t i = 0; i < grammar.size(); i++) {

			if (i == 2) {
				do {
					verify(tokens, "STRING");
					parseComma(tokens);
				} while (tokens.front().getType() == "STRING");
			}

			verify(tokens, grammar[i]);
		}
	}


	void parseRules(vector<Token>& tokens) {
		while (tokens.front().getType() != "QUERIES") {
			parseRule(tokens);
		}
	}


	void parseRule(vector<Token>& tokens) {
		vector<string> grammar = { /*headPredicate,*/ "COLON_DASH", /*predicate(s),*/ "PERIOD" };

		for (size_t i = 0; i < grammar.size(); i++) {

			switch (i) {
			case 0:
				parseHeadPredicate(tokens);
				break;
			case 1:
				do {
					parsePredicate(tokens);
					parseComma(tokens);
				} while (tokens.front().getType() != "PERIOD");
			}

			verify(tokens, grammar[i]);
		}
	}


	void parseHeadPredicate(vector<Token>& tokens) {
		vector<string> grammar = { "ID", "LEFT_PAREN", /*ID(s),*/ "RIGHT_PAREN" };

		for (size_t i = 0; i < grammar.size(); i++) {
			if (i == 2) {
				do {
					verify(tokens, "ID");
					parseComma(tokens);
				} while (tokens.front().getType() == "ID");
			}

			verify(tokens, grammar[i]);
		}
	}


	void parsePredicate(vector<Token>& tokens) {
		vector<string> grammar = { "ID", "LEFT_PAREN", /*parameters(s),*/ "RIGHT_PAREN" };

		for (size_t i = 0; i < grammar.size(); i++) {
			if (i == 2) {
				do {
					parseParameter(tokens);
					parseComma(tokens);
				} while (tokens.front().getType() != "RIGHT_PAREN");
			}

			verify(tokens, grammar[i]);
		}
	}


	void parseParameter(vector<Token>& tokens) {
		/*grammar = { STRING | ID }*/

		if (tokens.front().getType() == "STRING") {
			verify(tokens, "STRING");
		} else {
			verify(tokens, "ID");
		}
	}


	void parseQueries(vector<Token>& tokens) {
		do {
			parseQuery(tokens);
		} while (tokens.front().getType() != "EOF");
	}


	void parseQuery(vector<Token>& tokens) {
		/*grammar = { predicate(s), Q_MARK }*/

		parsePredicate(tokens);
		verify(tokens, "Q_MARK");
	}


	friend ostream& operator<< (ostream& out, Parser& parser) {					// Overloads the insertion operator to return a summary of the Parser
		out << parser.toString();
		return out;
	};


	const string toString() {													// Returns a summary of the Parser
		stringstream out;

		if (errorToken.getType() != "") {
			out << "Failure!" << endl;
			out << "  " << errorToken << endl;
		} else {
			out << "Success!" << endl;
		}

		return out.str();
	}
};
#endif