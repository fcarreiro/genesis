#ifndef __TIMER_H__
#define __TIMER_H__

//////////////////////////////////////////////////////////////////////////
// CTimer class
//////////////////////////////////////////////////////////////////////////

class CTimer {
public:
	// default contructor
	CTimer() { dwLastTick = GetTickCount(); }

	// updates the last tick
	void Update() { dwLastTick = GetTickCount(); }

	// gets the number of ms that have passed
	unsigned long GetTimePassed() const { return GetTickCount() - dwLastTick; }

private:
	// holds the last tick in ms
	unsigned long dwLastTick;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif