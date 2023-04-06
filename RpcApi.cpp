#include "RpcApi.h"

namespace media {

	Server::Server()
	{
		thread_ = new std::thread([&]() {
			srv_.reset(new rpc::server(8081));
			std::function<void(std::string)> fuc = std::bind(&Server::onMessage, this, std::placeholders::_1);
			srv_->bind("onMessage", fuc);

			std::function<void()> fuc2 = std::bind(&Server::onStop, this);
			srv_->bind("onStop", fuc2);

			fprintf(stderr, "start rpc srv!\n");
			srv_->run();
		});
	}

	Server::~Server()
	{}

	bool Server::isQuit()
	{
		return bQuit_;
	}

	void Server::onMessage(std::string message)
	{
		fprintf(stderr, "onMessage\n");
	}

	void Server::onStop()
	{
		fprintf(stderr, "onStop\n");
		bQuit_ = true;
	}



	Client::Client()
	{
		client_.reset(new rpc::client("127.0.0.1", 8080));
	}

	Client::~Client()
	{}
	void Client::test()
	{
		client_->call("foo");
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
}