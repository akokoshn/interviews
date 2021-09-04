"""
Research should answer on the follow question: does it make sense to emble new solution?
In anothe words cost of deliveries in some period (let it be June) with using local storages lower than only with main warehouse,
also need to rember about cost of implementation of the local storages (how many, how big each of them, etc).
For find best implementation need to answer onthe follow questions:
        1. How many local small storages need (may be 0, which means use just main warehouse)
        2. Where they should be located

Criteria of the best implementation is sum cost of all deliveries in June.
I have define cost of one delivery as
    distance (from main warehouse to the local storage + from the local storage to the final destination) +
    time of keeping in the local storage (it's may be important for define size of local sorage and affect final cost of the new solution)

I sugest to use math method - global optimization of the function f(x),
where f(x) = sum cost of all June deliveries,
x - vector of location small storages: {(storage1.lat, storage1.lon), ...}.
    we can't locate small starage at any place, so jut do it in the nearest applicable place
    if found best location of storageN near the main warehouse, no need this storage, so fo init search procees need only limit the maximum number of the small storages

Follow code just simple example of data model and optimization with gradient descent.

Gradient Descent is not the best method, I use it just for quick example. Another global function optimization algorithm more applicabe for this case.

"""
#!/usr/bin/env python

import sys
import argparse
import csv
from math import sqrt, radians, cos, sin, asin
import copy
import random

class Model():
    def __init__(self, data, state, center):
        self.data = list()
        for delivery in data:
            try:
                self.data.append([float(delivery[0]), float(delivery[1]), delivery[2]])
            except:
                pass
        self.state = state
        self.center = center

    def dist(self, p0, p1):
        return abs(p0[0] - p1[0]) + abs(p0[1] - p1[1])
        """
        Calculate the great circle distance between two points
        on the earth (specified in decimal degrees)
        """
        # convert decimal degrees to radians
        lon1, lat1, lon2, lat2 = map(radians, [p0[1], p0[0], p1[1], p1[0]])
        # haversine formula
        dlon = lon2 - lon1
        dlat = lat2 - lat1
        a = sin(dlat/2)**2 + cos(lat1) * cos(lat2) * sin(dlon/2)**2
        c = 2 * asin(sqrt(a))
        # Radius of earth in kilometers is 6371
        km = 6371* c
        return km

    def dayly_store_cost(self, mid):
        """
        Define cost of store one delivery in small storage based on distance fron the main storage,
        it's 0 for the main storage
        """
        return min(self.dist(mid, self.center) * 10, 200)

    def find_nearest(self, p, new_state = None):
        if new_state is None:
            nearest = min(self.state, key = lambda x: self.dist(p, x))
            idx = self.state.index(nearest)
        else:
            nearest = min(new_state, key = lambda x: self.dist(p, x))
            idx = new_state.index(nearest)
        return (nearest, idx)

    def cost(self, delivery, new_state = None):
        (mid, mid_idx) = self.find_nearest([delivery[0], delivery[1]], new_state)
        store_days = int(delivery[2].split('-')[-1])
        return (self.dist(mid, self.center) +
                self.dist(mid, [delivery[0], delivery[1]]) +
                store_days * self.dayly_store_cost(mid))

    def total_cost(self, new_state = None):
        return sum([self.cost(x, new_state) for x in self.data])

    def next(self, diff):
        i = 0
        for p in self.state:
            p[0] = p[0] + diff[i][0]
            p[1] = p[1] + diff[i][1]
            i += 1

    def new_state(self, diff):
        new_state = copy.deepcopy(self.state)
        i = 0
        for p in new_state:
            p[0] = p[0] + diff[i][0]
            p[1] = p[1] + diff[i][1]
            i += 1
        return new_state

    def delta(self, new_state):
        res = list()
        i = 0
        for e0 in new_state:
            res.append([abs(e0[0] - self.state[i][0]), abs(e0[1] - self.state[i][1])])
            i += 1
        return res

    def gradient(self):
        res = list()
        new_state = self.new_state([[random.uniform(-0.1, 0.1), random.uniform(-0.1, 0.1)] for x in self.state])
        dy = (self.total_cost(new_state) - self.total_cost())
        delta = self.delta(new_state)
        res = [[dy / x[0], dy / x[1]] for x in delta]
        return res


def gradient_descent(model, learn_rate = 0.01, n_iter = 10, tolerance = 1.0):
    for _ in range(n_iter):
        g = model.gradient()
        diff = [[-learn_rate * x[0], -learn_rate * x[1]] for x in g]
        diff_abs = sum([abs(x[0]) + abs(x[1]) for x in diff])
        if diff_abs < tolerance:
            return _
        model.next(diff)
    return n_iter

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', required=True)
    args = parser.parse_args()

    with open(args.input, newline='') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        data = list(reader)
        center = [32.0, 32.0]
        init_satate = [[32.0, 32.0], [32.0, 32.0], [32.0, 32.0]]
        model = Model([data[1]], init_satate, center)
        print("Search iterations = " + str(gradient_descent(model)))
        print("Locations of the local storages:")
        print(model.state)
