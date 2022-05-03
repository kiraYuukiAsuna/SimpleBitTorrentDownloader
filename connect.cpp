#include "connect.h"
#include "utils.h"
#include <thread>

asio::io_context io_context;
std::shared_ptr<std::thread> threadContext;

void init_connect() {
    threadContext = std::make_shared<std::thread>(
            []() {
                asio::io_context::work idleWork(io_context);
                io_context.run();
                while (1) {
                    std::cerr << "Error: Context null!\n";
                }
            });

}

std::shared_ptr<asio::ip::tcp::socket> createConnection(const std::string &ip, const int port) {
    using namespace asio::ip;
    try {
        auto clientSocket = std::make_shared<asio::ip::tcp::socket>(io_context);
        asio::error_code ec;
        clientSocket->connect(asio::ip::tcp::endpoint(asio::ip::address::from_string(ip), port),ec);
        if (ec.value()!=0){
            std::cerr<<"error code:"<<ec.message()<<std::endl;
        }
        return clientSocket;
    } catch (...) {
        std::cerr << "[ERROR] createConnection error!" << std::endl;
        return nullptr;
    }
}

void sendData(const std::shared_ptr<asio::ip::tcp::socket> &sock, const std::string &data) {
    try {
        if (sock->is_open()) {
            std::vector<char> buffer(data.length());
            buffer.assign(data.begin(), data.end());

            int ret = sock->send(asio::buffer(buffer.data(), buffer.size()));
            if (ret < 0) {
                throw std::runtime_error("Failed to write data to socket ");
            }
        }
    } catch (...) {
        std::cerr << "[ERROR] sendData error!" << std::endl;
    }
}

std::string receiveData(const std::shared_ptr<asio::ip::tcp::socket> &sock, uint32_t bufferSize) {
    std::string reply;
    // If buffer size is not specified, read the first 4 bytes of the message
    // to obtain the total length of the response.
    if (!bufferSize) {
        int bytesReadts = 0;
        char bufferts[4];
        memset(bufferts, 0, 4);
        bytesReadts = sock->receive(asio::buffer(bufferts, 4));

        if (bytesReadts != 4)
            return reply;

        std::string messageLengthStr;
        for (char i: bufferts)
            messageLengthStr += i;
        uint32_t messageLength = bytesToInt(messageLengthStr);
        bufferSize = messageLength;
    }

    // If the buffer size is greater than uint16_t max, a segfault will
    // occur when initializing the buffer
    if (bufferSize > std::numeric_limits<uint16_t>::max())
        throw std::runtime_error("Received corrupted data [Received buffer size greater than 2 ^ 16 - 1]");

    std::vector<char> buffer(bufferSize);
    memset(buffer.data(), 0, bufferSize);
    // Receives reply from the host
    // Keeps reading from the buffer until all expected bytes are received
    int bytesRead = 0;
    int bytesToRead = bufferSize;
    // If not all expected bytes are received within the period of time
    // specified by READ_TIMEOUT, the read process will stop.
    do {
        try {
            bytesRead = sock->receive(asio::buffer(buffer, bufferSize));
        } catch (...) {
            std::cerr << "[ERROR]receiveData error!" << std::endl;
            return reply;
        }

        if (bytesRead < 0)
            throw std::runtime_error("Failed to receive data from socket ");
        bytesToRead -= bytesRead;
        for (int i = 0; i < bytesRead; i++)
            reply.push_back(buffer[i]);
    } while (bytesToRead > 0);

    return reply;

}
