#include <iostream>

#include "TorrentFileParser.h"
#include <cpr/cpr.h>
#include "utils.h"
#include <stdio.h>
#include "PeersRetriever.h"
#include "PeerConnection.h"
#include "connect.h"
#include "TorrentClient.h"
#include <vector>

int bbmain(){
//    init_connect();
//    std::vector<char> buffer(20);
//    std::string p = "Hello From Client!";
//    buffer.assign(p.begin(),p.end());
//    auto sock = createConnection("127.0.0.1",14251);
//    sock->send(asio::buffer(buffer));
//    memset(buffer.data(),0,20);
//    sock->receive(asio::buffer(buffer,20));
//    try {
//        std::cout.write(buffer.data(), buffer.size());
//    }catch (std::exception& e){
//        std::cout<<"e"<<e.what();
//    }
//    while(1);
return 0;
}

int main() {
   // WRL::TorrentFileParser tfc("ss");
//    std::cout<<"announce:"<<tfc.getAnnounce()<<"\n";
//    std::cout<<"filename:"<<tfc.getFileName()<<"\n";
//    std::cout<<"filesize:"<<tfc.getFileSize()<<"\n";
//    std::cout<<"info hash:"<<tfc.getInfoHash()<<"\n";
//    auto hashs = tfc.splitPieceHashes();
//    for (auto& i : hashs) {
//        std::cout<<i<<"\n";
//    }

//    auto announceUrl = tfc.getAnnounce();
//    auto infoHash = tfc.getInfoHash();
//    auto fileSize = tfc.getFileSize();
//    auto peerId = "-UT2049-194819401936";
//    auto port = 14251;
//#define TRACKER_TIMEOUT 15000
//    WRL::PeersRetriever pr(peerId,announceUrl,infoHash,port,fileSize);
//    auto peers = pr.retrievePeers(0);
//    for (auto& i : peers) {
//        std::cout<<"Peers[IP: "<<i->ip<<" Port: "<<i->port<<"]\n";
//    }

//    auto sock = createConnection(peers.at(0)->ip,peers.at(0)->port);
//
//    if(sock->is_open()){
//        std::cout<<"Connect Successfully!\n";
//    }else{
//        std::cout<<"Connect Failed!\n";
//    }

    try
    {
        init_connect();
        TorrentClient torrentClient(100, false, "./");
        torrentClient.downloadFile(R"(C:\Users\KiraY\Desktop\SimpleBitTorrentDownloader\ubuntu-22.04-desktop-amd64.iso.torrent)", "./");
    }
    catch (std::exception& e)
    {
        std::cout << "Exception:" << e.what() << std::endl;
        return 1;
    }


}
