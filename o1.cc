#include <unordered_map>
#include <iostream>
using namespace std;

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

void do_and_print_inc(const string& key) {
}

void do_and_print_dec(const string& key) {
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
