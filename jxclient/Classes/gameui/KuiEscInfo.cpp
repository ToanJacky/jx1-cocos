#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiEscInfo.h"
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
KuiEscInfo::KuiEscInfo()
{
	isOpen = false;
	__isCanMove=true;
	m_bScrolling=false;
	//memset(Skills,0,sizeof(KUiSkillData) * 50);
	colorLayer        = NULL;
	red               = NULL;
	m_pMainUiLayer    = NULL;
	__nSelIndex       = -1;
}

KuiEscInfo::~KuiEscInfo()
{
	isOpen = false;
	colorLayer   = NULL;
	//CC_SAFE_RELEASE(m_array);
	if (red)
	    red->release();

}

//����ȷ��������
KuiEscInfo * KuiEscInfo::create(char * title,CCObject * callbackListener,SEL_CallFuncN callfun)
{
	KuiEscInfo * popLayer = KuiEscInfo::create();
	popLayer->addDialogData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//��ʼ��
bool KuiEscInfo::init()
{
	if ( !CCLayer::init() ){
		return false;
	}
	//����֡����
	//CCSpriteFrameCache * sfCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	//sfCache->addSpriteFramesWithFile("p_tips.plist");
	//���ģ̬����
	winSize = CCDirector::sharedDirector()->getWinSize();
	/*ccColor4B color = ccc4(112, 128, 144, 150);
	CCLayerColor * colorLayer = CCLayerColor::create(color);//��ɫ��
	colorLayer->setPosition(CCPointZero);
	//colorLayer->setContentSize(winSize);//���úʹ��ڵĴ�С
	this->addChild(colorLayer);
	*/
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\ϵͳ\\ϵͳ2.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	CCTexture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return false;
	CCSprite * spriteBox = CCSprite::createWithTexture(bgCur);
	ParentNode_Task= spriteBox;
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
	this->setTouchMode(kCCTouchesOneByOne);   //��ֹ��㴥��
	return true;
}

void KuiEscInfo::addDialogData()
{
	if  (!g_pCoreShell) return;

	/*pMoneyLabel = CCLabelTTF::create("","fonts/gb_song.ttf", 12);//ͳ˧
	pMoneyLabel->setAnchorPoint(ccp(0,0));
	pMoneyLabel->setPosition(ccp(70,35));
	ParentNode_Task->addChild(pMoneyLabel,1);
*/
	m_pMainUiLayer = UILayer::create();
	ParentNode_Task->addChild(m_pMainUiLayer);

	// Create the checkbox
	/*char nSprName[128]={0};
	int m_nWidth,m_nHeight,nFrams;
	CCTexture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\������\\�����Ϣҳ\\����ҳ-�����Ϣ.spr");
	g_StrLower(nSprName);
    
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	UIImageView *titleImg = UIImageView::create();
	titleImg->loadTextureByData(bgCur);
	titleImg->setPosition(ccp(m_size.width/2,m_size.height/2-15));
	m_pMainUiLayer->addWidget(titleImg);

	*/
	ccColor4B color = ccc4(112, 128, 144, 150);
	colorLayer = CCLayerColor::create(color);//��ɫ��
	colorLayer->setPosition(CCPointZero);
	colorLayer->setContentSize(CCSizeZero);  //���úʹ��ڵĴ�С
	ParentNode_Task->addChild(colorLayer,1000);
	//����һ������ �ı侫����ɫ
	//red = CCTintBy::create(0.2,0,-255,-255);//����ʱ��+��ɫ 
	//red->retain();
}

//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void KuiEscInfo::setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;
	
	char nSprName[128]={0};
	int m_nWidth,m_nHeight,nFrams;
	CCTexture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\ϵͳ\\ϵͳ���˳�.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCSprite * sprite_ExitGame_normal = CCSprite::createWithTexture(bgCur);
	sprite_ExitGame_normal->setAnchorPoint(ccp(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCSprite * sprite_ExitGame_select = CCSprite::createWithTexture(bgCur);
	sprite_ExitGame_select->setAnchorPoint(ccp(0,0));

	CCMenuItemSprite * ExitGameConfirm = CCMenuItemSprite::create(sprite_ExitGame_normal, sprite_ExitGame_select, this, menu_selector(KuiEscInfo::btnCallBackFunc));
	ExitGameConfirm->setPosition(ccp(m_size.width/2,m_size.height-15));
	ExitGameConfirm->setTag(1);
	//
	t_sprintf(nSprName,"\\spr\\Ui3\\ϵͳ\\ϵͳ������.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCSprite * sprite_GameHelp_normal = CCSprite::createWithTexture(bgCur);
	sprite_GameHelp_normal->setAnchorPoint(ccp(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCSprite * sprite_GameHelp_select = CCSprite::createWithTexture(bgCur);
	sprite_GameHelp_select->setAnchorPoint(ccp(0,0));

	CCMenuItemSprite * GameHelpConfirm = CCMenuItemSprite::create(sprite_GameHelp_normal, sprite_GameHelp_select, this, menu_selector(KuiEscInfo::btnCallBackFunc));
	GameHelpConfirm->setPosition(ccp(m_size.width/2,m_size.height-55));
	GameHelpConfirm->setTag(2);

	//
	t_sprintf(nSprName,"\\spr\\Ui3\\ϵͳ\\ϵͳ��ѡ��.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCSprite * sprite_Options_normal = CCSprite::createWithTexture(bgCur);
	sprite_Options_normal->setAnchorPoint(ccp(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCSprite * sprite_Options_select = CCSprite::createWithTexture(bgCur);
	sprite_Options_select->setAnchorPoint(ccp(0,0));

	CCMenuItemSprite * OptionsConfirm = CCMenuItemSprite::create(sprite_Options_normal, sprite_Options_select, this, menu_selector(KuiEscInfo::btnCallBackFunc));
	OptionsConfirm->setPosition(ccp(m_size.width/2,m_size.height-92));
	OptionsConfirm->setTag(3);

	//\spr\Ui3\ϵͳ\ϵͳ�������й�.spr
	t_sprintf(nSprName,"\\spr\\Ui3\\ϵͳ\\ϵͳ�������й�.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCSprite * sprite_offline_normal = CCSprite::createWithTexture(bgCur);
	sprite_offline_normal->setAnchorPoint(ccp(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCSprite * sprite_offline_select = CCSprite::createWithTexture(bgCur);
	sprite_offline_select->setAnchorPoint(ccp(0,0));

	CCMenuItemSprite * OfflineConfirm = CCMenuItemSprite::create(sprite_offline_normal, sprite_offline_select, this, menu_selector(KuiEscInfo::btnCallBackFunc));
	OfflineConfirm->setPosition(ccp(m_size.width/2,m_size.height-130));
	OfflineConfirm->setTag(4);


	//������Ϸ
	t_sprintf(nSprName,"\\spr\\Ui3\\ϵͳ\\ϵͳ������.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCSprite * sprite_Leave_normal = CCSprite::createWithTexture(bgCur);
	sprite_Leave_normal->setAnchorPoint(ccp(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCSprite * sprite_Leave_select = CCSprite::createWithTexture(bgCur);
	sprite_Leave_select->setAnchorPoint(ccp(0,0));

	CCMenuItemSprite * LeaveConfirm = CCMenuItemSprite::create(sprite_Leave_normal, sprite_Leave_select, this, menu_selector(KuiEscInfo::btnCallBackFunc));
	LeaveConfirm->setPosition(ccp(m_size.width/2,m_size.height-165));
	LeaveConfirm->setTag(5);
	//����˵�
	CCMenu * menu = CCMenu::create(ExitGameConfirm,GameHelpConfirm,OptionsConfirm,OfflineConfirm,LeaveConfirm,NULL);
	menu->setAnchorPoint(ccp(0,0));
	menu->setPosition(ccp(0,0));             //�˵�λ������Ϊ����������
	ParentNode_Task->addChild(menu);         //�ӵ�������
}


//�����ť�ص�����
void KuiEscInfo::oktouchEvent(CCObject *pSender, TouchEventType type)
{
	if  (!g_pCoreShell || !g_GameWorld) return;

	switch (type)
	{
	case TOUCH_EVENT_BEGAN://Touch Down
		{
			buttonCallBackFunc(NULL);
		}
		break;
	case TOUCH_EVENT_MOVED://Touch Move
		break;
	case TOUCH_EVENT_ENDED://Touch Up
		{
			if (pSender)
			{
				UIButton* nTbtn = (UIButton*)pSender;
				//nTbtn->setTitleColor(ccWHITE);
				//if  (nTbtn->getTag()==1)
				//{

				//}
			}
		}
		break;
	case TOUCH_EVENT_CANCELED:
		break;
	default:
		break;
	}
}

void KuiEscInfo::btnCallBackFunc(CCObject * pSender)
{
	if (!g_pCoreShell|| !g_GameWorld) return;

	CCNode * node = NULL;
	if (pSender)
	{
		node= dynamic_cast<CCNode *>(pSender);
		switch(node->getTag())
		{
		case 1:
			{
			   if (g_GameWorld)
				  g_GameWorld->_quitGame();
				buttonCallBackFunc(NULL);
			}
			break;
		case 2:
			break;
		case 3:
			break;
		case 4://����
			{
			  if(g_pCoreShell)
			     g_pCoreShell->OperationRequest(GOI_LIXIAN, 0, 0);  //������������
			   buttonCallBackFunc(NULL);
		    }
			break;
		case 5://������Ϸ
			 buttonCallBackFunc(NULL);
			break;
		default:
			break;
		}
	}
}
//ִ���ϲ����Ļص��������رյ�����
void KuiEscInfo::buttonCallBackFunc(CCObject * pSender)
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
void KuiEscInfo::closePopLayer(CCObject * pSender)
{
	this->removeFromParentAndCleanup(true);
}

//��д����ע�ắ�������¸�����������
void KuiEscInfo::registerWithTouchDispatcher()
{
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,128, true);
}

CCRect KuiEscInfo::getRect(CCNode* pNode,int i)
{
	CCRect  rc;
	rc.origin     = pNode->getPosition(); //��������ͼ��ƫ��
	//visibleSize
	rc.size       = pNode->getContentSize();

	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //��������
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //���ϱ�    //- rc.origin.y
	
	return rc;
}

void KuiEscInfo::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if (!m_bScrolling) return;
	m_bScrolling = false;

	//__loopTime = MAX_LOOP_TIME;
}

//�ƶ���
void KuiEscInfo::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if (!m_bScrolling || !__isCanMove) return;
	CCPoint point   = ParentNode_Task->convertToNodeSpace(CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView()));
	CCPoint diff    = ccpSub(point,m_lastPoint);                  //����ͼƬ��λ��
	ParentNode_Task->setPosition(ccpAdd(ParentNode_Task->getPosition(),diff));
	m_origin      = ParentNode_Task->getPosition();
	m_lastPoint = point;
}
//��������ccTouchBegan������true
bool KuiEscInfo::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint point = ParentNode_Task->convertToNodeSpace(CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView()));
	CCRect rect = CCRectMake(0, 0, winSize.width/*ParentNode_Task->getContentSize().width*/,winSize.height/* ParentNode_Task->getContentSize().height*/);
	m_bScrolling = rect.containsPoint(point); //����ڷ�Χ�� �Ϳ����϶�
	m_lastPoint = point;
	return true;
}

//--------------------------------------------------------------------------
//	���ܣ����¶�����Ϣ
//--------------------------------------------------------------------------
void KuiEscInfo::update(float delta)
{
	//CCMessageBox("����ѭ��","update");
	if (isOpen && g_pCoreShell)
	{
	}
}

void KuiEscInfo::draw()
{
	//CCMessageBox("����ѭ��","draw");
}