#include <unordered_map>
#include <unordered_set>
#include <list>
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
/*
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
*/

class Constant {
private:
    // Appart from value we also store the iterator to the corresponding current bucket.
    struct Value {
        unsigned int value;
        list<pair<unsigned int, unordered_set<string>>>::iterator bucket;
    };
    // List of buckets: value + set of keys with this value.
    // We keep this list sorted by value.
    list<pair<unsigned int, unordered_set<string>>> _buckets;
    // Map from value to count of keys with that value.
    unordered_map<unsigned int, unsigned int> _counts;
    // Map from key to Value.
    unordered_map<string, Value> _values;
public:
    void create_element(const string& key) {
        if (_buckets.size() == 0 || _buckets.front().first != 1) {
            _buckets.push_front(make_pair(1, unordered_set<string>{key}));
        } else {
            _buckets.front().second.insert(key);
        }
        
        auto it = _buckets.begin();
        
        _values.insert(make_pair(key, Value{1, it}));
        _counts[1]++;
    }
    
    void delete_element(unordered_map<string, Value>::iterator it) {
        _counts[1]--;
        
        if (_counts[1] == 0) {
            _buckets.pop_front();
        } else {
            _buckets.front().second.erase(it->first);
        }
        
        _values.erase(it);
    }
                       
    // Adjust element by +1/-1
    void adjust_element(unordered_map<string, Value>::iterator it, int adj) {
        unsigned current = it->second.value;
        unsigned next = current+adj;
        
        _counts[current]--;
        _counts[next]++;
        
        // remove the key from the current bucket
        it->second.bucket->second.erase(it->first);
        
        // no bucket change, just adjust the number in the bucket
        if (_counts[current] == 0 && _counts[next] == 1) {
            it->second.bucket->first += adj;
        }
        
        // no bucket change, we need to adjust iterator
        if (_counts[current] > 0 && _counts[next] > 1) {
            advance(it->second.bucket, adj);
        }
        
        // create a new next bucket
        if (_counts[current] > 0 && _counts[next] == 1) {
            if (adj > 0) // insert inserts before the element, if we are increasing value, we want first advance once
                advance(it->second.bucket, adj);
            it->second.bucket = _buckets.insert(it->second.bucket, make_pair(next, unordered_set<string>{it->first}));
        }
        
        // delete the current bucket
        if (_counts[current] == 0 && _counts[next] > 1) {
            it->second.bucket = _buckets.erase(it->second.bucket);
            if (adj < 0) // erase returns the iterator after the removed element, if we decreasing the value, we need to go one back
                advance(it->second.bucket, adj);
        }
        
        // add the bucket to the next bucket
        it->second.bucket->second.insert(it->first);
        it->second.value = next;
    }
                       
    /** Inserts a new key <Key> with value 1. Or increments an existing key by 1. */
    void inc(const string& key) {
        auto it = _values.find(key);
        if (it == _values.end()) {
            create_element(key);
        } else {
            adjust_element(it, 1);
        }
    }
    
    /** Decrements an existing key by 1. If Key's value is 1, remove it from the data structure. */
    void dec(const string& key) {
        auto it = _values.find(key);
        if (it == _values.end())
            return;
        
        if (it->second.value == 1) {
            delete_element(it);
        } else {
            adjust_element(it, -1);
        }
    }
    
    /** Returns one of the keys with maximal value. */
    string getMaxKey() const {
        if (_buckets.empty())
            return string("");
        return *_buckets.back().second.begin();
    }
    
    /** Returns one of the keys with Minimal value. */
    string getMinKey() const {
        if (_buckets.empty())
            return string("");
        return *_buckets.front().second.begin();
    }
};

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
