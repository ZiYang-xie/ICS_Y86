from flask import Flask, request
from flask_cors import CORS
import json
import os
import threading

ip = "localhost" #填写ip
app = Flask("backend")#实例化app对象
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
    args = ['python3', '../Backend/Assembler/yyas', filepath, '-r', '|', '../Backend/build/ICS_Y86', '>', '../Index/static/json/data.json']
    execute_command(args)

def handleYsFile(f):
    f.save('./static/source/task.ys')
    args1 = ['python3', '../Backend/Assembler/yyas', './static/source/task.ys', '>', '../Index/static/source/task.yo']
    execute_command(args1)
    filename_dict = [ {'filename':f.filename + " [编译模式] "} ]
    filename_json = json.dumps(filename_dict)
    with open('./static/json/filename.json', 'w') as name_file:
        name_file.write(filename_json)
    
    args2 = ['python3', '../Backend/Assembler/yyas', './static/source/task.yo', '-r', '|', '../Backend/build/ICS_Y86', '>', '../Index/static/json/data.json']
    execute_command(args2)

def execute_command(args):
    command = ""
    for arg in args:
        command = command + ' ' + arg
    os.system(command)

def run_flask_server():
    app.run(host=ip,
        port=7777,
        debug=True
    )

def run_anywhere():
    os.system('./anywhere')

if __name__ == '__main__':
    run_flask_server()
    