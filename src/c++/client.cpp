#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "192.168.1.100" 
#define SERVER_PORT 12345         

int main() {
    // Abrir arquivo para leitura
    std::ifstream file("sensor_data.txt");
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo.\n";
        return 1;
    }

    std::string sensor_data;
    std::getline(file, sensor_data);
    file.close();

    // Criar socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        std::cerr << "Erro ao criar o socket.\n";
        return 1;
    }

    // Configurar endereço do servidor
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Conectar ao servidor
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Erro ao conectar ao servidor.\n";
        close(client_socket);
        return 1;
    }

    // Enviar dados para o servidor
    if (send(client_socket, sensor_data.c_str(), sensor_data.size(), 0) < 0) {
        std::cerr << "Erro ao enviar os dados.\n";
        close(client_socket);
        return 1;
    }

    std::cout << "Dados enviados: " << sensor_data << "\n";

    // Fechar socket
    close(client_socket);
    return 0;
}
