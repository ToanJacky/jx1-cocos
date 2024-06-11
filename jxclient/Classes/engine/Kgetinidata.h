//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KCodec.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef Kgetinidata_H
#define Kgetinidata_H
#include "cocos2d.h"
#include "KbugInfo.h"
#include "ImageStore/KImageStore2.h"
//#include "KPalette.h"
USING_NS_CC;
//---------------------------------------------------------------------------
//#define CODEC_NONE	0
//#define CODEC_LHA		1
//#define CODEC_LZO		2
//---------------------------------------------------------------------------
typedef struct {
	unsigned char *		lpData;			// ��ѹ��&ѹ��ǰ ����ָ��
	unsigned long		dwDataLen;		// ��ѹ��&ѹ��ǰ ���ݳ���
	unsigned char *	    lpPack;			// ѹ����&ѹ���� ����ָ��
	unsigned long		dwPackLen;		// ѹ����&ѹ���� ���ݳ���
}TSprCodeInfo;

struct Palette_Colour
{
	unsigned char Blue;
	unsigned char Green;
	unsigned char Red;
	unsigned char Alpha;
};

typedef struct {
	long		x;			// �ü����X����
	long		y;			// �ü����Y����
	long		width;		// �ü���Ŀ��
	long		height;		// �ü���ĸ߶�
	long		left;		// ��߽�ü�����
	long		top;		// �ϱ߽�ü���
	long		right;		// �ұ߽�ü���
} KClipper;
//---------------------------------------------------------------------------
class  Kgetinidata
{
public:
	Kgetinidata(void);
	virtual ~Kgetinidata(){};
	 unsigned long	GetLen(unsigned long dwDataLen);
	// bool	SprEncode(TSprCodeInfo* pCodeInfo);
	// bool	SprDecode(TSprCodeInfo* pCodeInfo);
	 CCTexture2D * getinidata_new(char *nFileName,int nFrameIndex,int *mCurwidth, int *mCurheight,int *mFrams);
	 CCTexture2D * getinidata_one(char *nFileName,int nFrameIndex,int *mCurwidth, int *mCurheight,int *mFrams,SPRFRAMSINFO *nSprInfo=NULL);
	 CCTexture2D * getpicPath(char *picPath);
	 //lpData;dwDataLen;lpPack;������ȷ���ã�dwPackLen��Ϊ0����
	 //bool   SaveBMPFile(const char *filename,  const stImageInfo& m_info);
private:
	
	SPRHEAD     * pHeader;
	BYTE*	 	m_Palette;
	KPAL16*		m_pPal16;
	SPROFFS* 	m_pOffset;
	SPRFRAME*   nFrameData;
	PBYTE		m_pSprite;
	int			m_nWidth;
	int			m_nHeight;
	int	        m_nCenterX;
	int	        m_nCenterY;
	signed  int	m_nOffsetX;
	signed	int	m_nOffsetY;
	unsigned long	m_nFrames;
	int			m_nColors;
	unsigned long	m_nDirections;
	int			m_nInterval;
	unsigned char *data;
	unsigned long m_size;
	int          m_ReadModel;
	long  GetGreaterNear2Fold(long d);
	PBYTE RevertRowRGBA(PBYTE data, long &width, long &height);
	bool ReadFileData(char *nFileName,int nFrameIndex=0);
};
//---------------------------------------------------------------------------
// void	g_InitCodec(KCodec** ppCodec, int nCompressMethod);
// void	g_FreeCodec(KCodec** ppCodec, int nCompressMethod);
//---------------------------------------------------------------------------
#endif
