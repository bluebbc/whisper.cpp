#include "RpcApi.h"

namespace media {

	Server::Server(int port)
	{
		thread_ = new std::thread([&]() {
			srv_.reset(new rpc::server(port));
			std::function<void(float)> fuc = std::bind(&Server::onProgress, this, std::placeholders::_1);
			srv_->bind("progress", fuc);

			std::function<void()> fuc2 = std::bind(&Server::onStop, this);
			srv_->bind("stop", fuc2);

			fprintf(stderr, "start rpc srv!\n");
			srv_->run();
		});
	}

	Server::~Server()
    {
        srv_->stop();
        thread_->join();
    }

    int Server::stopServer()
    {
        srv_->stop();
        return 0;
    }

	bool Server::isQuit()
	{
		return bQuit_;
	}

	void Server::onProgress(float v)
	{
        fprintf(stderr, "onProgress:%f\n", v);
	}

	void Server::onStop()
	{
		fprintf(stderr, "onStop\n");
		bQuit_ = true;
	}

	Client::Client(int port)
	{
		client_.reset(new rpc::client("127.0.0.1", port));
        port_ = port;
	}

	Client::~Client()
	{}

	void Client::test()
    {
        if (is_connect())
            client_->call("foo");
	}

	void Client::progress(float v)
	{
        if (is_connect())
            client_->call("progress", v);
	}

	void Client::stop()
	{
        if (is_connect())
            client_->call("stop");
	}

    bool Client::is_connect()
    {
        auto state = client_->get_connection_state();
        if (state == rpc::client::connection_state::connected)
            client_.reset(new rpc::client("127.0.0.1", port_));

        state = client_->get_connection_state();
        if (state == rpc::client::connection_state::connected)
            return true;
        return false;
    }

	static Server *g_srv = nullptr;
	void setGlobalServer(Server *srv)
	{
		g_srv = srv;
	}

	Server *getGlobalServer()
	{
		return g_srv;
	}

	static Client *g_cli = nullptr;
	void setGlobalClient(Client *cli)
	{
		g_cli = cli;
	}

	Client *getGlobalClient()
	{
		return g_cli;
	}
}
