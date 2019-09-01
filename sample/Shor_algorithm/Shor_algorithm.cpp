#include "../../q++.hpp"
#include <iostream>

using namespace std;

typedef struct int2 {
	// s / r
	int r;
	int s;
} int2;

int2 renbun(double a) {
	vector<int> cont;
	while (floor(a) < 10000) {
		//printf("%f %f \n", floor(a), a - floor(a));
		cont.push_back(floor(a));
		a = 1 / (a - floor(a));
	}
	int2 bun;
	bun.r = cont[cont.size()-1];
	bun.s = 1;
	int k = 0;
	for (int i = cont.size()-2;i >= 0;i--) {
		k = bun.r * cont[i] + bun.s;
		bun.s = bun.r;
		bun.r = k;
	}
	bun.s = bun.r * cont[0] + bun.s;
	return bun;
}

Qubits quantum_fourier(Qubits x, int begin, int end) {
	for (int i = begin;i < end;i++) {
		for (int j = 1;j < end-i+1;j++) {
			if (j == 1)
				x.H(i);
			else
				x.CRx(i+j-1, i, 2 * M_PI / pow(2, j));
		}
	}

	for (int i = 0;i < x.bits / 2;i++) {
		x.SWAP(i, x.bits-1 - i);
	}
	return x;
}

Qubits quantum_reverse_fourier(Qubits x, int begin, int end) {
	for (int i = begin;i < (begin+end) / 2;i++) {
		x.SWAP(i, end-1 - i + begin);
	}

	for (int i = end-1;i >= begin;i--) {
		for (int j = end-i;j >= 1;j--) {
			if (j == 1)
				x.H(i);
			else
				x.CRx(i+j-1, i, - 2 * M_PI / pow(2, j));
		}
	}
	return x;
}

int gcd(int a, int b) {
	if (a % b == 0) return b;
	return gcd(b, a % b);
}

void ROR(Qubits q, int begin, int end) {
	for (int i = end;i > begin;i--) {
		q.SWAP(i, i-1);
	}
}

int Phase_estimation(int x, int M) {
	int n = 4;
	Qubits qbits = Qubits(1, 2 * n, false);

	qbits.print_s();
	for (int i = n;i < 2 * n;i++)
		qbits.H(i);
	qbits.print_s();

	qbits.CSWAP(4, 0, 3);
	qbits.CSWAP(4, 2, 3);
	qbits.CSWAP(4, 1, 2);

	qbits.CSWAP(5, 1, 3);
	qbits.CSWAP(5, 0, 2);
	qbits.print_s();

	quantum_reverse_fourier(qbits, 4, 8);
	qbits.print_s();

	int r = qbits.M_all();
	printf("%s\n", btoS(2 * n, r).c_str());

	return r;
}

int Shor_algorithm(int M) {
	int ans = 0;
	if (M % 2 == 0)
		return 2;

	int x = 2;
	ans = gcd(x, M);
	if (ans > 1)
		return ans;

	int r = Phase_estimation(x, M);

	if (r % 2 == 0 && (int)pow(x, r / 2) % M != M-1) {
		ans = gcd(pow(x, r / 2 - 1), M);
		if (M % ans == 0)
			return ans;
		ans = gcd(pow(x, r / 2 + 1), M);
		if (M % ans == 0)
			return ans;
	}

	return 0;
}

void fourier_test() {
	Qubits x = Qubits(9, 4, true);
	x.print();
	//printf("qft\n");
	quantum_fourier(x, 0, 4);
	x.print();
	//printf("iqft\n");
	quantum_reverse_fourier(x, 0, 4);
	x.print();
}

int main() {
	fourier_test();
	Qubits x = Qubits(9, 4, false);
	printf("%d\n", Shor_algorithm(15));
	delete[] x.elem;
	return 0;
}
