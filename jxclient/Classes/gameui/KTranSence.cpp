#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "KTranSence.h"
#include "Klogin_f.h"
#include "KSelPlayer.h"
#include "engine/CXmlStream.h"
#include "engine/KIniFile.h"
#include "gamecore/CoreShell.h"
#include "gameui/PopupLayer.h"
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
#include "gamecore/KNpc.h"
#include "gamecore/KNpcSet.h"
#include "gamecore/KPlayer.h"
#include "gamecore/KPlayerSet.h"
#include "gamecore/KSubWorldSet.h"
#include "gamescene/KUpdateLayer.h"
#include "gamescene/KgameWorld.h"
extern iCoreShell*	g_pCoreShell; //ȫ�ֵ���
//extern KImageStore2 m_ImageStore; //ȫ�ֵ���
//extern KPakList     g_pPakList;
//extern KPakList * g_pPakList;
#include <list>
USING_NS_CC;
KTranSence::KTranSence()
{
   ClearCachedata();
}

KTranSence::~KTranSence()
{
	ClearCachedata();
}

CCScene* KTranSence::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	KTranSence *layer = KTranSence::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool KTranSence::init()
{
	//////////////////////////////
	// 1. super init first
	if (!CCLayer::init())
	{
		return false;
	}

	/*if (g_GameWorld)
		CCMessageBox("g_GameWorld is true","KTranSence");
	else
		CCMessageBox("g_GameWorld is null","KTranSence");
	*/
	m_sprCount =0;
	m_nWidth   = 0;
	m_nHeight  = 0;
	setTouchEnabled(true);
	m_Status = LL_S_WAIT_INPUT_ACCOUNT;
	char nSprName[128]={0};
	int nFrams=0;

	/*t_sprintf(nSprName,"\\Spr\\Ui3\\�������\\����-���˺�-������ť\\��½����_6_ȷ��.spr");
	g_StrLower(nSprName);
	CCMenuItemImage *pCancelItem=NULL;
	CCMenuItemImage *pLoginItem=NULL;
	CCTexture2D *Login = _getinidata.getinidata_new(nSprName,0,&m_nWidth,&m_nHeight,&nFrams);
	if  (Login)
	{	
		char nSprFilePath[64]={0};
		DWORD nFielpahtdwid = g_FileName2Id(nSprName);
		t_sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,0); 
		CCTexture2D *pNormalTexture = CCTextureCache::sharedTextureCache()->isFileExist(nSprFilePath);
		t_sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,1);
		CCTexture2D *pSelectedTexture = CCTextureCache::sharedTextureCache()->isFileExist(nSprFilePath);
		t_sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,2);
		CCTexture2D *pDisabledTexture = CCTextureCache::sharedTextureCache()->isFileExist(nSprFilePath);
		pLoginItem = CCMenuItemImage::createWithTexture(
			pNormalTexture,
			pDisabledTexture,
			pSelectedTexture,this,menu_selector(KTranSence::mianLoginCallback));
		pLoginItem->setPosition(ccp(origin.x + visibleSize.width - pLoginItem->getContentSize().width/2-200 ,
			origin.y + pLoginItem->getContentSize().height/2+20));//һ����ť
	}
	*/
	pMainLabel = CCLabelTTF::create(UTEXT("����jx1���ζ�,�����Ŀڴ���",1).c_str(),UI_GAME_FONT_DEFAULT, 20);

	//pMainLabel->setString();//������ʾ������

	pMainLabel->setFontSize(12);
	pMainLabel->setPosition(ccp(origin.x + visibleSize.width/2,
		origin.y + visibleSize.height - pMainLabel->getContentSize().height-10));
	this->addChild(pMainLabel, 1);

	/*pBugLabel = CCLabelTTF::create("test role","Arial", 20);
	//pBugLabel->setString(UTEXT(nBugInfo,1).c_str());//������ʾ������
	pBugLabel->setFontSize(12);
	pBugLabel->setPosition(ccp(origin.x + visibleSize.width/2,
		origin.y + visibleSize.height/2));
	this->addChild(pBugLabel,4);
	*/
	CCSprite *bgSprite=CCSprite::create("transence.png");//��½����
	bgSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	float winw = visibleSize.width;   //��ȡ��Ļ���
	float winh = visibleSize.height;  //��ȡ��Ļ�߶�
	float spx  = bgSprite->getTextureRect().getMaxX();
	float spy  = bgSprite->getTextureRect().getMaxY();
	m_ScaleX   =  winw/spx;
	m_ScaleY   =  winw/spx;
	bgSprite->setScaleX(m_ScaleX); //���þ��������ű���
	bgSprite->setScaleY(m_ScaleY);
	m_sprCount++;
	this->addChild(bgSprite,0,m_sprCount);
	//-------------------------------------------
	CCAnimation*animation = CCAnimation::create();//����һ������
	DWORD nFielpahtdwid = g_FileName2Id(nSprName);
	for (int i=0;i<5;i++)
	{   
		//t_sprintf(nySprFilePath,"%u-%d",nFielpahtdwid,i);  
		//CCMessageBox(nSprFilePath,"animation");
		animation->addSpriteFrameWithFileName("transence.png");//�����������������֡
	}
	//���ö���֡��ʱ����
	animation->setDelayPerUnit(0.20f);
	//���ò���ѭ�� һֱ���� Ϊ-1
	animation->setLoops(1);
	//���ö���������ָ�����һ֡ setRestoreOriginalFrame
	animation->setRestoreOriginalFrame(TRUE);
	// ����һ����Զѭ���Ķ���
	CCAnimate *animate = CCAnimate::create(animation);
	//CCRepeatForever* forever = CCRepeatForever::create(animate);
	if (bgSprite)
		bgSprite->runAction(CCSequence::create(animate,CCCallFunc::create(this, callfunc_selector(KTranSence::startload)),NULL));


	//------------------------------------------------------------------
	m_nAllLoadResources = 3; //����Դ����
	m_nLoadedResources = 0;  //�Ѿ������˵�����
	m_fProgressIndex = 0.0;
	//m_fWaitTime = 0;
	m_pLabelLoading = CCLabelTTF::create("loading...", "Arial", 15);
	m_pLabelPercent = CCLabelTTF::create("%0", "Arial", 15);
	m_pLabelLoading->setPosition(ccp(size.width/2, size.height/4-70));
	m_pLabelPercent->setPosition(ccp(size.width/2+50, size.height/4-70));
	this->addChild(m_pLabelLoading,1);
	this->addChild(m_pLabelPercent,1);
	//loading�Ķ���Ч��
	m_pLoadBarStart = CCSprite::create("loadingStart.png");
	m_pLoadBarStart->setPosition(ccp(size.width/2, size.height/4-50));
	float sx  = m_pLoadBarStart->getTextureRect().getMaxX();
	//float sy  = m_pLoadBarStart->getTextureRect().getMaxY();
	m_pLoadBarStart->setScaleY(2.0f);
	m_pLoadBarStart->setScaleX(winw/sx);
	this->addChild(m_pLoadBarStart);

	CCSprite *m_pLoadEndSpr = CCSprite::create("loadingEnd.png");
	sx = m_pLoadEndSpr->getTextureRect().getMaxX();
	//m_pLoadEndSpr->setScaleX(winw/sx);

	m_pLoadBarEnd = CCProgressTimer::create(m_pLoadEndSpr);
	m_pLoadBarEnd->setPercentage(1.0f);
	m_pLoadBarEnd->setPosition(ccp(size.width/2, size.height/4-50));
	m_pLoadBarEnd->setType(kCCProgressTimerTypeBar);
	m_pLoadBarEnd->setBarChangeRate(ccp(1, 0));
	m_pLoadBarEnd->setMidpoint(ccp(0, 0));
	m_pLoadBarEnd->setScaleX(winw/sx);
	m_pLoadBarEnd->setScaleY(2.0f);
	this->addChild(m_pLoadBarEnd);

	//����һ���������������2���ڶ�ȡ��100%
	// 	CCProgressTo *action = CCProgressTo::create(2, 100);
	// 	m_pLoadBarEnd->runAction(CCSequence::create(action, CCCallFunc::create(this,
	// 	callfunc_selector(LoadingScene::loadSuccess)), NULL));
	//------------------------------------------------------------------
	this->scheduleUpdate();  //ң�˸��µ��ú���

	return true;
}

