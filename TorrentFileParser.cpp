#include "TorrentFileParser.h"

namespace WRL {

    void TorrentFileParser::getList(bc::list &list, std::stringstream& ss) {
        // int 0 str 1 list 2 dict 3
        ss<<"l";
        for (auto &i: list) {
            if (i.index() == 0) {
                ss<<std::get<bc::integer>(i);
                //std::cout << std::get<bc::integer>(i) << std::endl;
            } else if (i.index() == 1) {
                auto str = std::get<bc::string>(i);
                ss<<str.length()<<":"<<str;
                //std::cout << std::get<bc::string>(i) << std::endl;
            } else if (i.index() == 2) {
                auto listp = std::get<bc::list>(i);
                getList(listp, ss);
            } else if (i.index() == 3) {
                auto dictp = std::get<bc::dict>(i);
                getDict(dictp, ss);
            }
        }
        ss<<"e";
    }

    void TorrentFileParser::getDict(bc::dict &dict, std::stringstream& ss) {
        ss<<"d";
        for (auto &i: dict) {
            // std::cout << i.first << ":";
            ss<<i.first.length()<<":"<<i.first ;
            // int 0 str 1 list 2 dict 3
            auto d = i.second;
            if (i.second.index() == 0) {
                ss<<"i";
                ss<<std::get<bc::integer>(i.second);
                ss<<"e";
                //std::cout << std::get<bc::integer>(i.second) << std::endl;
            } else if (i.second.index() == 1) {
                auto str =std::get<bc::string>(i.second);
                ss<<str.length()<<":"<<str;
                //std::cout << std::get<bc::string>(i.second) << std::endl;
            } else if (i.second.index() == 2) {
                auto listp = std::get<bc::list>(i.second);
                getList(listp, ss);
            } else if (i.second.index() == 3) {
                auto dictp = std::get<bc::dict>(i.second);
                getDict(dictp, ss);
            }
        }
        ss<<"e";
    }

    TorrentFileParser::TorrentFileParser(const std::string& torrentFilePath) {
        try {
            std::ifstream torrentFile;
            torrentFile.open(torrentFilePath,std::ios::binary | std::ios::in);
            if (!torrentFile.is_open()) {
                std::cout << "Open Torrent File Failed!"<< std::endl;
            }
            // read file content
            auto buffer = torrentFile.rdbuf();
            std::istream &torrentStream = torrentFile;
            auto bData = bc::decode(torrentStream);
            mTorrentFileData = std::get<bc::dict>(bData);
        } catch (std::exception &e) {
            std::cout << "Exception:" << e.what() << std::endl;
        }

    }

    std::shared_ptr<bc::data> TorrentFileParser::getKeyValueFromList(const std::string &key, bencode::list &list) {
        for (auto &i: list) {
            if (i.index() == 2) {
                auto listp = std::get<bc::list>(i);
                auto ret = getKeyValueFromList(key,listp);
                if(ret != nullptr){
                    return ret;
                }
            } else if (i.index() == 3) {
                auto dictp = std::get<bc::dict>(i);
                auto ret = getKeyValueFromDict(key,dictp);
                if(ret != nullptr){
                    return ret;
                }
            }
        }
        return nullptr;
    }

    std::shared_ptr<bc::data> TorrentFileParser::getKeyValueFromDict(const std::string &key, bencode::dict &dict) {
        for (auto &i: dict) {
            if(i.first == key){
                auto value = std::make_shared<bc::data>(i.second);
                return value;
            } else if (i.second.index() == 2) {
                auto listp = std::get<bc::list>(i.second);
                auto ret = getKeyValueFromList(key,listp);
                if(ret != nullptr){
                    return ret;
                }
            } else if (i.second.index() == 3) {
                auto dictp = std::get<bc::dict>(i.second);
                auto ret = getKeyValueFromDict(key,dictp);
                if(ret != nullptr){
                    return ret;
                }
            }
        }
        return nullptr;
    }

    long long TorrentFileParser::getFileSize() {
        auto data = getKeyValueFromDict("length",mTorrentFileData);
        if(!data){
            throw "Invaid Torrent File!";
            return 0;
        }else{
            return std::get<bc::integer>(*data);
        }
    }

    long long TorrentFileParser::getPieceLength() {
        auto data = getKeyValueFromDict("piece length",mTorrentFileData);
        if(!data){
            throw "Invaid Torrent File!";
        }else{
            return std::get<bc::integer>(*data);
        }
    }

    std::string TorrentFileParser::getFileName() {
        auto data = getKeyValueFromDict("name",mTorrentFileData);
        if(!data){
            throw "Invaid Torrent File!";
        }else{
            return std::get<bc::string>(*data);
        }
    }

    std::string TorrentFileParser::getAnnounce() {
        auto data = getKeyValueFromDict("announce",mTorrentFileData);
        if(!data){
            throw "Invaid Torrent File!";
        }else{
            return std::get<bc::string>(*data);
        }
    }

    std::string TorrentFileParser::getInfoHash() {
        auto data = getKeyValueFromDict("info",mTorrentFileData);
        auto dict = std::get<bc::dict>(*data);
        std::stringstream ss;
        getDict(dict,ss);
        std::string infoString = ss.rdbuf()->str();
        if(infoString.empty()){
            throw "Invaid Torrent File!";
        }else{
            std::string sha1Hash = sha1(infoString);
            return sha1Hash;
        }
    }

    std::vector<std::string> TorrentFileParser::splitPieceHashes() {
        auto piecesValue = getKeyValueFromDict("pieces",mTorrentFileData);
        if (!piecesValue)
            throw std::runtime_error("Torrent file is malformed. [File does not contain key 'pieces']");
        std::string pieces = std::get<bc::string>(*piecesValue);

        std::vector<std::string> pieceHashes;

        assert(pieces.size() % HASH_LEN == 0);
        int piecesCount = (int) pieces.size() / HASH_LEN;
        pieceHashes.reserve(piecesCount);
        for (int i = 0; i < piecesCount; i++)
            pieceHashes.push_back(pieces.substr(i * HASH_LEN, HASH_LEN));
        return pieceHashes;
    }


} // WRL