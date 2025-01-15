import json
import requests

# Dados para envio
data = {
    "sensor_id": 1,
    "humidity": 45.2,
    "temperature": 22.5
}

# URL do servidor/API
url = "http://seu-servidor.com/api/dados"

try:
    # Enviando dados via POST
    response = requests.post(url, json=data)
    if response.status_code == 200:
        print("Dados enviados com sucesso!")
    else:
        print(f"Erro ao enviar dados: {response.status_code} - {response.text}")
except Exception as e:
    print(f"Erro na conexão: {e}")
