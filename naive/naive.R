x <- read.csv("r_train_data.csv", header=FALSE)
y <- read.csv("r_train_labels.csv", header=FALSE)
x_test <- read.csv("r_test_data.csv", header=FALSE)
y_test <- read.csv("r_test_labels.csv", header=FALSE)

library("e1071")

model <- naiveBayes(x, as.factor(y[,1]))
pred <- predict(model, x_test)
write.table(pred, file="naive.out", append=FALSE, quote=FALSE, row.names=FALSE, col.names=FALSE)
