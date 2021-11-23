#pragma once
#include <stdio.h>

class data_describe
{

public:
	long double Memo;
	long double Data;
	unsigned long iter;
	int size_y;

	void calc_mean();
	void data_sep(long double Memo, unsigned long iter, long double Data);
};

