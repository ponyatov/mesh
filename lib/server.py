from flask import Flask, render_template

app = Flask(__name__,template_folder='templates')

@app.route('/')
def index():
    return render_template('html.html')

@app.route('/css.css')
def css():
    return render_template('css.css')

@app.route('/js.js')
def js():
    return render_template('js.js')

if __name__ == '__main__':
    app.run(debug=True)
