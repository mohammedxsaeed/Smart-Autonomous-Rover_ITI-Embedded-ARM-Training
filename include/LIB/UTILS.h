/*
 * UTILS.h
 *
 * Created: 8/9/2025 11:13:28 AM
 *  Author: Mohammed Saeed
 */


#ifndef UTILS_H_
#define UTILS_H_
/*******************************************************************/
// SET BIT in Register by 1 , Select REG and Bit No.
#define SET_BIT(REG,BIT_NUM)  (REG |=(1<<BIT_NUM))

// SET BIT in Register by 0 , Select REG and Bit No.
#define CLR_BIT(REG,BIT_NUM)  (REG &=~(1<<BIT_NUM))

// Toggle Selected Pin in Register
#define TGL_BIT(REG,BIT_NUM)  (REG ^=(1<<BIT_NUM))

// Check or read Specific bit from Register ( 0 / 1)
#define GET_BIT(REG,BIT_NUM)  ((REG>>BIT_NUM) & (1))

/*******************************************************************/
// Set all Pins in Register by 1
#define SET_PORT(REG) REG=0XFF

// Set all Pins in Register by 0
#define CLR_PORT(REG) REG=0X00

/*****************************************************************/

#define NULL    ((void*)0)


#endif /* UTILS_H_ */
