from flask import Flask, request
from flask_cors import CORS
import json
import os
 
app = Flask("glue")#实例化app对象
 
CORS(app, resources=r'/*')

@app.route('/post',methods=['GET','POST'])#路由

@app.route('/')
def upload():
    if(request.method == "POST"):
        f = request.files['file']
        if(f.filename[-2:] == "yo"):
            handleYoFile(f)
            return 'Upload Yo Success'
        elif(f.filename[-2:] == "ys"):
            handleYsFile(f)
            return 'Upload Ys Success'
 
def handleYoFile(f):
    filepath = './static/source/task.yo'
    f.save(filepath)
    filename_dict = [ {'filename':f.filename} ]
    filename_json = json.dumps(filename_dict)
    with open('./static/json/filename.json', 'w') as name_file:
        name_file.write(filename_json)
    args = ['python3', './cpu/Assembler/assembler.py', filepath, '-r', '|', './cpu/ICS_Y86', '>', './static/json/data.json']
    command = ""
    for arg in args:
        command = command + ' ' + arg
    os.system(command)

def handleYsFile(f):
    filepath = './static/source/task.ys'
    f.save(filepath)
    filename_dict = [ {'filename':f.filename + " [编译模式] "} ]
    filename_json = json.dumps(filename_dict)
    with open('./static/json/filename.json', 'w') as name_file:
        name_file.write(filename_json)
    args = ['python3', './cpu/Assembler/assembler.py', filepath, '-r', '|', './cpu/ICS_Y86', '>', './static/json/data.json']
    command = ""
    for arg in args:
        command = command + ' ' + arg
    os.system(command)

 
if __name__ == '__main__':
    ip = "localhost" #填写ip
    app.run(host=ip,#任何ip都可以访问
        port=7777,#端口
        debug=True
    )