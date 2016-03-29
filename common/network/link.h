#ifndef __LINK_H__
#define __LINK_H__

//////////////////////////////////////////////////////////////////////////
// CLink class
//////////////////////////////////////////////////////////////////////////

class CLink : public CSocket
{
public:
	// default constructor & destructor
	CLink();
	virtual ~CLink();

public:
	// send packet
	bool Send(const CPacket *packet);

	// recieves packet
	bool Recieve(CPacket **packet);

private:
	// listens for incoming connections
	bool Listen(unsigned short port, int queue) { return false; }

	// accepts incoming connection if available (returns new socket)
	CSocket *Accept() const { return NULL; }

	// returns true if there is an incoming connection
	bool IncomingConnection() const { return false; }

	// returns true if we are listening
	bool IsListening() const { return false; }
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
