#/bin/python
#coding:utf-8
import time
import threading

gl_num = 0
lock = threading.RLock()

#调用acquire([timeout])时，线程将一直阻塞
#直到获得锁定或者直到timeout秒后(timeout参数可选)
#返回是否获得锁
def Func():
    lock.acquire()
    global gl_num
    gl_num += 1
    time.sleep(1)
    print gl_num
    lock.release()

for i in range(20):
    t = threading.Thread(target=Func)
    t.start()

print 'main thread stop'
