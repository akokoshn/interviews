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

		std::pair<K,V> prev_begin;
    	std::pair<K,V> new_end;
		bool need_prev_begin = false;
	    bool need_new_end = false;

        typename std::map<K,V>::iterator it_begin;
        it_begin = m_map.lower_bound(keyBegin);
        if (it_begin != m_map.end() && keyBegin < it_begin->first) {
            if (it_begin != m_map.begin()) {
                need_prev_begin = true;
                --it_begin;
                prev_begin = std::make_pair(it_begin->first, it_begin->second);
            }
        }

        typename std::map<K,V>::iterator it_end;
        it_end = m_map.lower_bound(keyEnd);
        if (it_end != m_map.end() && keyEnd < it_end->first) {
            need_new_end = true;
            typename std::map<K,V>::const_iterator it = it_end;
            --it;
            new_end = std::make_pair(keyEnd, it->second);
        }

        bool need_new_begin = true;
        if (need_prev_begin) {
            if (prev_begin.second == val)
                need_new_begin = false;
        } else {
            if (it_begin != m_map.begin()) {
                typename std::map<K,V>::const_iterator it = it_begin;
                --it;
                if (it->second == val)
                    need_prev_begin = false;
            }
        }


        if (need_new_end) {
            if ((need_new_begin && new_end.second == val) || (!need_new_begin && new_end.second == prev_begin.second) )
                need_new_end = false;
        } else if ((need_new_begin && it_end!=m_map.end() && it_end->second == val) ||
                   (!need_new_begin && it_end!=m_map.end() && it_end->second == prev_begin.second)) {
                m_map.erase(it_end);
        }

	    m_map.erase(it_begin, it_end);
		it_begin = it_end;

        // restore prev
     	if (need_prev_begin) {
		    it_begin = m_map.insert(it_begin, prev_begin);
		}
		// insert new
		if (need_new_begin) {
			m_map.insert(it_begin, std::make_pair(keyBegin, val));
		}
		// restore next
		if (need_new_end) {
		    m_map.insert(it_begin, new_end);
		}
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
