//-----XXTEA加密------>>

void sqlite3_activate_see(const char *info)
{
}

//不保存用户的原始密码.返回NULL.
void sqlite3CodecGetKey(sqlite3 *db, int nDb, void **ppKey, int *pnKeyLen)
{
	*ppKey = NULL;
	*pnKeyLen = 0;
}

#define CRYPT_OFFSET 8

/*
** This macro rounds values up so that if the value is an address it
** is guaranteed to be an address that is aligned to an 8-byte boundary.
*/
//#define FORCE_ALIGNMENT(X)   (((X)+7)&~7)

typedef struct _CryptBlock
{
	unsigned char*     ReadKey;     // 读数据库和写入事务的密钥
	unsigned char*     WriteKey;    // 写入数据库的密钥
	int       PageSize;    // 页的大小
	unsigned char*     Data;
} CryptBlock, *LPCryptBlock;

//创建或更新一个页的加密算法索引.此函数会申请缓冲区.
static LPCryptBlock CreateCryptBlock(unsigned char* hKey, int pageSize, LPCryptBlock pExisting)
{
	LPCryptBlock pBlock;

	if (!pExisting) //创建新加密块
	{
		pBlock = sqlite3_malloc(sizeof(CryptBlock));
		memset(pBlock, 0, sizeof(CryptBlock));
		pBlock->ReadKey = hKey;
		pBlock->WriteKey = hKey;
		pBlock->PageSize = pageSize;
		pBlock->Data = (unsigned char*)sqlite3_malloc(pBlock->PageSize + CRYPT_OFFSET);
	}
	else //更新存在的加密块
	{
		pBlock = pExisting;
		if ( pBlock->PageSize != pageSize && !pBlock->Data){
			sqlite3_free(pBlock->Data);
			pBlock->PageSize = pageSize;
			pBlock->Data = (unsigned char*)sqlite3_malloc(pBlock->PageSize + CRYPT_OFFSET);
		}
	}


	memset(pBlock->Data, 0, pBlock->PageSize + CRYPT_OFFSET);

	return pBlock;
}

/* POINTER defines a generic pointer type */
typedef unsigned char *POINTER;

/* UINT2 defines a two byte word */
typedef unsigned short int UINT2;

/* UINT4 defines a four byte word */
typedef unsigned long int UINT4;

/* MD5 context. */
typedef struct {
	UINT4 state[4];                                   /* state (ABCD) */
	UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

/* F, G, H and I are basic MD5 functions.
*/
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits.
*/
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
Rotation is separate from addition to prevent recomputation.
*/
#define FF(a, b, c, d, x, s, ac) { \
	(a) += F ((b), (c), (d)) + (x) + (UINT4)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}
#define GG(a, b, c, d, x, s, ac) { \
	(a) += G ((b), (c), (d)) + (x) + (UINT4)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}
#define HH(a, b, c, d, x, s, ac) { \
	(a) += H ((b), (c), (d)) + (x) + (UINT4)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}
#define II(a, b, c, d, x, s, ac) { \
	(a) += I ((b), (c), (d)) + (x) + (UINT4)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}

/* Decodes input (unsigned char) into output (UINT4). Assumes len is
a multiple of 4.
*/
static void Decode (output, input, len)
UINT4 *output;
unsigned char *input;
unsigned int len;
{
	unsigned int i, j;

	for (i = 0, j = 0; j < len; i++, j += 4)
		output[i] = ((UINT4)input[j]) | (((UINT4)input[j+1]) << 8) |
		(((UINT4)input[j+2]) << 16) | (((UINT4)input[j+3]) << 24);
}

static void MD5_memcpy (output, input, len)
POINTER output;
POINTER input;
unsigned int len;
{
	unsigned int i;

	for (i = 0; i < len; i++)
		output[i] = input[i];
}

/* Note: Replace "for loop" with standard memset if possible.
*/
static void MD5_memset (output, value, len)
POINTER output;
int value;
unsigned int len;
{
	unsigned int i;

	for (i = 0; i < len; i++)
		((char *)output)[i] = (char)value;
}

/* MD5 basic transformation. Transforms state based on block.
*/
static void MD5Transform (state, block)
UINT4 state[4];
unsigned char block[64];
{
	UINT4 a = state[0], b = state[1], c = state[2], d = state[3], x[16];

	Decode (x, block, 64);

	/* Round 1 */
	FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
	FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
	FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
	FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
	FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
	FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
	FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
	FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
	FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
	FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
	FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
	FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
	FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
	FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
	FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
	FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

	/* Round 2 */
	GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
	GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
	GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
	GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
	GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
	GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
	GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
	GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
	GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
	GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
	GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
	GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
	GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
	GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
	GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
	GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
	HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
	HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
	HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
	HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
	HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
	HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
	HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
	HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
	HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
	HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
	HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
	HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
	HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
	HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
	HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

	/* Round 4 */
	II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
	II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
	II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
	II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
	II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
	II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
	II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
	II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
	II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
	II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
	II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
	II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
	II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
	II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
	II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
	II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	/* Zeroize sensitive information.
	*/
	MD5_memset ((POINTER)x, 0, sizeof (x));
}

