#include "Header.h"
int mrker_l, mrker_ln, mrker_w;

int calc_sum_vec(vector<int> *v) {
	int sum = 0;
	for (int i = 0; i < v->size(); i++) {
		sum = sum + v->at(i);
	}
	return sum;
}

int calc_sum_criteria(vector<int> *v, int no) {
	int sum = 0;
	for (int i = 0; i < v->size(); i++) {
		if (v->at(i) >= no) {
			sum = sum + v->at(i);
		}
	}
	return sum;
}

int count_vector_criteria(vector<int> *v, int no) {
	int cnt = 0;
	for (int i = 0; i < v->size(); i++) {
		if (v->at(i) >= no) {
			cnt++;
		}
	}
	return cnt;
}

string return_str_vec(deque<char*> v) {
	char bf[256];

	string str = "";
	for (int i = 0; i < v.size(); i++) {
		sprintf_s(bf, "%s, ", v[i]);
		str += bf;
	}
	return str;
}

simulation::simulation(vector<int> *l_t_, vector<int> *w_t_, vector<int> *t_t_, string s) {
	t = lq = l = wq = w = 0;

	bl = bs = 0; 

	lt_cntr = wt_cntr = tt_cntr = 0;

	l_t = l_t_;
	w_t = w_t_;
	t_t = t_t_;

	bl_c = false;

	o_file.open(s);

	evnt_lst = new fel();
	srvc_lst = new fel();
}

simulation::simulation(no_gen *lt_, no_gen *wt_, no_gen *tt_, string s_) {
	t = lq = l = wq = w = 0;

	bl = bs = 0;

	lt_cntr = wt_cntr = tt_cntr = 0;

	lt = lt_;
	wt = wt_;
	tt = tt_;

	departures = 0;

	bl_c = false;

	o_file.open(s_);

	evnt_lst = new fel();
	srvc_lst = new fel();


}

void simulation::set_defaults() {
	lq = 3;
	l = 2;
	bl_c = true;
	mrker_l = 0;
	mrker_ln = l;
	wq = 0;
	w = 1;
	bl_w = true;
	mrker_w = 0;

	loader_q.push_back("DT4");
	loader_q.push_back("DT5");
	loader_q.push_back("DT6");

	

	evnt_lst->fel_insert(spawn_event("EL", lt->ret_rval(time(NULL)), "DT3"), *evnt_lst->return_peek());
	lt_cntr++;
	evnt_lst->fel_insert(spawn_event("EL", lt->ret_rval(time(NULL)), "DT2"), *evnt_lst->return_peek());
	lt_cntr++;
	evnt_lst->fel_insert(spawn_event("EW", wt->ret_rval(time(NULL)), "DT1"), *evnt_lst->return_peek());
	wt_cntr++;

	srvc_lst->fel_insert(spawn_event("DT1", 0), *srvc_lst->return_peek());
	srvc_lst->fel_insert(spawn_event("DT6", 0), *srvc_lst->return_peek());
	srvc_lst->fel_insert(spawn_event("DT5", 0), *srvc_lst->return_peek());
	srvc_lst->fel_insert(spawn_event("DT4", 0), *srvc_lst->return_peek());
	srvc_lst->fel_insert(spawn_event("DT2", 0), *srvc_lst->return_peek());
	srvc_lst->fel_insert(spawn_event("DT3", 0), *srvc_lst->return_peek());

	output_to_file(t, o_file);

	//o_file << t << setw(5) << lq << setw(10) << l << setw(10) << wq << setw(10) << w << setw(30)  << setw(30)<< evnt_lst->return_fel_str().c_str() << setw(20);
}

void simulation::begin_simulation() {
	set_defaults();

	if (strcmp(evnt_lst->get_peek()->evnt, "ALQ") == 0) {
		sim_alq(evnt_lst->get_peek()->time, o_file);
	}
	else if (strcmp(evnt_lst->get_peek()->evnt, "EW") == 0) {
		sim_ew(evnt_lst->get_peek()->time, o_file);
	}
	else if (strcmp(evnt_lst->get_peek()->evnt, "EL") == 0) {
		sim_el(evnt_lst->get_peek()->time, o_file);
	}

}


void simulation::sim_alq(int t, ofstream &o_file_) {

	if (t > 100) {
		o_file << "\n\n";
		o_file << "Average Loader Utilization: " << (bl*1.0 / 2) / (100 * 1.0) << endl;
		o_file << "Average Scale Utilization: " << (bs*1.0) / (100 * 1.0) << endl;
		o_file << "\n\n";
		o_file << "Average Response Times: " << calc_sum_vec(&response_times) / response_times.size() << endl;
		o_file << "Proportion of Customers with 30 minutes+ Response Times: " << (count_vector_criteria(&response_times, 30)*1.0)/ (departures*1.0);
		return;
	}

	if (bl_c == true) {
		bl = bl + (t - mrker_l)*l;
	}

	if (bl_w == true) {
		bs = bs + (t - mrker_w)*w;
	}

	if (l < 2) {
		l++;
		evnt_lst->fel_insert(spawn_event("EL", lt->ret_rval(time(NULL)) + t, evnt_lst->get_peek()->cli), *evnt_lst->return_peek());
		lt_cntr++;
	}
	else {
		lq++;
		loader_q.push_back(evnt_lst->get_peek()->cli);
	}

	srvc_lst->fel_insert(spawn_event(evnt_lst->get_peek()->evnt, t), *srvc_lst->return_peek());

	if (evnt_lst->get_peek() != NULL) {
		evnt_lst->pop_peek();
	}


	output_to_file(t, o_file);

	if (l > 0) {
		bl_c = true;
		mrker_l = t;
	}
	else bl_c = false;

	if (w > 0) {
		bl_w = true;
		mrker_w = t;
	}
	else bl_w = false;



	if (strcmp(evnt_lst->get_peek()->evnt, "ALQ") == 0) {
		sim_alq(evnt_lst->get_peek()->time, o_file);
	}
	else if (strcmp(evnt_lst->get_peek()->evnt, "EW") == 0) {
		sim_ew(evnt_lst->get_peek()->time, o_file);
	}
	else if (strcmp(evnt_lst->get_peek()->evnt, "EL") == 0) {
		sim_el(evnt_lst->get_peek()->time, o_file);
	}

}

