#ifndef REST_H
#define REST_H

#include <cpprest/http_client.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;

json::value make_rest_request(
   std::string uri,
   method mtd,
   utility::string_t const & path,
   json::value const & jvalue,
   unsigned int timeout = 60);

#endif // REST_H
