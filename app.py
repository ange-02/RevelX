from flask import Flask, render_template, request
import os
app = Flask(__name__)

@app.route("/")
def home():
    errs = []
    discs = [name for name in os.listdir("systemfolder") if os.path.isfile(os.path.join("systemfolder", name))]
    for disc in discs:
        if os.path.getsize(f"systemfolder/{disc}") == 0:
            errs.append(disc)
    err = {"errz": errs}
    return render_template('home.html', data=err)

@app.route('/delete', methods=['POST'])
def delete():
    output = request.get_json()
    print(output) # This is the output that was stored in the JSON within the browser
    print(type(output))
    os.remove(f"systemfolder/{output['disk']}.txt")
    open(f"systemfolder/{output['disk']}.txt", 'x')
    return output

if __name__ == '__main__':
   app.run()