#ifndef RELATION_H
#define RELATION_H
#include <vector>
#include <string>
#include <set>
#include <sstream>
#include <algorithm>
#include "Header.h"
#include "Tuple.h"
using namespace std;

class Relation {

private:
	string name;
	Header header;
	set<Tuple> tuples;

public:
	Relation(const string& name = "NO_NAME") {													// Default Constructor
		this->name = name;
	}

	Relation(const Header& header, const string& name = "NO_NAME") {							// Constructor
		this->name = name;
		this->header = header;
	}


	~Relation() {}																				// Destructor


	void addTuple(const Tuple& tuple) {															// Adds a tuple to the set
		tuples.insert(tuple);
	}


	void setTuples(const set<Tuple>& tuples) {													// Sets the set of tuples to the set given
		this->tuples = tuples;
	}


	set<Tuple> getTuples() const {																// Returns the set of tuples
		return tuples;
	}


	size_t getTupleCount() const {																// Returns the number of tuples
		return tuples.size();
	}


	bool empty() const {																		// Returns if the relation has tuples
		return tuples.empty();
	}


	void setHeader(const Header& header) {														// Sets the header to the header given
		this->header = header;
	}


	string getName() const {																	// Returns the name of the relation
		return name;
	}


	void setName(const string& name) {															// Sets the name to a given string
		this->name = name;
	}


	Header getHeader() const {																	// Returns the header
		return header;
	}


	vector<string> getAttributes() const {														// Returns the attributes of the header
		return header.getAttributes();
	}


	Tuple getTupleByIndex(const size_t& target) const {											// Returns a tuple at a given index, else blank tuple
		size_t index = 0;

		for (Tuple tuple : tuples) {
			if (index == target) {
				return tuple;
			}
			index++;
		}

		return Tuple();
	}


	Relation select(const int& index, const string& value) const {									// Selects tuples with the value equal to the value given at the given index
		Relation relation(header, name);

		for (Tuple tuple : tuples) {
			if (tuple.select(index, value)) {
				relation.addTuple(tuple);
			}
		}

		return relation;
	}


	Relation selectSame(const int& index1, const int& index2) const {								// Selects tuples with the same values in the given indexes
		Relation relation(header, name);

		for (Tuple tuple : tuples) {
			if (tuple.selectSame(index1, index2)) {
				relation.addTuple(tuple);
			}
		}

		return relation;
	}


	Relation project(const vector<int>& projectIndexes) const {										// Projects a relation
		Relation relation(header.project(projectIndexes), name);

		for (Tuple tuple : tuples) {
			relation.addTuple(tuple.project(projectIndexes));
		}

		return relation;
	}


	Relation rename(const vector<string>& attributes) const {										// Replaces the attributes of a relation
		Relation relation(Header(attributes), name);

		relation.setTuples(tuples);

		return relation;
	}


	vector<pair<int, int>> getDupIndexes(const Header& alpha, const Header& beta) const {			// Returns the indexes of duplicate pairs found in the given headers
		vector<pair<int, int>> dupIndexes;
		const vector<string>& alphaValues = alpha.getAttributes();
		const vector<string>& betaValues = beta.getAttributes();

		for (size_t i = 0; i < alphaValues.size(); i++) {
			for (size_t j = 0; j < betaValues.size(); j++) {
				if (alphaValues[i] == betaValues[j]) {
					dupIndexes.push_back(pair<int, int>(i, j));
					j = betaValues.size();
				}
			}
		}

		return dupIndexes;
	}


	bool ifValidJoin(const vector<pair<int, int>>& dupIndexes, const Tuple& alpha, const Tuple& beta) {
		bool ifValidJoin = true;

		for (size_t i = 0; i < dupIndexes.size(); i++) {
			if (alpha.getValueByIndex(dupIndexes[i].first) != beta.getValueByIndex(dupIndexes[i].second)) {
				ifValidJoin = false;
			}
		}

		return ifValidJoin;
	}


	Relation join(Relation& beta) {																	// Returns a relation, the result of this relation joined with another relation
		const vector<pair<int, int>>& dupIndexes = getDupIndexes(header, beta.getHeader());
		const Header joinHeader(dupIndexes, header, beta.getHeader());
		Relation joinRelation(joinHeader, name + "_" + beta.getName());
		const set<Tuple>& betaTuples = beta.getTuples();

		for (const Tuple& alphaTuple : tuples) {
			for (const Tuple& betaTuple : betaTuples) {
				if (ifValidJoin(dupIndexes, alphaTuple, betaTuple)) {
					joinRelation.addTuple(Tuple(dupIndexes, alphaTuple, betaTuple));
				}
			}
		}

		return joinRelation;
	}


	string toString(const bool& ifAllConstant) const {												// Returns a summary of the relation
		stringstream out;

		for (Tuple tuple : tuples) {
			const vector<string>& attributes = header.getAttributes();
			const vector<string>& tupleValues = tuple.getValues();

			// Prints indenting spaces if the parameters are not all constants
			if (!ifAllConstant) {
				out << "  ";
			}

			for (size_t i = 0; i < attributes.size(); i++) {
				out << attributes[i] << "=" << tupleValues[i];

				if (i != attributes.size() - 1) {
					out << ", ";
				} else {
					out << endl;
				}
			}
		}

		return out.str();
	}
};
#endif