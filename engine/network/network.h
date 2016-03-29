#ifndef __NETWORK_H__
#define __NETWORK_H__

//////////////////////////////////////////////////////////////////////////
// CNetworkSystem class
//////////////////////////////////////////////////////////////////////////

class CNetworkSystem
{
public:
	// constructor & destructor
	CNetworkSystem();
	~CNetworkSystem();

public:
	// functions
	bool Startup();
	void Shutdown();

	bool Connect(const char *host, unsigned short port);
	void Disconnect();

	bool IsConnected() { return (socket != 0); }

	void Control();

	// packets
	bool IncomingData();
	unsigned char RecievePacket(void **buf);
	bool SendPacket(unsigned char packetID, void *buffer, int size);

private:
	SOCKET socket;
	void *buffer;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
