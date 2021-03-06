// Copyright 2014. All Rights Reserved.
// Author: yeshunping@gmail.com (Shunping Ye)

//  A trivial static http webserver using Libevent's evhttp.

#include <stdio.h>
#include <signal.h>

#include "base/logging.h"
#include "base/callback.h"
#include "thirdparty/gflags/gflags.h"
#include "protorpc/http_server/http_request.h"
#include "protorpc/http_server/http_response.h"
#include "protorpc/http_server/http_server.h"
#include "protorpc/http_server/http_handler.h"
#include "protorpc/http_server/event_loop.h"
#include "thirdparty/event/include/event2/http.h"
#include "thirdparty/event/include/event2/buffer.h"

DEFINE_int32(listen_port, 10012, "");

static bool DumpRequestCallback(protorpc::HttpRequest* request,
                                protorpc::HttpResponse* response) {
  request->Dump();
  response->AppendBuffer("Dump done!");
  return true;
}

int main(int argc, char **argv) {
  google::ParseCommandLineFlags(&argc, &argv, false);

  protorpc::EventLoop event_loop;
  protorpc::HttpServer http_server(&event_loop, FLAGS_listen_port);

  base::ResultCallback2<bool, protorpc::HttpRequest*, protorpc::HttpResponse*>* callback =
      base::NewPermanentCallback(DumpRequestCallback);
  protorpc::DefaultHttpHandler dump_handler(callback);
  http_server.RegisterHttpHandler("/dump", &dump_handler);

  http_server.Start();
  event_loop.Loop();

  return 0;
}
