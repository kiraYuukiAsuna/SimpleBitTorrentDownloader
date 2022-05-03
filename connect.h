#ifndef BITTORRENTCLIENT_CONNECT_H
#define BITTORRENTCLIENT_CONNECT_H
#include <string>
#include <asio.hpp>
#include <iostream>

/**
 * Functions that handle network connection.
 */

void init_connect();
std::shared_ptr<asio::ip::tcp::socket> createConnection(const std::string& ip, int port);
void sendData(const std::shared_ptr<asio::ip::tcp::socket>& sock, const std::string& data);
std::string receiveData(const std::shared_ptr<asio::ip::tcp::socket>& sock, uint32_t bufferSize = 0);

#endif //BITTORRENTCLIENT_CONNECT_H
