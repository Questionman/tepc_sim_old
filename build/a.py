import operator
import csv
import string
#import matplotlib
f=open("jihong_data_nt_step.csv","r")
rdr = csv.reader(f)
dic =dict()
for i in range(361) :
  dic[i]=0
#:print(dic)


for line in rdr :  #  print(float(line[-1]))
  dic[int(line[0])]+=float(line[-1])
     
print(sorted(dic.items(),key=operator.itemgetter(0)))
wr = csv.writer(f)
f.close()
'''


#plt.plot(dic)
#plt.show()
#f.close()
'''
