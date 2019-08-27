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

int Phase_estimation() {
	int n = 4;
	Qubits x = Qubits(0, 2 * n);

	for (int i = 0;i < n;i++) {
		x.H(i);
	}
	for (int i = n;i < 2 * n;i++) {
		
	}

	return 0;
}

void quantum_reverse_fourier(Qubits x, int n) {
	for (int i = 0;i < n / 2;i++) {
		x.SWAP(i, n-1 - i);
	}

	for (int i = 0;i < n;i++) {
		for (int j = i+1;j >= 1;j--) {
			x.Rx(i, - 2 * M_PI / pow(2, j));
		}
	}
}


int Shor_algorism(int M) {
	if (M % 2 == 0)
		return 2;

	int2 test = renbun(3.333333333);
	printf("%d / %d\n", test.s, test.r);
	return 0;
}

int main() {
	printf("%d\n", Shor_algorism(15));
	return 0;
}
