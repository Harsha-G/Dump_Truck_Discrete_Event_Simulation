#include "Header.h"

using namespace std;

int main() {
	srand(time(NULL));
	vector<int> loading_time{ 5,10,15 };
	vector<double> cumulative_prob_lt{ 0.0, 0.30, 0.80, 1.00 };

	vector<int> weighing_time{ 12,16 };
	vector<double> cumulative_prob_wt{ 0.0, 0.70, 1.00 };

	vector<int> travel_time{ 40, 60, 80, 100 };
	vector<double> cumulative_prob_tt{ 0.0, 0.40, 0.7, 0.9, 1.00 };

	no_gen lt(&loading_time, &cumulative_prob_lt);
	no_gen wt(&weighing_time, &cumulative_prob_wt);
	no_gen tt(&travel_time, &cumulative_prob_tt);

	string s = ("C://Users//hgstr//Desktop//dumptruck_DES.txt");

	//simulation s1(&l_t, &w_t, &t_t, s);
	simulation s1(&lt, &wt, &tt, s);
	s1.begin_simulation();
}