#include <unordered_map>
#include <unordered_set>
#include <iostream>
using namespace std;

/*
class Constant {
	unordered_map<string, unsigned> _values;
public:
	void inc(const string& key);
	void dec(const string& key);
	string getMaxKey() const;
	string getMinKey() const;
};

void Constant::inc(const string& key) {
	_values[key]++;
}

void Constant::dec(const string& key) {
	_values[key]--;
	if (_values[key] == 0) {
		_values.erase(key);
	}
}

string Constant::getMaxKey() const {
	unsigned max = 0;
	string key = "";
	for (auto v : _values) {
		if (v.second > max) {
			key = v.first;
			max = v.second;
		}
	}

	return key;
}

string Constant::getMinKey() const {
	unsigned min = numeric_limits<unsigned>::max();
	string key = "";
	for (auto v : _values) {
		if (v.second < min) {
			key = v.first;
			min = v.second;
		}
	}

	return key;
}
*/

class Constant {
	unordered_map<string, unsigned> _values;
	unordered_map<unsigned, unordered_set<string>> _buckets;
	unsigned max = 0;
public:
	void inc(const string& key);
	void dec(const string& key);
	string getMaxKey() const;
	string getMinKey() const;
};

void Constant::inc(const string& key) {
	if (max == _values[key]) {
		max++;
	}

	_values[key]++;
	if (_values[key] > 1) {
		_buckets[_values[key]-1].erase(key);
	}
	_buckets[_values[key]].insert(key);
}

void Constant::dec(const string& key) {
	if (max == _values[key] && _buckets[_values[key]].size() == 1) {
		max--;
	}

	_values[key]--;
	_buckets[_values[key]+1].erase(key);
	if (_values[key] != 0) {
		_buckets[_values[key]].insert(key);
	} else {
		_values.erase(key);
	}
}

string Constant::getMaxKey() const {
	auto it = _buckets.find(max);
	if (it == _buckets.end()) return string("");
	return *it->second.begin();
}

string Constant::getMinKey() const {
	unsigned min = numeric_limits<unsigned>::max();
	string key = "";
	for (auto v : _values) {
		if (v.second < min) {
			key = v.first;
			min = v.second;
		}
	}

	return key;
}

int main() {
	Constant c;

	auto inc = [&c](const string& key) {
		c.inc(key);
		cout << "inc(\"" << key << "\") min = \"" << c.getMinKey() << "\" max = \"" << c.getMaxKey() << "\"" << endl;
	};

	auto dec = [&c](const string& key) {
		c.dec(key);
		cout << "dec(\"" << key << "\") min = \"" << c.getMinKey() << "\" max = \"" << c.getMaxKey() << "\"" << endl;

	};

	inc("a");
	inc("b");
	inc("b");
	inc("c");
	inc("c");
	inc("c");
	dec("a");
	dec("b");
	dec("b");
}
