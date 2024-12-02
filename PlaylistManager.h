#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <vector>
#include <algorithm>
#include <random>
#include <map>
#include "HashTable.h"

using namespace std;

enum class SortCriteria {
    POPULARITY,
    YEAR,
    DURATION,
    DANCEABILITY
};

class PlaylistManager {
private:
    HashTable songDatabase;
    vector<string> playlist; // Almacena track_ids en orden

    // Índices secundarios para búsqueda
    multimap<string, string> artistIndex;  // artist -> track_id
    multimap<string, string> genreIndex;   // genre -> track_id
    multimap<string, string> titleIndex;   // title -> track_id

public:
    void loadFromCSV(const string& filename);
    
    bool addToPlaylist(const string& trackId) {
        if (songDatabase.find(trackId)) {
            playlist.push_back(trackId);
            return true;
        }
        return false;
    }

    bool removeFromPlaylist(const string& trackId) {
        auto it = find(playlist.begin(), playlist.end(), trackId);
        if (it != playlist.end()) {
            playlist.erase(it);
            return true;
        }
        return false;
    }

    bool changeOrder(int currentPos, int newPos) {
        if (currentPos >= 0 && currentPos < playlist.size() && 
            newPos >= 0 && newPos < playlist.size()) {
            string trackId = playlist[currentPos];
            playlist.erase(playlist.begin() + currentPos);
            playlist.insert(playlist.begin() + newPos, trackId);
            return true;
        }
        return false;
    }

    void shufflePlaylist() {
        random_device rd;
        mt19937 gen(rd());
        shuffle(playlist.begin(), playlist.end(), gen);
    }

    vector<Song> searchByArtist(const string& prefix) {
        vector<Song> results;
        string lowerPrefix = prefix;
        transform(lowerPrefix.begin(), lowerPrefix.end(), lowerPrefix.begin(), ::tolower);
        
        for (const auto& pair : artistIndex) {
            string artistName = pair.first;
            transform(artistName.begin(), artistName.end(), artistName.begin(), ::tolower);
            if (artistName.substr(0, lowerPrefix.length()) == lowerPrefix) {
                Song* song = songDatabase.find(pair.second);
                if (song) results.push_back(*song);
            }
        }
        return results;
    }

    vector<Song> searchByTitle(const string& prefix) {
        vector<Song> results;
        string lowerPrefix = prefix;
        transform(lowerPrefix.begin(), lowerPrefix.end(), lowerPrefix.begin(), ::tolower);
        
        for (const auto& pair : titleIndex) {
            string title = pair.first;
            transform(title.begin(), title.end(), title.begin(), ::tolower);
            if (title.substr(0, lowerPrefix.length()) == lowerPrefix) {
                Song* song = songDatabase.find(pair.second);
                if (song) results.push_back(*song);
            }
        }
        return results;
    }

    vector<Song> searchByGenre(const string& genre) {
        vector<Song> results;
        string lowerGenre = genre;
        transform(lowerGenre.begin(), lowerGenre.end(), lowerGenre.begin(), ::tolower);
        
        for (const auto& pair : genreIndex) {
            string currentGenre = pair.first;
            transform(currentGenre.begin(), currentGenre.end(), currentGenre.begin(), ::tolower);
            if (currentGenre.find(lowerGenre) != string::npos) {
                Song* song = songDatabase.find(pair.second);
                if (song) results.push_back(*song);
            }
        }
        return results;
    }

    vector<Song> searchByDuration(double minDuration, double maxDuration) {
        vector<Song> results;
        auto allSongs = getAllSongs();
        
        for (const auto& song : allSongs) {
            double durationInSeconds = song.getDuration() / 1000.0; // Convertir de ms a segundos
            if (durationInSeconds >= minDuration && durationInSeconds <= maxDuration) {
                results.push_back(song);
            }
        }
        return results;
    }

    vector<Song> getSortedSongs(SortCriteria criteria) {
        vector<Song> songs = getAllSongs();
        
        switch (criteria) {
            case SortCriteria::POPULARITY:
                sort(songs.begin(), songs.end(), 
                    [](const Song& a, const Song& b) { return a.getPopularity() > b.getPopularity(); });
                break;
            case SortCriteria::YEAR:
                sort(songs.begin(), songs.end(), 
                    [](const Song& a, const Song& b) { return a.getYear() > b.getYear(); });
                break;
            case SortCriteria::DURATION:
                sort(songs.begin(), songs.end(), 
                    [](const Song& a, const Song& b) { return a.getDuration() < b.getDuration(); });
                break;
            case SortCriteria::DANCEABILITY:
                sort(songs.begin(), songs.end(), 
                    [](const Song& a, const Song& b) { return a.getDanceability() > b.getDanceability(); });
                break;
        }
        return songs;
    }

    vector<Song> getPlaylistSongs() {
        vector<Song> songs;
        for (const auto& trackId : playlist) {
            Song* song = songDatabase.find(trackId);
            if (song) {
                songs.push_back(*song);
            }
        }
        return songs;
    }

    vector<Song> getAllSongs() {
        return songDatabase.getAllSongs();
    }

    HashTable& getSongDatabase() {
        return songDatabase;
    }
};

#endif