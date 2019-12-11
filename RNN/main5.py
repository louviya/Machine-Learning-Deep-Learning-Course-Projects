from __future__ import absolute_import, division, print_function
import tensorflow as tf
import tensorflow.contrib.eager as tfe
tfe.enable_eager_execution()
from tensorflow.keras import layers
import numpy as np
import sklearn


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
x4 = x4[:,:,1:5]   


for line in open(filenamey):       #list
    line = line.strip('\n')#.replace('\t',' ').replace('  ', ' ').split(' ')
    y.append(line)                
y6 = tf.keras.utils.to_categorical(y)
y2=np.zeros((1084,1),dtype='float32')  #array
for i in range (1084):
    y2[i] = y[i]
    
#a = np.array([[1],[2]])
#b = np.zeros((2,7))
#print(a+b)    
tmp = np.zeros((1084,224),dtype='float32')     #to same dimension
y3 = tmp + y2

y4 = tf.keras.utils.to_categorical(y3)   


indices = np.arange(len(y4))
np.random.shuffle(indices)
x5 = x4[indices]
y5 = y4[indices]

t = (x5,y5)
#chunky = tf.data.Dataset.from_tensor_slices(y2).batch(seq_length+1, drop_remainder=True)
#for item in chunky.take(5):
  #print(item)

#dataset=chunks.concatenate(chunky)   #1084 input x2, the 1085th is the first of y2
#for item in dataset.take(1085):
  #print(item)     
X_train, X_test, y_train, y_test = sklearn.model_selection.train_test_split(x5, y6, test_size=0.1, random_state=1)


batch_size = 32
epoch = 5
timesteps = 224
data_dim = 4

model7 = tf.keras.Sequential()
#model7.add(layers.Embedding(max_features, embedding, input_length=maxlen))
#model7.add(layers.Dropout(0.5))
model7.add(layers.Conv1D(64, kernel_size=4, input_shape=(224, 4), activation='relu'))
model7.add(layers.MaxPooling1D(2))
model7.add(layers.Conv1D(64, kernel_size=4, input_shape=(224, 4), activation='relu'))
model7.add(layers.MaxPooling1D(2))

model7.add(layers.LSTM(3, #return_sequences=True,
               input_shape=(timesteps, data_dim)))
model7.add(layers.Dense(64,activation='relu'))
model7.add(layers.Dense(3,activation='softmax'))


model7.compile(loss='categorical_crossentropy',
              optimizer=tf.train.AdamOptimizer(0.001),#'rmsprop',
              metrics=['accuracy'])

model7.fit(X_train, y_train,
          batch_size=64, epochs=5,
          validation_data=(X_test, y_test))
 
model7.summary()    #0.7706
 







    
