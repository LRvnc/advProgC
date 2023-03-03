#include "base.h"
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>

int main (int argc, char* argv[]) {

    // Client connection
    int client_socket = clientConnection(MY_PORT, argc, argv);

    if (client_socket == -1) {
        std::cerr << "clientConnect() error" << std::endl;
        return -1;
    }

    int rt, flag = -1;
    time_t start = time(NULL);
    time_t now = time(NULL);

    while (true) {

        while ((now - start < 3)) {
            flag = -1;
            now = time(NULL);
        }

        flag = 0;
        write(client_socket, &flag, sizeof(int));

        // Take pic
        system("python3 capture_image.py");

        // File metadata
        struct stat st;

        if (stat("webcam.jpg", &st) == -1) {
            std::cerr << "stat() error" << std::endl;
            return -1;
        }

        // Send image stats
        rt = sendStat(client_socket, st);
        std::cout << "sendStat rt: " << rt << std::endl;

        // Send image
        char filename[20] = "webcam.jpg";
        rt = sendFile(client_socket, filename, st.st_size);
        std::cout << "sendFile rt: " << rt << std::endl;

        // Refresh
        start = time(NULL);

    }

    return 0;

}