void KTranSence::loadingCallBack(CCObject *obj)
{//���ý������Ľ���
	++m_nLoadedResources;
	char tmp[10];
	t_sprintf(tmp,"%%%d",(int)(((float)m_nLoadedResources/m_nAllLoadResources)*100));
	m_pLabelPercent->setString(tmp);
	m_fProgressIndex = (((float)m_nLoadedResources/m_nAllLoadResources)*100);
	m_pLoadBarEnd->setPercentage(m_fProgressIndex);
}

void KTranSence::mianCancelCallback(CCObject* pSender)
{
	if (nAcclient)
	{
		nAcclient->Cleanup();
		m_bIsClientConnecting=false;
	}
	CCDirector::sharedDirector()->replaceScene(Klogin_f::scene());
}

void KTranSence::loadSuccess()
{
	//this->removeAllChildren();

	//CCScene *loginSuccessScene = KSelPlayer::scene();
	//CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.1f, loginSuccessScene));
	CCDirector::sharedDirector()->replaceScene(KgameWorld::scene());
	//CCDirector::sharedDirector()->pushScene(loginSuccessScene);
}

//ѭ�������麯�� Ĭ���Զ�����
void KTranSence::update(float delta)
{//��Դ���������ת
	if (!nisgetinfo)
		mExit(NULL);  

	if (m_nLoadedResources >= m_nAllLoadResources 
		//&& m_fWaitTime > 5.0f
		)
	{
		m_nLoadedResources = 0;
		loadSuccess();
		//return;
	}
	//
	if (m_bIsPlayerInGame)
	{//����Ѿ�����Ϸ״̬
		m_NpcIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
	}
	
	if (/*m_bIsGameSevConnecting && */g_pClient)
	{
		unsigned int nSize;
		const char* pBuffer = NULL;

		while (true)
		{
			if (!g_pClient)	//GS�����ݰ�
				break;
			//����ѭ���ӷ�������ȡ����С��
			pBuffer = (const char*)g_pClient->GetPackFromServer(nSize);

			if (!(pBuffer && nSize))
				break;
			PROTOCOL_MSG_TYPE*	pMsg = (PROTOCOL_MSG_TYPE*)pBuffer; //��ǰ��
	
			while(pMsg < (PROTOCOL_MSG_TYPE*)(pBuffer + nSize))
			{//˧ѡ���ݰ�
				PROTOCOL_MSG_TYPE Msg = *pMsg;
				// ���������Э��
				if (Msg == s2c_notifyplayerexchange)
				{//ת����ͼ��ת��
					//ProcessSwitchGameSvrMsg(pMsg);
					break;
				}
				else if (g_pCoreShell)
				{
					if  (Msg <= s2c_clientbegin ||  Msg >= s2c_end || g_pCoreShell->GetProtocolSize(Msg)==0)
						break;

					g_pCoreShell->NetMsgCallbackFunc(pMsg);          //���ܴӷ�����������Э����Ϣ

					if (g_pCoreShell->GetProtocolSize(Msg) > 0)
						pMsg = (PROTOCOL_MSG_TYPE*)(((char*)pMsg) + g_pCoreShell->GetProtocolSize(Msg));
					else //û�����Ƴ��ȵ� -1
						pMsg = (PROTOCOL_MSG_TYPE*)(((char*)pMsg) + PROTOCOL_MSG_SIZE + (*(unsigned short*)(((char*)pMsg) + PROTOCOL_MSG_SIZE)));			
				}
			}
		}
	}

}

