from flask import Flask, request, jsonify
import psycopg2

app = Flask(__name__)

# Configurações do Banco de Dados PostgreSQL
DB_CONFIG = {
    'user': 'postgres',
    'password': 'postgres',
    'host': 'localhost',
    'port': 5432
}

def insert_data(sensor_id, humidity, temperature):
    try:
        connection = psycopg2.connect(**DB_CONFIG)
        cursor = connection.cursor()

        # Query para inserir dados
        query = """
            INSERT INTO sensores (sensor_id, humidity, temperature, timestamp)
            VALUES (%s, %s, %s, CURRENT_TIMESTAMP)
        """
        cursor.execute(query, (sensor_id, humidity, temperature))
        
        connection.commit()
        cursor.close()
        connection.close()
    except psycopg2.Error as e:
        raise Exception(f"Erro ao inserir dados no banco: {e}")

@app.route('/api/dados', methods=['GET','POST'])

def receive_data():
    data = request.get_json()
    try:
        # Validando e salvando os dados
        insert_data(data['sensor_id'], data['humidity'], data['temperature'])
        return jsonify({"status": "sucesso"}), 200
    except Exception as e:
        return jsonify({"erro": str(e)}), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8000)
