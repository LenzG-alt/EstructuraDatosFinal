#ifndef SONG_H
#define SONG_H

#include <string>

using namespace std;

class Song {
private:
    string artist_name;
    string track_name;
    string track_id;
    int popularity;
    int year;
    string genre;
    double duration_ms;
    double danceability;
    double energy;
    double tempo;
    
public:
    Song() {}
    
    Song(string artist, string track, string id, int pop, 
         int yr, string gen, double duration, double dance = 0.0, 
         double nrg = 0.0, double tmp = 0.0) {
        artist_name = artist;
        track_name = track;
        track_id = id;
        popularity = pop;
        year = yr;
        genre = gen;
        duration_ms = duration;
        danceability = dance;
        energy = nrg;
        tempo = tmp;
    }

    // Getters
    string getArtist() const { return artist_name; }
    string getTrackName() const { return track_name; }
    string getTrackId() const { return track_id; }
    int getPopularity() const { return popularity; }
    int getYear() const { return year; }
    string getGenre() const { return genre; }
    double getDuration() const { return duration_ms; }
    double getDanceability() const { return danceability; }
    double getEnergy() const { return energy; }
    double getTempo() const { return tempo; }

    // Para comparaciones
    bool operator==(const Song& other) const {
        return track_id == other.track_id;
    }
};

#endif