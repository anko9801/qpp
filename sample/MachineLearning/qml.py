import numpy as np
from sklearn import datasets
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import Normalizer
from sklearn.preprocessing import StandardScaler

import qsharp
from ML import QClassifier

stdsc = StandardScaler()
iris = datasets.load_iris()
x1 = iris.data[:,0].reshape(-1, 1)
x2 = iris.data[:,1].reshape(-1, 1)
y = iris.target
x1_norm = stdsc.fit_transform(x1)
x2_norm = stdsc.fit_transform(x2)

normalized_set = Normalizer().transform(np.hstack([x1_norm, x2_norm]))

data_list = normalized_set.tolist()
target_list = y.tolist()
data_list_12 = []
target_list_12 = []
for (data, target) in zip(data_list, target_list):
    if target != 2:
        data_list_12.append(data)
        target_list_12.append(target)

X_train, X_test, y_train, y_test = train_test_split(data_list_12,
                                                    target_list_12,
                                                    test_size=0.2)

for (input, desire) in zip(X_test, y_test):
    pacc, suc = 0, 0
    for i in range(1000):
        result = QClassifier.simulate(input=input,
                                      train=X_train[:64],
                                      y=y_train[:64])
        if result == desire:
            suc += 1
        if result != -1:
            pacc += 1
    print('test case ', input)
    print('pacc:', float(pacc)/1000.0)
    print('suc_rate:', float(suc) / float(pacc))
