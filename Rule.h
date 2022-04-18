#ifndef RULE_H
#define RULE_H
#include <vector>
#include <string>
#include "Predicate.h"
using namespace std;

class Rule {

private:
	Predicate headPredicate;
	vector<Predicate> predicates;

public:
	Rule() {}																		// Default Constructor

	Rule(const Predicate& headPredicate, const vector<Predicate>& predicates) {		// Constructor
		this->headPredicate = headPredicate;
		this->predicates = predicates;
	}


	~Rule() {}																		// Destructor


	bool operator==(const Rule& rule) const {										// Overloads the == operator
		return (headPredicate == rule.headPredicate && predicates == rule.predicates);
	}


	Predicate getHeadPredicate() const {											// Returns the head predicate
		return headPredicate;
	}


	string getHeadName() const {
		return headPredicate.getName();
	}


	vector<Predicate> getPredicates() const {										// Returns the predicates
		return predicates;
	}


	template <typename T>															// Returns a string of the values of the given vector, separated by commas
	string vectorToString(const vector<T>& vector) const {
		stringstream out;

		for (size_t i = 0; i < vector.size(); i++) {
			out << vector[i];
			if (i != vector.size() - 1) {
				out << ",";
			}
		}

		return out.str();
	}


	friend ostream& operator<< (ostream& out, Rule& rule) {							// Overloads the insertion operator to return a summary of the rule
		out << rule.toString();
		return out;
	};


	string toString() const {														// Returns a summary of the rule
		stringstream out;

		out << headPredicate << " :- " << vectorToString(predicates) << ".";

		return out.str();
	}
};
#endif