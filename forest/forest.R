x <- read.csv("r_train_data.csv", header=FALSE)
y <- read.csv("r_train_labels.csv", header=FALSE)
x_test <- read.csv("r_test_data.csv", header=FALSE)
y_test <- read.csv("r_test_labels.csv", header=FALSE)

library("randomForest")

model <- randomForest(x, as.factor(y[,1]), ntree=500)

pred <- predict(model, x_test)

pred
