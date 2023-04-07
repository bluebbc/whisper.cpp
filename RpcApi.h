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

	class  SERVER_API Server {
	public:
		Server(int port = 8081);
		~Server();
        int stopServer();
		bool isQuit();

	private:
		void onProgress(float v);
		void onStop();

	private:
		std::unique_ptr<rpc::server> srv_;
		std::thread* thread_;
		bool bQuit_ = false;
	};

	class SERVER_API  Client {
	public:
		Client(int port = 8080);
		~Client();
		void test();
		void progress(float v);
		void stop();
    private:
        bool is_connect();
	private:
		std::unique_ptr<rpc::client> client_;
        int port_;
	};

	SERVER_API void setGlobalServer(Server *srv);
	SERVER_API Server *getGlobalServer();

	SERVER_API void setGlobalClient(Client *cli);
	SERVER_API Client *getGlobalClient();

}
