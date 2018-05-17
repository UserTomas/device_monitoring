
// Returns a set of acceleration and raw magnetic readings from the cmp01a.
void read_data_raw(vector *a, vector *m);

// Returns a set of acceleration and adjusted magnetic readings from the cmp01a.
void read_data(vector *a, vector *m);

// Returns a heading (in degrees) given an acceleration vector a due to gravity, a magnetic vector m, and a facing vector p.
int get_heading(const vector *a, const vector *m, const vector *p);
void start();
