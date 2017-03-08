#/usr/bin/python
#coding:utf-8
import threading
import time

gl_num = 0

def show(arg):
    global gl_num
    time.sleep(1)
    gl_num += 1
    print gl_num


for i in range(20):
    t = threading.Thread(target=show, args=(i,))
    t.start()

print 'main thread stop'
