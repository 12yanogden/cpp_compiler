#ifndef HEADER_H
#define HEADER_H
#include <vector>
#include <string>
#include <sstream>
#include <set>
using namespace std;

class Header {

private:
	vector<string> attributes;

public:
	Header() {}																// Default Constructor

	Header(const vector<string>& attributes) {								// Constructor
		if (!attributes.empty()) {
			this->attributes = attributes;
		}
	}


	Header(const vector<pair<int, int>>& dupIndexes, const Header& alpha, const Header& beta) {		// Constructor, used to join
		attributes = alpha.getAttributes();
		const vector<string>& betaAttributes = beta.getAttributes();

		for (size_t i = 0; i < betaAttributes.size(); i++) {
			if (!ifDupIndex(dupIndexes, i)) {
				attributes.push_back(betaAttributes[i]);
			}
		}
	}


	~Header() {}															// Destructor


	bool ifDupIndex(const vector<pair<int, int>>& dupIndexes, int index) const {
		bool ifDupIndex = false;

		for (size_t i = 0; i < dupIndexes.size(); i++) {
			if (index == dupIndexes[i].second) {
				ifDupIndex = true;
			}
		}

		return ifDupIndex;
	}


	void addAttribute(const string& attribute) {
		attributes.push_back(attribute);
	}


	string getAttributeByIndex(const int& index) const {					// Returns the attribute at a given index
		return attributes[index];
	}


	int getIndexByAttribute(const string& attribute) const {				// Returns the index of a given attribute
		int index = -1;

		for (size_t i = 0; i < attributes.size(); i++) {
			if (attributes[i] == attribute) {
				index = i;
			}
		}

		return index;
	}


	vector<string> getAttributes() const {									// Returns a vector of all attributes
		return attributes;
	}


	Header project(const vector<int>& projectIndexes) const {
		Header header;

		for (size_t i = 0; i < projectIndexes.size(); i++) {
			header.addAttribute(attributes[projectIndexes[i]]);
		}

		return header;
	}


	friend ostream& operator<< (ostream& out, Header& header) {				// Overloads the insertion operator to return a summary of the header
		out << header.toString();
		return out;
	};


	string toString() const {												// Returns a summary of the header
		stringstream out;

		return out.str();
	}
};
#endif