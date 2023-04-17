#ifndef __HTTP_CLIENT_2_H_  //NOLINT
#define __HTTP_CLIENT_2_H_

#ifndef HTTP_CLIENT_BUFFER_SIZE
#define HTTP_CLIENT_BUFFER_SIZE 1024
#endif

#include "Particle.h"
// #include "spark_wiring_string.h"
// #include "spark_wiring_tcpclient.h"
// #include "spark_wiring_usbserial.h"
#include "IChunkStream.h"

/**
 * Defines for the HTTP methods.
 */
static const char* HTTP_METHOD_GET    = "GET";
static const char* HTTP_METHOD_POST   = "POST";
static const char* HTTP_METHOD_PUT    = "PUT";
static const char* HTTP_METHOD_DELETE = "DELETE";
static const char* HTTP_METHOD_PATCH  = "PATCH";

/**
 * This struct is used to pass additional HTTP headers such as API-keys.
 * Normally you pass this as an array. The last entry must have NULL as key.
 */
typedef struct {
  const char* header;
  const char* value;
} http_header_t;

/**
 * HTTP Request struct.
 * hostname request host
 * path	 request path
 * port     request port
 * body	 request body
 */
typedef struct {
  String hostname;
  IPAddress ip;
  String path;
  // TODO: Look at setting the port by default.  //NOLINT
  // int port = 80;
  int port;
  String body;
  uint16_t timeout;
} http_request_t;

/**
 * HTTP Response struct.
 * status  response status code.
 * body	response body
 */
typedef struct {
  int status;
  String body;
} http_response_t;

class HttpClient2 {
 public:
  /**
  * Public references to variables.
  */
  TCPClient client;
  char buffer[HTTP_CLIENT_BUFFER_SIZE];

  /**
  * Constructor.
  */
  HttpClient2(void);

  /**
  * HTTP request methods.
  * Can't use 'delete' as name since it's a C++ keyword.
  */
  void get(
      const http_request_t &aRequest,
      http_response_t &aResponse) {
    request(aRequest, aResponse, reinterpret_cast<http_header_t*>(NULL), HTTP_METHOD_GET);
  }

  void post(
      const http_request_t &aRequest,
      http_response_t &aResponse) {
    request(aRequest, aResponse, reinterpret_cast<http_header_t*>(NULL), HTTP_METHOD_POST);
  }

  void put(
      const http_request_t &aRequest,
      http_response_t &aResponse) {
    request(aRequest, aResponse, reinterpret_cast<http_header_t*>(NULL), HTTP_METHOD_PUT);
  }

  void del(
      const http_request_t &aRequest,
      http_response_t &aResponse) {
    request(aRequest, aResponse, reinterpret_cast<http_header_t*>(NULL), HTTP_METHOD_DELETE);
  }

  void get(
      const http_request_t &aRequest,
      http_response_t &aResponse,
      const http_header_t headers[]) {
    request(aRequest, aResponse, headers, HTTP_METHOD_GET);
  }

  void post(
      const http_request_t &aRequest,
      http_response_t &aResponse,
      const http_header_t headers[]) {
    request(aRequest, aResponse, headers, HTTP_METHOD_POST);
  }

  void post(
      const http_request_t &aRequest,
      http_response_t &aResponse,
      const http_header_t headers[],
      IChunkStream* stream) {
    request(aRequest, aResponse, headers, HTTP_METHOD_POST, stream);
  }

  void put(
      const http_request_t &aRequest,
      http_response_t &aResponse,
      const http_header_t headers[]) {
    request(aRequest, aResponse, headers, HTTP_METHOD_PUT);
  }

  void del(
      const http_request_t &aRequest,
      http_response_t &aResponse,
      const http_header_t headers[]) {
    request(aRequest, aResponse, headers, HTTP_METHOD_DELETE);
  }

  void patch(
      const http_request_t &aRequest,
      http_response_t &aResponse,
      const http_header_t headers[]) {
    request(aRequest, aResponse, headers, HTTP_METHOD_PATCH);
  }

 private:
  /**
  * Underlying HTTP methods.
  */
  void request(
      const http_request_t &aRequest,
      http_response_t &aResponse,
      const http_header_t headers[],
      const char* aHttpMethod);

  void request(
      const http_request_t &aRequest,
      http_response_t &aResponse,
      const http_header_t headers[],
      const char* aHttpMethod,
      IChunkStream* stream);

  void sendHeader(
      const char* aHeaderName,
      const char* aHeaderValue);

  void sendHeader(
      const char* aHeaderName,
      const int aHeaderValue);

  void sendHeader(
      const char* aHeaderName);
};

#endif /* __HTTP_CLIENT_2_H_ */  //NOLINT
