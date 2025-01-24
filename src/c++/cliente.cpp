#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <libpq-fe.h>

#define PORT 12345  

using namespace std;

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Falha ao criar o socket" << endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 192.168.15.13 ou qualquer endereço da rede conectada
    if (inet_pton(AF_INET, "192.168.15.3", &serv_addr.sin_addr) <= 0) {
        cerr << "Endereço inválido ou não pode conectar ao servidor" << endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Falha na conexão" << endl;
        return -1;
    }

    read(sock, buffer, sizeof(buffer));
    cout << "Dados recebidos: " << buffer << endl;

    PGconn *conn = PQconnectdb("user=postgres password=postgres host=localhost port=5432");

    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Falha na conexão com o banco de dados: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        return -1;
    }

    string query = "INSERT INTO dados (valor) VALUES (" + string(buffer) + ")";
    PGresult *res = PQexec(conn, query.c_str());

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Falha na execução da query: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        PQfinish(conn);
        return -1;
    }

    cout << "Dados inseridos no banco de dados com sucesso!" << endl;

    PQclear(res);
    PQfinish(conn);

    close(sock);

    return 0;
}
