#ifndef DATABASE_H
#define DATABASE_H
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include "Relation.h"
using namespace std;

class Database {

private:
	deque<Relation> relations;

public:
	Database() {}															// Constructor


	~Database() {}															// Destructor


	void addRelation(const Relation& relation) {							// Adds a relation to the deque of relations
		relations.push_back(relation);
	}


	Relation& getRelationByName(const string& name) {						// Returns a relation with the name given, else returns the first relation
		for (size_t i = 0; i < relations.size(); i++) {
			if (relations[i].getName() == name) {
				return relations[i];
			}
		}

		return relations[0];	// Never expected to happen
	}



	Relation getLastRelation() const {										// Returns the last relation added to the database
		return relations.back();
	}


	deque<Relation> getRelations() const {									// Returns a deque of all the relations of the database
		return relations;
	}

	friend ostream& operator<< (ostream& out, Database& database) {			// Overloads the insertion operator to return a summary of the database
		out << database.toString();
		return out;
	};


	const string toString() {												// Returns a summary of the database
		stringstream out;

		out << "" << endl;

		return out.str();
	}
};
#endif