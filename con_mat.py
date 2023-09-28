import numpy as np
import matplotlib.pyplot as plt
import os
import sys
import argparse

f=open(sys.argv[1], 'r+')
lines=f.readlines()
values = [line.split()for line in lines]
n1=0
n2=0

for value in values:
	if value[1] == '0':
		n1=n1+1
	elif value[1] == '1':
		n2=n2+1
f.close()
		
f=open(sys.argv[2], 'r+')
lines=f.readlines()
values = [line.split()for line in lines]
s1=0
s2=0

for value in values:
	if value[1] == '0':
		s1=s1+1
	elif value[1] == '1':
		s2=s2+1
f.close

path = str(sys.argv[3])
title = str(sys.argv[4])

classes = ['Neutral','Selection']
confusion_matrix = np.array([(n1,n2),(s1,s2)], dtype=np.float64)

plt.imshow(confusion_matrix, interpolation='nearest', cmap=plt.cm.Blues, vmin=0, vmax=100)
plt.title(sys.argv[4], fontsize=20)
plt.colorbar()
tick_marks = np.arange(len(classes))
plt.xticks(tick_marks, classes)
plt.yticks(tick_marks, classes, rotation=90)

thresh = confusion_matrix.max() / 2
iters = np.reshape([[[i,j] for j in range(2)] for i in range(2)], (confusion_matrix.size,2))
for i,j in iters:
	plt.text(j, i, format(confusion_matrix[i,j]), ha="center", va="center", fontsize=20)

plt.savefig('{path}/{title}.png'.format(path=path, title=title))	


