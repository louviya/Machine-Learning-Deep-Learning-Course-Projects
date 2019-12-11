#RandomForest分类
from sklearn.model_selection import cross_val_score
from sklearn.datasets import load_iris
from sklearn.datasets import load_breast_cancer
from sklearn.ensemble import RandomForestClassifier
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

iris = load_iris()
df = pd.DataFrame(iris.data, columns=iris.feature_names)
df['is_train'] = np.random.uniform(0, 1, len(df)) <= .75
df['species'] = pd.Categorical.from_codes(iris.target, iris.target_names)  #not use Factor
df.head()

train, test = df[df['is_train']==True], df[df['is_train']==False]

features = df.columns[:4]
clf = RandomForestClassifier(n_jobs=2,oob_score=True)
y, _ = pd.factorize(train['species'])
clf.fit(train[features], y)

preds = iris.target_names[clf.predict(test[features])]
print(pd.crosstab(test['species'], preds, rownames=['actual'], colnames=['preds']))
#print(clf.score(test,preds))
print(clf.oob_score_)



cancer = load_breast_cancer()
rf = RandomForestClassifier(random_state=10)
score = cross_val_score(rf,cancer.data,cancer.target,cv=10).mean()
print(score)  #0.9580

scores = []  #try to find the range of best n_estimators
for i in range(0,200,10):
    rf = RandomForestClassifier(n_estimators=i+1,random_state=10)
    score = cross_val_score(rf,cancer.data,cancer.target,cv=10).mean()
    scores.append(score)
print(max(scores))
plt.plot(range(1,201,10),scores)
plt.show()  #it is better to have the number of trees between 10 and 25

#n_estimators的学习曲线  最终确定
scores_2 = []
for i in range(10,25):
    rf = RandomForestClassifier(n_estimators=i+1,random_state=10)
    score = cross_val_score(rf,cancer.data,cancer.target,cv=10).mean()
    scores_2.append(score)
print(max(scores_2),scores_2.index(max(scores_2))+10+1)  #最后确定了是21棵树的时候模型效果最好
plt.plot(range(11,26),scores_2)
plt.show()

#接下来就进入网格搜索，我们将使用网格搜索对参数一个个进行调整。如果知道大致趋势那么使用学习曲线来确定范围和趋势，否则使用网格搜索比较好。
from sklearn.model_selection import GridSearchCV
grid_params = {"max_depth":np.arange(1,11,1)} #for each parameter
rf = RandomForestClassifier(n_estimators=21,random_state=20)
grid = GridSearchCV(rf,grid_params,cv = 10)
grid.fit(cancer.data,cancer.target)
print(grid.best_params_,grid.best_score_) #发现其实并没有有所提升，说明了调整了n_estimators后模型可能就几乎达到了最佳复杂度


from skmultilearn.dataset import load_dataset
from skmultilearn.problem_transform import BinaryRelevance
from sklearn.svm import SVC
import sklearn.metrics as metrics
X_train, y_train, feature_names, label_names = load_dataset('emotions', 'train')
X_test, y_test, _, _ = load_dataset('emotions', 'test')
clf = BinaryRelevance(classifier=SVC(), require_dense=[False, True])
clf.fit(X_train, y_train)
prediction = clf.predict(X_test)
metrics.hamming_loss(y_test, prediction)
metrics.accuracy_score(y_test, prediction)  #0.14356
print(y_test.shape)
print(np.unique(y_train.rows).shape, np.unique(y_test.rows).shape)

from skmultilearn.adapt import MLkNN
from sklearn.model_selection import GridSearchCV

parameters = {'k': range(1,3), 's': [0.5, 0.7, 1.0]}
clf = GridSearchCV(MLkNN(), parameters, scoring='f1_macro')
clf.fit(X_train, y_train)
print (clf.best_params_, clf.best_score_)


from skmultilearn.problem_transform import ClassifierChain, LabelPowerset
from sklearn.model_selection import GridSearchCV
from sklearn.naive_bayes import GaussianNB
from sklearn.ensemble import RandomForestClassifier
from skmultilearn.cluster.networkx import NetworkXLabelGraphClusterer
from skmultilearn.cluster import LabelCooccurrenceGraphBuilder
from skmultilearn.ensemble import LabelSpacePartitioningClassifier
from sklearn.svm import SVC

parameters = {
    'classifier': [LabelPowerset(), ClassifierChain()],
    'classifier__classifier': [RandomForestClassifier()],
    'classifier__classifier__n_estimators': [10, 20, 50],
    'clusterer' : [
        NetworkXLabelGraphClusterer(LabelCooccurrenceGraphBuilder(weighted=True, include_self_edges=False), 'louvain'),
        NetworkXLabelGraphClusterer(LabelCooccurrenceGraphBuilder(weighted=True, include_self_edges=False), 'lpa')
    ]
}

clf = GridSearchCV(LabelSpacePartitioningClassifier(), parameters, scoring = 'f1_macro')
clf.fit(X_train, y_train)

print (clf.best_params_, clf.best_score_)







#至少KNN是可以的
#from sklearn.neighbors import KNeighborsClassifier
#y_train_large = (y_train >= 7)
#y_train_odd = (y_train % 2 == 1)
#y_multilabel = np.c_[y_train_large, y_train_odd]
#knn_clf = KNeighborsClassifier()
#knn_clf.fit(f_norm, tlabels)

from skmultilearn.dataset import load_dataset
from skmultilearn.problem_transform import BinaryRelevance
from sklearn.svm import SVC
import sklearn.metrics as metrics
import sklearn.model_selection 
X_train, X_test, y_train, y_test = sklearn.model_selection.train_test_split(f_norm, tlabels, test_size=0.2, random_state=1)
tt = y_train.astype('int')
ttest = y_test.astype('int')
clf = BinaryRelevance(classifier=SVC(), require_dense=[False, True])
clf.fit(X_train, tt)
prediction = clf.predict(X_test)
metrics.hamming_loss(ttest, prediction)
metrics.accuracy_score(ttest, prediction) 
#Hamming loss measures how well the classifier predicts each of the labels, averaged over samples, then over labels
#accuracy score measures how well the classifier predicts label combinations, averaged over samples
#jaccard similarity measures the proportion of predicted labels for a sample to its correct assignment, averaged over samples
#precision measures how many samples with ,
#recall measures how many samples ,
#F1 score measures a weighted average of precision and recall, where both have the same impact on the score
from skmultilearn.adapt import MLkNN
classifier = MLkNN(k=3)
prediction = classifier.fit(X_train, y_train).predict(X_test)
metrics.hamming_loss(y_test, prediction)
