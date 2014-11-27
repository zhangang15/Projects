__author__ = 'ang'

from Client import Store
import leveldb
import time
import pymongo


con = pymongo.Connection("10.0.0.218", 27999)
db = con.mydb
mongo_time = time.time()
user = db.user
for i in range(1000):
    user.save({'id': 1, 'key': str(i), 'value': str(i)})
print "mongoDB  uses time : " + str(time.time() - mongo_time)

s = Store(['10.0.0.218:2181'])
t = time.time()
for i in range(1000):
    s.put(str(i), str(i))

print "my levelDB uses time : " + str(time.time() - t)

print"done"
