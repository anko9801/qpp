#include <iostream>
#include <utility>
#include <cmath>
#include <cstdlib>
#include <complex>

using namespace std;

class Qubits {
public:
	complex<double> *elem;
	int q_size;

	Qubits(int val, int size) : q_size(pow(2, size)) {
		elem = new complex<double>[q_size];
		for (int i = 0;i < size;i++) {
			elem[i] = complex<double>(0.0, 0.0);
		}
		elem[val] = complex<double>(1.0, 0.0);
	}

	~Qubits() {
		delete[] elem;
	}

	void CCNOT(int control1, int control2, int target) {
		complex<double> *new_comp = new complex<double>[q_size];

		int c_bitmask = (1 << control1) | (1 << control2);
		int t_bitmask = (1 << target);
		for (int i = 0;i < q_size;i++) {
			int act_index = i;
			if ((i & c_bitmask) == c_bitmask) {
				act_index = i ^ t_bitmask;
			}
			new_comp[act_index] = elem[i];
		}
		for (int i = 0;i < q_size;i++) {
			elem[i] = new_comp[i];
		}

		delete[] new_comp;
	}

	void CNOT(int control, int target) {
		CCNOT(control, control, target);
	}

	void SetZero(int index) {
		for (int i = 0;i < q_size;i++) {
			elem[i] = complex<double>(0.0, 0.0);
		}
		elem[index] = complex<double>(1.0, 0.0);
	}

	void H(int index) {
		int bitmask = (1 << index);

		for (int i = 0;i < q_size;i++) {
			if (i & bitmask) {
				elem[i ^ bitmask] = sqrt(2) * elem[i] / 2.0;
				elem[i]			  = - sqrt(2) * elem[i] / 2.0;
			}else{
				elem[i]			  = sqrt(2) * elem[i] / 2.0;
				elem[i ^ bitmask] = sqrt(2) * elem[i] / 2.0;
			}
		}
	}

	void X(int index) {
		int bitmask = (1 << index);

		for (int i = 0;i < q_size;i++) {
			if ((i & bitmask) == bitmask) {
				swap(elem[i], elem[i ^ bitmask]);
			}
		}
	}

	void Y(int index) {
		int bitmask = (1 << index);

		for (int i = 0;i < q_size;i++) {
			if ((i & bitmask) == bitmask) {
				swap(elem[i], elem[i ^ bitmask]);
				elem[i]			  = elem[i] * complex<double>(0.0, 1.0);
				elem[i ^ bitmask] = elem[i ^ bitmask] * complex<double>(0.0, -1.0);
			}
		}
	}

	void Z(int index) {
		int bitmask = (1 << index);

		for (int i = 0;i < q_size;i++) {
			if ((i & bitmask) == bitmask) {
				elem[i ^ bitmask] = elem[i ^ bitmask] * complex<double>(-1.0, 0.0);
			}
		}
	}

	void Rx(int index, double theta) {
		int bitmask = (1 << index);

		for (int i = 0;i < q_size;i++) {
			if (i & bitmask) {
				elem[i] = sqrt(2) * elem[i] * complex<double>(cos(theta), sin(theta)) / 2.0;
			}else{
				elem[i] = sqrt(2) * elem[i] / 2.0;
			}
		}
	}

	int M() {
		double rand_val = rand() / static_cast<double>(RAND_MAX);
		double cur_val = 0.0;
		int decide = -1;

		for (int i = 0;i < q_size;i++) {
			cur_val += pow(abs(elem[i]), 2.0);
			if (rand_val < cur_val) {
				elem[i] = 1.0;
			}else{
				elem[i] = 0.0;
				decide = i;
			}
		}

		return decide+1;
	}

	void SWAP(int a, int b) {
		CNOT(a, b);
		CNOT(b, a);
		CNOT(a, b);
	}
};
