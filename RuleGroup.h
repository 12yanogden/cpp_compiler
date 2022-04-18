#ifndef RULEGROUP_H
#define RULEGROUP_H
#include <vector>
#include <string>
#include <sstream>
using namespace std;

class RuleGroup {

private:
	map<int, Rule> rules;
	vector<pair<Rule, Relation>> evaluations;

public:
	RuleGroup() {}																	// Default Constructor

	RuleGroup(const set<int>& indexes, const vector<Rule>& rules) {					// Constructor
		setRules(indexes, rules);
	}


	~RuleGroup() {}																	// Destructor


	void addEvaluation(const pair<Rule, Relation>& evaluation) {					// Adds an evaluation (rule/relation pair) to evaluations
		evaluations.push_back(evaluation);
	}


	void setRules(const set<int>& indexes, const vector<Rule>& rules) {				// Populates the rules map
		int i = 0;

		for (const int& index : indexes) {
			this->rules.insert(pair<int, Rule>(index, rules[i]));
			i++;
		}
	}


	bool ifRule(const Rule& rule) const {											// Returns if a rule is present in the rule group
		bool ifRule = false;

		for (const pair<int, Rule>& rulePair : rules) {
			if (rulePair.second == rule) {
				ifRule = true;
			}
		}

		return ifRule;
	}


	Relation joinRelations(vector<Relation> relations) {
		while (relations.size() > 1) {
			relations.front() = relations.front().join(relations.back());
			relations.pop_back();
		}

		return relations.front();
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


	Relation evaluateRule(Database& schemesFacts, const Rule& rule) {
		const Predicate& headPredicate = rule.getHeadPredicate();
		const vector<string>& headPredicateValues = headPredicate.getParameterValues();
		const vector<Predicate>& predicates = rule.getPredicates();
		const vector<string>& schemeFactAttributes = schemesFacts.getRelationByName(headPredicate.getName()).getAttributes();
		vector<Relation> queryRelations;
		vector<int> dupIndexes;

		for (size_t i = 0; i < predicates.size(); i++) {
			Relation& schemeFact = schemesFacts.getRelationByName(predicates[i].getName());

			queryRelations.push_back(query(schemeFact, predicates[i].getParameters()));
		}

		const Relation& joinRelation = joinRelations(queryRelations);

		for (size_t i = 0; i < headPredicateValues.size(); i++) {
			const int& dupIndex = getDupIndex(joinRelation.getAttributes(), headPredicateValues[i]);

			if (dupIndex != -1) {
				dupIndexes.push_back(dupIndex);
			}
		}

		const Relation& projectRelation = joinRelation.project(dupIndexes);

		Relation renameRelation = projectRelation.rename(schemeFactAttributes);

		renameRelation.setName(rule.getHeadPredicate().getName());

		return renameRelation;
	}


	void unite(Database& schemesFacts, Relation& relation) {
		Relation& schemeFact = schemesFacts.getRelationByName(relation.getName());
		const set<Tuple>& schemeFactTuples = schemeFact.getTuples();

		for (const Tuple& schemeFactTuple : schemeFactTuples) {
			relation.addTuple(schemeFactTuple);
		}

		schemeFact = relation;
	}


	void evaluateRules(Database& schemesFacts) {
		vector<bool> ifNewTuples;

		do {
			ifNewTuples.clear();

			for (const pair<int, Rule>& rulePair : rules) {
				const Rule& rule = rulePair.second;
				const string& ruleName = rule.getHeadPredicate().getName();
				Relation ruleRelation = evaluateRule(schemesFacts, rule);

				ifNewTuples.push_back(setRuleEvaluations(schemesFacts, rule, ruleRelation));

				unite(schemesFacts, ruleRelation);
			}
		} while (ifNewTuple(ifNewTuples));
	}


	bool ifNewTuple(vector<bool>& ifNewTuples) const {
		bool ifContinue = false;

		for (size_t i = 0; i < ifNewTuples.size(); i++) {
			if (ifNewTuples[i]) {
				ifContinue = true;
				i = ifNewTuples.size();
			}
		}

		return ifContinue;
	}


	bool setRuleEvaluations(Database& schemesFacts, const Rule& rule, const Relation& relation) {
		bool ifContinue = false;
		const set<Tuple>& relationTuples = relation.getTuples();
		const Relation& schemeFact = schemesFacts.getRelationByName(relation.getName());
		const set<Tuple>& schemeFactTuples = schemeFact.getTuples();
		Relation ruleEvaluation(schemeFact.getHeader(), schemeFact.getName());

		for (const Tuple& relationTuple : relationTuples) {
			bool ifDup = false;

			for (const Tuple& schemeFactTuple : schemeFactTuples) {
				if (relationTuple == schemeFactTuple) {
					ifDup = true;
				}
			}

			if (!ifDup) {
				ruleEvaluation.addTuple(relationTuple);
				ifContinue = true;
			}
		}

		addEvaluation(pair<Rule, Relation>(rule, ruleEvaluation));

		if (ifSingleEval()) {
			ifContinue = false;
		}

		return ifContinue;
	}


	bool ifSingleEval() {															// Returns if the rule group should be evaluated only once
		bool ifSingleEval = false;

		if (rules.size() == 1) {
			for (const pair<int, Rule>& rulePair : rules) {
				const vector<Predicate>& predicates = rulePair.second.getPredicates();
				bool ifMatch = false;

				for (size_t i = 0; i < predicates.size(); i++) {
					if (rulePair.second.getHeadName() == predicates[i].getName()) {
						ifMatch = true;
						i = predicates.size();
					}
				}

				ifSingleEval = !ifMatch;
			}
		}

		return ifSingleEval;
	}


	string getRuleIndexesAsString() const {
		stringstream out;

		size_t index = 0;
		for (pair<int, Rule> rule : rules) {
			out << "R" << rule.first;

			if (index != rules.size() - 1) {
				out << ",";
			}

			index++;
		}

		return out.str();
	}


	friend ostream& operator<< (ostream& out, const RuleGroup& ruleGroup) {			// Overloads the insertion operator to return a summary of the Rule Group
		out << ruleGroup.toString();
		return out;
	};


	string toString() const {														// Returns a summary of the Rule Group
		stringstream out;
		const string& ruleIndexes = getRuleIndexesAsString();

		out << "SCC: ";

		out << ruleIndexes;

		out << endl;

		for (size_t i = 0; i < evaluations.size(); i++) {
			out << evaluations[i].first.toString() << endl;
			out << evaluations[i].second.toString(false);
		}

		out << evaluations.size() / rules.size() << " passes: " << ruleIndexes;

		return out.str();
	}
};
#endif