#ifndef SERVERGATEWAY_H
#define SERVERGATEWAY_H

using namespace std;

tuple<bool,
        string,
        string,
        string,
        int> send_script_to_srv(string uri,
                                string token,
                                string file,
                                string type,
                                string output_file,
                                string output_file_type,
                                string requirements,
                                string output_save_path,
                                string streams_save_path,
                                unsigned int timeout = 600);

tuple<vector<string>,
        vector<string>,
        string> get_server_params(string uri,
                                  unsigned int timeout = 600);

#endif // SERVERGATEWAY_H
