#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QShowEvent>
#include <QMovie>
#include <QString>

#include <iostream>
#include "vars.h" // provides SERVER_URI_ENDPOINT
#include "servergateway.h"
#include "utils.h"
#include "workers.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    movie = new QMovie(":/images/images/loading.gif");
    if (!movie->isValid())
        std::cout << "loading.gif could not be loaded." << std::endl;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent( QShowEvent* event ) {
    QWidget::showEvent( event );

    this->ui->loadingLabel->setMovie(this->movie);
    this->ui->serverLineEdit->setText(SERVER_URI_ENDPOINT);

    auto t = get_server_params(SERVER_URI_ENDPOINT, REQUEST_TIMEOUT);
    auto formats = get<0>(t);
    auto types = get<1>(t);
    auto version = get<2>(t);

    for (unsigned int i=0; i < types.size(); i++)
       ui->scriptTypeComboBox->addItem(QString::fromStdString(types[i]));

    for (unsigned int i=0; i < formats.size(); i++)
       ui->outputFormatComboBox->addItem(QString::fromStdString(formats[i]));

    ui->label->setText(QString::fromStdString(string_format("Welcome to the RemotePyLauncher, by <b>Itay Brandes</b>.<br>Server is online (v%s).", version.c_str())));
}

void MainWindow::on_pushButton_clicked()
{
    auto token = this->ui->serverTokenLineEdit->text().toStdString();
    auto output_filename = this->ui->outputFilenameLineEdit->text().toStdString();
    auto script_path = this->ui->chooseFileToLaunchLineEdit->text().toStdString();
    auto dependencies = this->ui->scriptDependenciesLineEdit->text().toStdString();
    auto type = this->ui->scriptTypeComboBox->currentText().toStdString();
    auto output_file_type = this->ui->outputFormatComboBox->currentText().toStdString();
    auto save_output_as = this->ui->savedOutputsDirectoryLineEdit->text().toStdString();
    auto save_streams = this->ui->SaveStreamsCheckbox->isChecked();

    // sanity checks
    if (script_path.empty()) {
        QMessageBox::critical(this, "Error", "Script path is empty.");
        return;
    }
    if (token.empty()) {
        QMessageBox::critical(this, "Error", "Server token is empty.");
        return;
    }
    if (save_output_as.empty() && save_streams) {
        QMessageBox::critical(this, "Error", "Cannot save standard streams if the save location is empty.");
        return;
    }

    // calculate save_streams_directory
    string save_streams_directory = "";
    if (save_streams)
        save_streams_directory = get_directory_path(save_output_as);
    this->save_streams_directory = save_streams_directory; // save for use later

    // everything ok
    this->start_loading();

    SendScriptToSrvWorker* t = new SendScriptToSrvWorker(SERVER_URI_ENDPOINT, token, script_path, type, output_filename, output_file_type, dependencies, save_output_as, save_streams_directory, REQUEST_TIMEOUT);
    connect(t, &SendScriptToSrvWorker::resultReady, this, &MainWindow::on_SendScriptToSrvWorker_finish);
    //connect(t, &SendScriptToSrvWorker::finished, this, &QObject::deleteLater);
    t->start();

}
void MainWindow::on_SendScriptToSrvWorker_finish(bool success,
                                                 std::string errorMsg,
                                                 std::string stdout,
                                                 std::string stderr,
                                                 int return_code)
{
    this->stop_loading();

    if (success) {
        string s = string_format("Execution is done successfully!<br><br><b>stdout</b>: %s<br><b>stderr</b>: %s<br><b>Return Code</b>: %d", replace_str(stdout, "\n", "<br>").c_str(), replace_str(stderr, "\n", "<br>").c_str(), return_code);
        QMessageBox::information(this, "Info", QString::fromStdString(s));
        if (!this->save_streams_directory.empty())
            QMessageBox::information(this, "Info", QString::fromStdString(string_format("The outputs have been saved successfully in <b>%s</b>.", this->save_streams_directory.c_str())));
    }
    else {
        if (!(errorMsg.empty()))
            QMessageBox::critical(this, "Executing Error", QString::fromStdString(errorMsg));
        else if (!(stderr.empty()))
            QMessageBox::critical(this, "Executing Error", QString::fromStdString(stderr));
        else
            QMessageBox::critical(this, "Executing Error", QString::fromStdString(string_format("Unknown Error. (return code is %d)", return_code)));
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString fn = QFileDialog::getOpenFileName(
                this,
                "Open File",
                "~",
                "Python Script (*.py);; All files (*)");
    this->ui->chooseFileToLaunchLineEdit->setText(fn);
}

void MainWindow::on_pushButton_3_clicked()
{
    QString defaultStr = this->ui->outputFilenameLineEdit->text();
    if (defaultStr.isEmpty())
            defaultStr = QString("output.txt");

    this->ui->outputFilenameLineEdit->text().toStdString();
    QString fn = QFileDialog::getSaveFileName(
                this,
                "Save File",
                defaultStr);
    this->ui->savedOutputsDirectoryLineEdit->setText(fn);
}

void MainWindow::start_loading()
{
    this->ui->loadingLabel->setGeometry(200, 0, 400, 400);
    this->ui->loadingLabel->show();
    this->movie->start();
}

void MainWindow::stop_loading()
{
    this->ui->loadingLabel->hide();
    this->ui->loadingLabel->setGeometry(0, 0, 0, 0);
    this->movie->stop();
}
