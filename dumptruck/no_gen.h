#pragma once

#include <random>
#include <vector>

using namespace std;

class no_gen {

	vector<int> *return_val;
	vector<double> *prob;

public:
	no_gen(vector<int> *return_val_, vector<double> *prob_);
	int ret_rval(float seed);
};