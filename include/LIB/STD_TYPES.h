/*
 * STD_TYPES.h
 *
 * Created: 8/9/2025 11:03:51 AM
 *  Author: Mohammed Saeed
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_
/***********************************************************************/
typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long int u32;
typedef unsigned long long u64;
/********************************************************************/
typedef signed char s8;
typedef signed int s16;
typedef signed long int s32;
typedef signed long long s64;
/************************************************************************/
typedef float f32;
typedef double f64;
/**********************************************************************/
// Boolean 0 , 1
typedef enum
{
	FALSE=0,
	TRUE=1
}boolean;

// ERROR
typedef enum
{
	OK,
	NOK,
	NULL_PTR,
	OUT_OF_RANGE,
	IN_PROGRESS,
	FULL,
	EMPTY,
	TIMEOUT
}error_t;

typedef void(*PTF)(void);


#endif /* STD_TYPES_H_ */