/* MD5 initialization. Begins an MD5 operation, writing a new context.
*/
void MD5Init (context)
MD5_CTX *context;                                        /* context */
{
	context->count[0] = context->count[1] = 0;
	/* Load magic initialization constants.
	*/
	context->state[0] = 0x67452301;
	context->state[1] = 0xefcdab89;
	context->state[2] = 0x98badcfe;
	context->state[3] = 0x10325476;
}

/* MD5 block update operation. Continues an MD5 message-digest
operation, processing another message block, and updating the
context.
*/
void MD5Update (context, input, inputLen)
MD5_CTX *context;                                        /* context */
unsigned char *input;                                /* input block */
unsigned int inputLen;                     /* length of input block */
{
	unsigned int i, index, partLen;

	/* Compute number of bytes mod 64 */
	index = (unsigned int)((context->count[0] >> 3) & 0x3F);

	/* Update number of bits */
	if ((context->count[0] += ((UINT4)inputLen << 3))
		< ((UINT4)inputLen << 3))
		context->count[1]++;
	context->count[1] += ((UINT4)inputLen >> 29);

	partLen = 64 - index;

	/* Transform as many times as possible.
	*/
	if (inputLen >= partLen) {
		MD5_memcpy
			((POINTER)&context->buffer[index], (POINTER)input, partLen);
		MD5Transform (context->state, context->buffer);

		for (i = partLen; i + 63 < inputLen; i += 64)
			MD5Transform (context->state, &input[i]);

		index = 0;
	}
	else
		i = 0;

	/* Buffer remaining input */
	MD5_memcpy
		((POINTER)&context->buffer[index], (POINTER)&input[i],
		inputLen-i);
}

