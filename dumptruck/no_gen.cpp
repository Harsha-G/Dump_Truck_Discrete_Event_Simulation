#include <ctime>
#include "no_gen.h"

no_gen::no_gen(vector<int> *return_val_, vector<double> *prob_) {
	return_val = return_val_;
	prob = prob_;
}

int no_gen::ret_rval(float seed) {
	
	srand(seed);
	float r_num = (rand() % 100)/100;

	for (int i = 0; i < prob->size(); i++) {
		int temp = i + 1;
		if (temp >= prob->size()) {
			return return_val->at(i);
		}
		if (r_num < prob->at(temp) && r_num >= prob->at(i)) {
			return return_val->at(i);
		}
	}

}