//�а�ť�ġ������롡�Ի���
//
#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiSortSkill_l.h"
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
#include "gameui/KuiSkilldesc.h"
//#include "gamecore/KItem.h"
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_CC;

KuiSortSkill_l::KuiSortSkill_l()
{
	isOpen = false;
	colorLayer    = NULL;
	red           = NULL;
	_sortCount_r  = NULL;
	ParentNode_ItemEx = NULL;
	memset(m_Skills,0,sizeof(KUiSkillData) * SKILLTREE_MAX_SKILL_COUNT);
}

KuiSortSkill_l::~KuiSortSkill_l()
{
	isOpen = false;
	colorLayer = NULL;
}

//����ȷ��������
KuiSortSkill_l * KuiSortSkill_l::create(int nKind,CCObject * callbackListener,SEL_CallFuncN callfun)
{
	KuiSortSkill_l * popLayer = KuiSortSkill_l::create();
	popLayer->addDialogData(nKind);
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}


//��ʼ��
bool KuiSortSkill_l::init()
{
	if ( !CCLayer::init() ){
		return false;
	}

	//����֡����
	//CCSpriteFrameCache * sfCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	//sfCache->addSpriteFramesWithFile("p_tips.plist");

	//���ģ̬����
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	ccColor4B color = ccc4(112,128,144,0);
	CCLayerColor * colorLayer = CCLayerColor::create(color);//��ɫ��
	colorLayer->setPosition(visibleSize.width/2+105,100);
	this->addChild(colorLayer);
	
	ParentNode_ItemEx = colorLayer;
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


void KuiSortSkill_l::addDialogData(int nKind)
{
	if  (!g_pCoreShell || !g_GameWorld) return;
	/*ccColor4B color = ccc4(112, 128, 144, 150);
	colorLayer = CCLayerColor::create(color);//��ɫ��
	colorLayer->setPosition(CCPointZero);
	colorLayer->setContentSize(CCSizeZero);//���úʹ��ڵĴ�С
	ParentNode_ItemEx->addChild(colorLayer,1000);
	*/
	//����һ������ �ı侫����ɫ 
	//red = CCTintBy::create(0.2,0,-255,-255);//����ʱ��+��ɫ 
	//red->retain();
	CCMenu* sortlistMenu = CCMenu::create(NULL);
	sortlistMenu->setAnchorPoint(ccp(0,0));
	sortlistMenu->setPosition(ccp(0,0));
	ParentNode_ItemEx->addChild(sortlistMenu,1);
	//if (g_pCoreShell)
	//	g_pCoreShell->GetGameData(GDI_CHESSMENT,0,0);

	memset(m_Skills,0,sizeof(KUiSkillData) * SKILLTREE_MAX_SKILL_COUNT);
	if (g_pCoreShell)
	   _sortCount_r	=g_pCoreShell->GetGameData(GDI_LEFT_ENABLE_SKILLS, (unsigned int)m_Skills, 0); //��ȡ��ҳ��������
	float offsetX,offsetY;
	if (_sortCount_r)
	{//��ʼ��� ��ť
		char nSprName[128]={0};
		int m_nWidth,m_nHeight,nFrams;
		CCTexture2D *bgCur = NULL;
		SPRFRAMSINFO nSprInfo;
		ZeroMemory(&nSprInfo,sizeof(nSprInfo));
		float offsetSizeH=0,offsetSizeW=0;
		CCSize nTempSize;
		nTempSize.width =0;
		int nColl=0;
		float nstartPosX=0;
		for (int i=0;i<_sortCount_r;i++)
		{
			if  (m_Skills[i].uId<=0 || m_Skills[i].uId>=MAX_SKILL) continue;
			ISkill *pSkill = g_SkillManager.GetSkill(m_Skills[i].uId,1);
			if (pSkill)
			{//�滭����ͼ��
			  t_sprintf(nSprName,pSkill->getIconPath().c_str());
			  g_StrLower(nSprName);
			  CCSprite* btn_sprite = NULL;
			  bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
			  if (!bgCur) continue;
			  
			  if (nTempSize.width<=0)
			     nTempSize.width =bgCur->getContentSize().width*3;

			  if  (nColl==3)
			  {//ÿ������
				  nColl = 0;
				  offsetSizeH +=bgCur->getContentSize().height; //�߶�����
				  nstartPosX  = 0;//���ȸ�ԭ
			  }
			  btn_sprite =CCSprite::createWithTexture(bgCur);
			  CCMenuItemSprite *tempSpr=CCMenuItemSprite::create(btn_sprite,btn_sprite,NULL,this,menu_selector(KuiSortSkill_l::skillCallback));
			  tempSpr->setPosition(ccp(nstartPosX,offsetSizeH));
			  nstartPosX +=bgCur->getContentSize().width;
			  sortlistMenu->addChild_(tempSpr,i+1);
			  nColl++;
			}
		}
		//sortlistMenu->alignItemsInRows(3,3,3,3,NULL);
		nTempSize.height = offsetSizeH; //�ܸ߶�
		ParentNode_ItemEx->setContentSize(nTempSize);
	}
}
//�˵��ص�
void KuiSortSkill_l::skillCallback(CCObject* pSender)
{
	if (ParentNode_ItemEx)
       ParentNode_ItemEx->removeChildByTag(1001);

	if (pSender)
	{
		CCMenuItemSprite *nCerBox  = (CCMenuItemSprite * )pSender;
		if (nCerBox)
		{
		  int nSkillIdx = nCerBox->getTag()-1;
		  if  (m_Skills[nSkillIdx].uId<=0 || m_Skills[nSkillIdx].uId>=MAX_SKILL) return;
		  ISkill *pSkill = g_SkillManager.GetSkill(m_Skills[nSkillIdx].uId,1);
		  if (pSkill)
		  {//�滭����ͼ��
			int nCurLevel = m_Skills[nSkillIdx].nLevel;
			char nIconName[128];
			ZeroMemory(nIconName,sizeof(nIconName));
		    t_sprintf(nIconName,pSkill->getIconPath().c_str());

			KUiItemdObject skillData;
			ZeroMemory(&skillData,sizeof(KUiItemdObject));
			//skillData.nRect;//= getRect(SkillNode,0);
			skillData.nRect.size   = nCerBox->getContentSize();
			skillData.nRect.origin = nCerBox->getPosition();

			skillData.uId    = m_Skills[nSkillIdx].uId;
			skillData.uGenre = m_Skills[nSkillIdx].uGenre;
			skillData.nindex = 0;//Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.FindSame(Skills[i].uId);
			//int nList = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.FindSame(Skills[i].uId);
			skillData.DataH  = 0;//Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetAddPoint(skillData.nindex);
			skillData.DataW  = 0;//Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetEnChance(skillData.nindex);
			CCLayer * sDescLayer = KuiSkilldesc::create(m_Skills[nSkillIdx].uId,nCurLevel,skillData,this,callfuncN_selector(KuiSortSkill_l::closeSkilldescPadCallback),1);
			ParentNode_ItemEx->addChild(sDescLayer,2,1001);

			if (g_GameWorld)
				g_GameWorld->setattackSprInfo(pSkill->GetSkillId(),m_Skills[nSkillIdx].uGenre,nIconName);
		    //if (g_GameWorld)
			 //  g_GameWorld->setaauxiliaryskillInfo(m_Skills[nSkillIdx].uId,m_Skills[nSkillIdx].uGenre,nIconName);
		  }
		}
	}
	
}

void KuiSortSkill_l::closeSkilldescPadCallback(CCNode *pNode)
{

}
// -------------------------------------------------------------------------
// ����	: ��������
// -------------------------------------------------------------------------
void KuiSortSkill_l::UpdateData()
{
	if (!g_GameWorld || !g_pCoreShell) return;
}

//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void KuiSortSkill_l::setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;
}
//ִ���ϲ����Ļص��������رյ�����
void KuiSortSkill_l::buttonCallBackFunc(CCObject * pSender)
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
void KuiSortSkill_l::closePopLayer(CCObject * pSender)
{
	this->removeFromParentAndCleanup(true);
}

//��д����ע�ắ�������¸�����������
void KuiSortSkill_l::registerWithTouchDispatcher()
{
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-128, true);
}

CCRect KuiSortSkill_l::getRect(CCNode* pNode,int i)
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
bool KuiSortSkill_l::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	//CCSetIterator it = pTouch->begin();
	//CCTouch* touch = (CCTouch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	CCPoint location = pTouch->getLocation();//Ŀ�ĵ�����?��Ļ�����
	buttonCallBackFunc(NULL);
	return true;
}

void KuiSortSkill_l::update(float delta)
{
	//CCMessageBox("����ѭ��","update");
	if (isOpen && g_pCoreShell)
	{
	   // m_nMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
	  //	m_nXu    = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_XU, 0, 0);
	}
}

void KuiSortSkill_l::draw()
{
	//CCMessageBox("����ѭ��","draw");
}


int KuiSortSkill_l::FilterTextColor(char* pMsgBuff, unsigned short nMsgLength)
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