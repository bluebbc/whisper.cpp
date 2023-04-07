#pragma once

#include "rpc/client.h"
#include "rpc/server.h"

#define SERVER_EXPORTS
#ifdef SERVER_EXPORTS
#define SERVER_API __declspec(dllexport)
#else
#define SERVER_API __declspec(dllimport)
#endif

namespace media {
enum APPState{
    stoped_ = 0,
    running_
};
class  SERVER_API SDKServer {
public:
    SDKServer(int port = 8081);
    ~SDKServer();
    int stopServer();

private:
    void onProgress(float v);
    int onState(int state);

private:
    std::unique_ptr<rpc::server> srv_;
    std::thread* thread_;
};

class SERVER_API  SDKClient {
public:
    SDKClient(int port = 8080);
    ~SDKClient();
    void stop();
private:
    bool is_connect();
private:
    std::unique_ptr<rpc::client> client_;
    int port_;
};

class  SERVER_API AppServer {
public:
    AppServer(int port = 8081);
    ~AppServer();
    int stopServer();
    bool isQuit();

private:
    void onStop();

private:
    std::unique_ptr<rpc::server> srv_;
    std::thread* thread_;
    bool bQuit_ = false;
};

class SERVER_API  AppClient {
public:
    AppClient(int port = 8080);
    ~AppClient();
    void progress(float v);
    int state(int s);
private:
    bool is_connect();
private:
    std::unique_ptr<rpc::client> client_;
    int port_;
};

SERVER_API void setGlobalAppServer(AppServer *srv);
SERVER_API AppServer *getGlobalAppServer();

SERVER_API void setGlobalAppClient(AppClient *cli);
SERVER_API AppClient *getGlobalAppClient();

}
