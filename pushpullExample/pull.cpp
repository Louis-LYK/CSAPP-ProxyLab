#include <iostream>
#include <string>
#include <zmq.h>

int main()
{
    // Initialize ZeroMQ context
    void* context = zmq_ctx_new();

    // Create a ZMQ_PULL socket
    void* pull_socket = zmq_socket(context, ZMQ_PULL);

    // Bind the pull socket to the port to receive messages
    zmq_bind(pull_socket, "tcp://*:5555");

    // Receive a message from the push socket
    
    while(1) {
        zmq_msg_t message;
        zmq_msg_init(&message);
        zmq_recv(pull_socket, &message, 1024, 0);

        std::string strn(static_cast<char*>(zmq_msg_data(&message)), zmq_msg_size(&message));
        std::cout << strn << std::endl;
        
        // Clean up ZeroMQ context and socket
        zmq_msg_close(&message);
    }
    zmq_close(pull_socket);
    zmq_ctx_destroy(context);
    return 0;
}






