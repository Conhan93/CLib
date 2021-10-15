#ifndef CLIB_DEF_H_INCLUDED
#define CLIB_DEF_H_INCLUDED


#define min(x, y) (x < y ? x : y)
#define max(x, y) (x > y ? x : y)
#define abs(x) (x > 0 ? x : (-x))
#define constrain(x, l, h) (x < l ? l : x > h ? h : x)


#endif