#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMovie>
#include <tuple>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QMovie* movie;
    std::string save_streams_directory;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    void showEvent( QShowEvent* event );
    void on_SendScriptToSrvWorker_finish(bool,
                                         std::string,
                                         std::string,
                                         std::string,
                                         int);
    void start_loading();
    void stop_loading();
};

#endif // MAINWINDOW_H
