#ifndef __LTINTEGER_H__
#define __LTINTEGER_H__


/*!
Portable integer types.
*/
typedef unsigned int        uint; // This is at least 16 bits

typedef char			int8;
typedef short int		int16;
typedef int				int32;

#ifdef __LINUX
typedef long long       int64;
#else
typedef __int64         int64;
#endif

typedef unsigned char		uint8;
typedef unsigned short int	uint16;
typedef unsigned int		uint32;

#ifdef __LINUX
typedef unsigned long long  uint64;
#else
typedef unsigned __int64    uint64;
#endif


#define LTFALSE		0
#define LTTRUE		1
#define MAX_PATH          260

template<class T, class TB>
__inline T LTMIN(T a, TB b) { return ((a < (T)b) ? a : (T)b); }
template<class T, class TB>
__inline T LTMAX(T a, TB b) { return ((a >(T)b) ? a : (T)b); }

#define VEC_MIN(v, a, b) \
{\
    (v).x = LTMIN((a).x, (b).x);\
    (v).y = LTMIN((a).y, (b).y);\
    (v).z = LTMIN((a).z, (b).z);\
}

#define VEC_MAX(v, a, b) \
{\
    (v).x = LTMAX((a).x, (b).x);\
    (v).y = LTMAX((a).y, (b).y);\
    (v).z = LTMAX((a).z, (b).z);\
}
#define STREAM_READ(_x_) fread(&(_x_),sizeof(_x_),1,pFile);

#define MATH_PI             3.141592653589793f




/*!
Base type values (enumeration emulation).
LTBOOL.
*/
#define LTFALSE		0
#define LTTRUE		1


enum
{

	/*!
	No problem.
	*/
	LT_OK = 0,

	/*!
	Problem.
	*/
	LT_ERROR = 1,
};

#define LTNULL nullptr

#define E_DEBUG printf("%d %s \n", __LINE__,__FUNCSIG__);
#endif
