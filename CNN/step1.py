from __future__ import absolute_import, division, print_function
import tensorflow as tf
from tensorflow.keras import layers
import numpy as np
from numpy import zeros_like
import sklearn
from sklearn.model_selection import cross_validate 
from keras.callbacks import Callback
from sklearn.metrics import f1_score,precision_score, recall_score, confusion_matrix,roc_curve,accuracy_score,auc
from sklearn.preprocessing import label_binarize
import matplotlib.pyplot as plt
from scipy import interp
from itertools import cycle

from keras.datasets import mnist
#download mnist data and split into train and test sets
(X_train, y_train), (X_test, y_test) = mnist.load_data()
import matplotlib.pyplot as plt
#plot the first image in the dataset
plt.imshow(X_train[0])
#check image shape
print(X_train[0].shape)

#reshape data to fit model
X_train = X_train.reshape(60000,28,28,1)
X_test = X_test.reshape(10000,28,28,1)
from keras.utils import to_categorical
#one-hot encode target column
y_train = to_categorical(y_train)
y_test = to_categorical(y_test)
print(y_train[0])

from keras.models import Sequential
from keras.layers import Dense, Conv2D, Flatten, MaxPooling2D
#create model
model = Sequential()
#add model layers
model.add(Conv2D(64, kernel_size=3, activation='relu', input_shape=(28,28,1)))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Conv2D(32, kernel_size=3, activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Flatten())
model.add(Dense(10000, activation='relu'))
model.add(Dense(10, activation='softmax'))

#compile model using accuracy to measure model performance
model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])

#train the model
model.fit(X_train, y_train, validation_data=(X_test, y_test), epochs=5, verbose=1)

#predict first 4 images in the test set
predict = model.predict(X_test)
yPredict=predict.reshape(10000,10).astype('int')
y_test=y_test.astype('int')
scores = model.evaluate(X_test, y_test, verbose=0)  
print('Test accuracy ', scores[1])
model.summary() 
cm = confusion_matrix(y_test[1], yPredict[1])
precision = precision_score(y_test[1], yPredict[1], average='macro')
recall = recall_score(y_test[1], yPredict[1], average='macro')
f1 = f1_score(y_test[1], yPredict[1], average='macro')
accuracy = accuracy_score(y_test[1], yPredict[1])

print("Precision_score:",precision)
print("Recall_score:",recall)
print("F1_score:",f1)
print("Accuracy_score:",accuracy)


