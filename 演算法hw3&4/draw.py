import matplotlib.pyplot as plt

filename = input("Please enter the data you want to test(dt4,dt5,dt6,ext):")
path=".\\"+filename+".\\point.txt"

if(filename=="ext"):
    path=".\\extra.\\point.txt"

f = open(path,'r')

x_origin=[]
y_origin=[]
for line in f.readlines():
    s=line.split(' ')
    x_origin.append(int(s[1]))
    y_origin.append(int(s[2]))

f.close()

#######
X=[]
Y=[]
anspath="ans_"+filename+".txt"
f=open(anspath,'r')
f.readline()

start=f.readline()
#print(start)
X.append(x_origin[int(start)-1])
Y.append(y_origin[int(start)-1])

for line in f.readlines():
    X.append(x_origin[int(line)-1])
    Y.append(y_origin[int(line)-1])

X.append(x_origin[int(start)-1])
Y.append(y_origin[int(start)-1])

f.close()

# # #製作 figure object
fig = plt.figure()


#plt.xlim(0,120)
#plt.ylim(0,120)

# #把ax這個object設定成figure object
ax = fig.add_subplot(1, 1, 1)  #這邊的(1, 1, 1)分別代表（行，列，場所）

# #設定ax散佈圖
ax.scatter(X, Y, color = 'blue',s=10)
ax.plot(X, Y, color = 'blue',linewidth=0.5)
# #設定圖例
# ax.legend(['Data_1'])

# #設定散佈圖名稱
ax.set_title(filename)
# #顯示散佈圖
plt.show()