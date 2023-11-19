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
    phd_and50 = X.filter((X["education"]=="Doctorate") & (X["income"]==">50K"))
    print(phd_and50.groupBy("occupation").agg({"age":"count"}).orderBy(col("count(age)").desc()).first()["occupation"])
    masters = X.filter((X["education"]=="Bachelors") | (X["education"]=="Masters") | (X["education"]=="Doctorate"))
    married = masters.filter((masters["marital-status"] == "Married-civ-spouse") | (masters["marital-status"] == "Married-civ-spouse") | (masters["marital-status"] == "Married-AF-spouse"))
    married_machine_bachelors = married.filter((married["occupation"] == "Machine-op-inspct"))
    return married_machine_bachelors.count()

def p3(X):
    unmarried = X.filter((X["marital-status"] == "Divorced") | (X["marital-status"] == "Never-married"))
    males = unmarried.filter((unmarried["sex"] == "Male"))
    local_govt = males.filter((males["workclass"] == "Local-gov"))
    return local_govt.count()

def p4(X, top_n=3):
    # Group by education level and calculate the average hours worked per week
    avg_hours_by_education = X.groupBy("education").agg({"hours-per-week": "avg"})
    
    # Order by average hours in descending order and select the top N
    top_education_levels = avg_hours_by_education.orderBy(col("avg(hours-per-week)").desc()).limit(top_n)
    
    return top_education_levels

def p5(X):
    # Filter individuals working in "Sales" roles
    sales_data = X.filter(X["occupation"] == "Sales")

    # Calculate the average age broken down by gender and native country
    avg_age_by_sales = sales_data.groupBy("sex", "native-country").agg({"age": "avg"})

    # Identify the top three native countries where the average age of male Sales workers is the highest
    top_countries_male_sales = (
        avg_age_by_sales.filter(avg_age_by_sales["sex"] == "Male")
        .orderBy(col("avg(age)").desc())
        .limit(3)
    )

    return avg_age_by_sales, top_countries_male_sales
if __name__ == "__main__":
    spSesh = SparkSession.builder.appName("Clustering").getOrCreate()


    adult = fetch_ucirepo(id=2) 
    Y = adult.data.target
    X = adult.data.original

    X = spSesh.createDataFrame(X)
    print(p5(X)[1].show())


    

