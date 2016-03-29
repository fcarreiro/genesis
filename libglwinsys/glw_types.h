#ifndef __GLW_TYPES_H__
#define __GLW_TYPES_H__

//////////////////////////////////////////////////////////////////////////
// Point structure
//////////////////////////////////////////////////////////////////////////

typedef struct glw_point {
	// constructors
	glw_point() : x(0), y(0) { }
	glw_point(int a, int b) : x(a), y(b) { }

	// variables
	int x;
	int y;
} glw_point;

//////////////////////////////////////////////////////////////////////////
// Color structures
//////////////////////////////////////////////////////////////////////////

typedef struct glw_color3 {
	// constructors
	glw_color3() : r(0), g(0), b(0) { }
	glw_color3(unsigned char a, unsigned char b,
		unsigned char c) : r(a), g(b), b(c) { }

	// variables
	unsigned char r;
	unsigned char g;
	unsigned char b;
} glw_color3;

typedef struct glw_color4 {
	// constructors
	glw_color4() : r(0), g(0), b(0), a(255) { }
	glw_color4(unsigned char a, unsigned char b,
		unsigned char c, unsigned char d) : r(a), g(b), b(c), a(d) { }

	// variables
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} glw_color4;

//////////////////////////////////////////////////////////////////////////
// Window messsages parameters
//////////////////////////////////////////////////////////////////////////

typedef unsigned long glw_param;

//////////////////////////////////////////////////////////////////////////
// Window messsages
//////////////////////////////////////////////////////////////////////////

typedef enum {
	// common window messages
	GLWM_NONE = 0,
	GLWM_KILL,
	GLWM_MOUSEDOWN,
	GLWM_MOUSEUP,
	GLWM_MOUSEMOVE,
	GLWM_KEYDOWN,
	GLWM_KEYUP,

	// add messages here
	GLWM_LAST
} glw_message;

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
