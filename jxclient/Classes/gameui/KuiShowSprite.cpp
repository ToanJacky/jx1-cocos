#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "Uiglobaldata.h"
#include "engine/CXmlStream.h"
#include "KuiShowSprite.h"
#include "engine/KIniFile.h"
#include "gamecore/CoreShell.h"
//#include "engine/KPalette.h"
#include "engine/KCodec.h"
#include "engine/KCodecLzo.h"
#include "engine/KFile.h"
#include "engine/KFilePath.h"
#include "engine/KList.h"
#include "engine/KMemBase.h"
#include "engine/KMemClass.h"
#include "engine/KMemStack.h"
#include "engine/KNode.h"
#include "engine/KPakFile.h"
#include "engine/KStrBase.h"
#include "engine/KPakList.h"
#include "engine/XPackFile.h"
#include "gamecore/ImgRef.h"
#include "gamecore/KSubWorld.h"
//#include "gamecore/KSkillManager.h"
//#include "gamecore/KItem.h"
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_CC;
#define MAX_LOOP_TIME	180
KuiShowSprite::KuiShowSprite()
{
	isOpen = false;
	red               = NULL;
	sFarmsSprite      = NULL;
	sFarmsTexture     = NULL;
	pPointLabel = NULL;
	__loopTime  = 0;
	_nFarms     = 0;
    _curFarms   = 0;
	_nModel     = 0;
	_nInterval  = 0;		        //֡���
	_nFlipTime  = 0;
	ZeroMemory(nSprNamePath,sizeof(nSprNamePath));
	_nXpos = 0;
	_nYpos = 0;
	_bRenderFlag=0;
	ptestLabel =NULL;
	_pHwidth = 0;
	_pHheight = 0;
}

KuiShowSprite::~KuiShowSprite()
{
	isOpen = false;
	if (red)
	    red->release();
}

