from __future__ import absolute_import, division, print_function
import tensorflow as tf
#import tensorflow.contrib.eager as tfe
#tfe.enable_eager_execution()
from tensorflow.keras import layers
import numpy as np
import sklearn
from sklearn.model_selection import cross_validate
# fix random seed for reproducibility
seed = 7
np.random.seed(seed)


x=[]   #list
y=[]
filenamex = r'seq.txt'
filenamey = r'ally2.txt'

data = open(filenamex,'r').read()
chars = list(set(data)) #unique characters to generate convert list
char_to_ix = {ch: i for i, ch in enumerate(sorted(chars))}
ix_to_char = {i: ch for i, ch in enumerate(sorted(chars))}
#print(ix_to_char)
ix_to_char.pop(0)
char_to_ix.pop('\n')
#print(char_to_ix)

#with open(filenamex) as f:  #line write to list
    #examples = f.readlines()
#examples = [x.strip() for x in examples]

#data input
for line in open(filenamex):
    line = line.strip('\n').replace('\t',' ').replace('  ', ' ').split(' ') 
    a = []
    for i in range(len(line[0])):
        a.append(char_to_ix.get(line[0][i]))
    x.append(a)
#print(x[0][219])
#print(len(x[0]))
#print([i for i in range(4)])

x2 = np.zeros((1084, 224))  #must use loop, the conversion np.array getting an object of different length and can't be used to one-hot
for i in range(1084):
    t=len(x[i])
    if t<=224:
        for j in range(t):  #must len with extra element as 0
            x2[i][j]=x[i][j]
assert(x2.shape==(1084, 224))

x3 = tf.keras.utils.to_categorical(x2)

x4 = x3.copy()
x4 = x4[:,:,1:5]     #delete vector by feature 0

#print(x4[0])
x6 = x4.reshape(x4.shape[0], 1, x4.shape[1], x4.shape[2]).astype('float32')

for line in open(filenamey):
    line = line.strip('\n')#.replace('\t',' ').replace('  ', ' ').split(' ')
    y.append(line)
y2 = tf.keras.utils.to_categorical(y)
assert(y2.shape==(1084, 3))



indices = np.arange(len(y2))
np.random.shuffle(indices)
x5 = x4[indices]
y5 = y2[indices]
x7 = x6[indices]



X_train, X_test, y_train, y_test = sklearn.model_selection.train_test_split(x5, y5, test_size=0.2, random_state=1)




data_dim = 4
timesteps = 224
# expected input data shape: (batch_size, timesteps, data_dim)
model4 = tf.keras.Sequential()
model4.add(layers.LSTM(64, return_sequences=True,input_shape=(timesteps, data_dim)))  # returns a sequence of vectors of dimension 32
model4.add(layers.LSTM(64, return_sequences=True))  # returns a sequence of vectors of dimension 32
model4.add(layers.LSTM(64))  # return a single vector of dimension 64
model4.add(layers.Flatten())
model4.add(layers.Dense(3, activation='softmax'))
model4.compile(loss='categorical_crossentropy',
              optimizer=tf.train.AdamOptimizer(0.001),#'rmsprop',
              metrics=['accuracy'])
model4.fit(X_train, y_train,
          batch_size=32, epochs=10,
          validation_data=(X_test, y_test), verbose=2)
scores = model4.evaluate(X_test, y_test, batch_size=32, verbose=0)   
print(scores)
print("Base Error:%.2f%%"%(100-scores[1]*100))
model4.summary()           
#0.6944, 0.7706, 22.94% for 0.1 size
#0.7463, 0.7650, 23.50% for 0.2 size

# Generate dummy training data
#x_train = np.random.random((1000, timesteps, data_dim))
#y_train = np.random.random((1000, num_classes))

# Generate dummy validation data
#x_val = np.random.random((100, timesteps, data_dim))
#y_val = np.random.random((100, num_classes))


