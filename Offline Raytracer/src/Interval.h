#pragma once

#include "Utils.h"


class Interval
{
public:
	double min, max;

	Interval() : min(+infinity), max(-infinity) {}

	Interval(double _min, double _max) : min(_min), max(_max) {}

	bool Contains(double x) const
	{
		return min <= x && x <= max;
	}

	bool Surrounds(double x) const
	{
		return min < x && x < max;
	}

	static const Interval empty, universe;
};

const static Interval empty(+infinity, -infinity);
const static Interval universe(-infinity, +infinity);