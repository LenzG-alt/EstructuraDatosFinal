#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <vector>
#include <algorithm>
#include <random>
#include "HashTable.h"

class PlaylistManager {
private:
    HashTable songDatabase;
    std::vector<std::string> playlist; // Almacena track_ids en orden

public:
    void loadFromCSV(const std::string& filename);
    
    bool addToPlaylist(const std::string& trackId) {
        if (songDatabase.find(trackId)) {
            playlist.push_back(trackId);
            return true;
        }
        return false;
    }

    bool removeFromPlaylist(const std::string& trackId) {
        auto it = std::find(playlist.begin(), playlist.end(), trackId);
        if (it != playlist.end()) {
            playlist.erase(it);
            return true;
        }
        return false;
    }

    bool changeOrder(int currentPos, int newPos) {
        if (currentPos >= 0 && currentPos < playlist.size() && 
            newPos >= 0 && newPos < playlist.size()) {
            std::string trackId = playlist[currentPos];
            playlist.erase(playlist.begin() + currentPos);
            playlist.insert(playlist.begin() + newPos, trackId);
            return true;
        }
        return false;
    }

    void shufflePlaylist() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(playlist.begin(), playlist.end(), gen);
    }

    std::vector<Song> getPlaylistSongs() {
        std::vector<Song> songs;
        for (const auto& trackId : playlist) {
            Song* song = songDatabase.find(trackId);
            if (song) {
                songs.push_back(*song);
            }
        }
        return songs;
    }

    std::vector<Song> getAllSongs() {
        return songDatabase.getAllSongs();
    }

    HashTable& getSongDatabase() {
        return songDatabase;
    }
};

#endif