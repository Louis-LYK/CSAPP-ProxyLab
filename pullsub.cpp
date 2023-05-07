#include <iostream>
#include <string>
#include <zmq.h>

int main(void)
{
    void* context = zmq_init(1);

    void* receiver = zmq_socket(context, ZMQ_PULL);
    zmq_bind(receiver, "tcp://*:53112");
    int sz = 1024;
    zmq_setsockopt(receiver, ZMQ_RCVBUF, &sz, sizeof(sz));
    
    void* subscriber = zmq_socket(context, ZMQ_SUB);
    zmq_connect(subscriber, "tcp://localhost:6448");
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);
    zmq_pollitem_t items [] = {
        {receiver, 0, ZMQ_POLLIN, 0},
        {subscriber, 0, ZMQ_POLLIN, 0}
    };
    while(1) {
        std::cout << "In polling...\n";
        zmq_msg_t message;
        zmq_poll(items, 2, -1);
        if(items[0].revents & ZMQ_POLLIN) {
            std::cout << "items[0]" << std::endl;
            zmq_msg_init(&message);
            zmq_recv(receiver, &message, 50, 0);
            void* p = zmq_msg_data(&message);
            std::string strn(static_cast<char*>(zmq_msg_data(&message)), zmq_msg_size(&message));
            std::cout << strn << std::endl;
            zmq_msg_close(&message);
        }
        if(items[1].revents & ZMQ_POLLIN) {
            std::cout << "items[1]" << std::endl;
            zmq_msg_init(&message);
            zmq_recv(subscriber, &message, 50, 0);
            void* p = zmq_msg_data(&message);
            std::string strn(static_cast<char*>(zmq_msg_data(&message)), zmq_msg_size(&message));
            std::cout << strn << std::endl;
            zmq_msg_close(&message);
        }
    }
    zmq_close(receiver);
    zmq_close(subscriber);
    zmq_term(context);
    return 0;
}