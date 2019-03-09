﻿/******************************************************************************
 *  Copyright (C) 2000 by Robert Hubley.                                      *
 *  All rights reserved.                                                      *
 *                                                                            *
 *  This software is provided ``AS IS'' and any express or implied            *
 *  warranties, including, but not limited to, the implied warranties of      *
 *  merchantability and fitness for a particular purpose, are disclaimed.     *
 *  In no event shall the authors be liable for any direct, indirect,         *
 *  incidental, special, exemplary, or consequential damages (including, but  *
 *  not limited to, procurement of substitute goods or services; loss of use, *
 *  data, or profits; or business interruption) however caused and on any     *
 *  theory of liability, whether in contract, strict liability, or tort       *
 *  (including negligence or otherwise) arising in any way out of the use of  *
 *  this software, even if advised of the possibility of such damage.         *
 *                                                                            *
 ******************************************************************************

 MD5.H - header file for MD5C.C
 
   Port to Win32 DLL by Robert Hubley 1/5/2000

   Original Copyright:

		Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
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
/******************************************************************************
 *  Copyright (C) 2000 by Robert Hubley.                                      *
 *  All rights reserved.                                                      *
 *                                                                            *
 *  This software is provided ``AS IS'' and any express or implied            *
 *  warranties, including, but not limited to, the implied warranties of      *
 *  merchantability and fitness for a particular purpose, are disclaimed.     *
 *  In no event shall the authors be liable for any direct, indirect,         *
 *  incidental, special, exemplary, or consequential damages (including, but  *
 *  not limited to, procurement of substitute goods or services; loss of use, *
 *  data, or profits; or business interruption) however caused and on any     *
 *  theory of liability, whether in contract, strict liability, or tort       *
 *  (including negligence or otherwise) arising in any way out of the use of  *
 *  this software, even if advised of the possibility of such damage.         *
 *                                                                            *
 ******************************************************************************
*/

/******************************************************************************
 *   2002-4-18 Modified by Liguangyi.									      *
 *   struct MD5_CTX ==> class MD5_CTX.								          *
 *   Take off the Globals Functions                                           *
 ******************************************************************************
*/

/******************************************************************************
 *   call：                                                               *
 *   CMD5 md5T;                                                               *
 *   unsigned char digest[16];                                                *
 *   md5T.MD5Update ((char*)"a", 1);                                          *
 *   md5T.MD5Final (digest);                                                  *
 *   md5T.MD5Update ((char*)"abc", 3);                                        *
 *   md5T.MD5Final (digest);                                                  *
 ******************************************************************************
*/

#ifndef _MD5_H_
#define _MD5_H_

typedef unsigned int uint32;

/* MD5 Class. */
class CMD5  
{
public:
	CMD5();
	virtual ~CMD5();

	void MD5Update ( unsigned char *input, unsigned int inputLen);
	void MD5Final (unsigned char digest[16]);
	void MD5ToHex(unsigned char digest[16],unsigned char dst[34]);

private:
	uint32 state[4];					/* state (ABCD) */
	uint32 count[2];					/* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64];       /* input buffer */
	unsigned char PADDING[64];		/* What? */

private:
	void MD5Init ();
	void MD5Transform (uint32 state[4], unsigned char block[64]);
	void MD5_memcpy (unsigned char* output, unsigned char* input,unsigned int len);
	void Encode (unsigned char *output, uint32 *input,unsigned int len);
	void Decode (uint32 *output, unsigned char *input, unsigned int len);
	void MD5_memset (unsigned char* output,int value,unsigned int len);

};
#endif //_MD5_H_
