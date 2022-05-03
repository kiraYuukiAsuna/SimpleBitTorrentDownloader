cmake_minimum_required (VERSION 3.22)
project(SimpleBitTorrentDownloader)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
        "ThirdParty/asio-1.22.1/include"
        "ThirdParty/bencode.hpp/include"
        "ThirdParty/cxxopts/include"
        "ThirdParty/curl/include"
        "ThirdParty/cpr-1.8.2/include"

)

link_directories(
        "ThirdParty/cpr-1.8.2/lib"
)

set(HEADERS
        TorrentFileParser.h
        utils.h
        PeersRetriever.h
        PeerConnection.h
        SharedQueue.h
        PieceManager.h
        Piece.h
        Block.h
        connect.h
        BitTorrentMessage.h
        TorrentClient.h
        )

set(SOURCES
        main.cpp
        TorrentFileParser.cpp
        ThirdParty/crypto/sha1.cpp
        utils.cpp
        PeersRetriever.cpp
        PeerConnection.cpp
        PieceManager.cpp
        Piece.cpp
        connect.cpp
        BitTorrentMessage.cpp
        TorrentClient.cpp
        )

add_executable(SimpleBitTorrentDownloader ${HEADERS} ${SOURCES})

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    message("Debug mode:${CMAKE_C_FLAGS_DEBUG}")
    target_link_libraries(SimpleBitTorrentDownloader libcurl-d_imp.lib zlib.lib cpr.lib)
elseif(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    message("Release mode:${CMAKE_C_FLAGS_RELEASE}")
    target_link_libraries(SimpleBitTorrentDownloader libcurl_imp.lib zlib.lib cpr.lib)
else()
    message("else:${CMAKE_BUILD_TYPE}")
    message("else:${CMAKE_C_FLAGS_RELEASE}")
    target_link_libraries(SimpleBitTorrentDownloader libcurl-d_imp.lib zlib.lib cpr.lib)
endif()

if(WIN32)

elseif(UNIX)
    target_link_libraries(SimpleBitTorrentDownloader pthread dl)
endif()
