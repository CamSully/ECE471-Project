# ECE 471 - Derek H, Cam S, Dustin K - Final Project Code - web.py
# This code runs the Flask web server.

from flask import Flask, render_template, url_for

app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])

def index():
	
	# Web code is in index.html
	return render_template('index.html')

if __name__ == '__main__':

	# Run the web server! It's threaded to improve speed significantly.
	app.run(debug = True, host = '0.0.0.0', threaded=True)