model5 = tf.keras.Sequential()    #3-dimensional data
model5.add(layers.Masking(mask_value=0., input_shape=(224, 4)))
model5.add(layers.LSTM(64, return_sequences=True))
model5.add(layers.LSTM(64))
model5.add(layers.Flatten())
model5.add(layers.Dense(64, activation='tanh'))
model5.add(layers.Dense(3,activation='softmax'))
model5.compile(loss='categorical_crossentropy', optimizer=tf.train.AdamOptimizer(0.001),metrics=['accuracy'])
model5.fit(X_train, y_train, batch_size=32, epochs=10, verbose=2, validation_data=(X_test, y_test))
scores = model5.evaluate(X_test, y_test, batch_size=32, verbose=0)   
print(scores)
print("Base Error:%.2f%%"%(100-scores[1]*100))
model5.summary()           
#0.8639, 0.8710, 12.90% for 0.2 size no shuffle
#0.6990, 0.7134, 28.57%
#0.8616, 0.8802, 11.987%  shuffle


x8 = x2/4 
x9 = x8.reshape(x8.shape[0], 1, x8.shape[1]).astype('float32')
x10 = x9[indices]
X_train2, X_test2, y_train2, y_test2 = sklearn.model_selection.train_test_split(x10, y5, test_size=0.2, random_state=1)
model6 = tf.keras.Sequential()    #3-dimensional data
model6.add(layers.Masking(mask_value=0., input_shape=(1, 224)))
model6.add(layers.LSTM(64, return_sequences=True))
model6.add(layers.LSTM(64))
model6.add(layers.Flatten())
model6.add(layers.Dense(64, activation='tanh'))
model6.add(layers.Dense(3,activation='softmax'))
model6.compile(loss='categorical_crossentropy', optimizer=tf.train.AdamOptimizer(0.001),metrics=['accuracy'])
model6.fit(X_train2, y_train2, batch_size=32, epochs=10, verbose=2, validation_data=(X_test2, y_test2))
scores = model6.evaluate(X_test2, y_test2, batch_size=32, verbose=0)   
print(scores)
print("Base Error:%.2f%%"%(100-scores[1]*100))
model6.summary()   
#0.9273, 0.9171, 8.297%  shuffle



model7 = tf.keras.Sequential()
#model7.add(layers.Embedding(max_features, embedding, input_length=maxlen))
#model7.add(layers.Dropout(0.5))
model7.add(layers.Conv1D(64, kernel_size=4, input_shape=(224, 4), activation='tanh'))
model7.add(layers.MaxPooling1D(2))
model7.add(layers.Conv1D(64, kernel_size=4, input_shape=(224, 4), activation='tanh'))
model7.add(layers.MaxPooling1D(2))
model7.add(layers.LSTM(64, input_shape=(224, 4)))#return_sequences=True,
model7.add(layers.Flatten())
model7.add(layers.Dense(64,activation='tanh'))
model7.add(layers.Dense(3,activation='softmax'))
model7.compile(loss='categorical_crossentropy', optimizer=tf.train.AdamOptimizer(0.001),metrics=['accuracy'])#'rmsprop',             
model7.fit(X_train, y_train, batch_size=32, epochs=10, validation_data=(X_test, y_test), verbose=2)
scores = model7.evaluate(X_test, y_test, batch_size=32, verbose=0)   
print(scores)
print("Base Error:%.2f%%"%(100-scores[1]*100))     
model7.summary()    #0.
#0.9539, 0.9078, 9.22%  shuffle


X_train3, X_test3, y_train3, y_test3 = sklearn.model_selection.train_test_split(x7, y5, test_size=0.2, random_state=1)
model8 = tf.keras.Sequential()
model8.add(layers.Conv2D(64, (1, 56), input_shape=(1,224, 4), activation='tanh'))
model8.add(layers.MaxPooling2D(pool_size=(1, 1)))
model8.add(layers.Dropout(0.2))
model8.add(layers.Flatten())
#model8.add(layers.Dense(128, activation='tanh'))
model8.add(layers.Dense(64, activation='tanh'))
model8.add(layers.Dense(3, activation='softmax'))
model8.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
model8.fit(X_train3, y_train3, validation_data=(X_test3, y_test3), epochs=10, batch_size=32, verbose=2)
scores = model8.evaluate(X_test3, y_test3, verbose=0)
print("Baseline Error: %.2f%%" % (100-scores[1]*100))
model8.summary()
#0.9885, 0.9862, 1.38%
