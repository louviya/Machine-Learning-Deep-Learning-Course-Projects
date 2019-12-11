import numpy as np
import sklearn
from sklearn.model_selection import cross_validate
import utils 
#import grad_check


import matplotlib.pyplot as plt
import tensorflow as tf
from tensorflow.python.framework import ops
import util_tf


x=[]   #list
y=[]
y2=[]
filenamex = r'allx.txt'
filenamey = r'ally.txt'
filenamey2 = r'y2.txt'

#data input
for line in open(filenamex):
    line = line.strip('\n').replace('\t',' ').replace('  ', ' ').split(' ')
    x.append(line)
#xx=np.array(x) 
#print(xx.size)    # 1804 rows, each is a list of ten elements   (1804, )
#list(['3579ebace3c49b8a810aba67a3ef4de9', 
#      'AGCGTTAATCGGAATTACTGGGCGTAAAGCGTGCGCAGGCGGTCTTGTAAGACAGAGGTGAAATCCCTGGCTCAACAGATGGCCTTGTACTGCAAGGCTGGAGTGCGGCAGAGGGGGATGGAATTCCGCGTGTAGCAGTGAAATGCGTAGATATGCGGAGGAACACCGATGGCGAAGGCAGTCCCCTGGGCCTGCACTGACGCTCATGCACGAAAGCGTG', 
#      'k__Bacteria', 'p_...', 'c__Betaproteobacteria', 'o__Burkholderiales', 'f__Comamonadaceae', 'g__Tepidimonas', 's__', '0.9990865811032412'])
#data = np.loadtxt('allx.txt')  #use for numbers
#print(data)
#print(x)
xd = np.empty((1084,1))  #array
for i in range(1084):
    xd[i][0] = x[i][-1]
#print(xd.shape)
#xxx=[]   #delete the p_... feature
#for i in range(xx.size):
    #xxx.append(xx[i][-1])
#print(xxx.size)
#xdata=np.array(xxx)
#print(xdata.shape)  #['0.9990865811032412',..., '0.7946427508165944', '0.999819835830198', '0.9999999893914407']
#assert(xdata.shape==(1084, 1)) 
#print(xdata.shape)
    
    
#data label y
for line in open(filenamey):
    line = line.strip('\n')#.replace('\t',' ').replace('  ', ' ').split(' ')
    y.append(line)
#yy=np.array(y) 
#print(yy.size)
yd = np.empty((1084,1))  #array
for i in range(1084):
    yd[i][0] = y[i][-1]
assert(yd.shape==(1084, 1)) 
#print(yd.shape)

#data label y 0/1
for line in open(filenamey2):
    line = line.strip('\n')#.replace('\t',' ').replace('  ', ' ').split(' ')
    y2.append(line)
#yy=np.array(y) 
#print(yy.size)
y2d = np.empty((1084,1))  #array
for i in range(1084):
    y2d[i][0] = y2[i][-1]
assert(y2d.shape==(1084, 1)) 
#print(y2d)


#two classes
train_x, test_x, train_y, test_y = sklearn.model_selection.train_test_split(xd, y2d, test_size=0.1, random_state=1)
#print(test_y)
trainx = train_x.T
testx = test_x.T
trainy = train_y.T
testy = test_y.T
#print(trainx.shape)

#multiple classes
X_train, X_test, y_train, y_test = sklearn.model_selection.train_test_split(xd, yd, test_size=0.1, random_state=1)
#print(X_train.size)
#print(X_test.size)
Xtrain = X_train.T
Xtest = X_test.T
#print(Xtrain.shape)
ytrain = y_train.T
ytest = y_test.T



#two layers NN
layers_dims2 = (1,7,1)  #输入层节点数量, 隐藏层节点数量,输出层节点数量
parameters2 = two_layer_model(trainx, trainy, layers_dims2, num_iterations = 2500, print_cost=True,isPlot=True)
predictions_train = predict(trainx, trainy, parameters2) #训练集
predictions_test = predict(testx, testy, parameters2) #测试集

#multiple layers NN
layers_dims4 = [1, 20, 12, 8, 1] #  4-layer model   维度为(n_y,n_h,···,n_h,n_y)
parameters4 = L_layer_model(trainx, trainy, layers_dims4, num_iterations = 2500, print_cost = True, initialization = "he", lambd =0.7, keep_prob=0.8, isPlot=True)
pred_train = predict(trainx, trainy, parameters4) #训练集
pred_test = predict(testx, testy, parameters4) #测试集

#print(parameters['W' + str(1)])
#A, cache = linear_activation_forward(A_prev, parameters2['W' + str(l)], parameters['b' + str(l)], "relu")
#print(cache)  #A,W,b,Z  len is 2

#A[0] = cache[0][0]
#print(A[0][0])
#test=parameters5['W' + str(1)]
#print(len(test))
#lsum = np.sum(np.square(test))
#print(lsum)
#parameters = initialize_parameters_he(layers_dims5)
#AL , caches = L_model_forward(trainx,parameters)
#gradients = L_model_backward(AL,trainy,caches,lambd=0)
#gradient_check_n(parameters5,gradients,trainx,trainy,epsilon=1e-7)




#Ytrain = tf.one_hot(ytrain, 7, axis=0)  #(7,1,975)
#sess=tf.Session()
#print(sess.run(Ytrain))
Ytrain = convert_to_one_hot(ytrain, 7)
Ytest = convert_to_one_hot(ytest, 7)
#LINEAR -> RELU -> LINEAR -> RELU -> LINEAR -> SOFTMAX
#layers_dims3 = [1, 20, 12, 7]
parameters = util_tf.model(Xtrain, Ytrain, Xtest, Ytest)


