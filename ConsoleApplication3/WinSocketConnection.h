namespace WinSocketConnection {
	class WinSocketServer {
		public:
			WinSocketServer( std::string ipAddress, int portNumber);
			~WinSocketServer();
			bool createWinSocket();
			bool sendMessage( char* s);
			void closeWinSocket();
	}; 
};