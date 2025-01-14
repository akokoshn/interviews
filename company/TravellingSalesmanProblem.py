"""

Use Kruskal's algorithm for find the mininum spanning tree (expect connected graph)
with modification: during check if elemnt in sorted array of edges accepteble for the current state
                   check both salesmens (if any of them can join edge)

Expected time complexity:
    1. Read input *.csv by lines + filling set of vertexes - O(n), because Python set is hash table.
       For this case number of items not so big, no expect collisions. Time complexity of "add" and "in" operations O(1)
    2. Kruskal algorithm (which includes quick sort of edges) - O(n*log(n)).
       We can say that modified algorithm has the same time complexity because check_edge() - O(1).
    Total: O(n + n * log(n))

"""
#!/usr/bin/env python

import sys
import argparse
import csv
from math import sqrt

class Salesmen():
    def __init__(self, start):
        self.path = [start]
        self.curr = start
        self.weight = 0

    def add_edge(self, edge, i):
        self.path.append(edge[i])
        self.curr = edge[i]
        self.weight += int(edge[2])

    def check_edge(self, i, data, vertexes):
        edge = data[i]
        if self.curr == edge[0]:
            try:
                vertexes.remove(edge[1])
                data.pop(i)
                self.add_edge(edge, 1)
                return False
            except:
                pass
        if self.curr == edge[1]:
            try:
                vertexes.remove(edge[0])
                data.pop(i)
                self.add_edge(edge, 0)
                return False
            except:
                pass
        return True

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', required=True)
    parser.add_argument('--num-salesmen', default=1, type=int)
    args = parser.parse_args()

    if not args.num_salesmen in [1, 2]:
        print("Wrong number of salesmens")
        sys.exit(-1)

    print("Read from " + args.input)
    with open(args.input, newline='') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        data = []
        vertexes = {"0"}
        for line in reader:
            data.append(line)
            vertexes.add(line[0])
            vertexes.add(line[1])

        num_visited = 0
        num_edges = len(data)
        num_vertexes = len(vertexes)

        salesmen0 = Salesmen(data[0][0])
        if args.num_salesmen == 2:
            salesmen1 = Salesmen(data[0][0])

        # quick sort of array O(e*log(e))
        def get_weight(edge):
            return int(edge[-1])
        data.sort(key=get_weight)

        print("Run: num_vertex = " + str(num_vertexes) + " num_edges = " + str(num_edges))
        i = 0
        # vertex 0 allways in vertexes
        while len(vertexes) > 1:
            # in case of using C++ std::list use iterator instead i (access and remove by iterator - O(1))
            is_skipped = False
            is_skipped = salesmen0.check_edge(i, data, vertexes)
            if args.num_salesmen == 2 and is_skipped:
                is_skipped = salesmen1.check_edge(i, data, vertexes)
            if is_skipped:
                i += 1
            else:
                i = 0

        print(str(salesmen0.path) + ": " + str(salesmen0.weight))
        if args.num_salesmen == 2:
            print(str(salesmen1.path) + ": " + str(salesmen1.weight))
        with open("result.csv", "w", newline='') as csvfile:
            writer = csv.writer(csvfile, delimiter=',')
            writer.writerow(salesmen0.path)
            if args.num_salesmen == 2:
                writer.writerow(salesmen1.path)

