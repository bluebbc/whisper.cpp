#pragma once

#include "rpc/client.h"
#include "rpc/server.h"

#ifdef SERVER_EXPORTS
#define SERVER_API __declspec(dllexport)
#else
#define SERVER_API __declspec(dllimport)
#endif

namespace media {

	class  SERVER_API Server {
	public:
		Server();
		~Server();
		bool isQuit();

	private:
		void onMessage(std::string message);
		void onStop();

	private:
		std::unique_ptr<rpc::server> srv_;
		std::thread* thread_;
		bool bQuit_ = false;
	};

	class SERVER_API  Client {
	public:
		Client();
		~Client();
		void test();

	private:
		std::unique_ptr<rpc::client> client_;
	};

	SERVER_API void setGlobalServer(Server *srv);
	SERVER_API Server *getGlobalServer();
}
