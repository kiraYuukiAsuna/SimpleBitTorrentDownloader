#ifndef SIMPLEBITTORRENTDOWNLOADER_TORRENTFILEPARSER_H
#define SIMPLEBITTORRENTDOWNLOADER_TORRENTFILEPARSER_H

#include <bencode.hpp>
#include <fstream>
#include <vector>
#include <string>
#include "ThirdParty/crypto/sha1.h"

namespace WRL {
    namespace bc = bencode;

#define HASH_LEN 20

    struct TorrentFileContent{
        std::string announce;
        std::vector<std::string> announce_list;
        std::string comment;
        std::string createdBy;
        long long createDate;

        long long length;
        std::string name;
        long long pieceLength;
        std::vector<std::vector<char>> pieces;


    };

    class TorrentFileParser {
    private:
        bencode::dict mTorrentFileData;
        TorrentFileContent torrentContent;
    public:

    public:
        TorrentFileParser(const std::string& torrentFilePath);
        long long getFileSize();
        long long getPieceLength();
        std::string getFileName();
        std::string getAnnounce();
        std::string getInfoHash();
        std::vector<std::string> splitPieceHashes();
    private:
        std::shared_ptr<bc::data> getKeyValueFromList(const std::string &key, bencode::list &list);
        std::shared_ptr<bc::data> getKeyValueFromDict(const std::string& key, bc::dict& dict);

        void getList(bc::list& list,std::stringstream& ss);
        void getDict(bc::dict& dict,std::stringstream& ss);
    };

} // WRL

#endif //SIMPLEBITTORRENTDOWNLOADER_TORRENTFILEPARSER_H
