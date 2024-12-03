#include <iostream>
#include <limits>
#include <iomanip>
#include "PlaylistManager.h"

using namespace std;

void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pausar() {
    cout << "\nPresione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void mostrarMenu() {
    limpiarPantalla();
    cout << "\n=== Gestor de Lista de Reproducción ===" << endl;
    cout << "1. Cargar canciones desde CSV" << endl;
    cout << "2. Agregar canción a la playlist" << endl;
    cout << "3. Eliminar canción de la playlist" << endl;
    cout << "4. Cambiar orden de una canción" << endl;
    cout << "5. Reproducción aleatoria" << endl;
    cout << "6. Mostrar playlist actual" << endl;
    cout << "7. Mostrar todas las canciones disponibles" << endl;
    cout << "8. Búsqueda avanzada" << endl;
    cout << "9. Ordenar canciones" << endl;
    cout << "10. Salir" << endl;
    cout << "Seleccione una opción: ";
}

void mostrarMenuBusqueda() {
    limpiarPantalla();
    cout << "\n=== Búsqueda Avanzada ===" << endl;
    cout << "1. Buscar por artista" << endl;
    cout << "2. Buscar por nombre de canción" << endl;
    cout << "3. Buscar por género" << endl;
    cout << "4. Volver al menú principal" << endl;
    cout << "Seleccione una opción: ";
}

void mostrarMenuOrdenamiento() {
    limpiarPantalla();
    cout << "\n=== Ordenar Canciones ===" << endl;
    cout << "1. Por popularidad" << endl;
    cout << "2. Por año" << endl;
    cout << "3. Por duración" << endl;
    cout << "4. Por bailabilidad" << endl;
    cout << "5. Volver al menú principal" << endl;
    cout << "Seleccione una opción: ";
}

void mostrarCanciones(const vector<Song>& songs) {
    int index = 0;
    cout << left << setw(5) << "Idx" 
              << setw(30) << "Artista"
              << setw(40) << "Canción"
              << setw(10) << "Año"
              << setw(15) << "Duración (s)"
              << setw(15) << "Popularidad"
              << setw(20) << "Género" << endl;
    cout << string(135, '-') << endl;

    for (const auto& song : songs) {
        cout << left << setw(5) << index++
                  << setw(30) << (song.getArtist().length() > 27 ? song.getArtist().substr(0, 27) + "..." : song.getArtist())
                  << setw(40) << (song.getTrackName().length() > 37 ? song.getTrackName().substr(0, 37) + "..." : song.getTrackName())
                  << setw(10) << song.getYear()
                  << setw(15) << fixed << setprecision(2) << (song.getDuration() / 1000.0)
                  << setw(15) << song.getPopularity()
                  << setw(20) << (song.getGenre().length() > 17 ? song.getGenre().substr(0, 17) + "..." : song.getGenre())
                  << endl;
    }
}

