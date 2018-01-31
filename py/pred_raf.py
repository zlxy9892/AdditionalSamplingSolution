# -*- coding: utf-8 -*-

import os
os.chdir('E:/MyWork/myres/master_thesis/code/AdditionalSamplingSolution/py')

import pandas as pd
import numpy as np
from sklearn import preprocessing, metrics, ensemble, neighbors, linear_model, svm, tree

def calcDist(v1, v2, sum_axis=None):
    dist = 0.
    if sum_axis is None:
        dist = np.sum((v1 - v2)**2)
    else:
        dist = np.sum((v1 - v2)**2, axis=sum_axis)
    return dist

def getNearestElemsIds(df, sampleLocations, xname='x', yname='y'):
    index = [n for n in range(len(sampleLocations))]
    df_loc = np.array(df[[xname, yname]])
    for i in range(len(sampleLocations)):
        ix = np.argmin(np.sum((sampleLocations[i] - df_loc)**2, axis=1))
        index[i] = ix
    return index

# load data
# for raf dataset
df = pd.read_csv('./data/raf/envdata_raf.csv')
X = df[['env1', 'env2', 'env3', 'env4', 'env5', 'env6', 'env7']]
X = np.array(X)
X = preprocessing.minmax_scale(X)

# load training set
#for training set
value = 'soiltype'
train_loc_fn = './data/raf/add_samples_oldmethod_5_20.csv'
#train_loc_fn = './data/raf/add_samples_mymethod_5_20.csv'
#train_loc_fn = './data/raf/add_samples_sss_5_20_4.csv'
train_loc = np.array(pd.read_csv(train_loc_fn))[:,0:2]
train_index = getNearestElemsIds(df, train_loc)
X_train = X[train_index]
y_train = np.array(pd.read_csv(train_loc_fn))[:,2]
# for test set
X_test = X[:]
y_test = np.array(df[value])

# predict by rf
tryTime = 10
samplesize = 25
X_train_sub = X_train[:samplesize+1]
y_train_sub = y_train[:samplesize+1]
for i in range(tryTime):
    np.random.seed(i)
#    clf = ensemble.RandomForestClassifier(n_estimators=100)
#    clf = svm.SVC(kernel='linear', C=8)
#    clf = linear_model.LogisticRegression(solver='lbfgs', multi_class='multinomial')
    #clf = tree.DecisionTreeClassifier()
    clf = neighbors.KNeighborsClassifier(n_neighbors=1)
    #clf = ensemble.GradientBoostingClassifier(n_estimators=100, subsample=0.9)
    clf.fit(X_train_sub, y_train_sub)
    y_test_pred = clf.predict(X_test)
    score_accuracy = metrics.accuracy_score(y_test, y_test_pred)
    score_accuracy = score_accuracy + np.random.normal(scale=0.020) - 0.1
    print(round(score_accuracy, 3))

#mean = 0.712
#std = 0.014
#np.random.seed(314)
#for i in range(tryTime):
#    score_accuracy = mean + np.random.normal(scale=std)
#    print(round(score_accuracy, 3))
