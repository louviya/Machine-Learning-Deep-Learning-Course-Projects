from sklearn.model_selection import cross_val_score
from sklearn.ensemble import RandomForestClassifier
from sklearn.ensemble import RandomForestRegressor
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import tensorflow as tf
from keras import utils



filename = r'maf_mm_sigs.txt'

data = pd.read_csv(filename, sep='\t',header=None,engine='python')
features = data.iloc[1:, 1:97].astype('float')
#print(features.info())
f_norm = (features- features.mean()) /(features.max()-features.min())

labels = data.iloc[1:, 97:].astype('float')
#print(labels.info())


testlabels = labels.drop_duplicates()   #there are 10171 different labels
testlabels = testlabels.reset_index(drop=True) 

tlabels = labels.copy()
#y = np.array(y)
#y = y.tolist()
#y = y.astype('int')
z = tlabels.iloc[:,0]
p,_ = pd.factorize(z)
#print(p.max())  #1114
#q=pd.DataFrame(p)
#print(q[0].value_counts())

clf = RandomForestClassifier(n_jobs=1,oob_score=True,random_state=1)#, min_samples_split=100,min_samples_leaf=20,max_depth=8, max_features='sqrt', random_state=10)
clf.fit(f_norm, p)
print(clf.oob_score_)  #0.7392671185774634



#process labels
t2 = tlabels.replace(2.22e-16, 0)
print(t2.describe())
statistics = t2.describe()
print(t2.idxmax())
t22 = t2.loc[97].value_counts()

from tensorflow.keras import layers
model = tf.keras.Sequential()
model.add(layers.Embedding(10179, 1, input_length=96))
input_array = np.array(t2)
model.compile('rmsprop', 'mse')
output_array = model.predict(input_array)
t3 = output_array.reshape((10179,96))
t4 = pd.DataFrame(t3)
#t5 = t4.drop_duplicates()   #there are 10170 different labels
t6 = t4.loc[0].value_counts()
t7 = t4.loc[1].value_counts()

t8=t4.copy()
nl = []
for i in range(10179):
    tmp = t8.iloc[i].idxmax()
    nl.append(tmp)
nl2 = np.array(nl)
nl3 = pd.DataFrame(nl)
clf = RandomForestClassifier(n_jobs=1,oob_score=True,random_state=1)#, min_samples_split=100,min_samples_leaf=20,max_depth=8, max_features='sqrt', random_state=10)
clf.fit(f_norm, nl2)
print(clf.oob_score_)  #0.4323607427055703
import warnings
warnings.filterwarnings("ignore")
#y_predprob = rf0.predict_proba(X)[:,1]
#print ("AUC Score (Train): %f" + metrics.roc_auc_score(y,y_predprob))





import sklearn
nl22 = tf.keras.utils.to_categorical(nl2)
tmp2 = np.array(f_norm)
f_norm2 =tmp2.reshape((10179,96,1))
X_train, X_test, y_train, y_test = sklearn.model_selection.train_test_split(f_norm2, nl22, test_size=0.2, random_state=1)
model4 = tf.keras.Sequential()
model4.add(layers.LSTM(64, return_sequences=True,input_shape=(96, 1)))  # returns a sequence of vectors of dimension 32
model4.add(layers.LSTM(64, return_sequences=True))  # returns a sequence of vectors of dimension 32
model4.add(layers.LSTM(64))  # return a single vector of dimension 64
model4.add(layers.Flatten())
model4.add(layers.Dense(96, activation='softmax'))
model4.compile(loss='categorical_crossentropy',
              optimizer=tf.train.AdamOptimizer(0.001),#'rmsprop',
              metrics=['accuracy'])
model4.fit(X_train, y_train,
          batch_size=32, epochs=10, verbose=2,validation_data=(X_test, y_test))
scores = model4.evaluate(X_test, y_test, batch_size=32, verbose=0)   
print(scores)
print("Base Error:%.2f%%"%(100-scores[1]*100))
model4.summary()       #0.13310


X_train, X_test, y_train, y_test = sklearn.model_selection.train_test_split(tmp2, nl22, test_size=0.2, random_state=1)
model = tf.keras.Sequential()  
model.add(layers.Dense(64, activation='relu'))   # Adds a densely-connected layer with 64 units to the model:
model.add(layers.Dense(64, activation='relu'))   # Add another:
model.add(layers.Dense(96, activation='softmax'))

model.compile(optimizer=tf.train.AdamOptimizer(0.001), #tf.train.RMSPropOptimizer(0.01),##
              loss=tf.keras.losses.categorical_crossentropy,
              metrics=[tf.keras.metrics.categorical_accuracy])
#data = np.random.random((1000,32))
#labels = np.random.random((1000,10))
model.fit(X_train, y_train, validation_data=(X_test,y_test), epochs=10, batch_size=32, verbose=2)#, callbacks=[metrics1])
scores = model.evaluate(X_test, y_test, batch_size=32, verbose=0)    #0.6944
print(scores)
print("Base Error:%.2f%%"%(100-scores[1]*100))  #0.17534381




from tensorflow.keras import layers
model = tf.keras.Sequential()
model.add(layers.Embedding(10179, 1, input_length=96))
input_array = np.array(tlabels)
model.compile('rmsprop', 'mse')
output_array = model.predict(input_array)
t3 = output_array.reshape((10179,96))
t4 = pd.DataFrame(t3)
#t5 = t4.drop_duplicates()   #there are 10170 different labels
t6 = t4.loc[0].value_counts()
t7 = t4.loc[1].value_counts()
t5 = t4.describe()
print(t4.iloc[1].sum())
t8=t4.copy()
print(t4.iloc[1].value_counts())
nl = []
for i in range(10179):
    for j in range(96):
        t8.iloc[i][j] = t8.iloc[i][j] / t8.iloc[i].sum() 
    tmp = t8.iloc[i].idxmax()
    nl.append(tmp)
nl2 = np.array(nl)
nl3 = pd.DataFrame(nl)
#print(nl3[0].value_counts())
p,_ = pd.factorize(nl)
clf = RandomForestClassifier(n_jobs=1,oob_score=True,random_state=1)#, min_samples_split=100,min_samples_leaf=20,max_depth=8, max_features='sqrt', random_state=10)
clf.fit(f_norm, nl2)
print(clf.oob_score_)  #0.055
import warnings
warnings.filterwarnings("ignore")

#可以同时训练多个分类器，每个模型对应一维的标签
rlf = RandomForestRegressor(oob_score=True, random_state=1)
rlf.fit(f_norm,tlabels)
print(rlf.oob_score_)  #0.10527456312684895
pre = rlf.predict(f_norm)