void simulation::sim_el(int t, ofstream &o_file_) {

	if (t > 100) {
		o_file << "\n\n";
		o_file << "Average Loader Utilization: " << (bl*1.0 / 2) / (100 * 1.0) << endl;
		o_file << "Average Scale Utilization: " << (bs*1.0) / (100 * 1.0) << endl;
		o_file << "\n\n";
		o_file << "Average Response Times: " << calc_sum_vec(&response_times) / response_times.size() << endl;
		o_file << "Proportion of Customers with 30 minutes+ Response Times: " << (count_vector_criteria(&response_times, 30)*1.0) / (departures*1.0);
		return;
	}

	if (bl_c == true) {
		bl = bl + (t - mrker_l)*l;
	}

	if (bl_w == true) {
		bs = bs + (t - mrker_w)*w;
	}

	if (lq > 0) {
		lq--;
		evnt_lst->fel_insert(spawn_event("EL", lt->ret_rval(time(NULL)) + t, loader_q.front()), *evnt_lst->return_peek());
		lt_cntr++;
		loader_q.pop_front();
	}
	else l --;

	//Transitioning to the weighing section
	if (w == 0) {
		w = 1;
		evnt_lst->fel_insert(spawn_event("EW", wt->ret_rval(time(NULL)) + t, evnt_lst->get_peek()->cli), *evnt_lst->return_peek());
		wt_cntr++;
	}
	else {
		wq++;
		weight_q.push_back(evnt_lst->get_peek()->cli);
	}

	if (evnt_lst->get_peek() != NULL) {
		evnt_lst->pop_peek();
	}

	output_to_file(t, o_file);

	if (l > 0) {
		bl_c = true;
		mrker_l = t;
	}
	else bl_c = false;

	if (w > 0) {
		bl_w = true;
		mrker_w = t;
	}
	else bl_w = false;
	

	if (strcmp(evnt_lst->get_peek()->evnt, "ALQ") == 0) {
		sim_alq(evnt_lst->get_peek()->time, o_file);
	}
	else if (strcmp(evnt_lst->get_peek()->evnt, "EW") == 0) {
		sim_ew(evnt_lst->get_peek()->time, o_file);
	}
	else if (strcmp(evnt_lst->get_peek()->evnt, "EL") == 0) {
		sim_el(evnt_lst->get_peek()->time, o_file);
	}

}

void simulation::sim_ew(int t, ofstream &o_file_) {

	if (t > 100) {
		o_file << "\n\n";
		o_file << "Average Loader Utilization: " << (bl*1.0 / 2) / (100 * 1.0) << endl;
		o_file << "Average Scale Utilization: " << (bs*1.0) / (100 * 1.0) << endl;
		o_file << "\n\n";
		o_file << "Average Response Times: " << calc_sum_vec(&response_times) / response_times.size() << endl;
		o_file << "Proportion of Customers with 30 minutes+ Response Times: " << (count_vector_criteria(&response_times, 30)*1.0) / (departures*1.0);
		return;
	}

	if (bl_c == true) {
		bl = bl + (t - mrker_l)*l;
	}

	if (bl_w == true) {
		bs = bs + (t - mrker_w)*w;
	}

	if (wq > 0) {
		wq--;
		evnt_lst->fel_insert(spawn_event("EW", wt->ret_rval(time(NULL)) + t, weight_q.front()), *evnt_lst->return_peek());
		wt_cntr++;
		weight_q.pop_front();
	}
	else w = 0;

	evnt_lst->fel_insert(spawn_event("ALQ", tt->ret_rval(time(NULL)) + t, evnt_lst->get_peek()->cli), *evnt_lst->return_peek());
	tt_cntr++;

	response_times.push_back(evnt_lst->get_peek()->time - srvc_lst->get_peek()->time);

	departures++;

	if (evnt_lst->get_peek() != NULL) {
		evnt_lst->pop_peek();
	}

	if (srvc_lst->get_peek() != NULL) {
		srvc_lst->pop_peek();
	}

	output_to_file(t, o_file);

	if (l > 0) {
		bl_c = true;
		mrker_l = t;
	}
	else bl_c = false;

	if (w > 0) {
		bl_w = true;
		mrker_w = t;
	}
	else bl_w = false;

	if (strcmp(evnt_lst->get_peek()->evnt, "ALQ") == 0) {
		sim_alq(evnt_lst->get_peek()->time, o_file);
	}
	else if (strcmp(evnt_lst->get_peek()->evnt, "EW") == 0) {
		sim_ew(evnt_lst->get_peek()->time, o_file);
	}
	else if (strcmp(evnt_lst->get_peek()->evnt, "EL") == 0) {
		sim_el(evnt_lst->get_peek()->time, o_file);
	}

}

void simulation::output_to_file(int t_ ,ofstream &o_file_) {
	o_file << t_ << setw(5) << lq << setw(10) << l << setw(10) << wq << setw(10) << w<< setw(20)  << bl << setw(20)<<bs<<setw(20)<< return_str_vec(loader_q) << setw(20) << return_str_vec(weight_q)<< "\t\t" <<setw(30)<< evnt_lst->return_fel_str().c_str()<<endl;

}