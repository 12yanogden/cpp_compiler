#ifndef DATALOGPROGRAM_H
#define DATALOGPROGRAM_H
#include <vector>
#include <set>
#include <string>
#include "Parameter.h"
#include "Predicate.h"
#include "Rule.h"
#include "Token.h"
using namespace std;

class DatalogProgram {

private:
	vector<Predicate> schemes;
	vector<Predicate> facts;
	vector<Rule> rules;
	vector<Predicate> queries;
	set<string> domain;

public:
	DatalogProgram() {}																				// Default Constructor

	DatalogProgram(vector<Token> tokens) {															// Constructor
		setSchemes(tokens);
		setFacts(tokens);
		setRules(tokens);
		setQueries(tokens);
		setDomain(tokens);
	}


	~DatalogProgram() {}																			// Destructor


	const void cropTokens(const string& typeName, const string& endToken, vector<Token>& tokens) {	// Removes all elements of the tokens vector which are not of interest
		for (size_t i = 0; i < tokens.size(); i++) {
			if (tokens[i].getType() == typeName) {
				tokens.erase(tokens.begin(), tokens.begin() + (i + 2));
				i = tokens.size();
			}
		}
		for (size_t i = 0; i < tokens.size(); i++) {
			if (tokens[i].getType() == endToken) {
				tokens.erase(tokens.begin() + i, tokens.end());
				i = tokens.size();
			}
		}
	}


	const void pop_front(vector<Token>& tokens) {													// Removes first token from input tokens
		tokens.erase(tokens.begin());
	}

	const void popPunct(vector<Token>& tokens) {													// Removes punctuation from input tokens
		if (!tokens.empty()) {
			if (tokens.front().getType() == "COMMA" ||
				tokens.front().getType() == "Q_MARK") {
				pop_front(tokens);
			}
		}
	}


	const Predicate createPredicate(vector<Token>& tokens) {										// Creates a predicate from tokens
		const string name = tokens.front().getValue();
		vector<Parameter> parameters;

		pop_front(tokens);
		pop_front(tokens);

		do {
			parameters.push_back(Parameter(tokens.front().getType(), tokens.front().getValue()));
			pop_front(tokens);
			popPunct(tokens);
		} while (tokens.front().getType() != "RIGHT_PAREN");

		pop_front(tokens);
		popPunct(tokens);

		return Predicate(name, parameters);
	}


	void setSchemes(vector<Token> tokens) {															// Sets the schemes
		cropTokens("SCHEMES", "FACTS", tokens);

		do {
			schemes.push_back(createPredicate(tokens));
		} while (!tokens.empty());
	}


	void setFacts(vector<Token> tokens) {															// Sets the facts
		cropTokens("FACTS", "RULES", tokens);

		while (!tokens.empty()) {
			facts.push_back(createPredicate(tokens));
			pop_front(tokens);
		}
	}


	void setRules(vector<Token> tokens) {															// Sets the rules
		cropTokens("RULES", "QUERIES", tokens);

		while (!tokens.empty()) {
			Predicate headPredicate = createPredicate(tokens);
			vector<Predicate> predicates;

			pop_front(tokens);

			do {
				predicates.push_back(createPredicate(tokens));
			} while (tokens.front().getType() != "PERIOD");

			pop_front(tokens);

			rules.push_back(Rule(headPredicate, predicates));
		}
	}


	void setQueries(vector<Token> tokens) {															// Sets the queries
		cropTokens("QUERIES", "EOF", tokens);

		do {
			queries.push_back(createPredicate(tokens));
		} while (!tokens.empty());
	}


	void setDomain(vector<Token> tokens) {															// Sets the domain
		for (size_t i = 0; i < facts.size(); i++) {
			vector<string> factParameters = facts[i].getParameterValues();

			for (size_t j = 0; j < factParameters.size(); j++) {
				domain.insert(factParameters[j]);
			}
		}
	}


	vector<Predicate> getSchemes() const {															// Returns the schemes
		return schemes;
	}


	vector<Predicate> getFacts() const {															// Returns the facts
		return facts;
	}


	vector<Rule> getRules() const {																	// Returns the rules
		return rules;
	}


	Rule getRuleByName(const string& name) const {													// Returns a rule with a head predicate that has the name given
		for (size_t i = 0; i < rules.size(); i++) {
			const string& ruleName = rules[i].getHeadPredicate().getName();

			if (ruleName == name) {
				return rules[i];
			}
		}

		return Rule(Predicate(), vector<Predicate>());
	}


	vector<Rule> getRulesByIndexes(const set<int>& indexes) {
		vector<Rule> rules;

		for (const int& index : indexes) {
			rules.push_back(this->rules[index]);
		}

		return rules;
	}


	vector<Predicate> getQueries() const {															// Returns the queries
		return queries;
	}


	template <typename T>																			// Returns a string list of a vector's contents
	const string vectorToString(vector<T>& vector, const string& specialEnd = "") {
		stringstream out;

		for (size_t i = 0; i < vector.size(); i++) {
			out << "  " << vector[i] << specialEnd << endl;
		}

		return out.str();
	}


	template <typename T>																			// Returns a string list of a set's contents
	const string setToString(const set<T>& set) {
		stringstream out;

		for (string outString : set) {
			out << "  " << outString << endl;
		}

		return out.str();
	}


	friend ostream& operator<< (ostream& out, DatalogProgram& datalogProgram) {						// Overloads the insertion operator to return a summary of the DatalogProgram
		out << datalogProgram.toString();
		return out;
	};


	const string toString() {																		// Returns a summary of the DatalogProgram
		stringstream out;

		out << "Schemes(" << schemes.size() << "):" << endl;
		out << vectorToString(schemes);

		out << "Facts(" << facts.size() << "):" << endl;
		out << vectorToString(facts, ".");

		out << "Rules(" << rules.size() << "):" << endl;
		out << vectorToString(rules);

		out << "Queries(" << queries.size() << "):" << endl;
		out << vectorToString(queries, "?");

		out << "Domain(" << domain.size() << "):" << endl;
		out << setToString(domain);

		return out.str();
	}
};
#endif
