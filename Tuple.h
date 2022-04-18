#ifndef TUPLE_H
#define TUPLE_H
#include <vector>
#include <string>
#include <sstream>
using namespace std;

class Tuple {

private:
	vector<string> values;

public:
	Tuple(const vector<string>& values = {}) {							// Constructor
		this->values = values;
	}


	Tuple(vector<pair<int, int>> dupIndexes, const Tuple& alpha, const Tuple& beta) {	// Constructor, used for join
		values = alpha.getValues();
		const vector<string>& betaValues = beta.getValues();

		for (size_t i = 0; i < betaValues.size(); i++) {
			if (!ifDupIndex(dupIndexes, i)) {
				values.push_back(betaValues[i]);
			}
		}
	}


	~Tuple() {}															// Destructor


	bool ifDupIndex(const vector<pair<int, int>>& dupIndexes, int index) const {
		bool ifDupIndex = false;

		for (size_t i = 0; i < dupIndexes.size(); i++) {
			if (index == dupIndexes[i].second) {
				ifDupIndex = true;
			}
		}

		return ifDupIndex;
	}


	bool operator==(const Tuple& tuple) const {							// Overloads the < operator for looping through tuple sets
		return (values == tuple.values);
	}


	bool operator<(const Tuple& tuple) const {							// Overloads the < operator for looping through tuple sets
		return (values < tuple.values);
	}


	void addValue(const string& value) {								// Adds a value to the tuple
		values.push_back(value);
	}


	bool ifEmpty() {													// Returns if the tuple is empty
		return (values.size() != 0);
	}


	void clearValues() {
		values = {};
	}


	vector<string> getValues() const {									// Returns values of the tuple
		return values;
	}


	string getValueByIndex(const size_t& index) const {					// Returns the value of the tuple at a given index
		return values[index];
	}


	bool select(const int& index, const string& value) {				// Returns if the value at the given index is equal to the given value
		return (values[index] == value);
	}


	bool selectSame(const int& index1, const int& index2) {				// Returns if the values at the given indexes are equal
		return (values[index1] == values[index2]);
	}


	Tuple project(const vector<int>& projectIndexes) {					// Returns a new tuple with only the projected attributes
		Tuple tuple;

		for (size_t i = 0; i < projectIndexes.size(); i++) {
			tuple.addValue(values[projectIndexes[i]]);
		}

		return tuple;
	}
};
#endif