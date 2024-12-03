#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupConnections();
    setupStyles();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupStyles()
{
    // Apply modern styles to the main window
    setStyleSheet(R"(
        QMainWindow {
            background-color: #f5f5f5;
        }
        QTableWidget {
            background-color: white;
            border-radius: 8px;
            border: 1px solid #e0e0e0;
        }
        QPushButton {
            background-color: #2196F3;
            color: white;
            border-radius: 4px;
            padding: 8px 16px;
            border: none;
        }
        QPushButton:hover {
            background-color: #1976D2;
        }
        QLineEdit, QComboBox {
            padding: 6px;
            border-radius: 4px;
            border: 1px solid #e0e0e0;
            background-color: white;
        }
        QLabel {
            color: #333333;
        }
    )");
}

void MainWindow::setupConnections()
{
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::loadCSV);
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::addToPlaylist);
    connect(ui->removeButton, &QPushButton::clicked, this, &MainWindow::removeFromPlaylist);
    connect(ui->shuffleButton, &QPushButton::clicked, this, &MainWindow::shufflePlaylist);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::searchSongs);
    connect(ui->sortComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::sortSongs);
}

void MainWindow::loadCSV()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open CSV File", "", "CSV Files (*.csv)");
    if (!filename.isEmpty()) {
        manager.loadFromCSV(filename.toStdString());
        updateSongTable(manager.getAllSongs(), ui->songTable);
        QMessageBox::information(this, "Success", "CSV file loaded successfully!");
    }
}

void MainWindow::addToPlaylist()
{
    auto selectedItems = ui->songTable->selectedItems();
    if (!selectedItems.isEmpty()) {
        int row = selectedItems.first()->row();
        QString trackId = ui->songTable->item(row, 2)->text();
        if (manager.addToPlaylist(trackId.toStdString())) {
            updateSongTable(manager.getPlaylistSongs(), ui->playlistTable);
        }
    }
}

void MainWindow::removeFromPlaylist()
{
    auto selectedItems = ui->playlistTable->selectedItems();
    if (!selectedItems.isEmpty()) {
        int row = selectedItems.first()->row();
        QString trackId = ui->playlistTable->item(row, 2)->text();
        if (manager.removeFromPlaylist(trackId.toStdString())) {
            updateSongTable(manager.getPlaylistSongs(), ui->playlistTable);
        }
    }
}

void MainWindow::shufflePlaylist()
{
    manager.shufflePlaylist();
    updateSongTable(manager.getPlaylistSongs(), ui->playlistTable);
}

void MainWindow::searchSongs()
{
    QString searchText = ui->searchInput->text();
    QString searchType = ui->searchComboBox->currentText();
    std::vector<Song> results;

    if (searchType == "Artist") {
        results = manager.searchByArtist(searchText.toStdString());
    } else if (searchType == "Title") {
        results = manager.searchByTitle(searchText.toStdString());
    } else if (searchType == "Genre") {
        results = manager.searchByGenre(searchText.toStdString());
    }

    updateSongTable(results, ui->songTable);
}

void MainWindow::sortSongs()
{
    QString sortType = ui->sortComboBox->currentText();
    std::vector<Song> sortedSongs;

    if (sortType == "Popularity") {
        sortedSongs = manager.getSortedSongs(SortCriteria::POPULARITY);
    } else if (sortType == "Year") {
        sortedSongs = manager.getSortedSongs(SortCriteria::YEAR);
    } else if (sortType == "Duration") {
        sortedSongs = manager.getSortedSongs(SortCriteria::DURATION);
    } else if (sortType == "Danceability") {
        sortedSongs = manager.getSortedSongs(SortCriteria::DANCEABILITY);
    }

    updateSongTable(sortedSongs, ui->songTable);
}

void MainWindow::updateSongTable(const std::vector<Song>& songs, QTableWidget* table)
{
    table->setRowCount(songs.size());
    int row = 0;
    for (const auto& song : songs) {
        table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(song.getArtist())));
        table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(song.getTrackName())));
        table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(song.getTrackId())));
        table->setItem(row, 3, new QTableWidgetItem(QString::number(song.getYear())));
        table->setItem(row, 4, new QTableWidgetItem(QString::number(song.getPopularity())));
        table->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(song.getGenre())));
        row++;
    }
}