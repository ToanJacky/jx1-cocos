//�а�ť�ġ������롡�Ի���
//
#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiProgressbar.h"
#include "engine/Text.h"
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
#include "gamecore/KSkillManager.h"
#include "gamecore/KItem.h"
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_CC;

KuiProgressbar::KuiProgressbar()
{
	isOpen = false;
	//memset(Skills,0,sizeof(KUiSkillData) * 50);
	//pMoneyLabel       = NULL;
	//pXuLabel          = NULL;
	colorLayer        = NULL;
	red               = NULL;

	//m_nMoney = 0;
	//m_nXu = 0;
	//m_AnswerCount = 0;
	m_StartPos.x=0;
	m_StartPos.y=0;
	ZeroMemory(&proData,sizeof(nJinDuClient));
	proSpr = NULL;
	starttime = 0;
}

KuiProgressbar::~KuiProgressbar()
{
	isOpen = false;
	//pMoneyLabel  = NULL;
	colorLayer        = NULL;
	//if (red)
	  //  red->release();
	//red               = NULL;
}

//����ȷ��������
KuiProgressbar * KuiProgressbar::create(KUiJinDuTiaoInfo* pJinDu,CCObject * callbackListener,SEL_CallFuncN callfun)
{
	if (!pJinDu)
		return NULL;

	KuiProgressbar * popLayer = KuiProgressbar::create();
	popLayer->proData.nMsgidx = pJinDu->nMsgIndex;

	popLayer->proData.nTimeVal= pJinDu->nTimes;//��ʱ��
	if (pJinDu->nTimes<=0)
       popLayer->proData.nTimeVal =10;

	t_sprintf(popLayer->ntitle,pJinDu->nTitle);
	t_sprintf(popLayer->proData.szCallFun,pJinDu->nCallFuc);
	t_sprintf(popLayer->proData.szParma,pJinDu->szParma);
	t_sprintf(popLayer->proData.szParmb,pJinDu->szParmb);
	t_sprintf(popLayer->proData.szParmc,pJinDu->szParmc);
	popLayer->addDialogData();
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}


//��ʼ��
bool KuiProgressbar::init()
{
	if ( !CCLayer::init() ){
		return false;
	}

	//����֡����
	//CCSpriteFrameCache * sfCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	//sfCache->addSpriteFramesWithFile("p_tips.plist");

	//���ģ̬����
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	/*ccColor4B color = ccc4(112, 128, 144, 150);
	CCLayerColor * colorLayer = CCLayerColor::create(color);//��ɫ��
	colorLayer->setPosition(CCPointZero);
	//colorLayer->setContentSize(winSize);//���úʹ��ڵĴ�С
	this->addChild(colorLayer);
	*/
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\progressbar\\progressbar_back.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	CCTexture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return false;
	CCSprite * spriteBox = CCSprite::createWithTexture(bgCur);
	ParentNode_ItemEx = spriteBox;
	spriteBox->setPosition(ccp(winSize.width/2,winSize.height/2));
	this->addChild(spriteBox);
	
	//��ñ���ͼƬ�Ĵ�С
	CCSize contentSize = spriteBox->getContentSize();
	m_size = contentSize;
	m_origin = spriteBox->getPosition();
	//colorLayer->setContentSize(contentSize);//���úʹ��ڵĴ�С
	//����������Ӧ
	this->setTouchEnabled(true);
	this->scheduleUpdate();                   //���ú���
	//this->setTouchMode(kCCTouchesOneByOne); //��ֹ��㴥��
	return true;
}


void KuiProgressbar::addDialogData()
{
	if  (!g_pCoreShell) return;
	CCSize nSize;
	nSize.width =128;
	nSize.height=24;
	m_StartPos.x =10;//+ITEM_CELL_SIZE/2;
	m_StartPos.y =140-24;//+ITEM_CELL_SIZE/2;
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\progressbar\\progressbar_bit.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	CCTexture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	
	if (bgCur==NULL)
		return;

	proSpr = CCSprite::createWithTexture(bgCur);
	proSpr->setAnchorPoint(ccp(0,0));
	proSpr->setPosition(ccp(10,7));
	proSpr->setScaleX(0); //��������Ϊ��;
	ParentNode_ItemEx->addChild(proSpr);

	CCLabelTTF *ptextLabel = CCLabelTTF::create(UTEXT(ntitle,1).c_str(),UI_GAME_FONT_DEFAULT,14,nSize,kCCTextAlignmentCenter);//Arial
	ptextLabel->setColor(ccYELLOW);
	ptextLabel->setPosition(ccp(m_size.width/2,m_size.height-15));
	ParentNode_ItemEx->addChild(ptextLabel);

	/*cc_timeval m_pStartUpdate;
	CCTime::gettimeofdayCocos2d(&m_pStartUpdate,NULL);
	starttime = m_pStartUpdate.tv_sec*1000+m_pStartUpdate.tv_usec/1000;//����timeGetTime();���ϵͳ���е�ʱ��
	*/
	KUiPlayerGameTime	pInfo;
	memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
	ZeroMemory(&pInfo,sizeof(pInfo));
	if (g_pCoreShell)
		g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (int)&pInfo, 0);
	
	starttime = pInfo.npTime;  //��¼�򿪴��ڵ�����
	/*ccColor4B color = ccc4(112, 128, 144, 150);
	colorLayer = CCLayerColor::create(color);//��ɫ��
	colorLayer->setPosition(CCPointZero);
	colorLayer->setContentSize(CCSizeZero);//���úʹ��ڵĴ�С
	ParentNode_ItemEx->addChild(colorLayer,1000);
	*/
	//����һ������ �ı侫����ɫ
	//red = CCTintBy::create(0.2,0,-255,-255);//����ʱ��+��ɫ 
	//red->retain();
}


