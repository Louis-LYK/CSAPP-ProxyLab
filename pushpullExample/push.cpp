#include <iostream>
#include <string>
#include <zmq.h>
#include <chrono>
#include <thread>
#include <cstring>

int main()
{
    // Initialize ZeroMQ context
    void* context = zmq_ctx_new();

    // Create a ZMQ_PUSH socket
    void* push_socket = zmq_socket(context, ZMQ_PUSH);

    // Connect the push socket to the pull socket
    zmq_connect(push_socket, "tcp://localhost:5555");

    while(1) {
        // Send a message from the push socket to the pull socket
        zmq_msg_t message;
        const char* msg = "Hello!";

        zmq_msg_init_size(&message, strlen(msg) + 1);
        memcpy(zmq_msg_data(&message), msg, strlen(msg) + 1);
        zmq_send(push_socket, &message, strlen(msg)+40, 0);

        std::cout << msg << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        // Clean up ZeroMQ context and socket
    }
    zmq_close(push_socket);
    zmq_ctx_destroy(context);
    return 0;
}