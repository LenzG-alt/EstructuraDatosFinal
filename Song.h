#ifndef SONG_H
#define SONG_H

#include <string>

class Song {
private:
    std::string artist_name;
    std::string track_name;
    std::string track_id;
    int popularity;
    int year;
    std::string genre;
    double duration_ms;
    
public:
    Song() {}
    
    Song(std::string artist, std::string track, std::string id, int pop, 
         int yr, std::string gen, double duration) {
        artist_name = artist;
        track_name = track;
        track_id = id;
        popularity = pop;
        year = yr;
        genre = gen;
        duration_ms = duration;
    }

    // Getters
    std::string getArtist() const { return artist_name; }
    std::string getTrackName() const { return track_name; }
    std::string getTrackId() const { return track_id; }
    int getPopularity() const { return popularity; }
    int getYear() const { return year; }
    std::string getGenre() const { return genre; }
    double getDuration() const { return duration_ms; }

    // Para comparaciones
    bool operator==(const Song& other) const {
        return track_id == other.track_id;
    }
};

#endif