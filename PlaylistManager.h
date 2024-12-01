#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <vector>
#include <algorithm>
#include <random>
#include <map>
#include "HashTable.h"

enum class SortCriteria {
    POPULARITY,
    YEAR,
    DURATION,
    DANCEABILITY
};

class PlaylistManager {
private:
    HashTable songDatabase;
    std::vector<std::string> playlist; // Almacena track_ids en orden

    // Índices secundarios para búsqueda
    std::multimap<std::string, std::string> artistIndex;  // artist -> track_id
    std::multimap<std::string, std::string> genreIndex;   // genre -> track_id
    std::multimap<std::string, std::string> titleIndex;   // title -> track_id

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

    std::vector<Song> searchByArtist(const std::string& prefix) {
        std::vector<Song> results;
        std::string lowerPrefix = prefix;
        std::transform(lowerPrefix.begin(), lowerPrefix.end(), lowerPrefix.begin(), ::tolower);
        
        for (const auto& pair : artistIndex) {
            std::string artistName = pair.first;
            std::transform(artistName.begin(), artistName.end(), artistName.begin(), ::tolower);
            if (artistName.substr(0, lowerPrefix.length()) == lowerPrefix) {
                Song* song = songDatabase.find(pair.second);
                if (song) results.push_back(*song);
            }
        }
        return results;
    }

    std::vector<Song> searchByTitle(const std::string& prefix) {
        std::vector<Song> results;
        std::string lowerPrefix = prefix;
        std::transform(lowerPrefix.begin(), lowerPrefix.end(), lowerPrefix.begin(), ::tolower);
        
        for (const auto& pair : titleIndex) {
            std::string title = pair.first;
            std::transform(title.begin(), title.end(), title.begin(), ::tolower);
            if (title.substr(0, lowerPrefix.length()) == lowerPrefix) {
                Song* song = songDatabase.find(pair.second);
                if (song) results.push_back(*song);
            }
        }
        return results;
    }

    std::vector<Song> searchByGenre(const std::string& genre) {
        std::vector<Song> results;
        std::string lowerGenre = genre;
        std::transform(lowerGenre.begin(), lowerGenre.end(), lowerGenre.begin(), ::tolower);
        
        for (const auto& pair : genreIndex) {
            std::string currentGenre = pair.first;
            std::transform(currentGenre.begin(), currentGenre.end(), currentGenre.begin(), ::tolower);
            if (currentGenre.find(lowerGenre) != std::string::npos) {
                Song* song = songDatabase.find(pair.second);
                if (song) results.push_back(*song);
            }
        }
        return results;
    }

    std::vector<Song> searchByDuration(double minDuration, double maxDuration) {
        std::vector<Song> results;
        auto allSongs = getAllSongs();
        
        for (const auto& song : allSongs) {
            double durationInSeconds = song.getDuration() / 1000.0; // Convertir de ms a segundos
            if (durationInSeconds >= minDuration && durationInSeconds <= maxDuration) {
                results.push_back(song);
            }
        }
        return results;
    }

    std::vector<Song> getSortedSongs(SortCriteria criteria) {
        std::vector<Song> songs = getAllSongs();
        
        switch (criteria) {
            case SortCriteria::POPULARITY:
                std::sort(songs.begin(), songs.end(), 
                    [](const Song& a, const Song& b) { return a.getPopularity() > b.getPopularity(); });
                break;
            case SortCriteria::YEAR:
                std::sort(songs.begin(), songs.end(), 
                    [](const Song& a, const Song& b) { return a.getYear() > b.getYear(); });
                break;
            case SortCriteria::DURATION:
                std::sort(songs.begin(), songs.end(), 
                    [](const Song& a, const Song& b) { return a.getDuration() < b.getDuration(); });
                break;
            case SortCriteria::DANCEABILITY:
                std::sort(songs.begin(), songs.end(), 
                    [](const Song& a, const Song& b) { return a.getDanceability() > b.getDanceability(); });
                break;
        }
        return songs;
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