void KTranSence::draw()
{
}
//
bool KTranSence::isFileExist(const char* pFileName)
{
	if( !pFileName ) return false;
	//strFilePathName is :/data/data/ + package name
	std::string filePath = CCFileUtils::sharedFileUtils()->getWritablePath();//getWriteablePath();
	
	filePath += pFileName;

	FILE *fp = fopen(filePath.c_str(),"r");
	if(fp)
	{
		fclose(fp);
		return true;
	}
	return false;
}

void KTranSence::copyData(const char* pFileName)
{
	std::string strPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pFileName);
	unsigned long len = 0;
	unsigned char * data = NULL;
	data = CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(),"r",&len);
	std::string destPath = CCFileUtils::sharedFileUtils()->getWritablePath();//getWriteablePath();
	destPath += pFileName;
	FILE *fp = fopen(destPath.c_str(),"w+");
	fwrite(data,sizeof(unsigned char),len,fp);
	fclose(fp);
	delete [] data;
	data = NULL;
}

void KTranSence::startload()
{
	//��ȡ��Դ
	CCTextureCache::sharedTextureCache()->addImageAsync("transence.png", this, callfuncO_selector(KTranSence::loadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync("transence.png", this, callfuncO_selector(KTranSence::loadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync("transence.png", this, callfuncO_selector(KTranSence::loadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync("transence.png", this, callfuncO_selector(KTranSence::loadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync("transence.png", this, callfuncO_selector(KTranSence::loadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync("transence.png", this, callfuncO_selector(KTranSence::loadingCallBack));
}
//������Դ�߳�
void KTranSence::Run()
{

}
/*
void KTranSence::onEnter()
{

}

void KTranSence::onExit()
{

}
//����� �� �������
void KTranSence::onEnterTransitionDidFinish()
{

}*/

bool KTranSence::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}
void KTranSence::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}

void KTranSence::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{

}
// default implements are used to call script callback if exist
void KTranSence::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{

}

void KTranSence::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{

}

void KTranSence::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);
	CCPoint location = touch->getLocation();//Ŀ�ĵ�����?��Ļ�����
}

void KTranSence::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{

}

void KTranSence::didAccelerate(CCAcceleration* pAccelerationValue)
{
	
}

void KTranSence::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{//convertTouchToNodeSpace

}

void KTranSence::ClearAccountPassword(bool bAccount, bool bPassword)
{
	if (bAccount)
		memset(m_ClientChoices.Account, 0xff, sizeof(m_ClientChoices.Account));
	if (bPassword)
		memset(&m_ClientChoices.Password, 0xff, sizeof(m_ClientChoices.Password));
}

void KTranSence::mExit(CCObject* pSender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif

}

void KTranSence::ClearCachedata()
{
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	CCAnimationCache::sharedAnimationCache()->removeAllAnimation();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
}