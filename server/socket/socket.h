#ifndef __SOCKET_H__
#define __SOCKET_H__

//////////////////////////////////////////////////////////////////////////
// CSocket class
//////////////////////////////////////////////////////////////////////////

class CSocket
{
public:
	// default constructor & destructor
	CSocket(SOCKET sock = 0);
	virtual ~CSocket();

public:
	// read data from the socket
	void ReadData();

	// returns the internal socket number
	SOCKET GetSocket() const { return m_iSocket; }
	
	// returns true if there is data pending to go out
	virtual bool IsDataPendingOut() const { return false; }

	// process what we've read
	virtual void ProcessData() { }

protected:
	// packet database type
	typedef std::list<CPacket*> PACKET_DB;

protected:
	// internal socket number
	SOCKET m_iSocket;

	// last recieved timestamp
	unsigned long m_dwLast;

	// list of packet pointers
	PACKET_DB m_Packets;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
