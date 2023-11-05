from ucimlrepo import fetch_ucirepo 
from sklearn.cluster import KMeans  
from sklearn.preprocessing import LabelEncoder
LE=LabelEncoder()

adult = fetch_ucirepo(id=2) 
X = adult.data.features

X['workclass_code'] = LE.fit_transform(X['workclass'])
X['marital_code'] = LE.fit_transform(X['marital-status'])
X['occupation_code'] = LE.fit_transform(X['occupation'])
X['relationship_code'] = LE.fit_transform(X['relationship'])
X['race_code'] = LE.fit_transform(X['race'])
X['sex_code'] = LE.fit_transform(X['sex'])

X = X[["age","workclass_code","fnlwgt","education-num","marital_code","occupation_code","capital-gain","capital-loss","hours-per-week","relationship_code","race_code","sex_code"]]
#print(X)

kmeans = KMeans(n_init='auto',n_clusters=3).fit(X)

print("Cluster Centers :", kmeans.cluster_centers_)
print("Sum Squared Error :", kmeans.inertia_)
