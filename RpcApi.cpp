#include "RpcApi.h"

namespace media {

SDKServer::SDKServer(int port)
{
    thread_ = new std::thread([&,port]() {
        srv_.reset(new rpc::server(port));
        std::function<void(float)> fuc = std::bind(&SDKServer::onProgress, this, std::placeholders::_1);
        srv_->bind("progress", fuc);

        std::function<int(int)> fuc2 = std::bind(&SDKServer::onState, this, std::placeholders::_1);
        srv_->bind("state", fuc2);

        fprintf(stderr, "start rpc srv!\n");
        srv_->run();
    });
}

SDKServer::~SDKServer()
{
    stopServer();
    delete thread_;
}

int SDKServer::stopServer()
{
    srv_->stop();
    thread_->join();
    return 0;
}

void SDKServer::setCallback(std::function<void(int state, float progress)> cb)
{
    cb_ = cb;
}

void SDKServer::onProgress(float v)
{
    fprintf(stderr, "%s - %d %f\n", __FUNCTION__, __LINE__, v);
    if (cb_)
    {
        cb_(Running, v);
    }
}

int SDKServer::onState(int state)
{
    fprintf(stderr, "%s - %d state:%d\n", __FUNCTION__, __LINE__, state);
    if (cb_)
    {
        cb_(state, 100);
    }
    return 0;
}

SDKClient::SDKClient(int port)
{
    client_.reset(new rpc::client("127.0.0.1", port));
    port_ = port;
}

SDKClient::~SDKClient()
{}

void SDKClient::stop()
{
    if (is_connect())
        client_->call("stop");
}

bool SDKClient::is_connect()
{
    auto state = client_->get_connection_state();
    if (state != rpc::client::connection_state::connected)
        client_.reset(new rpc::client("127.0.0.1", port_));

    state = client_->get_connection_state();
    if (state == rpc::client::connection_state::connected)
        return true;
    return false;
}

AppServer::AppServer(int port)
{
    thread_ = new std::thread([&,port]() {
        srv_.reset(new rpc::server(port));

        std::function<void()> fuc2 = std::bind(&AppServer::onStop, this);
        srv_->bind("stop", fuc2);

        fprintf(stderr, "start rpc srv!\n");
        srv_->run();
    });
}

AppServer::~AppServer()
{
    stopServer();
}

int AppServer::stopServer()
{
    srv_->stop();
    thread_->join();
    return 0;
}

bool AppServer::isQuit()
{
    return bQuit_;
}

void AppServer::onStop()
{
    fprintf(stderr, "onStop\n");
    bQuit_ = true;
}

AppClient::AppClient(int port)
{
    client_.reset(new rpc::client("127.0.0.1", port));
    port_ = port;
}

AppClient::~AppClient()
{}

void AppClient::progress(float v)
{
    if (is_connect())
        client_->call("progress", v);
}

int AppClient::state(int s)
{
    if (is_connect())
        client_->call("state", s);
    return 0;
}

bool AppClient::is_connect()
{
    auto state = client_->get_connection_state();
    if (state != rpc::client::connection_state::connected)
        client_.reset(new rpc::client("127.0.0.1", port_));

    state = client_->get_connection_state();
    if (state == rpc::client::connection_state::connected)
        return true;
    return false;
}

static AppServer *g_srv = nullptr;
void setGlobalAppServer(AppServer *srv)
{
    g_srv = srv;
}

AppServer *getGlobalAppServer()
{
    return g_srv;
}

static AppClient *g_cli = nullptr;
void setGlobalAppClient(AppClient *cli)
{
    g_cli = cli;
}

AppClient *getGlobalAppClient()
{
    return g_cli;
}
}
