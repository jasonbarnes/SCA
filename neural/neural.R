x <- read.csv("r_train_data.csv", header=FALSE)
y <- read.csv("r_train_labels.csv", header=FALSE)
x_test <- read.csv("r_test_data.csv", header=FALSE)
y_test <- read.csv("r_test_labels.csv", header=FALSE)

library("nnet")

model <- nnet(x, y, size=100, MaxNWts=10000)

pred <- predict(model, x_test)

pred