//����ȷ��������
KuiShowSprite * KuiShowSprite::create(char * nFilePath,int nXpos,int nYpos,CCObject * callbackListener,SEL_CallFuncN callfun,bool nTouchesModel,int nRenderFlag)
{
	if (!nFilePath) return NULL;

	KuiShowSprite * popLayer = KuiShowSprite::create();

	popLayer->mianDialogSet(nFilePath,nXpos,nYpos,nTouchesModel,nRenderFlag);

	popLayer->addDialogData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//��ʼ��
bool KuiShowSprite::init()
{
	if ( !CCLayer::init() ){
		return false;
	}
	//����֡����
	//CCSpriteFrameCache * sfCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	//sfCache->addSpriteFramesWithFile("p_tips.plist");
	//����������Ӧ
	this->setTouchEnabled(true);
	this->scheduleUpdate();                   //���ú���
	//this->setTouchMode(kCCTouchesOneByOne); //��ֹ��㴥��
	return true;
}

void KuiShowSprite::mianDialogSet(char * nFilePath,int nXpos,int nYpos,bool nTouchesModel,int nRenderFlag)
{
	if  (nTouchesModel)
	  this->setTouchMode(kCCTouchesOneByOne); //��ֹ��㴥��
	else
      this->setTouchMode(kCCTouchesAllAtOnce);

	int m_nWidth,m_nHeight,nFrams;
	//CCTexture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	//ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprNamePath,nFilePath);
	g_StrLower(nSprNamePath);

	sFarmsTexture	=_getinidata.getinidata_one(nSprNamePath,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	//bgCur   =_getinidata.getinidata_new(nSprNamePath,0,&m_nWidth,&m_nHeight,&nFrams);
	
	if  (!sFarmsTexture) return;

	_bRenderFlag = nRenderFlag;
	_nFarms    = nFrams;
	_nInterval = nSprInfo.nInterval;
	_nFlipTime = IR_GetCurrentTime();
	_nXpos = nXpos;
	_nYpos = nYpos;
	_pHwidth = nSprInfo.pHwidth;
	_pHheight = nSprInfo.pHheight;
	winSize = CCDirector::sharedDirector()->getWinSize();
	ccColor4B color = ccc4(0,0,0,0);
	CCLayerColor * colorLayer = CCLayerColor::create(color);//��ɫ��
	colorLayer->setAnchorPoint(ccp(0,0));
	bgSize  = CCSizeMake(nSprInfo.pHwidth,nSprInfo.pHheight);;//CCSizeMake(376,326);
	colorLayer->setContentSize(bgSize);
	ParentNode_Team= colorLayer;//-1+45  +43
	colorLayer->setPosition(ccp(nXpos,nYpos));
	this->addChild(colorLayer);
	//��ñ���ͼƬ�Ĵ�С
	//CCSize contentSize = colorLayer->getContentSize();
	m_size   = bgSize;
	m_origin = colorLayer->getPosition();
}

void KuiShowSprite::addDialogData()
{
	if  (!g_pCoreShell) return;

	/*int nRoleMpsX,nRoleMpsY,nMapidx;
	nMapidx = g_GameWorld->getMapIdx();
	g_ScenePlace.GetSceneFocus(nMapidx,nRoleMpsX,nRoleMpsY);
	nRoleDisX = nRoleMpsX-maxMapRc.left*512;     //�������ԭ��ľ��� offx  ���
	nRoleDisY = nRoleMpsY-maxMapRc.top*1024;     //�������ԭ��ľ��� offy  �߶�
	*/
	sFarmsSprite=CCSprite::createWithTexture(sFarmsTexture);
	sFarmsSprite->setAnchorPoint(ccp(0,0));
	sFarmsSprite->setPosition(ccp(0,0));
	ParentNode_Team->addChild(sFarmsSprite);

	/*ptestLabel = CCLabelTTF::create("x:0/y:0",UI_GAME_FONT_DEFAULT,14);//Arial
	ptestLabel->setAnchorPoint(ccp(0,0));
	ptestLabel->setColor(ccYELLOW);
	ptestLabel->setPosition(ccp(0,0));
	ParentNode_Team->addChild(ptestLabel,1);
	*/
	if  (_nFarms>0)
	{//����ͼ

	}
}

//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void KuiShowSprite::setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;
	
	return;
}

//ִ���ϲ����Ļص��������رյ�����
void KuiShowSprite::buttonCallBackFunc(CCObject * pSender)
{
	CCNode * node = NULL;
	if (pSender)
		node= dynamic_cast<CCNode *>(pSender);
	//node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��
	if (m_callfun &&  m_callbackListener)
		(m_callbackListener->*m_callfun)(node);
	this->removeFromParentAndCleanup(true);
}

//�رյ�����
void KuiShowSprite::closePopLayer(CCObject * pSender)
{
	this->removeFromParentAndCleanup(true);
}

void KuiShowSprite::setRegisterWithTouchDispatcher(int nVer)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,nVer,true);
}
//��д����ע�ắ�������¸�����������
/*void KuiShowSprite::registerWithTouchDispatcher()
{
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,1000,true);
}*/

CCRect KuiShowSprite::getRect(CCNode* pNode,int i)
{
	CCRect  rc;
	rc.origin     = pNode->getPosition(); //��������ͼ��ƫ��
	rc.size       = pNode->getContentSize();
	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //��������
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //���ϱ�    //- rc.origin.y
	return rc;
}
//��������ccTouchBegan������true
bool KuiShowSprite::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return false;
}
//�ƶ���
void KuiShowSprite::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	__loopTime = MAX_LOOP_TIME;
}
//�ƶ�����
void KuiShowSprite::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	__loopTime = MAX_LOOP_TIME;
}

void KuiShowSprite::setShowPosition(int nPosX,int nPosY)
{
	if (ParentNode_Team)
	    ParentNode_Team->setPosition(ccp(nPosX,nPosY));
}

void KuiShowSprite::removeShowSprite()
{
	buttonCallBackFunc(NULL);
}

CCSize KuiShowSprite::getShowSize()
{
	return m_size;
}

void KuiShowSprite::update(float delta)
{
	//CCMessageBox("����ѭ��","update");
	if (g_GameWorld && isOpen && g_pCoreShell)
	{	
	}
}


