#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>
#include "PlaylistManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadCSV();
    void addToPlaylist();
    void removeFromPlaylist();
    void shufflePlaylist();
    void searchSongs();
    void sortSongs();
    void updateSongTable(const std::vector<Song>& songs, QTableWidget* table);

private:
    Ui::MainWindow *ui;
    PlaylistManager manager;
    void setupConnections();
    void setupStyles();
};
#endif // MAINWINDOW_H