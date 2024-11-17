#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

struct Cancion {
    string artist_name;
    string track_name;
    string track_id;
    int popularity;
    int year;
    string genre;
    float danceability;
    float energy;
    int key;
    float loudness;
    int mode;
    float speechiness;
    float acousticness;
    float instrumentalness;
    float liveness;
    float valence;
    float tempo;
    int duration_ms;
    int time_signature;
};
vector<Cancion> leer_csv(const string& filename) {
    ifstream archivo(filename);
    vector<Cancion> canciones;

    string linea;
    bool esPrimeraLinea = true;  // Para ignorar el encabezado

    while (getline(archivo, linea)) {
        if (esPrimeraLinea) {
            esPrimeraLinea = false;  // Saltar la primera línea (encabezados)
            continue;
        }

        stringstream ss(linea);
        string campo;
        Cancion cancion;

        // Leer cada campo, considerando las comillas
        getline(ss, campo, ','); // Ignorar la columna vacía inicial
        getline(ss, cancion.artist_name, ','); // Leer artista
        /*
        // Leer el nombre de la canción, que podría tener comas
        if (cancion.artist_name[0] == '"') {
            // Si comienza con comillas, leer hasta encontrar el par de comillas
            getline(ss, campo, '"');
            cancion.artist_name = campo + '"'; // Guardar la parte hasta las comillas
            getline(ss, campo, ','); // Descartar la coma
        }*/

        getline(ss, cancion.track_name, ','); // Leer el nombre de la canción

        if (cancion.track_name[0] == '"') {
            // Leer nombre de la canción con comillas
            getline(ss, campo, '"');
            cancion.track_name = campo + '"';
            getline(ss, campo, ','); // Descartar la coma
        }

        getline(ss, cancion.track_id, ',');
        ss >> cancion.popularity;
        ss.ignore(); // Ignorar la coma
        ss >> cancion.year;
        ss.ignore();
        getline(ss, cancion.genre, ',');
        ss >> cancion.danceability;
        ss.ignore();
        ss >> cancion.energy;
        ss.ignore();
        ss >> cancion.key;
        ss.ignore();
        ss >> cancion.loudness;
        ss.ignore();
        ss >> cancion.mode;
        ss.ignore();
        ss >> cancion.speechiness;
        ss.ignore();
        ss >> cancion.acousticness;
        ss.ignore();
        ss >> cancion.instrumentalness;
        ss.ignore();
        ss >> cancion.liveness;
        ss.ignore();
        ss >> cancion.valence;
        ss.ignore();
        ss >> cancion.tempo;
        ss.ignore();
        ss >> cancion.duration_ms;
        ss.ignore();
        ss >> cancion.time_signature;

        canciones.push_back(cancion);
    }

    return canciones;
}

int main() {
    string archivo_csv = "songs.csv"; // Ruta del archivo CSV
    vector<Cancion> canciones = leer_csv(archivo_csv);

    // Mostrar los resultados (opcional)
    for (const auto& cancion : canciones) {
        cout << "Artista: " << cancion.artist_name << " | Canción: " << cancion.track_name << endl;
        cout << "ID: " << cancion.track_id << " | Popularidad: " << cancion.popularity << " | Año: " << cancion.year << endl;
        // Agregar el resto de los campos que desees mostrar
        cout << "Género: " << cancion.genre << " | Tempo: " << cancion.tempo << " | Duración (ms): " << cancion.duration_ms << endl;
        cout << "---------------------------------------------" << endl;
    }

    return 0;
}
