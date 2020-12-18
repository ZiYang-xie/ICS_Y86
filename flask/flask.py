from flask import Flask, request
from flask_cors import CORS
import json
import os
 
app = Flask("glue")#实例化app对象
 
testInfo = {}
CORS(app, resources=r'/*')

@app.route('/post',methods=['GET','POST'])#路由
 

@app.route('/')
def upload():
    if(request.method == "POST"):
        f = request.files['file']
        filepath = './static/source/tmp.yo'
        f.save(filepath)
        args = ['python3', './cpu/striper.py', filepath, '|', './cpu/ICS_Y86', '>', 'data.json']
        for arg in args:
            command = arg + ' '
        os.system(command)
    return 'OK'
 
 
if __name__ == '__main__':
    ip = "127.0.0.1" #填写ip
    app.run(host=ip,#任何ip都可以访问
        port=7777,#端口
        debug=True
    )