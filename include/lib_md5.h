/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : md5.h
* Author       :
* Description  :
* Function List:
* History      :
************************************************************************************
*/

/* MD5.H - header file for MD5C.cpp
 */

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
 */

#ifndef MD5_H
#define MD5_H

#include <stdio.h>
#include <string.h>

typedef unsigned char *    POINTER;
typedef unsigned short int UINT2;
typedef unsigned long  int UINT4;

//MD5 context
typedef struct {
  UINT4 state[4];                                   /* state (ABCD) */
  UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;

//MD5基本加密函数
static void MD5Init (MD5_CTX *);
static void MD5Update(MD5_CTX *, unsigned char *, unsigned int);
static void MD5Final (unsigned char [16], MD5_CTX *);

//加密字符串
static int MD5String(char* inputBuf, char* outBuf);

//加密文件
static int MD5File(const char* fileName, char* outStr);

//将MD5的加密字符串变成16进制的可以显示的字符串
static int MD5_Hex2Char(char inputBuf[16], char outBuf[33]);

#endif /*MD5_H*/

/********************************** END *******************************************/

