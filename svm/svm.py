import numpy as np
import mlpy

def match(a,b):
	total=0
	less_val=0
	for i in range(len(a)):
		c = a[i]
		d = b[i]
		if(c == 0 or d == 0):
			less_val = less_val + 1
		if(int(c) == int(d)):
			total = total + 1
	return (float(total) / float(len(a)-less_val))

x = np.loadtxt("svm_train.txt", delimiter=',')
y = np.loadtxt("svm_test.txt", delimiter=',')
train_labels = x[:,0]
test_labels = y[:,0]
train_data = x[:,1:-1]
test_data = y[:,1:-1]

#C_val = [1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10]
#gamma_val = [1e1, 1.0, 1e-1, 1e-2, 1e-3, 1e-4, 1e-5, 1e-6, 1e-7, 1e-8]
C_val = [1e8, 1e9, 1e10]
gamma_val = [1e-6, 1e-7, 1e-8]

max_val = 0.0
max_gamma = 0.0
max_C = 0.0

iters = 0
total_iters = len(C_val) * len(gamma_val)
print "In svm.py"

for c in C_val:
	for g in gamma_val:
		iters = iters + 1
		svm = mlpy.LibSvm(svm_type='c_svc', kernel_type='rbf', gamma = g, C = c)
		svm.learn(train_data,train_labels)
		p = svm.pred(test_data)
		#for a in p:
			#print int(a)
		temp = match(p,test_labels)
		print temp
		if(temp > max_val):
			max_val = temp
			max_gamma = g
			max_C = c
print "Max Vals, gamma, C"
print max_val
print max_gamma
print max_C

svm.save_model("svm.model")
