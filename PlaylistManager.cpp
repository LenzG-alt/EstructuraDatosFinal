#include "PlaylistManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Función auxiliar para parsear una línea CSV con campos entrecomillados
std::vector<std::string> parseCSVLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool inQuotes = false;
    
    for (char c : line) {
        if (c == '"') {
            inQuotes = !inQuotes;
        }
        else if (c == ',' && !inQuotes) {
            // Eliminar comillas si existen
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
    
    // Añadir el último campo
    if (field.size() >= 2 && field.front() == '"' && field.back() == '"') {
        field = field.substr(1, field.size() - 2);
    }
    fields.push_back(field);
    
    return fields;
}

void PlaylistManager::loadFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: No se pudo abrir el archivo " << filename << std::endl;
        return;
    }

    std::string line;
    int lineCount = 0;
    
    // Saltar la primera línea (encabezados)
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        try {
            lineCount++;
            
            // Usar el nuevo parser CSV
            auto tokens = parseCSVLine(line);

            // Verificar que tengamos suficientes campos
            if (tokens.size() < 18) {
                if (lineCount % 1000 == 0) {
                    std::cout << "Advertencia: Línea " << lineCount << " ignorada - campos insuficientes" << std::endl;
                }
                continue;
            }

            // Convertir valores numéricos con manejo de errores
            int popularity = 0;
            int year = 0;
            double duration_ms = 0.0;

            try {
                // Limpiar y convertir valores numéricos
                std::string popStr = tokens[4];
                std::string yearStr = tokens[5];
                std::string durStr = tokens[17];

                // Eliminar espacios en blanco y caracteres no numéricos
                popStr.erase(std::remove_if(popStr.begin(), popStr.end(), 
                    [](unsigned char c) { return !std::isdigit(c); }), popStr.end());
                yearStr.erase(std::remove_if(yearStr.begin(), yearStr.end(), 
                    [](unsigned char c) { return !std::isdigit(c); }), yearStr.end());
                
                // Convertir a números
                popularity = popStr.empty() ? 0 : std::stoi(popStr);
                year = yearStr.empty() ? 0 : std::stoi(yearStr);
                duration_ms = durStr.empty() ? 0.0 : std::stod(durStr);
                
            } catch (const std::exception& e) {
                if (lineCount % 1000 == 0) {
                    std::cout << "Advertencia: Error al procesar valores numéricos en línea " << lineCount << std::endl;
                }
                continue;
            }

            // Verificar que los campos obligatorios no estén vacíos
            if (tokens[1].empty() || tokens[2].empty() || tokens[3].empty()) {
                if (lineCount % 1000 == 0) {
                    std::cout << "Advertencia: Línea " << lineCount << " ignorada - campos obligatorios vacíos" << std::endl;
                }
                continue;
            }

            Song song(
                tokens[1],    // artist_name
                tokens[2],    // track_name
                tokens[3],    // track_id
                popularity,
                year,
                tokens[6],    // genre
                duration_ms
            );
            
            songDatabase.insert(song);
            
            // Mostrar progreso cada 10000 canciones
            if (lineCount % 10000 == 0) {
                std::cout << "Procesadas " << lineCount << " canciones..." << std::endl;
            }

        } catch (const std::exception& e) {
            if (lineCount % 1000 == 0) {
                std::cout << "Error al procesar línea " << lineCount << ": " << e.what() << std::endl;
            }
            continue;
        }
    }
    
    std::cout << "Proceso completado. Total de líneas procesadas: " << lineCount << std::endl;
}