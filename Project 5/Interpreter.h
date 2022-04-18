#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <vector>
#include <string>
#include <sstream>
#include "DatalogProgram.h"
#include "Database.h"
#include "Graph.h"
#include "RuleGroup.h"
using namespace std;

class Interpreter {

private:
	DatalogProgram datalog;
	Database schemesFacts;
	Graph dependencyGraph;
	Graph reverseGraph;
	vector<RuleGroup> ruleGroups;
	Database queries;


public:
	Interpreter(DatalogProgram datalog) {														// Constructor
		this->datalog = datalog;
		setSchemesFacts();
		setRuleGroups();
		evaluateRules();
		setQueries();
	}


	~Interpreter() {}																			// Destructor


	void setSchemesFacts() {																	// Populates schemesFacts
		const vector<Predicate>& schemes = datalog.getSchemes();
		const vector<Predicate>& facts = datalog.getFacts();

		// Populates schemeFacts with relations
		for (size_t i = 0; i < schemes.size(); i++) {
			const string& name = schemes[i].getName();
			const vector<string>& headerValues = schemes[i].getParameterValues();

			schemesFacts.addRelation(Relation(Header(headerValues), name));
		}

		// Populates schemeFact relations with tuples
		for (size_t i = 0; i < facts.size(); i++) {
			const string& relationName = facts[i].getName();
			const Tuple tuple(facts[i].getParameterValues());

			schemesFacts.getRelationByName(relationName).addTuple(tuple);
		}
	}


	void setRuleGroups() {
		dependencyGraph = Graph(datalog.getRules());
		reverseGraph = dependencyGraph.reverseGraph();
		reverseGraph.setPostOrder();
		dependencyGraph.setPostOrder(reverseGraph.getPostOrder());
		dependencyGraph.setRuleGroups();

		vector<set<int>> ruleGroupIndexes = dependencyGraph.getRuleGroups();
		for (size_t i = 0; i < ruleGroupIndexes.size(); i++) {
			vector<Rule> rules = datalog.getRulesByIndexes(ruleGroupIndexes[i]);
			ruleGroups.push_back(RuleGroup(ruleGroupIndexes[i], rules));
		}
	}


	void evaluateRules() {
		const vector<Rule>& rules = datalog.getRules();
		vector<bool> ifNewTuples;

		for (size_t i = 0; i < ruleGroups.size(); i++) {
			ruleGroups[i].evaluateRules(schemesFacts);
		}
	}


	int getDupIndex(const vector<string>& attributes, const string& value) const {				// Returns index of the duplicate of the value, -1 if false
		int dupIndex = -1;

		for (size_t i = 0; i < attributes.size(); i++) {
			if (attributes[i] == value) {
				dupIndex = i;
				i = attributes.size();
			}
		}

		return dupIndex;
	}


	Relation query(Relation relation, const vector<Parameter>& parameters) {					// Returns a relation with data filtered by the parameters given
		vector<string> attributes;
		vector<int> projectIndexes;

		for (size_t i = 0; i < parameters.size(); i++) {
			const string& value = parameters[i].getValue();

			if (parameters[i].isConstant()) {
				relation = relation.select(i, value);
			} else {
				const int& dupIndex = getDupIndex(attributes, value);
				if (dupIndex == -1) {
					attributes.push_back(value);
					projectIndexes.push_back(i);
				} else {
					relation = relation.selectSame(dupIndex, i);
				}
			}
		}

		relation = relation.project(projectIndexes);
		relation = relation.rename(attributes);

		return relation;
	}


	void setQueries() {																			// Populates the queries database with relations
		const vector<Predicate>& queries = datalog.getQueries();

		for (size_t i = 0; i < queries.size(); i++) {
			// Finds where a query and a relation have the same name
			const Relation& schemeFact = schemesFacts.getRelationByName(queries[i].getName());

			// Creates new relation based on query
			const Relation& relation = query(schemeFact, queries[i].getParameters());

			// Adds new relation to query database
			this->queries.addRelation(relation);
		}
	}


	bool ifAllConstant(const Predicate& query) {												// Returns if the parameters of the given Predicate are all constants
		const vector<Parameter>& parameters = query.getParameters();
		bool ifAllConstant = true;

		for (size_t i = 0; i < parameters.size(); i++) {
			if (!parameters[i].isConstant()) {
				ifAllConstant = false;
				i = parameters.size();
			}
		}

		return ifAllConstant;
	}


	friend ostream& operator<< (ostream& out, Interpreter& interpreter) {						// Overloads the insertion operator to return a summary of the interpreter
		out << interpreter.toString();
		return out;
	};


	string toString() {																			// Returns a summary of the interpreter
		stringstream out;
		const vector<Rule>& rules = datalog.getRules();
		const vector<Predicate>& queries = datalog.getQueries();

		out << "Dependency Graph" << endl;
		out << dependencyGraph << endl;

		out << "Rule Evaluation" << endl;

		for (size_t i = 0; i < ruleGroups.size(); i++) {
			out << ruleGroups[i] << endl;
		}

		out << endl;

		out << "Query Evaluation" << endl;

		for (size_t i = 0; i < queries.size(); i++) {
			const deque<Relation> relations = this->queries.getRelations();

			out << queries[i] << "? ";

			if (relations[i].empty()) {
				out << "No" << endl;
			} else {
				out << "Yes(" << relations[i].getTupleCount() << ")" << endl;

				out << relations[i].toString(ifAllConstant(queries[i]));
			}
		}

		out << endl;

		return out.str();
	}
};
#endif