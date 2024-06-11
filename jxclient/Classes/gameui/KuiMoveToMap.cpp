#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "Uiglobaldata.h"
#include "engine/CXmlStream.h"
#include "KuiMoveToMap.h"
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
#include "gamescene/KScenePlaceC.h"
#include "gamecore/KSubWorld.h"
//#include "gamecore/KSkillManager.h"
//#include "gamecore/KItem.h"
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_CC;
#define	WORLD_MAP_POS_FILE		"\\ui\\С��ͼ_ɽ����ͼ��_͸����ť.ini"
KuiMoveToMap::KuiMoveToMap()
{
	isOpen = false;
	red               = NULL;
	clipper           = NULL;
	content_Map       = NULL;
	m_PosCount        = 0;
	nMapPicSpr = NULL;
	ZeroMemory(m_MoveMapBtn,sizeof(mapListData)*MAX_COUNT_MAPS);
}

KuiMoveToMap::~KuiMoveToMap()
{
	isOpen = false;
	if (red)
	    red->release();
}

//����ȷ��������
KuiMoveToMap * KuiMoveToMap::create(char * title,CCObject * callbackListener,SEL_CallFuncN callfun)
{
	KuiMoveToMap * popLayer = KuiMoveToMap::create();
	popLayer->addDialogData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//��ʼ��
bool KuiMoveToMap::init()
{
	if ( !CCLayer::init() ){
		return false;
	}
	//����֡����
	//CCSpriteFrameCache * sfCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	//sfCache->addSpriteFramesWithFile("p_tips.plist");
	//���ģ̬����
	winSize = CCDirector::sharedDirector()->getWinSize();
	ccColor4B color = ccc4(255,255,255,0);
	CCLayerColor * colorLayer = CCLayerColor::create(color);//��ɫ��
	bgSize  = winSize;//CCSizeMake(376,326);
	colorLayer->setContentSize(winSize); //������Ļ
	ParentNode_Team= colorLayer;         //-1+45  +43
	colorLayer->setPosition(ccp(winSize.width/2-bgSize.width/2,winSize.height/2-bgSize.height/2));
	this->addChild(colorLayer);
	//��ñ���ͼƬ�Ĵ�С
	CCSize contentSize = colorLayer->getContentSize();
	m_size = contentSize;
	m_origin = colorLayer->getPosition();

	//colorLayer->setContentSize(contentSize);//���úʹ��ڵĴ�С
	//����������Ӧ
	this->setTouchEnabled(true);
	this->scheduleUpdate();                   //���ú���
	this->setTouchMode(kCCTouchesOneByOne); //��ֹ��㴥��
	return true;
}

void KuiMoveToMap::addDialogData()
{
	if  (!g_pCoreShell) return;

	/*int nRoleMpsX,nRoleMpsY,nMapidx;
	nMapidx = g_GameWorld->getMapIdx();
	g_ScenePlace.GetSceneFocus(nMapidx,nRoleMpsX,nRoleMpsY);
	nRoleDisX = nRoleMpsX-maxMapRc.left*512;     //�������ԭ��ľ��� offx  ���
	nRoleDisY = nRoleMpsY-maxMapRc.top*1024;     //�������ԭ��ľ��� offy  �߶�
	/*char msg[64];
	int  NewscrX= nRoleMpsX,NewscrY= nRoleMpsY;
	SubWorld[0].GetLocalPositionByMps(NewscrX,NewscrY,0);      //�������꣭��������Ļ����
	sprintf(msg,"x:%d,y:%d",NewscrX,NewscrY);
	CCMessageBox(msg,"DSTEN");*/
	//\Spr\Ui3\С��ͼ\�����ɽ��ͼ.spr


	char nSprName[128]={0};
	int m_nWidth,m_nHeight,nFrams;
	CCTexture2D* nPicTexture=NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));

	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\С��ͼ\\�����ɽ��ͼ.spr");
	g_StrLower(nSprName);

	nPicTexture = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (nPicTexture==NULL)
		return ;

	nTextureSize = nPicTexture->getContentSize();
	ccColor4B color = ccc4(0,0,0,150);
	CCLayerColor * bgLayer = CCLayerColor::create(color);//��ɫ��
	bgLayer->setAnchorPoint(ccp(0.5,0.5));
	bgLayer->setContentSize(CCSizeMake(376,326));  //CCSizeMake(376,326)
	bgLayer->setPosition(ccp(m_size.width/2-bgLayer->getContentSize().width/2+45,m_size.height/2-bgLayer->getContentSize().height/2+43));
	ParentNode_Team->addChild(bgLayer);

	clipper = CCClippingNode::create();//����һ������ʾ����
	clipper->setTag(1);
	clipper->setContentSize(CCSizeMake(370,320)); //CCSizeMake(370,320)
	clipper->setAnchorPoint(ccp(0.5,0.5));
	//clipper->setPosition(ccp(m_size.width/2-clipper->getContentSize().width/2+45,m_size.height/2-clipper->getContentSize().height/2+43));
	clipper->setPosition(ccp(m_size.width/2+45,m_size.height/2+43));//GL POIT
	//clipper->runAction(CCRepeatForever::create(CCRotateBy::create(1, 45)));
	ParentNode_Team->addChild(clipper);

	CCSize nSize;
	nSize.width =376;
	nSize.height=16;
	CCLabelTTF *pdescInfo =  CCLabelTTF::create(UTEXT("����հ�(������)���϶�,�����Ӧ��ͼ�ƶ�",1).c_str(),UI_GAME_FONT_DEFAULT,14,nSize,kCCTextAlignmentCenter);//Arial
	//pdescInfo->setColor(ccYELLOW);
	pdescInfo->setPosition(ccp(m_size.width/2+40,m_size.height/2-clipper->getContentSize().height/2+35));
	ParentNode_Team->addChild(pdescInfo,10);

	content_Map = CCSprite::createWithTexture(nPicTexture);
	content_Map->setTag(2);
	content_Map->setAnchorPoint(ccp(0,0));
	//content->setPosition(ccp(clipper->getContentSize().width/2,clipper->getContentSize().height/2));
	//content_Map->setPosition(ccp(-nRoleDisX/16+clipper->getContentSize().width/2,-(nPicTexture->getContentSize().height-nRoleDisY/32)+clipper->getContentSize().height/2));
	//nRoleDisX/16��nRoleDisY/32��ͼ�ڵ����ƫ�ƾ������ǵ�λ��(�����ͼƬ�Ĵ�С��˵)
	content_Map->setPosition(ccp(0,0));//������ڵ������
	clipper->addChild(content_Map);//���ñ��ü��ľ���

	m_pUiLayer = UILayer::create();
	//ParentNode_Team->addChild(m_pUiLayer);
	content_Map->addChild(m_pUiLayer);

	KIniFile	Ini;
	if (Ini.Load(WORLD_MAP_POS_FILE))
	{
		Ini.GetInteger("BUTTON_COUNT","Count",0,&m_PosCount);
		//Width=754
		//Height=578
		if (m_PosCount>0 && m_PosCount<MAX_COUNT_MAPS)
		{
			char nBtn[32]={0};
			int nLeft,nTop,nWidth,nHeight,nSubwordId,nPosX,nPosY;
			for (int i=0;i<m_PosCount;++i)
			{   
				sprintf(nBtn,"BUTTON_%d",i);
				Ini.GetInteger(nBtn,"Left",0,&nLeft);
				Ini.GetInteger(nBtn,"Top",0,&nTop);
				Ini.GetInteger(nBtn,"Width",0,&nWidth);
				Ini.GetInteger(nBtn,"Height",0,&nHeight);
				Ini.GetInteger(nBtn,"SubwordId",0,&nSubwordId);
				Ini.GetInteger(nBtn,"TargetPosX",0,&nPosX);
				Ini.GetInteger(nBtn,"TargetPosY",0,&nPosY);
				m_MoveMapBtn[i].m_SubwordId = nSubwordId;
				m_MoveMapBtn[i].m_PosX      = nPosX;
				m_MoveMapBtn[i].m_PosY      = nPosY;
				m_MoveMapBtn[i].m_MoveMapBtn = UIButton::create();
				m_MoveMapBtn[i].m_MoveMapBtn->setTouchEnabled(true);
				m_MoveMapBtn[i].m_MoveMapBtn->loadTextures("btn_n.png", "loadingStart.png","");
				m_MoveMapBtn[i].m_MoveMapBtn->setScale9Enabled(true);
				m_MoveMapBtn[i].m_MoveMapBtn->setSize(CCSizeMake(nWidth,nHeight));
				//m_MoveMapBtn[i].m_MoveMapBtn->setContentSize(CCSizeMake(nWidth,nHeight));
				//m_MoveMapBtn[i].m_MoveMapBtn->ignoreContentAdaptWithSize(true);
				m_MoveMapBtn[i].m_MoveMapBtn->setPosition(ccp(nLeft+20,nTextureSize.height-nTop-8));
				m_MoveMapBtn[i].m_MoveMapBtn->setTag(i+1);
				m_MoveMapBtn[i].m_MoveMapBtn->setTagbyKey(nBtn);
				m_MoveMapBtn[i].m_MoveMapBtn->addTouchEventListener(this,toucheventselector(KuiMoveToMap::oktouchEvent));
				/*char nTemp[64],mapName[128];
				ZeroMemory(nTemp,sizeof(nTemp));
				ZeroMemory(mapName,sizeof(mapName));
				t_sprintf(nTemp,"%d_name",nSubwordId);
				g_NpcMapDropRate.GetString("List",nTemp,0,mapName,sizeof(mapName));
				m_MoveMapBtn[i].m_MoveMapBtn->setTitleText(UTEXT(mapName,1).c_str());
				*/
				m_pUiLayer->addWidget(m_MoveMapBtn[i].m_MoveMapBtn);
			} 
		}

	}
	Ini.Clear(); 

	CCDrawNode *stencil = CCDrawNode::create();
	CCPoint rectangle[4];
	rectangle[0] = ccp(0,0);
	rectangle[1] = ccp(clipper->getContentSize().width, 0);
	rectangle[2] = ccp(clipper->getContentSize().width, clipper->getContentSize().height);
	rectangle[3] = ccp(0,clipper->getContentSize().height);

	ccColor4F white = {1, 1, 1, 1};
	stencil->drawPolygon(rectangle,4,white,1,white);
	clipper->setStencil(stencil); //����ģ��

	m_bScrolling = false;
}

