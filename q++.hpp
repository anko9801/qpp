#include <iostream>
#include <utility>
#include <cmath>
#include <cstdlib>
#include <complex>
#include <random>

using namespace std;

std::random_device rnd;

string btoS(int size, int val) {
	string str;
	while (size != 0) {
		if ((val & 1) == 0) {
			str.insert(str.begin(), '0');
		}else{
			str.insert(str.begin(), '1');
		}
		val >>= 1;
		size--;
	}
	return str;
}

class Qubit {
	complex<double> zero;
	complex<double> one;
};

class Qubits {
public:
	complex<double> *elem;
	int q_size;
	int bits;

	Qubits(int val, int size) : q_size(pow(2, size)), bits(size) {
		elem = new complex<double>[q_size];
		for (int i = 0;i < size;i++) {
			elem[i] = complex<double>(0.0, 0.0);
		}
		elem[val] = complex<double>(1.0, 0.0);
	}

	/*
	~Qubits() {
		;
	}*/

	void print() {
		printf("-----Qubits-----\n");
		for (int i = 0;i < q_size;i++) {
			printf("|%s> %f + %fi\n", btoS(bits, i).c_str(), elem[i].real(), elem[i].imag());
		}
	}

	void print_s() {
		printf("-----Qubits-----\n");
		for (int i = 0;i < q_size;i++) {
			if (abs(elem[i]) != 0)
				printf("|%s> %f + %fi\n", btoS(bits, i).c_str(), elem[i].real(), elem[i].imag());
		}
	}

	void size_assert() {
		double size = 0;
		for (int i = 0;i < q_size;i++) {
			size += pow(abs(elem[i]), 2.0);
		}
		printf("size");
		if (size < 0.9 || size > 1.1) {
			printf("assert %f\n", size);
		}else{
			printf("ok\n");
		}
	}

	void ControlledX(int target, int begin, int end) {
		complex<double> *new_elem = new complex<double>[q_size];

		int c_bitmask = 0;
		for (int i = begin;i < end;i++) {
			c_bitmask |= 1 << i;
		}
		int t_bitmask = (1 << target);
		for (int i = 0;i < q_size;i++) {
			int act_index = i;
			if ((i & c_bitmask) == c_bitmask) {
				act_index = i ^ t_bitmask;
			}
			new_elem[act_index] = elem[i];
		}
		for (int i = 0;i < q_size;i++) {
			elem[i] = new_elem[i];
		}

		delete[] new_elem;
	}

	void CCNOT(int control1, int control2, int target) {
		complex<double> *new_elem = new complex<double>[q_size];

		int c_bitmask = (1 << control1) | (1 << control2);
		int t_bitmask = (1 << target);
		for (int i = 0;i < q_size;i++) {
			int act_index = i;
			if ((i & c_bitmask) == c_bitmask) {
				act_index = i ^ t_bitmask;
			}
			new_elem[act_index] = elem[i];
		}
		for (int i = 0;i < q_size;i++) {
			elem[i] = new_elem[i];
		}

		delete[] new_elem;
	}

	void CNOT(int control, int target) {
		CCNOT(control, control, target);
	}

	void Set(int index) {
		double one = 0;
		for (int i = 0;i < q_size;i++) {
			if (pow(abs(elem[i]), 2.0) > 0 || index == i)
				one++;
		}
		one = sqrt(1 / one);
		for (int i = 0;i < q_size;i++) {
			if (pow(abs(elem[i]), 2.0) > 0)
				elem[i] = complex<double>(one, 0.0);
		}
		elem[index] = complex<double>(one, 0.0);
	}

	void SetEx(int index) {
		for (int i = 0;i < q_size;i++) {
			elem[i] = complex<double>(0.0, 0.0);
		}
		elem[index] = complex<double>(1.0, 0.0);
	}

	void Reset() {
		for (int i = 0;i < q_size;i++) {
			elem[i] = complex<double>(0.0, 0.0);
		}
		elem[0] = complex<double>(1.0, 0.0);
	}

	/*Qubit get(int bit) {
		
	}*/

