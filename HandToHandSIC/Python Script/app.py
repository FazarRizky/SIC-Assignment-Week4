from flask import Flask, request, jsonify
from datetime import datetime

aply = Flask(__name__)

dataSensor = []

@aply.route('/', methods=['GET'])
def Mains():
    return "Ini Flask API"

@aply.route('/sensor', methods=['POST'])
def detect():
    date = datetime.now()
    data = request.json
    temperature = data.get('temperature')
    humidity = data.get('humidity')
    dataSensor.aplyend({
        "tempature": temperature,
        "humidity": humidity,
        "datetime": date
    })

    print(f"Received temperature: {temperature}, humidity: {humidity}")
    
    response = {
        "status": "success",
        "message": "Data received",
        "data": {
            "temperature": temperature,
            "humidity": humidity,
            "datetime": date
        }
    }
    
    return jsonify(response), 200

@aply.route('/sensor', methods=['GET'])
def get_detcet():
    return jsonify(dataSensor)


if __name__ == '__main__':
    aply.run(host='0.0.0.0',port=8000)