//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void KuiMoveToMap::setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;
	
	return;
}

//ִ���ϲ����Ļص��������رյ�����
void KuiMoveToMap::buttonCallBackFunc(CCObject * pSender)
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
void KuiMoveToMap::closePopLayer(CCObject * pSender)
{
	this->removeFromParentAndCleanup(true);
}
//�����ť�ص�����
void KuiMoveToMap::oktouchEvent(CCObject *pSender, TouchEventType type)
{
	if  (!g_pCoreShell || !g_GameWorld) return;

	switch (type)
	{
	case TOUCH_EVENT_BEGAN://Touch Down
		{
			if (pSender)
			{
				UIButton* nTbtn = (UIButton*)pSender;
				if (nTbtn)
				{
					nTbtn->setTitleColor(ccYELLOW);
					int nSelIndex = nTbtn->getTag()-1;
					g_pCoreShell->OperationRequest(GOI_MAP_SUNYI,m_MoveMapBtn[nSelIndex].m_SubwordId,1); 
				}
				buttonCallBackFunc(NULL);
				break;
/*				
				CCNode *clipper = ParentNode_Team->getChildByTag(1);
				CCNode *content = clipper->getChildByTag(2);
				CCPoint diff = ccpSub(content->getPosition(),ccp(0,0));//ƫ����ԭ���ĵط��ľ��������ֵ
				CCPoint curPos = ccpAdd(nTbtn->getPosition(),diff);
				curPos = ccpAdd(curPos,ccp(clipper->getPosition().x-clipper->getContentSize().width/2,clipper->getPosition().y-clipper->getContentSize().height/2));
				//CCPoint point = clipper->convertToNodeSpace(curPos);
				//CCRect rect = CCRectMake(0, 0, clipper->getContentSize().width, clipper->getContentSize().height);
				//m_bScrolling = rect.containsPoint(point);
				char nTempPoit[128];
				sprintf(nTempPoit,"%s:%f/%f",nTbtn->getTitleText(),curPos.x,curPos.y);
				CCMessageBox(nTempPoit,"test");
*/
			}
		}
		break;
	case TOUCH_EVENT_MOVED://Touch Move
		break;
	case TOUCH_EVENT_ENDED://Touch Up
		{
			if (pSender)
			{
				UIButton* nTbtn = (UIButton*)pSender;
				nTbtn->setTitleColor(ccWHITE);
				if  (nTbtn->getTag()==1)
				{

				}
			}
		}
		break;
	case TOUCH_EVENT_CANCELED:
		break;

	default:
		break;
	}
}
//��д����ע�ắ�������¸�����������
void KuiMoveToMap::registerWithTouchDispatcher()
{
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,2,true);
}