int main() {
    PlaylistManager manager;
    int opcion;
    string filename = "spotify_data.csv";
    bool archivoYaCargado = false;

    do {
        mostrarMenu();
        
        if (!(cin >> opcion)) {
            limpiarBuffer();
            continue;
        }
        
        limpiarBuffer();

        switch (opcion) {
            case 1: {
                if (archivoYaCargado) {
                    cout << "El archivo ya fue cargado anteriormente." << endl;
                } else {
                    cout << "Cargando canciones desde " << filename << "..." << endl;
                    manager.loadFromCSV(filename);
                    archivoYaCargado = true;
                    cout << "Canciones cargadas exitosamente!" << endl;
                }
                pausar();
                break;
            }
            case 2: {
                if (!archivoYaCargado) {
                    cout << "Primero debe cargar el archivo CSV (opción 1)." << endl;
                    pausar();
                    break;
                }
                
                cout << "Ingrese el ID de la canción a agregar: ";
                string trackId;
                getline(cin, trackId);
                
                if (manager.addToPlaylist(trackId)) {
                    cout << "Canción agregada exitosamente!" << endl;
                } else {
                    cout << "No se pudo agregar la canción." << endl;
                }
                pausar();
                break;
            }
            case 3: {
                if (!archivoYaCargado) {
                    cout << "Primero debe cargar el archivo CSV (opción 1)." << endl;
                    pausar();
                    break;
                }
                
                cout << "Playlist actual:" << endl;
                auto playlistSongs = manager.getPlaylistSongs();
                mostrarCanciones(playlistSongs);
                
                if (playlistSongs.empty()) {
                    cout << "La playlist está vacía." << endl;
                    pausar();
                    break;
                }
                
                cout << "\nIngrese el índice de la canción a eliminar: ";
                int idx;
                if (cin >> idx && idx >= 0 && idx < playlistSongs.size()) {
                    if (manager.removeFromPlaylist(playlistSongs[idx].getTrackId())) {
                        cout << "Canción eliminada exitosamente!" << endl;
                    } else {
                        cout << "No se pudo eliminar la canción." << endl;
                    }
                } else {
                    cout << "Índice inválido." << endl;
                }
                limpiarBuffer();
                pausar();
                break;
            }
            case 4: {
                if (!archivoYaCargado) {
                    cout << "Primero debe cargar el archivo CSV (opción 1)." << endl;
                    pausar();
                    break;
                }
                
                auto playlistSongs = manager.getPlaylistSongs();
                if (playlistSongs.empty()) {
                    cout << "La playlist está vacía." << endl;
                    pausar();
                    break;
                }
                
                cout << "Playlist actual:" << endl;
                mostrarCanciones(playlistSongs);
                
                int posActual, nuevaPos;
                cout << "Ingrese la posición actual: ";
                if (!(cin >> posActual)) {
                    cout << "Entrada inválida." << endl;
                    limpiarBuffer();
                    pausar();
                    break;
                }
                
                cout << "Ingrese la nueva posición: ";
                if (!(cin >> nuevaPos)) {
                    cout << "Entrada inválida." << endl;
                    limpiarBuffer();
                    pausar();
                    break;
                }
                
                if (manager.changeOrder(posActual, nuevaPos)) {
                    cout << "Orden cambiado exitosamente!" << endl;
                } else {
                    cout << "No se pudo cambiar el orden." << endl;
                }
                limpiarBuffer();
                pausar();
                break;
            }
            case 5: {
                if (!archivoYaCargado) {
                    cout << "Primero debe cargar el archivo CSV (opción 1)." << endl;
                    pausar();
                    break;
                }
                
                auto playlistSongs = manager.getPlaylistSongs();
                if (playlistSongs.empty()) {
                    cout << "La playlist está vacía." << endl;
                } else {
                    manager.shufflePlaylist();
                    cout << "Playlist reorganizada aleatoriamente!" << endl;
                    cout << "\nNuevo orden:" << endl;
                    mostrarCanciones(manager.getPlaylistSongs());
                }
                pausar();
                break;
            }
            case 6: {
                if (!archivoYaCargado) {
                    cout << "Primero debe cargar el archivo CSV (opción 1)." << endl;
                    pausar();
                    break;
                }
                
                cout << "Playlist actual:" << endl;
                auto playlistSongs = manager.getPlaylistSongs();
                if (playlistSongs.empty()) {
                    cout << "La playlist está vacía." << endl;
                } else {
                    mostrarCanciones(playlistSongs);
                }
                pausar();
                break;
            }
            case 7: {
                if (!archivoYaCargado) {
                    cout << "Primero debe cargar el archivo CSV (opción 1)." << endl;
                    pausar();
                    break;
                }
                
                cout << "Todas las canciones disponibles:" << endl;
                auto songs = manager.getAllSongs();
                if (songs.empty()) {
                    cout << "No hay canciones cargadas." << endl;
                } else {
                    mostrarCanciones(songs);
                }
                pausar();
                break;
            }
            case 8: {
                if (!archivoYaCargado) {
                    cout << "Primero debe cargar el archivo CSV (opción 1)." << endl;
                    pausar();
                    break;
                }

                int opcionBusqueda;
                do {
                    mostrarMenuBusqueda();
                    if (!(cin >> opcionBusqueda)) {
                        limpiarBuffer();
                        continue;
                    }
                    limpiarBuffer();

                    switch (opcionBusqueda) {
                        case 1: {
                            cout << "Ingrese el prefijo del nombre del artista: ";
                            string prefijo;
                            getline(cin, prefijo);
                            auto resultados = manager.searchByArtist(prefijo);
                            cout << "\nResultados encontrados: " << resultados.size() << endl;
                            mostrarCanciones(resultados);
                            pausar();
                            break;
                        }
                        case 2: {
                            cout << "Ingrese el prefijo del nombre de la canción: ";
                            string prefijo;
                            getline(cin, prefijo);
                            auto resultados = manager.searchByTitle(prefijo);
                            cout << "\nResultados encontrados: " << resultados.size() << endl;
                            mostrarCanciones(resultados);
                            pausar();
                            break;
                        }
                        case 3: {
                            cout << "Ingrese el género a buscar: ";
                            string genero;
                            getline(cin, genero);
                            auto resultados = manager.searchByGenre(genero);
                            cout << "\nResultados encontrados: " << resultados.size() << endl;
                            mostrarCanciones(resultados);
                            pausar();
                            break;
                        }
                    }
                } while (opcionBusqueda != 4);
                break;
            }
            case 9: {
                if (!archivoYaCargado) {
                    cout << "Primero debe cargar el archivo CSV (opción 1)." << endl;
                    pausar();
                    break;
                }

                int opcionOrden;
                do {
                    mostrarMenuOrdenamiento();
                    if (!(cin >> opcionOrden)) {
                        limpiarBuffer();
                        continue;
                    }
                    limpiarBuffer();

                    vector<Song> resultados;
                    switch (opcionOrden) {
                        case 1:
                            resultados = manager.getSortedSongs(SortCriteria::POPULARITY);
                            cout << "\nCanciones ordenadas por popularidad:" << endl;
                            break;
                        case 2:
                            resultados = manager.getSortedSongs(SortCriteria::YEAR);
                            cout << "\nCanciones ordenadas por año:" << endl;
                            break;
                        case 3:
                            resultados = manager.getSortedSongs(SortCriteria::DURATION);
                            cout << "\nCanciones ordenadas por duración:" << endl;
                            break;
                        case 4:
                            resultados = manager.getSortedSongs(SortCriteria::DANCEABILITY);
                            cout << "\nCanciones ordenadas por bailabilidad:" << endl;
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
                cout << "¡Gracias por usar el programa!" << endl;
                break;
            }
            default: {
                cout << "Opción inválida. Por favor, intente de nuevo." << endl;
                pausar();
            }
        }
    } while (opcion != 10);

    return 0;
}