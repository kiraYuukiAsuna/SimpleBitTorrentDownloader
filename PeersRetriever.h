#ifndef SIMPLEBITTORRENTDOWNLOADER_PEERSRETRIEVER_H
#define SIMPLEBITTORRENTDOWNLOADER_PEERSRETRIEVER_H

#include <string>
#include <vector>

namespace WRL {

    struct Peer
    {
        std::string ip;
        int port;
    };

/**
 * Retrieves a list of peers by sending a GET request to the tracker.
 */
    class PeersRetriever
    {
    private:
        std::string announceUrl;
        std::string infoHash;
        std::string peerId;
        int port;
        const unsigned long fileSize;
        std::vector<Peer*> decodeResponse(std::string response);
    public:
        explicit PeersRetriever(std::string peerId, std::string announceUrL, std::string infoHash, int port, unsigned long fileSize);
        std::vector<Peer*> retrievePeers(unsigned long bytesDownloaded = 0);
    };

} // WRL

#endif //SIMPLEBITTORRENTDOWNLOADER_PEERSRETRIEVER_H
