#include <iostream>
#include <limits>
#include <iomanip>
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
    std::cout << "8. Búsqueda avanzada" << std::endl;
    std::cout << "9. Ordenar canciones" << std::endl;
    std::cout << "10. Salir" << std::endl;
    std::cout << "Seleccione una opción: ";
}

void mostrarMenuBusqueda() {
    limpiarPantalla();
    std::cout << "\n=== Búsqueda Avanzada ===" << std::endl;
    std::cout << "1. Buscar por artista" << std::endl;
    std::cout << "2. Buscar por nombre de canción" << std::endl;
    std::cout << "3. Buscar por género" << std::endl;
    std::cout << "4. Buscar por duración" << std::endl;
    std::cout << "5. Volver al menú principal" << std::endl;
    std::cout << "Seleccione una opción: ";
}

void mostrarMenuOrdenamiento() {
    limpiarPantalla();
    std::cout << "\n=== Ordenar Canciones ===" << std::endl;
    std::cout << "1. Por popularidad" << std::endl;
    std::cout << "2. Por año" << std::endl;
    std::cout << "3. Por duración" << std::endl;
    std::cout << "4. Por bailabilidad" << std::endl;
    std::cout << "5. Volver al menú principal" << std::endl;
    std::cout << "Seleccione una opción: ";
}

void mostrarCanciones(const std::vector<Song>& songs) {
    int index = 0;
    std::cout << std::left << std::setw(5) << "Idx" 
              << std::setw(30) << "Artista"
              << std::setw(40) << "Canción"
              << std::setw(10) << "Año"
              << std::setw(15) << "Duración (s)"
              << std::setw(15) << "Popularidad"
              << std::setw(20) << "Género" << std::endl;
    std::cout << std::string(135, '-') << std::endl;

    for (const auto& song : songs) {
        std::cout << std::left << std::setw(5) << index++
                  << std::setw(30) << (song.getArtist().length() > 27 ? song.getArtist().substr(0, 27) + "..." : song.getArtist())
                  << std::setw(40) << (song.getTrackName().length() > 37 ? song.getTrackName().substr(0, 37) + "..." : song.getTrackName())
                  << std::setw(10) << song.getYear()
                  << std::setw(15) << std::fixed << std::setprecision(2) << (song.getDuration() / 1000.0)
                  << std::setw(15) << song.getPopularity()
                  << std::setw(20) << (song.getGenre().length() > 17 ? song.getGenre().substr(0, 17) + "..." : song.getGenre())
                  << std::endl;
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
                
                std::cout << "\nIngrese el índice de la canción a eliminar: ";
                int idx;
                if (std::cin >> idx && idx >= 0 && idx < playlistSongs.size()) {
                    if (manager.removeFromPlaylist(playlistSongs[idx].getTrackId())) {
                        std::cout << "Canción eliminada exitosamente!" << std::endl;
                    } else {
                        std::cout << "No se pudo eliminar la canción." << std::endl;
                    }
                } else {
                    std::cout << "Índice inválido." << std::endl;
                }
                limpiarBuffer();
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
                    std::cout << "\nNuevo orden:" << std::endl;
                    mostrarCanciones(manager.getPlaylistSongs());
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
                if (!archivoYaCargado) {
                    std::cout << "Primero debe cargar el archivo CSV (opción 1)." << std::endl;
                    pausar();
                    break;
                }

                int opcionBusqueda;
                do {
                    mostrarMenuBusqueda();
                    if (!(std::cin >> opcionBusqueda)) {
                        limpiarBuffer();
                        continue;
                    }
                    limpiarBuffer();

                    switch (opcionBusqueda) {
                        case 1: {
                            std::cout << "Ingrese el prefijo del nombre del artista: ";
                            std::string prefijo;
                            std::getline(std::cin, prefijo);
                            auto resultados = manager.searchByArtist(prefijo);
                            std::cout << "\nResultados encontrados: " << resultados.size() << std::endl;
                            mostrarCanciones(resultados);
                            pausar();
                            break;
                        }
                        case 2: {
                            std::cout << "Ingrese el prefijo del nombre de la canción: ";
                            std::string prefijo;
                            std::getline(std::cin, prefijo);
                            auto resultados = manager.searchByTitle(prefijo);
                            std::cout << "\nResultados encontrados: " << resultados.size() << std::endl;
                            mostrarCanciones(resultados);
                            pausar();
                            break;
                        }
                        case 3: {
                            std::cout << "Ingrese el género a buscar: ";
                            std::string genero;
                            std::getline(std::cin, genero);
                            auto resultados = manager.searchByGenre(genero);
                            std::cout << "\nResultados encontrados: " << resultados.size() << std::endl;
                            mostrarCanciones(resultados);
                            pausar();
                            break;
                        }
                        case 4: {
                            double minDur, maxDur;
                            std::cout << "Ingrese la duración mínima (en segundos): ";
                            if (!(std::cin >> minDur)) {
                                std::cout << "Entrada inválida para duración mínima." << std::endl;
                                limpiarBuffer();
                                pausar();
                                break;
                            }
                            limpiarBuffer();

                            std::cout << "Ingrese la duración máxima (en segundos): ";
                            if (!(std::cin >> maxDur)) {
                                std::cout << "Entrada inválida para duración máxima." << std::endl;
                                limpiarBuffer();
                                pausar();
                                break;
                            }
                            limpiarBuffer();

                            auto resultados = manager.searchByDuration(minDur, maxDur);
                            std::cout << "\nResultados encontrados: " << resultados.size() << std::endl;
                            mostrarCanciones(resultados);
                            pausar();
                            break;
                        }
                    }
                } while (opcionBusqueda != 5);
                break;
            }
            case 9: {
                if (!archivoYaCargado) {
                    std::cout << "Primero debe cargar el archivo CSV (opción 1)." << std::endl;
                    pausar();
                    break;
                }

                int opcionOrden;
                do {
                    mostrarMenuOrdenamiento();
                    if (!(std::cin >> opcionOrden)) {
                        limpiarBuffer();
                        continue;
                    }
                    limpiarBuffer();

                    std::vector<Song> resultados;
                    switch (opcionOrden) {
                        case 1:
                            resultados = manager.getSortedSongs(SortCriteria::POPULARITY);
                            std::cout << "\nCanciones ordenadas por popularidad:" << std::endl;
                            break;
                        case 2:
                            resultados = manager.getSortedSongs(SortCriteria::YEAR);
                            std::cout << "\nCanciones ordenadas por año:" << std::endl;
                            break;
                        case 3:
                            resultados = manager.getSortedSongs(SortCriteria::DURATION);
                            std::cout << "\nCanciones ordenadas por duración:" << std::endl;
                            break;
                        case 4:
                            resultados = manager.getSortedSongs(SortCriteria::DANCEABILITY);
                            std::cout << "\nCanciones ordenadas por bailabilidad:" << std::endl;
                            break;
                    }

                    if (opcionOrden >= 1 && opcionOrden <= 4) {
                        mostrarCanciones(resultados);
                        pausar();
                    }
                } while (opcionOrden != 5);
                break;
            }
            case 10: {
                std::cout << "¡Gracias por usar el programa!" << std::endl;
                break;
            }
            default: {
                std::cout << "Opción inválida. Por favor, intente de nuevo." << std::endl;
                pausar();
            }
        }
    } while (opcion != 10);

    return 0;
}