CCRect KuiMoveToMap::getRect(CCNode* pNode,int i)
{
	CCRect  rc;
	rc.origin     = pNode->getPosition(); //��������ͼ��ƫ��
	rc.size       = pNode->getContentSize();
	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //��������
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //���ϱ�    //- rc.origin.y
	return rc;
}
//��������ccTouchBegan������true
bool KuiMoveToMap::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	//CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCNode *clipper = ParentNode_Team->getChildByTag(1);
	CCPoint point = clipper->convertToNodeSpace(CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView()));
	CCRect rect = CCRectMake(0, 0, clipper->getContentSize().width, clipper->getContentSize().height);
	m_bScrolling = rect.containsPoint(point); //����ڷ�Χ�� �Ϳ����϶�
	m_lastPoint = point;
	/*if (!m_bScrolling)
	    button->setTouchEnabled(false);
	else
		button->setTouchEnabled(true);*/
	return true;
}
//�ƶ���
void KuiMoveToMap::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if (!m_bScrolling) return;
	//CCTouch *touch = (CCTouch*)pTouches->anyObject();
	//if  (nPartNode)
	//	nPartNode->setVisible(false);
	CCNode *clipper = ParentNode_Team->getChildByTag(1);
	CCPoint point = clipper->convertToNodeSpace(CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView()));
	CCPoint diff = ccpSub(point,m_lastPoint);
	CCNode *content = clipper->getChildByTag(2);//����ͼƬ��λ��
	content->setPosition(ccpAdd(content->getPosition(),diff));	
	m_lastPoint = point;
}
//�ƶ�����
void KuiMoveToMap::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	//if  (nPartNode)
	//	nPartNode->setVisible(true);

	if (!m_bScrolling) return;
	   m_bScrolling = false;

}


