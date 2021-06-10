import pyrebase
import json

with open("auto.json") as f:
    config = json.load(f)
    
firebase = pyrebase.initialize_app(config)
db = firebase.database()







data_1 = {"name":"apple","price":2000}
db.child("MainData","MainData_1").set(data_1)
data_2 = {"name":"peach","price":4000}
db.child("MainData","MainData_2").set(data_2)