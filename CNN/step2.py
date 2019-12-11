from __future__ import absolute_import, division, print_function
import numpy as np
from sklearn.model_selection import cross_validate, StratifiedKFold
from sklearn.metrics import f1_score,precision_score, recall_score, confusion_matrix,roc_curve,accuracy_score,auc
import matplotlib.pyplot as plt
import sklearn
#import pandas as pd
import os
from keras.models import Sequential
from keras.layers import Dense, Conv2D, Flatten, MaxPooling2D, Dropout, BatchNormalization
import cv2
from keras.utils import to_categorical
import tensorflow as tf
import keras

batch_size=3;
num_classes=3;
epochs =1;
bio_dir=os.path.join(os.getcwd(), 'Biological')
fib_dir=os.path.join(os.getcwd(), 'Fibres')
pow_dir=os.path.join(os.getcwd(), 'Powder')

#example show
img=cv2.imread(os.path.join(bio_dir, 'L7_0d1cea9177bbe38daba7ae306bcfd6d8.jpg'))
#cv2.imshow('img', img)
#cv2.waitKey(0)
#cv2.destroyAllWindows()

#get image data
bio = []   #empty dataframe
for root,dirs,files in os.walk(bio_dir):  #traverse
    files=[f for f in files if not f[0]=="."]     #ignore hidden files
    for filename in files:
        file = os.path.join(root,filename)   #combine str to file path 
        tmp = cv2.imread(file)  #for each file of the path, read content
        bio.append(tmp) 
bio_data = np.array(bio).reshape(53, 768, 1024, 3)
bio_label = np.zeros((53, 1)).astype('uint8')
#print(type(bio_data))

fib = []   #empty dataframe
for root,dirs,files in os.walk(fib_dir):  #traverse
    files=[f for f in files if not f[0]=="."]     #ignore hidden files
    for filename in files:
        file = os.path.join(root,filename)   #combine str to file path 
        tmp = cv2.imread(file)  #for each file of the path, read content
        fib.append(tmp) 
fib_data = np.array(fib).reshape(40, 768, 1024, 3)
fib_label = np.ones((40, 1)).astype('uint8')
#print(type(fib_data))

powd = []   #empty dataframe
for root,dirs,files in os.walk(pow_dir):  #traverse
    files=[f for f in files if not f[0]=="."]     #ignore hidden files
    for filename in files:
        file = os.path.join(root,filename)   #combine str to file path 
        tmp = cv2.imread(file)  #for each file of the path, read content
        powd.append(tmp) 
pow_data = np.array(powd).reshape(40, 768, 1024, 3)
pow_label = np.ones((40, 1)).astype('uint8')
pow_label = pow_label *2
#print(type(pow_data))

xdata = np.concatenate((bio_data, fib_data), axis=0)
xdata = np.concatenate((xdata, pow_data), axis=0)
ylabel = np.concatenate((bio_label, fib_label, pow_label), axis=0)

x_train, x_test, y_train, y_test = sklearn.model_selection.train_test_split(xdata, ylabel, test_size=0.2, random_state=1)#, shuffle=True)
#print(x_train.shape)
#x_train = x_train.astype('float32')
#x_test = x_test.astype('float32')
#x_train /= 255
#x_test /=255
#print('x_train shape:', x_train.shape)
y_train = to_categorical(y_train)
y_test = to_categorical(y_test)
#print(y_train[0])


#create model
model = Sequential()
#add model layers
model.add(Conv2D(32, kernel_size=3, activation='relu', input_shape=(768,1024,3)))
model.add(BatchNormalization())
model.add(Conv2D(64, kernel_size=3, activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Dropout(0.25))
model.add(Conv2D(32, kernel_size=3, activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Dropout(0.25))
model.add(Flatten())
model.add(Dense(100, activation='relu'))
model.add(Dropout(0.5))
model.add(Dense(3, activation='softmax'))

#compile model using accuracy to measure model performance
model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])

#train the model
model.fit(x_train, y_train, batch_size=batch_size, epochs=epochs, validation_split=1/16, verbose=1)

#predict first 4 images in the test set
predict = model.predict(x_test, batch_size=1)
yPredict=predict.reshape(27,3).astype('uint8')
y_test=y_test.astype('uint8')
scores = model.evaluate(x_test, y_test, verbose=0, batch_size=1)  
print('Test accuracy ', scores)
model.summary() 
cm = confusion_matrix(y_test, yPredict)
precision = precision_score(y_test, yPredict, average='macro')
recall = recall_score(y_test, yPredict, average='macro')
f1 = f1_score(y_test, yPredict, average='macro')
accuracy = accuracy_score(y_test, yPredict)
print("Precision_score:",precision)
print("Recall_score:",recall)
print("F1_score:",f1)
print("Accuracy_score:",accuracy)




kfold = StratifiedKFold(n_splits=16, shuffle=True, random_state=1)
cvscores2 =[]
for train, test in kfold.split(x_train, y_train, groups=None):
    model2 = Sequential()
    model2.add(Conv2D(32, kernel_size=3, activation='relu', input_shape=(768,1024,3)))
    model2.add(Conv2D(64, kernel_size=3, activation='relu'))
    model2.add(MaxPooling2D(pool_size=(2, 2)))
    model2.add(Dropout(0.25))
    model2.add(Conv2D(32, kernel_size=3, activation='relu'))
    model2.add(MaxPooling2D(pool_size=(2, 2)))
    model2.add(Dropout(0.25))
    model2.add(Flatten())
    #model2.add(Dense(10, activation='relu'))
    #model2.add(Dropout(0.5))
    model2.add(Dense(3, activation='softmax'))
    model2.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])
    y_train = to_categorical(y_train)
    y_test = to_categorical(y_test)
    model2.fit(x_train[train], y_train[train], batch_size=batch_size, epochs=epochs, verbose=1)
    scores2 = model.evaluate(x_train[test], y_train[test], verbose=0)
    print("%s: %.2f%%" % (model2.metrics_names[1], scores2[1]*100))
    cvscores2.append(scores2[1] * 100)
print("%.2f%% (+/- %.2f%%)" % (np.mean(cvscores2), np.std(cvscores2)))



