#include <iostream>
#include <zmq.h>
#include <chrono>
#include <string>
#include <cstring>
#include <thread>

int main(void)
{
    void* context = zmq_init(1);
    void* backend = zmq_socket(context, ZMQ_PUB);
    void* push_sock = zmq_socket(context, ZMQ_PUSH);

    int hwm = 1000;
    //zmq_setsockopt(push_sock, ZMQ_SNDHWM, &hwm, sizeof(hwm));
    
    zmq_connect(push_sock, "tcp://localhost:53112");
    zmq_bind(backend, "tcp://127.0.0.1:6448");
    while(1) {
        zmq_msg_t message;
        zmq_msg_t message2;

        const char* msg = "Hello!";

        zmq_msg_init_size(&message, strlen(msg) + 1);
        memcpy(zmq_msg_data(&message), msg, strlen(msg) + 1);
        zmq_send(push_sock, &message, 50, 0);
        
        zmq_msg_init_size(&message2, strlen(msg) + 1);
        memcpy(zmq_msg_data(&message2), msg, strlen(msg) + 1);
        zmq_send(backend, &message2, 50, 0);
        std::cout << msg << std::endl;
        
        zmq_msg_close(&message);
        zmq_msg_close(&message2);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
    zmq_close(backend);
    zmq_close(push_sock);
    zmq_term(context);
    return 0;
}