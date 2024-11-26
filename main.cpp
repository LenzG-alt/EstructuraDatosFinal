#include <iostream>
#include <limits>
#include "PlaylistManager.h"

void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pausar() {
    std::cout << "\nPresione Enter para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void limpiarBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void mostrarMenu() {
    limpiarPantalla();
    std::cout << "\n=== Gestor de Lista de Reproducción ===" << std::endl;
    std::cout << "1. Cargar canciones desde CSV" << std::endl;
    std::cout << "2. Agregar canción a la playlist" << std::endl;
    std::cout << "3. Eliminar canción de la playlist" << std::endl;
    std::cout << "4. Cambiar orden de una canción" << std::endl;
    std::cout << "5. Reproducción aleatoria" << std::endl;
    std::cout << "6. Mostrar playlist actual" << std::endl;
    std::cout << "7. Mostrar todas las canciones disponibles" << std::endl;
    std::cout << "8. Salir" << std::endl;
    std::cout << "Seleccione una opción: ";
}

void mostrarCanciones(const std::vector<Song>& songs) {
    int index = 0;
    for (const auto& song : songs) {
        std::cout << index++ << ". " << song.getArtist() << " - " 
                 << song.getTrackName() << " (" << song.getYear() << ")" 
                 << " [ID: " << song.getTrackId() << "]" << std::endl;
    }
}

int main() {
    PlaylistManager manager;
    int opcion;
    std::string filename = "spotify_data.csv";
    bool archivoYaCargado = false;

    do {
        mostrarMenu();
        
        if (!(std::cin >> opcion)) {
            limpiarBuffer();
            continue;
        }
        
        limpiarBuffer();

        switch (opcion) {
            case 1: {
                if (archivoYaCargado) {
                    std::cout << "El archivo ya fue cargado anteriormente." << std::endl;
                } else {
                    std::cout << "Cargando canciones desde " << filename << "..." << std::endl;
                    manager.loadFromCSV(filename);
                    archivoYaCargado = true;
                    std::cout << "Canciones cargadas exitosamente!" << std::endl;
                }
                pausar();
                break;
            }
            case 2: {
                if (!archivoYaCargado) {
                    std::cout << "Primero debe cargar el archivo CSV (opción 1)." << std::endl;
                    pausar();
                    break;
                }
                
                std::cout << "Canciones disponibles:" << std::endl;
                auto songs = manager.getAllSongs();
                mostrarCanciones(songs);
                
                std::cout << "Ingrese el ID de la canción a agregar: ";
                std::string trackId;
                std::getline(std::cin, trackId);
                
                if (manager.addToPlaylist(trackId)) {
                    std::cout << "Canción agregada exitosamente!" << std::endl;
                } else {
                    std::cout << "No se pudo agregar la canción." << std::endl;
                }
                pausar();
                break;
            }
            case 3: {
                if (!archivoYaCargado) {
                    std::cout << "Primero debe cargar el archivo CSV (opción 1)." << std::endl;
                    pausar();
                    break;
                }
                
                std::cout << "Playlist actual:" << std::endl;
                auto playlistSongs = manager.getPlaylistSongs();
                mostrarCanciones(playlistSongs);
                
                if (playlistSongs.empty()) {
                    std::cout << "La playlist está vacía." << std::endl;
                    pausar();
                    break;
                }
                
                std::cout << "Ingrese el ID de la canción a eliminar: ";
                std::string trackId;
                std::getline(std::cin, trackId);
                
                if (manager.removeFromPlaylist(trackId)) {
                    std::cout << "Canción eliminada exitosamente!" << std::endl;
                } else {
                    std::cout << "No se pudo eliminar la canción." << std::endl;
                }
                pausar();
                break;
            }
            case 4: {
                if (!archivoYaCargado) {
                    std::cout << "Primero debe cargar el archivo CSV (opción 1)." << std::endl;
                    pausar();
                    break;
                }
                
                auto playlistSongs = manager.getPlaylistSongs();
                if (playlistSongs.empty()) {
                    std::cout << "La playlist está vacía." << std::endl;
                    pausar();
                    break;
                }
                
                std::cout << "Playlist actual:" << std::endl;
                mostrarCanciones(playlistSongs);
                
                int posActual, nuevaPos;
                std::cout << "Ingrese la posición actual: ";
                if (!(std::cin >> posActual)) {
                    std::cout << "Entrada inválida." << std::endl;
                    limpiarBuffer();
                    pausar();
                    break;
                }
                
                std::cout << "Ingrese la nueva posición: ";
                if (!(std::cin >> nuevaPos)) {
                    std::cout << "Entrada inválida." << std::endl;
                    limpiarBuffer();
                    pausar();
                    break;
                }
                
                if (manager.changeOrder(posActual, nuevaPos)) {
                    std::cout << "Orden cambiado exitosamente!" << std::endl;
                } else {
                    std::cout << "No se pudo cambiar el orden." << std::endl;
                }
                limpiarBuffer();
                pausar();
                break;
            }
            case 5: {
                if (!archivoYaCargado) {
                    std::cout << "Primero debe cargar el archivo CSV (opción 1)." << std::endl;
                    pausar();
                    break;
                }
                
                auto playlistSongs = manager.getPlaylistSongs();
                if (playlistSongs.empty()) {
                    std::cout << "La playlist está vacía." << std::endl;
                } else {
                    manager.shufflePlaylist();
                    std::cout << "Playlist reorganizada aleatoriamente!" << std::endl;
                }
                pausar();
                break;
            }
            case 6: {
                if (!archivoYaCargado) {
                    std::cout << "Primero debe cargar el archivo CSV (opción 1)." << std::endl;
                    pausar();
                    break;
                }
                
                std::cout << "Playlist actual:" << std::endl;
                auto playlistSongs = manager.getPlaylistSongs();
                if (playlistSongs.empty()) {
                    std::cout << "La playlist está vacía." << std::endl;
                } else {
                    mostrarCanciones(playlistSongs);
                }
                pausar();
                break;
            }
            case 7: {
                if (!archivoYaCargado) {
                    std::cout << "Primero debe cargar el archivo CSV (opción 1)." << std::endl;
                    pausar();
                    break;
                }
                
                std::cout << "Todas las canciones disponibles:" << std::endl;
                auto songs = manager.getAllSongs();
                if (songs.empty()) {
                    std::cout << "No hay canciones cargadas." << std::endl;
                } else {
                    mostrarCanciones(songs);
                }
                pausar();
                break;
            }
            case 8: {
                std::cout << "¡Gracias por usar el programa!" << std::endl;
                break;
            }
            default: {
                std::cout << "Opción inválida. Por favor, intente de nuevo." << std::endl;
                pausar();
            }
        }
    } while (opcion != 8);

    return 0;
}