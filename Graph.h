#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include "Rule.h"
using namespace std;

class Graph {

private:
	map <int, bool> ifVisited;
	map<int, set<int>> edges;
	list<int> postOrder;
	vector<set<int>> ruleGroups;
	vector<int> ruleOrder;

public:
	Graph() {}																						// Default Constructor


	Graph(const vector<Rule>& rules) {																// Constructor
		setIfVisited(rules);
		setEdges(rules);
	}


	Graph(const map<int, bool>& ifVisited, const map<int, set<int>>& edges) {						// Constructor
		this->ifVisited = ifVisited;
		this->edges = edges;
	}


	~Graph() {}																// Destructor


	void setIfVisited(const vector<Rule>& rules) {										// Populates the ifVisited map
		for (unsigned int i = 0; i < rules.size(); i++) {
			ifVisited.insert(pair<int, bool>(i, false));
		}
	}


	void setEdges(const vector<Rule>& rules) {
		for (size_t i = 0; i < rules.size(); i++) {
			set<int> dependencies;
			vector<Predicate> rulePredicates = rules[i].getPredicates();

			for (size_t j = 0; j < rulePredicates.size(); j++) {
				for (size_t k = 0; k < rules.size(); k++) {
					if (rules[k].getHeadName() == rulePredicates[j].getName()) {
						dependencies.insert(k);
					}
				}
			}

			edges.insert(pair<int, set<int>>(i, dependencies));
		}
	}


	Graph reverseGraph() {
		map<int, set<int>> reverseEdges;

		for (pair<int, set<int>> edge : edges) {
			reverseEdges.insert(pair<int, set<int>>(edge.first, set<int>()));
		}

		for (pair<int, set<int>> edge : edges) {
			for (int dependency : edge.second) {
				reverseEdges[dependency].insert(edge.first);
			}
		}

		return Graph(ifVisited, reverseEdges);
	}


	list<int> getPostOrder() {
		return postOrder;
	}


	string getPostOrderAsString() {
		stringstream out;
		
		if (postOrder.empty()) {
			setPostOrder();
		}

		size_t index = 0;
		for (const int& i : postOrder) {
			out << i;
			
			if (index != postOrder.size() - 1) {
				out << ",";
			} else {
				out << endl;
			}

			index++;
		}

		return out.str();
	}


	void setPostOrder(const list<int>& postOrder) {
		this->postOrder = postOrder;
	}


	void setPostOrder() {
		for (size_t i = 0; i < edges.size(); i++) {
			setPostOrderRecursor(i);
		}
	}


	void setPostOrderRecursor(const int& index) {
		if (!ifVisited[index]) {
			ifVisited[index] = true;

			for (int dependency : edges[index]) {
				setPostOrderRecursor(dependency);
			}

			postOrder.push_back(index);
		}
	}


	string getRuleGroupsAsString() {
		stringstream out;

		for (size_t i = 0; i < ruleGroups.size(); i++) {
			out << "{ ";

			size_t index = 0;
			for (const int& rule : ruleGroups[i]) {
				out << "R" << rule;

				if (index != ruleGroups[i].size() - 1) {
					out << ",";
				}

				index++;
			}

			out << " }" << endl;
		}

		return out.str();
	}


	void setRuleGroups() {
		postOrder.reverse();

		for (const int& i : postOrder) {
			set<int> ruleGroup;
			ruleGroup = setRuleGroupsRecursor(i, ruleGroup);

			if (!ruleGroup.empty()) {
				ruleGroups.push_back(ruleGroup);
			}
		}
	}


	set<int> setRuleGroupsRecursor(const int& index, set<int> ruleGroup) {
		if (!ifVisited[index]) {
			ifVisited[index] = true;

			for (const int& dependency : edges[index]) {
				ruleGroup = setRuleGroupsRecursor(dependency, ruleGroup);
			}

			ruleGroup.insert(index);
		}

		return ruleGroup;
	}


	vector<set<int>> getRuleGroups() const {
		return ruleGroups;
	}



	friend ostream& operator<< (ostream& out, const Graph& graph) {			// Overloads the insertion operator to return a summary of the graph
		out << graph.toString();
		return out;
	};


	string toString() const {														// Returns a summary of the graph
		stringstream out;

		for (pair<int, set<int>> edge : edges) {
			out << "R" << edge.first << ":";
			
			size_t index = 0;
			for (int dependency : edge.second) {
				out << "R" << dependency;
				if (index != edge.second.size() - 1) {
					out << ",";
				}
				index++;
			}
			out << endl;
		}

		return out.str();
	}
};
#endif