//CMD5 类：计算 md5 的类
//CMD5File 类：计算文件的 md5 值
//
//调用方式：
//CMD5 md5T;
//unsigned char digest[16];
//md5T.MD5Update ((char*)"a", 1);
//md5T.MD5Final (digest);
//md5T.MD5Update ((char*)"abc", 3);
//md5T.MD5Final (digest);

#ifndef _MD5EX_H_
#define _MD5EX_H_
#include "MD5.h"
#include "MD5File.h"
#pragma comment(lib,"md5.lib")
#endif //_MD5EX_H_