CCTexture2D * KuiShowSprite::ccgetTxtData_s(char *sprPath,int nFramIndex,int nMpsX,int nMpsY,int nMpsZ,int *mScreeX,int *mScreeY,int bRenderFlag,int bSinglePlaneCoord,int *mHeadw,int *mHeadh)
{
	    if  (sprPath==NULL)
			return NULL;

	    char nSprName[128]={0};
		ZeroMemory(nSprName,sizeof(nSprName));
		t_sprintf(nSprName,sprPath);
		g_StrLower(nSprName);
		int m_nWidth,m_nHeight,nFrams;
		CCTexture2D *bgCur = NULL;
		SPRFRAMSINFO nSprInfo;
		ZeroMemory(&nSprInfo,sizeof(nSprInfo));
		//if ((bgCur = CCTextureCache::sharedTextureCache()->isFileExist(nSprFilePath))==NULL)
		bgCur = _getinidata.getinidata_one(nSprName,nFramIndex,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
		
		if (bgCur==NULL)
			return NULL;

		*mHeadw = nSprInfo.pHwidth;
		*mHeadh = nSprInfo.pHheight;

#define CENTERX		160
#define	CENTERY		192
		//ת������������
		//SubWorld[0].GetMpsByLocalPosition(nMpsX,nMpsY,0);
		
		int nX = nMpsX;  //λ��
		int nY = nMpsY;  //λ��

		if (bSinglePlaneCoord == false)//ת������Ļ���� 
			SubWorld[0].GetLocalPositionByMps(nX,nY,nMpsZ);

		/*if (bRenderFlag & RUIMAGE_RENDER_FLAG_REF_SPOT)
		{//��Ⱦģʽ
		//****to be modify****
		  int nCenterX = nSprInfo.pHCenterX; //���ĵ�ˮƽλ��
		  int nCenterY = nSprInfo.pHCenterY; //���ĵĴ�ֱλ��

		   if (nCenterX || nCenterY)
		   {
			 nX -= nCenterX;
			 nY -= nCenterY;
		   }
		   else if (nSprInfo.pHwidth > CENTERX)
		   {
			  nX -= CENTERX;
			  nY -= CENTERY;
		   }
		}*/

		if ((bRenderFlag & RUIMAGE_RENDER_FLAG_FRAME_DRAW) == 0)
		{//֡����ƫ�Ƶ�
			nX += nSprInfo.pFoffsetX;
			nY += nSprInfo.pFoffsetY;
		}
		//��ʼ�ü�
		
		*mScreeX = nX;
		*mScreeY = nY;

	return bgCur;
}

void KuiShowSprite::draw()
{
	if  (isOpen && _nFarms>0){
		int nbackFarms =_curFarms;
	   IR_NextFrame(_curFarms,_nFarms,_nInterval,_nFlipTime);
	   if (_curFarms!=nbackFarms)
	   {//�Ѿ���֡��
		   int m_nWidth,m_nHeight,nFrams,mScreeX,mScreeY;
		   //CCTexture2D *bgCur = NULL;
		   SPRFRAMSINFO nSprInfo;
		   ZeroMemory(&nSprInfo,sizeof(nSprInfo));
		   sFarmsTexture = ccgetTxtData_s(nSprNamePath,_curFarms,0,0,0,&mScreeX,&mScreeY,_bRenderFlag,true,&m_nWidth,&m_nHeight);
		   //sFarmsTexture	=_getinidata.getinidata_one(nSprNamePath,_curFarms,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
		   if (sFarmsSprite && sFarmsTexture)
		   {//���þ�������
			   CCRect CCRectSize = CCRectMake(0,0,0,0);
			   CCRectSize.size   = sFarmsTexture->getContentSize();
			   sFarmsSprite->setTexture(sFarmsTexture);
			   sFarmsSprite->setTextureRect(CCRectSize,false,CCRectSize.size);
			   sFarmsSprite->setPosition(ccp(mScreeX,bgSize.height-mScreeY-CCRectSize.size.height));
			   /*if (ptestLabel)
			   {
				   char nTemp[64];
				   t_sprintf(nTemp,"F:%d W:%d H:%d X:%d Y:%d",_nFarms,m_nWidth,m_nHeight,mScreeX,mScreeY);
				   ptestLabel->setString(nTemp);
			   }*/
		   }
	   }
	}
}