#include <iostream>
#include <zmq.h>
#include <string>
#include <cstring>

int main(void)
{
    void* context = zmq_init(1);
    void* frontend = zmq_socket(context, ZMQ_SUB);
    zmq_connect(frontend, "tcp://127.0.0.1:6448");
    
    void* backend = zmq_socket(context, ZMQ_PUB);
    zmq_bind(backend, "tcp://localhost:43080");

    zmq_setsockopt(frontend, ZMQ_SUBSCRIBE, "", 0);

    while(1) {
        zmq_msg_t message;
        int64_t more;

        zmq_msg_init(&message);
        zmq_recv(frontend, &message, 42, 0);
        void* p = zmq_msg_data(&message);
        std::cout << zmq_msg_size(&message) << ' ' << (char*)p << std::endl;
        std::string data(static_cast<char*>(zmq_msg_data(&message)), zmq_msg_size(&message));
        std::cout << data << ' ' << data.size() << std::endl;

        // size_t more_size = sizeof(more);
        // zmq_getsockopt(frontend, ZMQ_RCVMORE, &more, &more_size);
        // zmq_send(backend, &message, 10, more? ZMQ_SNDMORE: 0);
        // zmq_msg_close(&message);
        // if(!more) {
        //     break;
        // }
    }
    zmq_close(frontend);
    zmq_close(backend);
    zmq_term(context);
    return 0;

}