#include "MD5File.h"

#define BUFLENGTH	512

#pragma warning(disable:4996)
CMD5File::CMD5File()
{

}

CMD5File::~CMD5File()
{

}
bool CMD5File::GetFileMD5Bin(LPCTSTR strFileName, unsigned char* strMD5Var, int nMD5VarLen)
{
	// 如果存放 MD5 值的缓冲区长度太小则返回
	if(nMD5VarLen <16)
	{
		return false;
	}
	
	HANDLE hFile = ::CreateFile(strFileName,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	CMD5 MD5;
	unsigned char strFileBuf[BUFLENGTH] = {0};
	DWORD dwBytesRead = 0;
	while(::ReadFile(hFile,strFileBuf,BUFLENGTH,&dwBytesRead,NULL))
	{
		if (0 == dwBytesRead)
		{
			break;
		}
		MD5.MD5Update(strFileBuf,dwBytesRead);
	}

	// 得到 MD5 值
	MD5.MD5Final(strMD5Var);

	CloseHandle(hFile);
	return 1;
}
bool CMD5File::GetFileMD5ASCII(LPCTSTR strFileName,TCHAR* strMD5Var, int nMD5VarLen)
{
	// 得到该文件的 md5 码
	unsigned char digest[16] = {0};
	//int nResult = GetFileMD5Bin (strFileName,digest,16);
	if(!GetFileMD5Bin (strFileName,digest,16))
	{
		return false;
	}
	// 将 md5 码转换为字符串的形式
	TCHAR strMD5[40] = {0};
	for(int j = 0;j < 16;j++)
	{
		TCHAR ch[4] = _T("");
		_stprintf(ch,_T("%.2X"),digest[j]);
		_tcscat(strMD5,ch);
	}

	if(nMD5VarLen < (int)_tcslen(strMD5))
	{
		return false;
	}

	_tcscpy(strMD5Var , strMD5);

	return true;
}

#pragma warning(default:4996)