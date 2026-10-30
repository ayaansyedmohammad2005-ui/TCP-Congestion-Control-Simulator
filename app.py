import subprocess
import csv
from flask import Flask, render_template, request, jsonify

app = Flask(__name__)

def read_csv(filename):
    data = []
    try:
        with open(filename, 'r') as file:
            reader = csv.DictReader(file)
            for row in reader:
                data.append({
                    'round': int(row['Round']),
                    'cwnd': int(row['cwnd']),
                    'ssthresh': int(row['ssthresh'])
                })
    except FileNotFoundError:
        pass
    return data

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/run', methods=['POST'])
def run_simulation():
    req = request.get_json()
    packets = req.get('packets', 100)
    loss = req.get('loss', 10)

    result = subprocess.run(
        ['simulator.exe', str(packets), str(loss)],
        capture_output=True,
        text=True,
        check=True
    )

    tahoe_data = read_csv('tahoe_data.csv')
    reno_data = read_csv('reno_data.csv')

    return jsonify({
        'status': 'success',
        'tahoe': tahoe_data,
        'reno': reno_data,
        'logs': result.stdout
    })

if __name__ == '__main__':
    app.run(debug=True, port=5000)