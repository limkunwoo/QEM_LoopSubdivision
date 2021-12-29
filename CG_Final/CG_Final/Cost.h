#pragma once
#include "Edge.h"

class Cost {
public:
	Vec3<double> _newPos;
	Edge* _edge;
	double _cost;
public:
	Cost() {}
	Cost(Vec3<double> newPos, Edge* edge, double cost) {
		_newPos = newPos;
		_edge = edge;
		_cost = cost;
	}
	bool operator>(const Cost& c) const {
		if (_cost == c._cost) {
			return _edge->_index > c._edge->_index;
		}
		return _cost > c._cost;
	}
	bool operator>=(const Cost& c) const {
		if (_cost == c._cost) {
			return _edge->_index >= c._edge->_index;
		}
		return _cost >= c._cost;
	}
	bool operator<(const Cost& c) const {
		if (_cost == c._cost) {
			return _edge->_index < c._edge->_index;
		}
		return _cost < c._cost;
	}
	bool operator<=(const Cost& c) const {
		if (_cost == c._cost) {
			return _edge->_index <= c._edge->_index;
		}
		return _cost <= c._cost;
	}
	bool operator== (const Cost& c) const {
		if ((_edge)->_index == (c._edge)->_index)
			return true;
		return false;
	}
	bool operator!= (const Cost& c) const {
		if (_cost != c._cost)
			return true;
		return false;
	}
};