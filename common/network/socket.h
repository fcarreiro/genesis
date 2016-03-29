#ifndef __SOCKET_H__
#define __SOCKET_H__

//////////////////////////////////////////////////////////////////////////
// CSocket class
//////////////////////////////////////////////////////////////////////////

class CSocket
{
public:
	// default constructor (creates socket)
	CSocket();

	// constructs and assigns 
	CSocket(SOCKET former_sock);

	// default destructor
	virtual ~CSocket();

public:
	// connects socket to a host
	bool Connect(const std::string & address, unsigned short port);

	// disconnects socket
	void Disconnect();

	// listens for incoming connections
	bool Listen(unsigned short port, int queue = 5);

	// send data through the socket
	int Send(const char *ptr, int len);

	// recieves data from the other side
	int Recieve(char *ptr, int len);

	// accepts incoming connection if available (returns new socket)
	CSocket *Accept() const;

	// returns true if there is an incoming connection
	bool IncomingConnection() const;

	// returns true if there is incoming data
	bool IncomingData() const;

	// returns true if there was some kind of error
	bool IsError() const;

	// returns true if we can write to the socket
	bool IsWriteable() const;

	// returns true if we are connected
	bool IsConnected() const { return m_bConnected; }

	// returns true if we are listening
	bool IsListening() const { return m_bListening; }

	// returns a string with the hostname
	std::string GetHostName() const { return m_strHostName; }

	// returns a string with IP adress
	std::string GetHostIP() const { return m_strHostIP; }

	// time passed from last activity
	unsigned long Ping() const { return SDL_GetTicks() - m_dwTick; }

private:
	// creates a new socket
	static inline SOCKET Create();

private:
	// O.S. socket handle
	SOCKET m_iSocket;

	// true if connected to a host
	bool m_bConnected;

	// true if listening
	bool m_bListening;

	// holds connected host name
	std::string m_strHostName;

	// holds connected host ip
	std::string m_strHostIP;

protected:
	// last action tick
	unsigned long m_dwTick;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
