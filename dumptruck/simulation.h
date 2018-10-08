#pragma once

using namespace std;

class simulation {
	int t;
	int  lq, l, wq, w, bl, bs;
	int lt_cntr, wt_cntr, tt_cntr;
	int departures;

	bool bl_c, bl_w;

	vector<int> *l_t, *w_t, *t_t;

	vector<int> response_times;

	deque<char*> loader_q, weight_q;

	fel *evnt_lst, *srvc_lst;

	no_gen *lt, *wt, *tt;

	
public:
	ofstream o_file;
	simulation(vector<int> *l_t_, vector<int> *w_t_, vector<int> *t_t_ , string s_);
	simulation(no_gen *lt_, no_gen *wt_, no_gen *tt_, string s_);
	void set_defaults();
	void begin_simulation();
	void sim_alq(int t, ofstream &o_file_);
	void sim_el(int t, ofstream &o_file_);
	void sim_ew(int t, ofstream &o_file_);
	void output_to_file(int t_, ofstream &o_file_);

};