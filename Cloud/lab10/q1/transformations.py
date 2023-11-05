from ucimlrepo import fetch_ucirepo
from pyspark.sql import SparkSession
from pyspark.sql.functions import col, when

def p1(X):
    xusa = X.filter(X["native-country"] != "United-States")
    xusa_adults = xusa.filter(X["age"] > 18)
    count_country = xusa_adults.groupBy("native-country").agg({"age":"count"})
    max_count_row = count_country.orderBy(col("count(age)").desc()).first()
    return max_count_row["native-country"]

def p2(X):
    atleast_masters = X.filter((X["education"]=="Masters") | (X["education"]=="Doctorate"))
    tech_support = atleast_masters.filter(atleast_masters["occupation"] == "Tech-Support")
    return atleast_masters.count()

def p3(X):
    unmarried = X.filter((X["marital-status"] == "Divorced") | (X["marital-status"] == "Never-married"))
    males = unmarried.filter((unmarried["sex"] == "Male"))
    local_govt = males.filter((males["workclass"] == "Local-gov"))
    return local_govt.count()

if __name__ == "__main__":
    spSesh = SparkSession.builder.appName("Clustering").getOrCreate()


    adult = fetch_ucirepo(id=2) 
    X = adult.data.features

    X = spSesh.createDataFrame(X)

    print(p1(X))
    print(p2(X))
    print(p3(X))

    