//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void KuiProgressbar::setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;
}
//ִ���ϲ����Ļص��������رյ�����
void KuiProgressbar::buttonCallBackFunc(CCObject * pSender)
{
	CCNode * node = NULL;
	if (pSender)
		node = dynamic_cast<CCNode *>(pSender);
	//node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��
	if (m_callfun &&  m_callbackListener)
		(m_callbackListener->*m_callfun)(node);

	this->removeFromParentAndCleanup(true);
}

//�رյ�����
void KuiProgressbar::closePopLayer(CCObject * pSender)
{
	this->removeFromParentAndCleanup(true);
}

//��д����ע�ắ�������¸�����������
void KuiProgressbar::registerWithTouchDispatcher()
{
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-128, true);
}

CCRect KuiProgressbar::getRect(CCNode* pNode,int i)
{
	CCRect  rc;
	rc.origin     = pNode->getPosition(); //��������ͼ��ƫ��
	//visibleSize
	rc.size       = pNode->getContentSize();

	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //��������
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //���ϱ�    //- rc.origin.y
	
	return rc;
}
//��������ccTouchBegan������true
bool KuiProgressbar::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	//CCSetIterator it = pTouch->begin();
	//CCTouch* touch = (CCTouch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	CCPoint location = pTouch->getLocation();//Ŀ�ĵ�����?��Ļ�����
	//case SMCT_UI_RENASCENCE:	//ѡ������	
	//	if (g_pCoreShell)
	//	   g_pCoreShell->OperationRequest(GOI_PLAYER_RENASCENCE,0,true);//nSelAction=0��ԭ������, nSelAction=1���س�

    buttonCallBackFunc(NULL);
	return true;
}

void KuiProgressbar::update(float delta)
{
	//CCMessageBox("����ѭ��","update");
	if (isOpen && g_pCoreShell && proSpr)
	{
		KUiPlayerGameTime	pInfo;
		memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
		ZeroMemory(&pInfo,sizeof(pInfo));
		if (g_pCoreShell)
			g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (int)&pInfo, 0);
	   float ncTime=0;
	      ncTime=pInfo.npTime-starttime;	//ʱ���

       if (ncTime>0 && ncTime<=proData.nTimeVal)
	   {
		 float sX = ncTime/proData.nTimeVal;
		 proSpr->setScaleX(sX);
	     //m_Progress.Set2IntValue(ncTime,proData.nTimeVal);//���ý���
		 if (ncTime==proData.nTimeVal)
		 {//��ʼ���ͻص�����ִ�нű�
			 if (atoi(proData.szParmc)==1) 
			 {//�Ƿ�ѭ��ִ��--����
				 starttime=pInfo.npTime;        //��¼ִ�е�ʱ��
				 g_pCoreShell->OperationRequest(GOI_JINDUTIAO_CALLBACK,(unsigned int)&proData,0);
				 proSpr->setScaleX(0);
				 //m_pSelf->m_Progress.Set2IntValue(0,proData.nTimeVal); //���ý���
			 }
             else
			 {//һ���� �ɼ�
			     starttime=pInfo.npTime; 
				 g_pCoreShell->OperationRequest(GOI_JINDUTIAO_CALLBACK,(unsigned int)&proData,0);
			     buttonCallBackFunc(NULL);
			 } 
		 }
	   }
	}
}

void KuiProgressbar::draw()
{
	//CCMessageBox("����ѭ��","draw");
}


int KuiProgressbar::FilterTextColor(char* pMsgBuff, unsigned short nMsgLength)
{
	nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_ENTER);          //��ֹ�س�����
	nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_COLOR);          //��ֹ��ɫ����
	nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_COLOR_RESTORE);  //��ֹ��ɫ��ԭ
	nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_BORDER_COLOR);   //��ֹ�������
	nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_BORDER_RESTORE); //��ֹ�������
	nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_INLINE_PIC);     //��ֹ��ͼ����
	nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_LINK);           //��ֹ��������
	//	return TEncodeText(pMsgBuff, nMsgLength);
	return  nMsgLength;
}