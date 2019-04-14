#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <chrono>

using namespace web;
using namespace web::http;
using namespace web::http::client;

void display_json(json::value const & jvalue) {
    std::cout << jvalue.serialize() << std::endl;
}

pplx::task<http_response> make_task_request(
   http_client & client,
   method mtd,
   utility::string_t const & path,
   json::value const & jvalue)
{
   return (mtd == methods::GET) ?
      client.request(mtd, path) :
      client.request(mtd, path, jvalue);
}

json::value make_rest_request(
   std::string uri,
   method mtd,
   utility::string_t const & path,
   json::value const & jvalue,
   unsigned int timeout)
{
   http_client_config httpconf;
   httpconf.set_timeout(std::chrono::seconds(timeout));

   http_client client(U(uri), httpconf);

   json::value res = json::value::null();
   status_code httpcode;

   std::cout << "REQUEST:" << std::endl;
   std::cout << mtd << " " << path << std::endl;
   display_json(jvalue);

   make_task_request(client, mtd, path, jvalue)
      .then([&httpcode](http_response response)
      {
         httpcode = response.status_code();
         try {
            return response.extract_json();
         } catch (http_exception const & e) {
            return pplx::task_from_result(json::value());
         }
      })
      .then([&res, &httpcode](pplx::task<json::value> previousTask)
      {
         try
         {
            std::cout << "RESPONSE:" << std::endl;
            std::cout << "HTTP CODE " << httpcode << std::endl;
            res = previousTask.get();
            display_json(res);
         }
         catch (http_exception const & e)
         {
            std::cout << "ERROR: \n" << e.what() << std::endl;
         }
      })
      .wait();

   return res;
}
