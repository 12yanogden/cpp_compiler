#ifndef PARAMETER_H
#define PARAMETER_H
#include <vector>
#include <string>
#include <sstream>
using namespace std;

class Parameter {

private:
	string type;
	string value;

public:
	Parameter(const string& type = "NO_TYPE", const string& value = "NO_VALUE") {	// Constructor
		setType(type);
		this->value = value;
	}


	~Parameter() {}																	// Destructor


	bool operator==(const Parameter& parameter) const {								// Overloads the == operator
		return (type == parameter.type && value == parameter.value);
	}


	string getValue() const {														// Returns the value of the parameter
		return value;
	}


	bool isConstant() const {														// Returns if the parameter value is a constant
		return (type == "STRING");
	}


	void setType(const string& type) {												// Sets the type of the parameter
		if (type == "STRING" || type == "ID") {
			this->type = type;
		}
	}


	friend ostream& operator<< (ostream& out, const Parameter& parameter) {			// Overloads the insertion operator to return a summary of the Parameter
		out << parameter.toString();
		return out;
	};


	string toString() const {														// Returns a summary of the Parameter
		return value;
	}
};
#endif