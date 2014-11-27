__author__ = 'azhang5'

from Client import Store
import leveldb

# #
db1 = leveldb.LevelDB("10.0.0.218:27000")
db2 = leveldb.LevelDB("10.0.0.222:27001")
db3 = leveldb.LevelDB("10.0.0.223:27002")

test_value = True

for i in range(1000):
    if db1.Get(str(i)) != str(i):
        print "In DB1, they are not equal!"
        break
        test_value = False
    if db2.Get(str(i)) != str(i):
        print "In DB2, they are not equal!"
        break
        test_value = False
    if db3.Get(str(i)) != str(i):
        print "In DB3, they are not equal!"
        break
        test_value = False
print "They are all equal." + "The test result is " + str(test_value)
