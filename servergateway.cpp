#include "rest.h"
#include "utils.h"

#include <cpprest/http_client.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace std;

tuple<bool, string, string, string, int> send_script_to_srv(
        string uri, string token, string filepath, string type,
        string output_file, string output_file_type,
        string requirements, string output_save_path,
        string streams_save_path,
        unsigned int timeout) {
    // Convert script to Base64
    unsigned int size;
    char* buff = read_file_to_buff(filepath, size);
    if (buff == NULL)
        return make_tuple(false, string_format("Cannot open file %s for reading.", filepath), "", "", 1);
    string base64EncodedFile = base64_encode(reinterpret_cast<const unsigned char*>(buff), size);
    delete[] buff;

    // construct POST request json payload
    auto jvalue = json::value::object();
    jvalue["code"] = json::value::string(base64EncodedFile);
    jvalue["token"] = json::value::string(token);
    if (!(requirements.empty()))
        jvalue["requirements"] = json::value::string(requirements);
    if (!(output_file.empty()))
        jvalue["output_file"] = json::value::string(output_file);
    if (!(output_file_type.empty()))
        jvalue["output_file_type"] = json::value::string(output_file_type);

    // send POST request
    auto jres = make_rest_request(uri, "POST", string_format("/exec/%s", type.c_str()), jvalue, timeout);

    // Parse response
    bool success;
    string errorMsg = "";
    string stdout = "";
    string stderr = "";
    string output;
    int exit_code = 1;

    success = jres.at(U("success")).as_bool();
    if (jres.has_field(U("errorMsg")))
        errorMsg = jres.at(U("errorMsg")).as_string();
    if (jres.has_field(U("stdout")))
        stdout = jres.at(U("stdout")).as_string();
    if (jres.has_field(U("stderr")))
        stderr = jres.at(U("stderr")).as_string();
    if (jres.has_field(U("output")))
        try {
            output = jres.at(U("output")).as_string();
        } catch (json::json_exception) {
            output = jres.at(U("output")).serialize();
        }

    if (jres.has_field(U("exit_code")))
        exit_code = jres.at(U("exit_code")).as_integer();

    // output parsing and saving
    if (!output.empty() && !output_save_path.empty()) {
        std::string decoded_file = output;
        if (output_file_type == "base64_encoded_binary")
            decoded_file = base64_decode(output);
        if (write_buff_to_file(output_save_path, decoded_file))
            return make_tuple(false, string_format("Cannot open file %s for saving.", output_save_path.c_str()), "", "", 1);
    }

    if (!streams_save_path.empty()) {
        string stdout_path = join_path(streams_save_path, "stdout.txt");
        if (write_buff_to_file(stdout_path, stdout))
            return make_tuple(false, string_format("Cannot open file %s for saving.", stdout_path.c_str()), "", "", 1);

        string stderr_path = join_path(streams_save_path, "stderr.txt");
        if (write_buff_to_file(stderr_path, stderr))
            return make_tuple(false, string_format("Cannot open file %s for saving.", stderr_path.c_str()), "", "", 1);
    }

    return make_tuple(success, errorMsg, stdout, stderr, exit_code);
}

tuple<vector<string>, vector<string>, string> get_server_params(string uri, unsigned int timeout)
{
    json::value jvalue;

    jvalue = make_rest_request(uri, "GET", "/params", json::value::null(), timeout);
    auto jtypes = jvalue.at(U("types")).as_array();
    auto jformats = jvalue.at(U("formats")).as_array();
    string version = jvalue.at(U("version")).as_string();

    vector<string> formats;
    for (auto it=jformats.cbegin();it!=jformats.cend();++it) {
        formats.push_back(it->as_string());
    }

    vector<string> types;
    for (auto it=jtypes.cbegin();it!=jtypes.cend();++it) {
        types.push_back(it->as_string());
    }

    return make_tuple(formats, types, version);
}
