#include "workers.h"
#include "servergateway.h"

using namespace std;

SendScriptToSrvWorker::SendScriptToSrvWorker(string uri, string token,
                                             string filepath, string type,
                                             string output_file, string output_file_type,
                                             string requirements, string output_save_path,
                                             string streams_save_path,
                                             unsigned int timeout)
{
    this->uri = uri;
    this->token = token;
    this->filepath = filepath;
    this->type = type;
    this->output_file = output_file;
    this->output_file_type = output_file_type;
    this->requirements = requirements;
    this->output_save_path = output_save_path;
    this->streams_save_path = streams_save_path;
    this->timeout = timeout;

    qRegisterMetaType<string>("string");
}

void SendScriptToSrvWorker::run() {
    auto res = send_script_to_srv(this->uri, this->token, this->filepath, this->type, this->output_file, this->output_file_type, this->requirements, this->output_save_path, this->streams_save_path, this->timeout);
    auto success = get<0>(res);
    auto errorMsg = get<1>(res);
    auto stdout = get<2>(res);
    auto stderr = get<3>(res);
    auto return_code = get<4>(res);
    emit resultReady(success, errorMsg, stdout, stderr, return_code);
}
