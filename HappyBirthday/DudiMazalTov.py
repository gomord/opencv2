import numpy as np
import cv2
import time
import random
age = 30
img_len = 2**9
img_size = (img_len,img_len*2,3)
greds = ["mazal tov moti!!","we love you!!"]

# Create a black image
img = np.zeros(img_size, np.uint8)
temp = np.zeros(img_size, np.uint8)
#Draw a diagonal blue line with thickness of 5 px
#img = cv2.line(img,(0,0),(511,511),(255,0,0),5)
cv2.namedWindow('image', cv2.CV_WINDOW_AUTOSIZE)
def centerText(img,st,scale):
    # point h,w
    point, base = cv2.getTextSize(st, cv2.FONT_HERSHEY_COMPLEX,scale, 10);
    w = len(img)
    h = len(img[0])
    cw = (w + point[1])/2
    ch = (h - point[0])/2
    return ch,cw

for i in range(0,age,1):

    for j in range(15):
        temp = img.copy()
        cv2.putText(temp,str(i), centerText(temp,str(i),j), \
                    cv2.FONT_HERSHEY_SIMPLEX, j, 255,10)
        #print centerText(temp,str(i),j)
        cv2.imshow('image',temp)
        cv2.waitKey(4)
        
    for j in range(15,0,-1):
        temp = img.copy()
        cv2.putText(temp,str(i), centerText(temp,str(i),j), \
                    cv2.FONT_HERSHEY_SIMPLEX, j, 255,10)
        
        cv2.imshow('image',temp)
        cv2.waitKey(4)
temp = img.copy()
for i in range(100):
    radius = random.randint(0,150)
    center = (random.randint(0,img_len*2),random.randint(0,img_len))
    cv2.circle(temp,center, radius, \
               (random.randint(0,255),random.randint(0,255),random.randint(0,255)),-1)
    img = temp.copy()
    cv2.putText(img,str(age), centerText(temp,str(age),15), \
                    cv2.FONT_HERSHEY_SIMPLEX, 15, 255,10)
    cv2.imshow('image',img)
    cv2.waitKey(70)
#temp = img.copy()
for i in range(4):
    img = temp.copy()
    for j in range(600):
        cv2.putText(img,greds[i%len(greds)], centerText(temp,greds[i%len(greds)],3), \
                            cv2.FONT_HERSHEY_SIMPLEX, 3,\
                    cv2.cvtColor(np.uint8([[[j % 255,230,150 ]]]),cv2.COLOR_HSV2RGB)[0][0].tolist(),10)
        cv2.imshow('image',img)
        cv2.waitKey(5)
cv2.waitKey(200)
    #time.sleep(0.5)
cv2.destroyAllWindows()
