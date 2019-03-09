#ifndef _MD5FILE_H_
#define _MD5FILE_H_

#include <Windows.h>
#include <TCHAR.h>
#include "md5.h"

class CMD5File  
{
public:
	CMD5File();
	virtual ~CMD5File();

	/**
	 * 得到文件的 md5 值
	 *
	 * @param strFileName -[in] -为文件名
	 *
	 * @param strMD5Var -[in] -为返回的 md5 值
	 *
	 * @param nMD5VarLen -[in] -strMD5Var 的长度
	 *
	 * @return -[成功执行返回true,否则返回false]
	 */
	bool GetFileMD5Bin(LPCTSTR strFileName, unsigned char* strMD5Var, int nMD5VarLen);
	/**
	 * 得到文件的 md5 值
	 *
	 * @param strFileName -[in] -为文件名
	 *
	 * @param strMD5Var -[in] -为返回的 md5 值
	 *
	 * @param nMD5VarLen -[in] -strMD5Var 的长度
	 *
	 * @return -[成功执行返回true,否则返回false]
	 */
	bool GetFileMD5ASCII(LPCTSTR strFileName,TCHAR* strMD5Var, int nMD5VarLen);
};

#endif //_MD5FILE_H_