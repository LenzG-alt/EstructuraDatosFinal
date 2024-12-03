#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <list>
#include "Song.h"

using namespace std;

class HashTable {
private:
    static const int TABLE_SIZE = 10007; // Número primo para mejor distribución
    vector<list<Song>> table;

    int hash(const string& key) const {
        int hash = 0;
        for (char c : key) {
            hash = (hash * 31 + c) % TABLE_SIZE;
        }
        return hash;
    }

public:
    HashTable() : table(TABLE_SIZE) {}

    void insert(const Song& song) {
        int index = hash(song.getTrackId());
        // Verificar si la canción ya existe
        for (const Song& s : table[index]) {
            if (s.getTrackId() == song.getTrackId()) {
                return; // La canción ya existe
            }
        }
        table[index].push_back(song);
    }

    bool remove(const string& trackId) {
        int index = hash(trackId);
        auto& bucket = table[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->getTrackId() == trackId) {
                bucket.erase(it);
                return true;
            }
        }
        return false;
    }

    Song* find(const string& trackId) {
        int index = hash(trackId);
        for (auto& song : table[index]) {
            if (song.getTrackId() == trackId) {
                return &song;
            }
        }
        return nullptr;
    }

    vector<Song> getAllSongs() const {
        vector<Song> songs;
        for (const auto& bucket : table) {
            for (const auto& song : bucket) {
                songs.push_back(song);
            }
        }
        return songs;
    }
};

#endif