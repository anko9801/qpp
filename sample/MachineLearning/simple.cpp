#include "../../q++.hpp"
#include <iostream>
#include <boost/python.hpp>
using namespace std;

int QClassifier(/*double *input, double **train, int *y*/) {
	cout << "Inside C++" << endl;
	return 0;
}


BOOST_PYTHON_MODULE(simple) {
	boost::python::def("QClassifier", QClassifier);
}
