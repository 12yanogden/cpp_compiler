#ifndef PREDICATE_H
#define PREDICATE_H
#include <vector>
#include <string>
#include "Parameter.h"
using namespace std;

class Predicate {

private:
	string name;
	vector<Parameter> parameters;

public:
	Predicate() {};

	Predicate(const string& name, const vector<Parameter>& parameters) {		// Constructor
		this->name = name;
		this->parameters = parameters;
	}


	~Predicate() {}																// Destructor


	bool operator==(const Predicate& predicate) const {							// Overloads the == operator
		return (name == predicate.name && parameters == predicate.parameters);
	}


	string getName() const {													// Returns the name of the predicate
		return name;
	}


	vector<Parameter> getParameters() const {									// Returns the parameters of the predicate
		return parameters;
	}


	vector<string> getParameterValues() const {									// Returns a vector of the values of the parameters of the predicate
		vector<string> parameterValues;

		for (size_t i = 0; i < parameters.size(); i++) {
			parameterValues.push_back(parameters[i].getValue());
		}

		return parameterValues;
	}


	friend ostream& operator<< (ostream& out, const Predicate& predicate) {		// Overloads the insertion operator to return a summary of the predicate
		out << predicate.toString();
		return out;
	};


	string toString() const {													// Returns a summary of the predicate
		stringstream out;

		out << name << "(";

		for (size_t i = 0; i < parameters.size(); i++) {
			out << parameters[i];

			if (i != parameters.size() - 1) {
				out << ",";
			}
		}

		out << ")";

		return out.str();
	}
};
#endif