import json
import requests
import time

# Dados para envio
data = {
    "sensor_id": 1,
    "humidity": 45.2,
    "temperature": 22.5
}

# URL do servidor/API
url = "http://127.0.0.1:8000/api/dados"

while (1):
    try:
        # Enviando dados via POST
        response = requests.post(url, json=data)
        #response = requests.post(url, json=payload, headers=headers)

        if response.status_code == 200:
            print("Dados enviados com sucesso!")
        else:
            print(f"Erro ao enviar dados: {response.status_code} - {response.text}")

        time.sleep(10)
    except Exception as e:
        print(f"Erro na conexão: {e}")
