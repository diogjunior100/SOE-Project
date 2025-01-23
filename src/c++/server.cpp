#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <libpq-fe.h>

#define PORT 12345
#define BUFFER_SIZE 1024

using namespace std;

// Função para atualizar o banco de dados PostgreSQL
void update_database(const string& data) {
    const char* conninfo = "user=postgres password=postgres hostaddr=127.0.0.1 port=5432";
    PGconn* conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Erro ao conectar ao banco de dados: " << PQerrorMessage(conn) << "\n";
        PQfinish(conn);
        return;
    }

    string query = "INSERT INTO sensor_data (value) VALUES ('" + data + "');";
    PGresult* res = PQexec(conn, query.c_str());

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Erro ao executar a query: " << PQerrorMessage(conn) << "\n";
    } else {
        cout << "Dado inserido no banco de dados: " << data << "\n";
    }

    PQclear(res);
    PQfinish(conn);
}

int main() {
    // Criar socket do servidor
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        cerr << "Erro ao criar o socket.\n";
        return 1;
    }

    // Configurar endereço do servidor
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind do socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "Erro ao fazer o bind.\n";
        close(server_socket);
        return 1;
    }

    // Escutar por conexões
    if (listen(server_socket, 5) < 0) {
        cerr << "Erro ao escutar no socket.\n";
        close(server_socket);
        return 1;
    }

    cout << "Servidor aguardando conexões na porta " << PORT << "...\n";

    // Aceitar conexão do cliente
    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket < 0) {
        cerr << "Erro ao aceitar a conexão.\n";
        close(server_socket);
        return 1;
    }

    cout << "Conexão aceita.\n";

    // Receber dados do cliente
    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        cerr << "Erro ao receber os dados.\n";
    } else {
        string received_data(buffer, bytes_received);
        cout << "Dados recebidos: " << received_data << "\n";

        // Atualizar banco de dados
        update_database(received_data);
    }

    // Fechar sockets
    close(client_socket);
    close(server_socket);

    return 0;
}
