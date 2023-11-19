from ucimlrepo import fetch_ucirepo
from sklearn.preprocessing import LabelEncoder
from pyspark.sql import SparkSession
from pyspark.ml.feature import VectorAssembler
from pyspark.ml.clustering import KMeans


FEATURE_NAMES = ["age","workclass_code","fnlwgt","education-num","marital_code","occupation_code","relationship_code","race_code","sex_code","capital-gain","capital-loss","hours-per-week","native_country_code"]

if __name__ == "__main__":
    spSesh = SparkSession.builder.appName("Clustering").getOrCreate() 
    LE=LabelEncoder()

    adult = fetch_ucirepo(id=2) 
    X = adult.data.features

    X['workclass_code'] = LE.fit_transform(X['workclass'])
    X['marital_code'] = LE.fit_transform(X['marital-status'])
    X['occupation_code'] = LE.fit_transform(X['occupation'])
    X['relationship_code'] = LE.fit_transform(X['relationship'])
    X['race_code'] = LE.fit_transform(X['race'])
    X['sex_code'] = LE.fit_transform(X['sex'])
    X['native_country_code'] = LE.fit_transform(X['native-country'])

    X = X[FEATURE_NAMES]
    X = spSesh.createDataFrame(X)

    assembler = VectorAssembler(inputCols=FEATURE_NAMES, outputCol="features")
    a_df = assembler.transform(X)

    #model = KMeans().setK(3).setSeed(1).fit(a_df)
    #predictions = model.transform(a_df)
    #predictions.select("prediction").show()
    
    kmeans = KMeans(
        k=3, maxIter=20, seed=1
    )
    kmeans = kmeans.fit(a_df)

    print("Centers:", kmeans.clusterCenters())


    #print(X)
    spSesh.stop()
