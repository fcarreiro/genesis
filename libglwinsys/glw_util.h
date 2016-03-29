#ifndef __GLW_UTIL_H__
#define __GLW_UTIL_H__

//////////////////////////////////////////////////////////////////////////
// isInBounds() : Returns true if the point is inside the box
//////////////////////////////////////////////////////////////////////////

inline bool isInBounds(int x, int y, int bx1, int by1, int bx2, int by2)
{
	if(x > bx1 &&
	   y > by1 &&
	   x < bx2 &&
	   y < by2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