void KuiMoveToMap::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCNode *clipper = ParentNode_Team->getChildByTag(1);
	CCPoint point = clipper->convertToNodeSpace(CCDirector::sharedDirector()->convertToGL(touch->getLocationInView()));
	CCRect rect = CCRectMake(0, 0, clipper->getContentSize().width, clipper->getContentSize().height);
	m_bScrolling = rect.containsPoint(point);
	m_lastPoint = point;
}

void KuiMoveToMap::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	if (!m_bScrolling) return;
	CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCNode *clipper = ParentNode_Team->getChildByTag(1);
	CCPoint point = clipper->convertToNodeSpace(CCDirector::sharedDirector()->convertToGL(touch->getLocationInView()));
	CCPoint diff = ccpSub(point, m_lastPoint);
	CCNode *content = clipper->getChildByTag(2);
	content->setPosition(ccpAdd(content->getPosition(),diff));
	m_lastPoint = point;
}

void KuiMoveToMap::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	if (!m_bScrolling) return;
	m_bScrolling = false;
}
void KuiMoveToMap::update(float delta)
{
	//CCMessageBox("����ѭ��","update");
	if (g_GameWorld && isOpen && g_pCoreShell)
	{
		if  (clipper)
		{
			CCNode *content = clipper->getChildByTag(2);//����ͼƬ��λ��)
			if (!content) return;
			//CCPoint point = clipper->convertToNodeSpace(CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView()));
			for (int i=0;i<m_PosCount;++i)
			{
				if (m_MoveMapBtn[i].m_SubwordId>0 && m_MoveMapBtn[i].m_MoveMapBtn)
				{

					CCPoint diff = ccpSub(content->getPosition(),ccp(0,0));//ƫ����ԭ���ĵط��ľ��������ֵ
					CCPoint curPos = ccpAdd(m_MoveMapBtn[i].m_MoveMapBtn->getPosition(),diff);
					curPos = ccpAdd(curPos,ccp(clipper->getPosition().x-clipper->getContentSize().width/2,clipper->getPosition().y-clipper->getContentSize().height/2));

					CCPoint newPoint = clipper->convertToNodeSpace(curPos);
					CCRect rect = CCRectMake(0, 0, clipper->getContentSize().width, clipper->getContentSize().height);
					bool m_IsIn = rect.containsPoint(newPoint);

					//CCRect rect = CCRectMake(0, 0, clipper->getContentSize().width, clipper->getContentSize().height);
					//bool m_IsIn = rect.containsPoint(clipper->convertToNodeSpace(ccpAdd(content->getPosition(),diff)));
					if  (m_IsIn)//�ڷ�Χ��
						m_MoveMapBtn[i].m_MoveMapBtn->setTouchEnabled(true);
					else
						m_MoveMapBtn[i].m_MoveMapBtn->setTouchEnabled(false);
				}
			}
			
		}
		return;
	}
}

void KuiMoveToMap::draw()
{
	//CCMessageBox("����ѭ��","draw");
}