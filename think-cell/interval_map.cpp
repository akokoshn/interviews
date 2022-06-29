#include <iostream>
#include <cstdio>
#include <map>

template<typename K, typename V>
class interval_map {
    friend void IntervalMapTest();
    V m_valBegin;
    std::map<K, V> m_map;
public:
    interval_map(V const& val): m_valBegin(val) {}
    void assign(K const& keyBegin, K const& keyEnd, V const& val) {
        if (!(keyBegin < keyEnd)) {
            return;
        }
        typename std::map<K, V>::iterator start_it = m_map.lower_bound(keyBegin);// time = O(log(n))
        while (start_it != m_map.end() && start_it->first < keyEnd) {// an interval inside new interval, time = O(n*log(n))
            m_map.erase(start_it);
            start_it = m_map.lower_bound(keyBegin);
        }
        // time = 2*O(log(n))
        if (start_it != m_map.end() && keyBegin < start_it->first && start_it->second == m_valBegin) {// insert new interval inside another
            start_it--;
            m_map.insert(std::pair<K, V>(keyEnd, start_it->second));
        } else {
            m_map.insert(std::pair<K, V>(keyEnd, m_valBegin));
        }
        m_map.insert(std::pair<K, V>(keyBegin, val));

    }
    V const& operator[](K const& key) const {
        typename std::map<K, V>::const_iterator it = m_map.upper_bound(key);
        if (it == m_map.begin()) {
            return m_valBegin;
        } else {
            return (--it)->second;
        }
    }
};

void IntervalMapTest() {
    printf("Test\n");
    interval_map<int, int> my_map(1);
    my_map.assign(3, 5, 2);
    my_map.assign(4, 8, 3);
    my_map.assign(5, 6, 4);
    for (int i = -1; i < 10; i++) {
        printf("%d: %d\n", i, my_map[i]);
    }
};
