#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 12345  
using namespace std;

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    const char *message = "10"; 

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Falha ao criar o socket");
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // Aceitar qualquer endereço de IP
    address.sin_port = htons(PORT); 

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Falha ao fazer bind");
        return -1;
    }

    while (1){
        if (listen(server_fd, 3) < 0) {
            perror("Falha ao escutar");
            return -1;
        }

        cout << "Aguardando conexão..." << endl;

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Falha ao aceitar a conexão");
            return -1;
        }

        cout << "Conexão estabelecida!" << endl;

        send(new_socket, message, strlen(message), 0);
        cout << "Dados enviados: " << message << endl;
    }

    close(new_socket);
    close(server_fd);

    return 0;
}