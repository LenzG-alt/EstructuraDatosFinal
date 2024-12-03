#include "PlaylistManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

vector<string> parseCSVLine(const string& line) {
    vector<string> fields;
    string field;
    bool inQuotes = false;
    
    for (char c : line) {
        if (c == '"') {
            inQuotes = !inQuotes;
        }
        else if (c == ',' && !inQuotes) {
            if (field.size() >= 2 && field.front() == '"' && field.back() == '"') {
                field = field.substr(1, field.size() - 2);
            }
            fields.push_back(field);
            field.clear();
        }
        else {
            field += c;
        }
    }
    
    if (field.size() >= 2 && field.front() == '"' && field.back() == '"') {
        field = field.substr(1, field.size() - 2);
    }
    fields.push_back(field);
    
    return fields;
}

void PlaylistManager::loadFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: No se pudo abrir el archivo " << filename << endl;
        return;
    }

    string line;
    int lineCount = 0;
    
    // Saltar la primera línea (encabezados)
    getline(file, line);
    
    while (getline(file, line)) {
        try {
            lineCount++;
            
            auto tokens = parseCSVLine(line);

            if (tokens.size() < 18) {
                if (lineCount % 1000 == 0) {
                    cout << "Advertencia: Línea " << lineCount << " ignorada - campos insuficientes" << endl;
                }
                continue;
            }

            int popularity = 0;
            int year = 0;
            double duration_ms = 0.0;
            double danceability = 0.0;
            double energy = 0.0;
            double tempo = 0.0;

            try {
                string popStr = tokens[4];
                string yearStr = tokens[5];
                string durStr = tokens[17];
                string danceStr = tokens[7];
                string energyStr = tokens[8];
                string tempoStr = tokens[16];

                popStr.erase(remove_if(popStr.begin(), popStr.end(), 
                    [](unsigned char c) { return !isdigit(c); }), popStr.end());
                yearStr.erase(remove_if(yearStr.begin(), yearStr.end(), 
                    [](unsigned char c) { return !isdigit(c); }), yearStr.end());
                
                popularity = popStr.empty() ? 0 : stoi(popStr);
                year = yearStr.empty() ? 0 : stoi(yearStr);
                duration_ms = durStr.empty() ? 0.0 : stod(durStr);
                danceability = danceStr.empty() ? 0.0 : stod(danceStr);
                energy = energyStr.empty() ? 0.0 : stod(energyStr);
                tempo = tempoStr.empty() ? 0.0 : stod(tempoStr);
                
            } catch (const exception& e) {
                if (lineCount % 1000 == 0) {
                    cout << "Advertencia: Error al procesar valores numéricos en línea " << lineCount << endl;
                }
                continue;
            }

            if (tokens[1].empty() || tokens[2].empty() || tokens[3].empty()) {
                if (lineCount % 1000 == 0) {
                    cout << "Advertencia: Línea " << lineCount << " ignorada - campos obligatorios vacíos" << endl;
                }
                continue;
            }

            Song song(
                tokens[1],    // artist_name
                tokens[2],    // track_name
                tokens[3],    // track_id
                popularity,
                year,
                tokens[6],    // genreadad
                duration_ms,
                danceability,
                energy,
                tempo
            );
            
            songDatabase.insert(song);
            
            // Actualizar índices
            artistIndex.insert({tokens[1], tokens[3]});
            titleIndex.insert({tokens[2], tokens[3]});
            genreIndex.insert({tokens[6], tokens[3]});

            if (lineCount % 10000 == 0) {
                cout << "Procesadas " << lineCount << " canciones..." << endl;
            }

        } catch (const exception& e) {
            if (lineCount % 1000 == 0) {
                cout << "Error al procesar línea " << lineCount << ": " << e.what() << endl;
            }
            continue;
        }
    }
    
    cout << "Proceso completado. Total de líneas procesadas: " << lineCount << endl;
}