	void H(int index) {
		complex<double> *new_elem = new complex<double>[q_size];

		int bitmask = (1 << index);

		for (int i = 0;i < q_size;i++) {
			if ((i & bitmask) == bitmask) {
				new_elem[i ^ bitmask] += sqrt(2) * elem[i] / 2.0;
				new_elem[i]			  += - sqrt(2) * elem[i] / 2.0;
			}else{
				new_elem[i ^ bitmask] += sqrt(2) * elem[i] / 2.0;
				new_elem[i]			  += sqrt(2) * elem[i] / 2.0;
			}
		}

		for (int i = 0;i < q_size;i++)
			elem[i] = new_elem[i];
		delete[] new_elem;
	}

	void X(int index) {
		complex<double> *new_elem = new complex<double>[q_size];
		int bitmask = (1 << index);

		for (int i = 0;i < q_size;i++) {
			if ((i & bitmask) == bitmask) {
				new_elem[i] = elem[i ^ bitmask];
				new_elem[i ^ bitmask] = elem[i];
			}
		}

		for (int i = 0;i < q_size;i++)
			elem[i] = new_elem[i];
		delete[] new_elem;
	}

	void CX(int index, int value) {
		if (value == 1) {
			X(index);
		}
	}

	void CZ(int index, int value) {
		if (value == 1) {
			Z(index);
		}
	}

	void Y(int index) {
		complex<double> *new_elem = new complex<double>[q_size];
		int bitmask = (1 << index);

		for (int i = 0;i < q_size;i++) {
			if ((i & bitmask) == bitmask) {
				new_elem[i] = elem[i ^ bitmask] * complex<double>(0.0, 1.0);
				new_elem[i ^ bitmask] = elem[i] * complex<double>(0.0, -1.0);
			}
		}

		for (int i = 0;i < q_size;i++)
			elem[i] = new_elem[i];
		delete[] new_elem;
	}

	void Z(int index) {
		complex<double> *new_elem = new complex<double>[q_size];
		int bitmask = (1 << index);

		for (int i = 0;i < q_size;i++) {
			if ((i & bitmask) == bitmask) {
				new_elem[i ^ bitmask] = elem[i ^ bitmask] * complex<double>(-1.0, 0.0);
			}
		}

		for (int i = 0;i < q_size;i++)
			elem[i] = new_elem[i];
		delete[] new_elem;
	}

	void Rx(int index, double theta) {
		int bitmask = (1 << index);

		for (int i = 0;i < q_size;i++) {
			if (i & bitmask) {
				elem[i] = elem[i] * complex<double>(cos(theta), sin(theta));
			}
		}
	}

	/*
	void C(int c_bit, int = ([=]{ return 0; })) {
		int bitmask = 1 << c_bit;
		for (int i = 0;i < q_size;i++) {
			if ((i & bitmask) == bitmask) {
				
			}
		}
	}*/

	void CRx(int c_bit, int index, double theta) {
		int bitmask = 1 << c_bit;
		for (int i = 0;i < q_size;i++) {
			if ((i & bitmask) == bitmask) {
				Rx(index, theta);
			}
		}
	}

	int M(int index) {
		double rand_val = rnd() / 0xffffffff;
		int bitmask = 1 << index;

		double zero = 0, one = 0;
		for (int i = 0;i < q_size;i++) {
			if ((i & bitmask) == bitmask)
				one += pow(abs(elem[i]), 2.0);
			else
				zero += pow(abs(elem[i]), 2.0);
		}

		if (rand_val < zero) {
			zero = 1 / sqrt(zero);
			for (int i = 0;i < q_size;i++) {
				if ((i & bitmask) == bitmask)
					elem[i] = 0.0;
				else
					elem[i] *= zero;
			}
			return 0;
		}else{
			one = 1 / sqrt(one);
			for (int i = 0;i < q_size;i++) {
				if ((i & bitmask) == bitmask)
					elem[i] *= one;
				else
					elem[i] = 0.0;
			}
			return 1;
		}
	}

	int M_all() {
		double rand_val = rnd() / 0xffffffff;
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

	void CSWAP(int c, int target1, int target2) {
		CCNOT(c, target1, target2);
		CCNOT(c, target2, target1);
		CCNOT(c, target1, target2);
	}
};
