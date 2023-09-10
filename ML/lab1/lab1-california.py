
import numpy as np
import matplotlib.pyplot as plt
from sklearn import datasets,model_selection

def gradientDescent(y_exp, y_pred,x, n):
    error = y_exp-y_pred
    slope_gradient = (-1/2*n)* np.dot(x.T, error)
    intercept_gradient =  (-1/2*n)*np.sum(error)
    return slope_gradient, intercept_gradient   

def predict(x, slope, intercept):
    return np.dot(x, slope) + intercept

def train(x,y,slope,intercept,learningRate, iterations, n):
    for _ in range(iterations):
        y_pred = predict(x,slope,intercept)
        slope_gradient, intercept_gradient = gradientDescent(y,y_pred, x,n)

        slope -= slope_gradient*learningRate
        intercept -= intercept_gradient*learningRate
        # print(slope, intercept)

    return slope, intercept

california = datasets.fetch_california_housing()
x = california["data"]
y = california["target"]

X_train, X_test, y_train, y_test = model_selection.train_test_split(x, y, test_size=0.001)

n = X_train.shape[0]
y_train = np.log1p(y_train)
y_test = np.log1p(y_test)

slope = np.zeros(X_train.shape[1])
intercept = np.zeros(1)
learningRate = 0.0000000000000005
iterations = 20000
slope, intercept = train(X_train, y_train,slope,intercept, learningRate, iterations, n)
y_pred = predict(X_test,slope,intercept)
mse = np.mean((y_test - y_pred)**2)

print("MSE:", mse)
plt.scatter(y_test, y_pred, color='blue', label='Predicted vs. Actual')
plt.plot([min(y_test),max(y_test)],[min(y_test),max(y_test)],color='red', linewidth='2')
plt.show()



 


