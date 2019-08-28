#include "../q++.hpp"
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

Qubits quantum_fourier(Qubits x, int n) {
	for (int i = 0;i < n;i++) {
		for (int j = 1;j <= n - i;j++) {
			if (j == 1)
				x.H(i);
			else
				x.Rx(i, 2 * M_PI / pow(2, j));
		}
	}

	for (int i = 0;i < x.bits / 2;i++) {
		x.SWAP(i, x.bits-1 - i);
	}
	return x;
}

Qubits quantum_reverse_fourier(Qubits x, int n) {
	for (int i = 0;i < x.bits / 2;i++) {
		x.SWAP(i, x.bits-1 - i);
	}

	for (int i = n-1;i >= 0;i--) {
		for (int j = n-i;j >= 1;j--) {
			if (j == 1)
				x.H(i);
			else
				x.Rx(i, - 2 * M_PI / pow(2, j));
		}
	}
	return x;
}

int gcd(int a, int b) {
	if (a % b == 0) return b;
	return gcd(b, a % b);
}

int Phase_estimation() {
	int n = 4;
	Qubits x = Qubits(15, 2 * n);

	for (int i = n;i < 2 * n;i++) {
		x.H(i);
	}

	for (int i = 0;i < n;i++) {
		//x.CNOT(i, );
	}

	quantum_reverse_fourier(x, n);

	int ans = x.M_all();
	printf("%s\n", btoS(2 * n, ans).c_str());

	return 0;
}

int Shor_algorism(int M) {
	if (M % 2 == 0)
		return 2;

	return 0;
}

void fourier_test() {
	Qubits x = Qubits(9, 4);
	x.print();
	printf("qft\n");
	quantum_fourier(x, 4);
	x.print();
	printf("iqft\n");
	quantum_reverse_fourier(x, 4);
	x.print();
	delete[] x.elem;
}

int main() {
	Qubits x = Qubits(9, 4);
	printf("%d\n", gcd(15, 3));
	printf("%d\n", Shor_algorism(15));
	delete[] x.elem;
	return 0;
}
