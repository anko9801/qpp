#include "../../q++.hpp"
#include <iostream>
#include <boost/python.hpp>
using namespace std;

int QClassifier(double input[], double train[][], int y[]) {
	int size = size(train) * size(*train);
	int result = -1;
	Qubits x = Qubits(0, size + 1 + 1 + 1, true);
	for (int i = 0;i < size + 1;i++) {
		x.H(i);
	}
	for (int i = 0;i < size(train);i++) {
		prepareState();
	}
	x.H(size);
	if (M(size) == 0) {
		if (M(size+1) == 0) {
			result = 0;
		}else{
			result = 1;
		}
	}
	cout << size << endl;
	return 0;
}

void prepareState() {

}


BOOST_PYTHON_MODULE(simple) {
	boost::python::def("QClassifier", QClassifier);
}