static unsigned char PADDING[64] = {
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* Encodes input (UINT4) into output (unsigned char). Assumes len is
a multiple of 4.
*/
static void Encode (output, input, len)
unsigned char *output;
UINT4 *input;
unsigned int len;
{
	unsigned int i, j;

	for (i = 0, j = 0; j < len; i++, j += 4) {
		output[j] = (unsigned char)(input[i] & 0xff);
		output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
		output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
		output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
	}
}

/* MD5 finalization. Ends an MD5 message-digest operation, writing the
the message digest and zeroizing the context.
*/
void MD5Final (digest, context)
unsigned char digest[16];                         /* message digest */
MD5_CTX *context;                                       /* context */
{
	unsigned char bits[8];
	unsigned int index, padLen;

	/* Save number of bits */
	Encode (bits, context->count, 8);

	/* Pad out to 56 mod 64.
	*/
	index = (unsigned int)((context->count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	MD5Update (context, PADDING, padLen);

	/* Append length (before padding) */
	MD5Update (context, bits, 8);

	/* Store state in digest */
	Encode (digest, context->state, 16);

	/* Zeroize sensitive information.
	*/
	MD5_memset ((POINTER)context, 0, sizeof (*context));
}


// 从用户提供的缓冲区中得到一个加密密钥
static unsigned char * DeriveKey(const void *pKey, int nKeyLen)
{
	unsigned char *  hKey = NULL;
	unsigned char *  digest = NULL;
	MD5_CTX md5ctx;


	if ((pKey == NULL) || (nKeyLen <1 ))
	{
		return NULL;
	}


	digest = sqlite3_malloc(16);

	MD5Init(&md5ctx);

	MD5Update(&md5ctx, (unsigned char*)pKey, nKeyLen);
	MD5Final(digest, &md5ctx);

	hKey = digest;
	return hKey;
}

void * sqlite3pager_get_codecarg(Pager *pPager)
{
	return (pPager->xCodec) ? pPager->pCodec: NULL;
}

int xxtea( int * v, int n , int * k ) {
	unsigned int z/*=v[n-1]*/, y=v[0], sum=0,  e,    DELTA=0x9e3779b9 ;
	int p, q ;
	if ( n>1) {
		/* Coding Part */
		z = v[n-1];
		q = 6+52/n ;
		while ( q-- > 0 ) {
			sum += DELTA ;
			e = sum >> 2&3 ;
			for ( p = 0 ; p < n-1 ; p++ ){
				y = v[p+1],
					z = v[p] += (z>>5^y<<2)+(y>>3^z<<4)^(sum^y)+(k[(p&3)^e]^z);
			}
			y = v[0] ;
			z = v[n-1] += (z>>5^y<<2)+(y>>3^z<<4)^(sum^y)+(k[(p&3)^e]^z);
		}
		return 0 ;

		/* Decoding Part */
	}else if ( n <-1 ) {
		n = -n ;
		q = 6+52/n ;
		sum = q*DELTA ;
		while (sum != 0) {
			e = sum>>2 & 3 ;
			for (p = n-1 ; p > 0 ; p-- ){
				z = v[p-1],
					y = v[p] -= (z>>5^y<<2)+(y>>3^z<<4)^(sum^y)+(k[(p&3)^e]^z);
			}
			z = v[n-1] ;
			y = v[0] -= (z>>5^y<<2)+(y>>3^z<<4)^(sum^y)+(k[(p&3)^e]^z);
			sum -= DELTA ; 
		}
		return 0 ;
	}

	return 1 ;
} /* Signal n=0,1,-1 */

//销毁一个加密块及相关的缓冲区,密钥.
static void DestroyCryptBlock(LPCryptBlock pBlock)
{
	//销毁读密钥.
	if (pBlock->ReadKey){
		sqlite3_free(pBlock->ReadKey);
	}

	//如果写密钥存在并且不等于读密钥,也销毁.
	if (pBlock->WriteKey && pBlock->WriteKey != pBlock->ReadKey){
		sqlite3_free(pBlock->WriteKey);
	}

	if(pBlock->Data){
		sqlite3_free(pBlock->Data);
	}

	//释放加密块.
	sqlite3_free(pBlock);
}

//释放与一个页相关的加密块
void sqlite3pager_free_codecarg(void *pArg)
{
	if (pArg)
		DestroyCryptBlock((LPCryptBlock)pArg);
}

void OnCodecSizeChng(void *pBlock, int pageSize, int nReserve)
{
	// 确保pager的页长度和加密块的页长度相等.如果改变,就需要调整.
	CreateCryptBlock(0, pageSize, pBlock);
}

//加密/解密函数, 被pager调用

void * sqlite3Codec(void *pArg, void *vdata, Pgno nPageNum, int nMode)
{
	LPCryptBlock pBlock = (LPCryptBlock)pArg;
	int len = 0;
	unsigned char* data = (unsigned char*)vdata;

	if (!pBlock) return data;

	switch(nMode)
	{
	case 0: // Undo a "case 7" journal file encryption
	case 2: //重载一个页
	case 3: //载入一个页
		if (!pBlock->ReadKey) break;


		len = 0 - (pBlock->PageSize/4);
		xxtea((int*)data, len, (int*)pBlock->ReadKey);

		break;
	case 6: //加密一个主数据库文件的页
		if (!pBlock->WriteKey) break;

		memcpy(pBlock->Data + CRYPT_OFFSET, data, pBlock->PageSize);
		data = pBlock->Data + CRYPT_OFFSET;


		len = pBlock->PageSize/4;
		xxtea((int*)data , len, (int*)pBlock->WriteKey);
		break;
	case 7: //加密事务文件的页
		/*在正常环境下, 读密钥和写密钥相同. 当数据库是被重新加密的,读密钥和写密钥未必相同.
		回滚事务必要用数据库文件的原始密钥写入.因此,当一次回滚被写入,总是用数据库的读密钥,
		这是为了保证与读取原始数据的密钥相同.
		*/
		if (!pBlock->ReadKey) break;

		memcpy(pBlock->Data + CRYPT_OFFSET, data, pBlock->PageSize);
		data = pBlock->Data + CRYPT_OFFSET;


		len = pBlock->PageSize/4;
		xxtea((int*)data, len, (int*)pBlock->ReadKey);
		break;
	}

	return data;
}

//被sqlite 和 sqlite3_key_interop 调用, 附加密钥到数据库.
int sqlite3CodecAttach(sqlite3 *db, int nDb, const void *pKey, int nKeyLen)
{
	int rc = SQLITE_ERROR;
	unsigned char* hKey = 0;

	//如果没有指定密匙,可能标识用了主数据库的加密或没加密.
	if (!pKey || !nKeyLen)
	{
		if (!nDb)
		{
			return SQLITE_OK; //主数据库, 没有指定密钥所以没有加密.
		}
		else //附加数据库,使用主数据库的密钥. 
		{
			//获取主数据库的加密块并复制密钥给附加数据库使用
			LPCryptBlock pBlock = (LPCryptBlock)sqlite3pager_get_codecarg(sqlite3BtreePager(db->aDb[0].pBt));

			if (!pBlock) return SQLITE_OK; //主数据库没有加密
			if (!pBlock->ReadKey) return SQLITE_OK; //没有加密

			memcpy(pBlock->ReadKey, &hKey, 16);
		}
	}
	else //用户提供了密码,从中创建密钥.
	{
		hKey = DeriveKey(pKey, nKeyLen);
	}

	//创建一个新的加密块,并将解码器指向新的附加数据库. 
	if (hKey)
	{
		LPCryptBlock pBlock = CreateCryptBlock(hKey, sqlite3BtreePager(db->aDb[nDb].pBt)->pageSize, NULL);
		sqlite3PagerSetCodec(sqlite3BtreePager(db->aDb[nDb].pBt), sqlite3Codec, OnCodecSizeChng, sqlite3pager_free_codecarg, pBlock);

		rc = SQLITE_OK;
	}
	return rc;
}


//密钥并不保留到临时空间,仅保存于主数据库.
int sqlite3_key_interop(sqlite3 *db, const void *pKey, int nKeySize)
{
	return sqlite3CodecAttach(db, 0, pKey, nKeySize);
}


int sqlite3_key(sqlite3 *db, const void *pKey, int nKey)
{
	return sqlite3_key_interop(db, pKey, nKey);
}

int sqlite3_key_v2(sqlite3 *db,const char *zDbName,const void *pKey,int nKey)
{
	return sqlite3_key(db, pKey, nKey);
}

//改变已有数据库的加密密钥
int sqlite3_rekey_interop(sqlite3 *db, const void *pKey, int nKeySize)
{
	Btree *pbt = db->aDb[0].pBt;
	Pager *p = sqlite3BtreePager(pbt);
	LPCryptBlock pBlock = (LPCryptBlock)sqlite3pager_get_codecarg(p);
	unsigned char * hKey = DeriveKey(pKey, nKeySize);
	int rc = SQLITE_ERROR;
	Pgno nPage;
	Pgno nSkip;
	void *pPage;
	Pgno n;


	if (!pBlock && !hKey) return SQLITE_OK; 

	//重新加密一个数据库,改变pager的写密钥, 读密钥依旧保留.
	if (!pBlock) //加密一个未加密的数据库
	{
		pBlock = CreateCryptBlock(hKey, p->pageSize, NULL);
		pBlock->ReadKey = 0; // 原始数据库未加密
		sqlite3PagerSetCodec(sqlite3BtreePager(pbt), sqlite3Codec, OnCodecSizeChng, sqlite3pager_free_codecarg, pBlock);
	}
	else // 改变已加密数据库的写密钥
	{
		pBlock->WriteKey = hKey;
	}

	// 开始一个事务
	rc = sqlite3BtreeBeginTrans(pbt, 1);

	if (!rc)
	{
		// 用新密钥重写所有的页到数据库。
		
		// 这里必须接收返回值，否则会有奇怪的编译错误
		int tmp_page = 0;
		sqlite3PagerPagecount(p, &tmp_page);
		nPage = (Pgno)tmp_page;
		nSkip = PAGER_MJ_PGNO(p);
		

		for(n = 1; rc == SQLITE_OK && n <= nPage; n ++)
		{
			if (n == nSkip) continue;
			rc = sqlite3PagerGet(p, n, (DbPage**)&pPage);
			if(!rc)
			{
				rc = sqlite3PagerWrite(pPage);
				sqlite3PagerUnref(pPage);
			}
		}
	}

	// 如果成功，提交事务。
	if (!rc)
	{
		rc = sqlite3BtreeCommit(pbt);
	}

	// 如果失败，回滚。
	if (rc)
	{
		sqlite3BtreeRollback(pbt, SQLITE_OK, 0);
	}



	// 如果成功，销毁先前的读密钥。并使读密钥等于当前的写密钥。
	if (!rc)
	{
		if (pBlock->ReadKey)
		{
			sqlite3_free(pBlock->ReadKey);
		}
		pBlock->ReadKey = pBlock->WriteKey;
	}
	else// 如果失败，销毁当前的写密钥，并恢复为当前的读密钥。
	{
		if (pBlock->WriteKey)
		{
			sqlite3_free(pBlock->WriteKey);
		}
		pBlock->WriteKey = pBlock->ReadKey;
	}



	// 如果读密钥和写密钥皆为空，就不需要再对页进行编解码。
	// 销毁加密块并移除页的编解码器
	if (!pBlock->ReadKey && !pBlock->WriteKey)
	{
		sqlite3PagerSetCodec(p, NULL, NULL, NULL, NULL);
	}

	return rc;
}


int sqlite3_rekey(sqlite3 *db, const void *pKey, int nKey)
{
	return sqlite3_rekey_interop(db, pKey, nKey);
}

int sqlite3_rekey_v2(sqlite3 *db,const char *zDbName,const void *pKey, int nKey)
{
	return sqlite3_rekey(db, pKey, nKey);
}
