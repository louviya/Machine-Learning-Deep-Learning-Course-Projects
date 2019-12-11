import os
import pandas as pd
import numpy as np
from sklearn.impute import SimpleImputer
from sklearn.preprocessing import OneHotEncoder
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import Normalizer
from sklearn.preprocessing import StandardScaler, MinMaxScaler
from sklearn.neighbors import KNeighborsClassifier
from sklearn.ensemble import RandomForestClassifier

#read data
cwd = os.getcwd()
filename = 'melb_data.csv'
file = os.path.join(cwd,filename)
data = pd.read_csv(file, sep=',')   #(13580,21)
#print(data)
stat = data.describe()  #13 columns of numerial and others are non-numerial
#print(data.columns)
#'Suburb0','Address1','Type3','Method5','SellerG6', 'Date7', 'Postcode9','CouncilArea16', 'Regionname19'
#from stat, the columns missing data are'car12','buildingArea14','YearBuilt15', 'CouncilArea16'
#seperate data
nonnum = data.iloc[:,[0,3,5,6,9,16,19]]  #the address and datetime are not included as non-numerial attributes
num = data.iloc[:,[2,8,10,11,12,13,14,15,17,18,20]].astype('float')
label = data.iloc[:,[4]].astype('float')


#part 1 filling
my_imputer = SimpleImputer(strategy='mean') #median, most_common, constant
numfill = my_imputer.fit_transform(num) #filling the missing values
normsd = StandardScaler().fit(numfill)  #normalization
numnorm = normsd.transform(numfill)
#test different ways
normmm = MinMaxScaler().fit(numfill)
numnorm2 = normmm.transform(numfill)
norml2 = Normalizer(norm='l2')  #normalization with L1 distance
numnorm3 = norml2.fit_transform(numfill)
numnorm4 = (numfill-numfill.min(axis=0))/(numfill.max(axis=0)-numfill.min(axis=0))



#part 2 one-hot
nonnumtrans = nonnum.apply(lambda x:pd.Categorical(x).codes).astype('float')
statnon = nonnumtrans.describe() 
enc = OneHotEncoder(categories='auto')
nonnumonehot = enc.fit_transform(nonnumtrans)
nominal = nonnumonehot.toarray()  #13580*830
norml1 = Normalizer(norm='l1')  #normalization with L1 distance
nominalnorm = norml1.fit_transform(nominal)


#part 3 sort samples by the prices
x = np.hstack((numnorm,nominalnorm))  #training set
# x = numnorm.copy()
# x = numnorm2.copy()
# x = numnorm3.copy()
# x = numnorm4.copy()
stat2 = label.describe()  #label set
lmax = label.max()
lmin = label.min()
s1 = (lmax-lmin)/5
s2 = 2*(lmax-lmin)/5
s3 = 3*(lmax-lmin)/5
s4 = 4*(lmax-lmin)/5
#top value = 0, high value=1, medium value=2, low value=3, bottom value=4
yy= label.copy()
yy[yy>s4] = 4
yy[yy>s3]= 3
yy[yy>s2]= 2
yy[yy>s1]= 1
yy[yy>4] = 0
stat3 = yy.describe()
y = np.array(yy)


#from sklearn.feature_selection import SelectKBest, chi2
#xnew= SelectKBest(chi2,k=2).fit_transform(np.abs(x), y)


#splitting
xTrain, xTest, yTrain, yTest = train_test_split(x, y, test_size = 0.34, random_state = 0)
#KNN
from sklearn.metrics import f1_score,precision_score, recall_score, confusion_matrix, accuracy_score, classification_report, roc_curve, auc
#from sklearn.model_selection import cross_validate
import matplotlib.pyplot as plt 
knn = KNeighborsClassifier(n_neighbors=5)
knn.fit(xTrain, yTrain)
yPredict=knn.predict(xTest)
yPredict=yPredict.reshape(4618,1).astype('int')
yTest=yTest.astype('int')
probility=knn.predict_proba(xTest) 
score=knn.score(xTest,yTest,sample_weight=None)
print('Prediction：',yPredict,'Actural：',yTest,'score：',score)
cm = confusion_matrix(yTest, yPredict)
precision = precision_score(yTest, yPredict, average='macro')
recall = recall_score(yTest, yPredict, average='macro')
f1 = f1_score(yTest, yPredict, average='macro')
accuracy = accuracy_score(yTest, yPredict)

fpr, tpr, thresholds = roc_curve(yTest, yPredict, pos_label=2)
aucnum = auc(fpr,tpr)
report = classification_report(yTest, yPredict)
plt.figure()
plt.title('Receiver Operating Characteristic')
plt.plot(fpr, tpr, 'b', label='AUC = %0.2f'% aucnum)
plt.legend(loc='lower right')
plt.plot([0,1],[0,1],'r--')
plt.xlim([0.0,1.0])
plt.ylim([0.0,1.0])
plt.ylabel('True Positive Rate')
plt.xlabel('False Positive Rate')
plt.show()




#random forest
rf = RandomForestClassifier(random_state=0, oob_score=True)
#score2 = cross_validate(rf,xTrain,yTrain,cv=10)
rf.fit(xTrain, yTrain)
yPredict2=rf.predict(xTest)
cm2 = confusion_matrix(yTest, yPredict2)
precision2 = precision_score(yTest, yPredict2, average='macro')
recall2 = recall_score(yTest, yPredict2, average='macro')
f12 = f1_score(yTest, yPredict2, average='macro')
accuracy2 = accuracy_score(yTest, yPredict2)
print(rf.oob_score_)
fpr2, tpr2, thresholds2 = roc_curve(yTest, yPredict2, pos_label=2)
aucnum2 = auc(fpr2,tpr2)
report2 = classification_report(yTest, yPredict2)


