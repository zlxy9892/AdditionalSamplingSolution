# -*- coding: utf-8 -*-

import os
os.chdir('E:/MyWork/myres/master_thesis/code/AdditionalSamplingSolution/py')

import pandas as pd
import numpy as np
from sklearn import preprocessing
from sklearn import ensemble, neighbors, linear_model, svm, metrics

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
# for xc dataset
df = pd.read_csv('./data/envdata_xc.csv')
X = df[['geo', 'slope', 'planc', 'profc', 'preci', 'tempr', 'twi']]
X = np.array(X)
#X = preprocessing.minmax_scale(X)
#y = np.array(df['soiltype'])

# load training set
#for training set
value = 'SOMA'
#train_loc_fn = './data/dtrain_mymethod.csv'
train_loc_fn = './data/dtrain_soma_sss_3.csv'
train_loc = np.array(pd.read_csv(train_loc_fn))[:,0:2]
train_index = getNearestElemsIds(df, train_loc)
X_train = X[train_index]
y_train = np.array(pd.read_csv(train_loc_fn))[:,2]
# for test set
df_test = pd.read_csv('./data/samples_validation.csv')
X_test = np.array(df_test[['geo', 'slope', 'planc', 'profc', 'preci', 'tempr', 'twi']])
y_test = np.array(df_test[value])

# predict by rf
samplesize_min = 10
samplesize_max = 40
for samplesize in range(samplesize_min+1, samplesize_max+1):
    X_train_sub = X_train[:samplesize]
    y_train_sub = y_train[:samplesize]
    np.random.seed(314)
    regr = ensemble.RandomForestRegressor(n_estimators=100)
    #regr = neighbors.KNeighborsRegressor(n_neighbors=3)
    regr.fit(X_train_sub, y_train_sub)
    y_test_pred = regr.predict(X_test)
    rmse = np.sqrt(metrics.mean_squared_error(y_test, y_test_pred))
    #print('rmse:', round(rmse, 3))
    print(round(rmse, 3))
