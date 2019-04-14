#ifndef WORKERS_H
#define WORKERS_H

#include <QMainWindow>
#include <QThread>
#include <QMetaType>

using namespace std;

class SendScriptToSrvWorker : public QThread
{
Q_OBJECT
public:
    SendScriptToSrvWorker(string uri,
                    string token,
                    string filepath,
                    string type,
                    string output_file,
                    string output_file_type,
                    string requirements,
                    string output_save_path,
                    string streams_save_path,
                    unsigned int timeout = 600);

    private:
        string uri;
        string token;
        string filepath;
        string type;
        string output_file;
        string output_file_type;
        string requirements;
        string output_save_path;
        string streams_save_path;
        unsigned int timeout;

        void run() override;
    signals:
        void resultReady(bool,
                         string,
                         string,
                         string,
                         int);
};

#endif // WORKERS_H
