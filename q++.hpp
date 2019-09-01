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

class Qubits {
public:
	complex<double> *elem;
	int q_size;
	int bits;
	bool isCompile;

	Qubits(int val, int size, bool compile) : q_size(pow(2, size)), bits(size), isCompile(compile) {
		if (compile) {
			printf("OPENQASM 2.0;\n");
			printf("include \"qelib1.inc\";\n");
			printf("qreg q[%d];\n", size);
			printf("creg c[%d];\n", size);
		}else{
			elem = new complex<double>[q_size];
			for (int i = 0;i < size;i++) {
				elem[i] = complex<double>(0.0, 0.0);
			}
			elem[val] = complex<double>(1.0, 0.0);
		}
	}

	void print() {
		if (!isCompile) {
			printf("-----Qubits-----\n");
			for (int i = 0;i < q_size;i++) {
				printf("|%s> %f + %fi\n", btoS(bits, i).c_str(), elem[i].real(), elem[i].imag());
			}
		}
	}

	void print_s() {
		if (!isCompile) {
			printf("-----Qubits-----\n");
			for (int i = 0;i < q_size;i++) {
				if (abs(elem[i]) != 0)
					printf("|%s> %f + %fi\n", btoS(bits, i).c_str(), elem[i].real(), elem[i].imag());
			}
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
		if (isCompile)
			printf("ccx q[%d], q[%d], q[%d];\n", control1, control2, target);
		else{
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
	}

	void CNOT(int control, int target) {
		if (isCompile)
			printf("cx q[%d], q[%d];\n", control, target);
		else
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
		SetEx(0);
	}

	void H(int index) {
		if (isCompile)
			printf("h q[%d];\n", index);
		else{
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
	}

	void X(int index) {
		if (isCompile)
			printf("x q[%d];\n", index);
		else{
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
	}

	void Y(int index) {
		if (isCompile)
			printf("y q[%d];\n", index);
		else{
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
	}

	void Z(int index) {
		if (isCompile)
			printf("z q[%d];\n", index);
		else{
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
	}

	void Rx(int bit, double theta) {
		if (isCompile)
			printf("rx(%f) q[%d];\n", theta, bit);
		else{
			int bitmask = (1 << bit);

			for (int i = 0;i < q_size;i++) {
				if (i & bitmask) {
					elem[i] = elem[i] * complex<double>(cos(theta), sin(theta));
				}
			}
		}
	}

	void CRx(int c_bit, int t_bit, double theta) {
		if (isCompile)
			printf("cu1(%f) q[%d], q[%d];\n", theta, c_bit, t_bit);
		else{
			int cbitmask = 1 << c_bit;
			for (int i = 0;i < q_size;i++) {
				if ((i & cbitmask) == cbitmask) {
					Rx(t_bit, theta);
				}
			}
		}
	}

	int M(int index) {
		if (isCompile)
			printf("measure q[%d] -> c[%d];\n", index, index);
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
		if (isCompile)
			for (int i = 0;i < bits;i++)
				printf("measure q[%d] -> c[%d];\n", i, i);
		else {
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
		return -1;
	}


	// extended functions
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

	void HalfAdder(int a, int b, int c){
		CCNOT(a, b, c);
		CNOT (b, a);
	}

	void FullAdder(int a, int b, int c0, int c1) {
		CCNOT(a, b, c1);
		CNOT (a, b);
		CCNOT(b, c0, c1);
		CNOT (b, c0);
	}

	void HalfSubtractor(int a, int b, int c){
		CNOT (b, a);
		CCNOT(a, b, c);
	}

	void FullSubtractor(int a, int b, int c0, int c1) {
		CCNOT(a, b, c1);
		CNOT (a, b);
		CCNOT(b, c0, c1);
		CNOT (b, c0);
	}
};
