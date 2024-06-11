#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiViewRoleState.h"
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
#include "gameui/KuiItemdesc.h"
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_CC;

enum WAIT_OTHER_WND_OPER_PARAM
{
	UISTOREBOX_IS_AVATAR, 
};
// ---> �����ؼ���UIEP_*�Լ��ɽ�����Ʒ�����͵Ķ�Ӧ��ϵ
static struct UE_CTRL_MAP_VIEW
{
	int				nPosition;
	const char*		pIniSection;
	int             nCellx;    //��ռ�ü�������
	int             nCelly;    //��ռ�ü�������
	float           nOffsetX;  //������е�λ��ƫ�ƹ̶���
	float           nOffsetY;  //������е�λ��ƫ�ƹ̶���
}ViewCtrlItemMap[_ITEM_COUNT] =
{
	{ UIEP_HEAD,		"Cap"		,2,2,119,51},	//0װ��-ñ��
	{ UIEP_HAND,		"Weapon"	,2,4,217,122},	//1װ��-����
	{ UIEP_NECK,		"Necklace"	,2,1,218,83},	//2װ��-����
	{ UIEP_FINESSE,		"Bangle"	,1,2,46,95},	//3װ��-����
	{ UIEP_BODY,		"Cloth"		,2,3,119,111},	//4װ��-�·�
	{ UIEP_WAIST,		"Sash"		,2,1,119,199},	//5װ��-����
	{ UIEP_FINGER1,		"Ring1"		,1,1,46,153},	//6װ��-��ָ
	{ UIEP_FINGER2,		"Ring2"		,1,1,46,183},	//7װ��-��ָ
	{ UIEP_WAIST_DECOR,	"Pendant"	,1,2,46,216},	//8װ��-��׺
	{ UIEP_FOOT,		"Shoes"		,2,2,217,239},	//9װ��-Ь��
	{ UIEP_HORSE,		"Horse"		,2,3,119,240},	//10װ��-��
	{ UIEP_MASK,		"Mask"		,1,1,46,51},	//11װ��-���	// mat na
	{ UIEP_PIFENG,	    "Mantle"	,1,1,46,305},	//12����
	{ UIEP_YINJIAN,		"Signet"	,1,1,213,297},	//13ӡ��
	{ UIEP_SHIPING,	    "Shipin"	,1,1,247,297},	//14��Ʒ
};

KuiViewRoleState::KuiViewRoleState()
{
	isOpen = false;
	//memset(Skills,0,sizeof(KUiSkillData) * 50);
	colorLayer        = NULL;
	red               = NULL;
	ItemBoxNode       = NULL;
	m_nMoney = 0;
	m_nXu = 0;
	m_nNumObjects = 0;
	m_StartPos.x=0;
	m_StartPos.y=0;
	m_descPadIsOpen =false;
	m_CurSelItemIndex = 0;
	for (int i=0;i<_ITEM_COUNT;i++)
	    m_itemIndexBox[i]=0;

	pRoleName=NULL;
	pRoleTitle=NULL;
	//pRoleTitle_1=NULL;
	pPk=NULL;

	pZhuanSheng=NULL;
	pZhuanShengLevel=NULL;

	__RemainPoint = 0;
	ncurDest = NULL;
}

KuiViewRoleState::~KuiViewRoleState()
{
	isOpen = false;
	colorLayer        = NULL;
	if (red)
	   red->release();
	//red               = NULL;
}

//����ȷ��������
KuiViewRoleState * KuiViewRoleState::create(KUiPlayerItem* pDest,CCObject * callbackListener,SEL_CallFuncN callfun)
{
	KuiViewRoleState * popLayer = KuiViewRoleState::create();
	popLayer->ncurDest = pDest;

	//memcpy(&m_pSelf->m_ZhuangBeiPad.m_UseData, m_UseData, sizeof(KUiPlayerItem));     //װ����Ϣ
	popLayer->addDialogData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;

	popLayer->UpdateData(pDest);
	return popLayer;
}

//��ʼ��
bool KuiViewRoleState::init()
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
	/*char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\���\\���2.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	CCTexture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return false;*/
	CCSprite * spriteBox = CCSprite::create("ui/state/view_other.png");
	ParentNode_Team= spriteBox;
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
	this->setTouchMode(kCCTouchesOneByOne); //��ֹ��㴥��
	return true;
}

void KuiViewRoleState::addDialogData()
{
	if  (!g_pCoreShell) return;

	/*ItemBoxNode = CCNode::create();
	ItemBoxNode->setPosition(ccp(0,0));
	ItemBoxNode->setAnchorPoint(ccp(0,0));
	ParentNode_Team->addChild(ItemBoxNode,2);*/
	//Left=24
	//Top=72
	m_StartPos.x =24;//+ITEM_CELL_SIZE/2;
	m_StartPos.y =72;//+ITEM_CELL_SIZE/2;

	ccColor4B color = ccc4(112, 128, 144, 150);
	colorLayer = CCLayerColor::create(color);//��ɫ��
	colorLayer->setPosition(CCPointZero);
	colorLayer->setContentSize(CCSizeZero);  //���úʹ��ڵĴ�С
	ParentNode_Team->addChild(colorLayer,1000);
	//����һ������ �ı侫����ɫ
	red = CCTintBy::create(0.2,0,-255,-255);//����ʱ��+��ɫ 
	red->retain();
	float nfontsize =12;
	float nScale = nfontsize/16;
	//��ʼ�����ı���Ϣ
	pRoleName = CCLabelTTF::create(UTEXT("���Բ���1",1).c_str(),UI_GAME_FONT_DEFAULT, nfontsize);//Arial
	pRoleName->setAnchorPoint(ccp(0,0));
	pRoleName->setPosition(ccp(50,m_size.height/2+142));
	ParentNode_Team->addChild(pRoleName);

	pRoleTitle = CCLabelTTF::create(UTEXT("���Բ���2",1).c_str(),UI_GAME_FONT_SONG, nfontsize);   //Arial
	pRoleTitle->setAnchorPoint(ccp(0,0));
	pRoleTitle->setPosition(ccp(50,m_size.height/2+124));
	pRoleTitle->setColor(ccGREEN);
	ParentNode_Team->addChild(pRoleTitle);

	/*pRoleTitle_1 = CCLabelTTF::create(UTEXT("���Բ���3",1).c_str(),UI_GAME_FONT_SONG,nfontsize);   //Arial
	pRoleTitle_1->setAnchorPoint(ccp(0,0));
	pRoleTitle_1->setPosition(ccp(360,m_size.height/2+140));
	ParentNode_Team->addChild(pRoleTitle_1);*/

	pPk   = CCLabelBMFont::create("0",UI_GAME_FONT_NUMBER);
	pPk->setAnchorPoint(ccp(0,0));
	pPk->setPosition(ccp(m_size.width-55,m_size.height/2+138));
	ParentNode_Team->addChild(pPk);

	pZhuanSheng = CCLabelTTF::create(UTEXT("ת������",1).c_str(),UI_GAME_FONT_SONG, 12);   //Arial
	pZhuanSheng->setAnchorPoint(ccp(0,0));
	pZhuanSheng->setPosition(ccp(m_size.width-100,m_size.height/2+124));
	ParentNode_Team->addChild(pZhuanSheng);

	pZhuanShengLevel = CCLabelTTF::create(UTEXT("ת���ȼ�",1).c_str(),UI_GAME_FONT_SONG, 12);   //Arial
	pZhuanShengLevel->setAnchorPoint(ccp(0,0));
	pZhuanShengLevel->setPosition(ccp(m_size.width-100,m_size.height/2+108));
	ParentNode_Team->addChild(pZhuanShengLevel);

}

//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void KuiViewRoleState::setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;

	//CCSprite * sprite_tips_btn_confirm_normal =  CCSprite::create("close.png");//CCSprite::createWithSpriteFrame(f_tips_btn_confirm_normal);
	//CCSpriteFrame * f_tips_btn_confirm_select = sfCache->spriteFrameByName("btn_confirm_select.png");
	//CCSprite * sprite_tips_btn_confirm_select =  CCSprite::create("close.png");//CCSprite::createWithSpriteFrame(f_tips_btn_confirm_select);
	//"\\spr\\UI3\\������\\�ر�.spr"
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\activityguide\\closebutton.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	CCTexture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
    CCSprite * sprite_tips_btn_confirm_normal = CCSprite::createWithTexture(bgCur);

	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	CCSprite * sprite_tips_btn_confirm_select = CCSprite::createWithTexture(bgCur);
	CCMenuItemSprite * itemConfirm   = CCMenuItemSprite::create(sprite_tips_btn_confirm_normal, sprite_tips_btn_confirm_select, this, menu_selector(KuiViewRoleState::buttonCallBackFunc));
	itemConfirm->setPosition(ccp(0,0));
	
	 CCMenu * menu = CCMenu::create(itemConfirm,NULL);
	 menu->setPosition(ccp(m_size.width/2,13));
	 ParentNode_Team->addChild(menu);          //�ӵ�������
	//CCPoint bgSpritePoint = ParentNode_Team->getPosition();
}


//ִ���ϲ����Ļص��������رյ�����
void KuiViewRoleState::buttonCallBackFunc(CCObject * pSender)
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
void KuiViewRoleState::closePopLayer(CCObject * pSender)
{
	this->removeFromParentAndCleanup(true);
}

//��д����ע�ắ�������¸�����������
void KuiViewRoleState::registerWithTouchDispatcher()
{
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-128,true);
}


void KuiViewRoleState::UpdateAllEquips()
{
	if  (!g_GameWorld || !g_pCoreShell) return;

	KUiObjAtRegion	Equips[_ITEM_COUNT];
	int nCount = g_pCoreShell->GetGameData(GDI_PARADE_EQUIPMENT, (unsigned int)&Equips, 0);
	
	int	i;

	//for (i = 0; i < _ITEM_COUNT; ++i)
		//m_EquipBox[i].Celar();

	for (i = 0; i < nCount; ++i)
	{
		if (Equips[i].Obj.uGenre != CGOG_NOTHING)
			UpdateEquip(&Equips[i], true);  //���ӻ����
		else
		    UpdateEquip(&Equips[i], false); //ɾ���������
	}
}

void KuiViewRoleState::addpicBox(int index,int nItemIndx,bool isAdd)
{
	char nKey[32];
	t_sprintf(nKey,"%s",ViewCtrlItemMap[index].pIniSection);
	std::string m_ItemBoxKey = nKey;
	t_sprintf(nKey,"%s_color",ViewCtrlItemMap[index].pIniSection);
	std::string m_colorbgKey = nKey;//��ɫ���key
	if  (isAdd)
	{//���� 
		if (nItemIndx<=0 || nItemIndx>=MAX_ITEM) return;
		if (ParentNode_Team)
		{
			char nItemSprPath[256];
			ZeroMemory(nItemSprPath,sizeof(nItemSprPath));
			CCTexture2D *bgCur = NULL;
			int nItemW = Item[nItemIndx].GetWidth()*ITEM_CELL_SIZE;
			int nItemH = Item[nItemIndx].GetHeight()*ITEM_CELL_SIZE;
			m_itemIndexBox[index]=nItemIndx;
			if  (Item[nItemIndx].GetImagePath())
			{
				t_sprintf(nItemSprPath,Item[nItemIndx].GetImagePath());
			}
			else
				t_sprintf(nItemSprPath,"\\spr\\others\\�ʺ�.spr");

			if  (nItemSprPath[0])
			{
				//t_sprintf(nItemSprPath,Item[nItemIndx].GetImagePath());
				g_StrLower(nItemSprPath);
				int m_nWidth,m_nHeight,nFrams;
				SPRFRAMSINFO nSprInfo;
				ZeroMemory(&nSprInfo,sizeof(nSprInfo));
				bgCur = _getinidata.getinidata_one(nItemSprPath,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
			}

			if (bgCur==NULL)
				return;

		   CCSprite * tempSpr = (CCSprite *)ParentNode_Team->getChildByTagKey(m_ItemBoxKey);
		   CCLayerColor *bgcolorLayer = (CCLayerColor *)ParentNode_Team->getChildByTagKey(m_colorbgKey);
		   
		   CCSize bgsize;
		   bgsize.width  = ViewCtrlItemMap[index].nCellx*ITEM_CELL_SIZE;
		   bgsize.height = ViewCtrlItemMap[index].nCelly*ITEM_CELL_SIZE;
		   CCSize nTempSize = bgCur->getContentSize();
		   float tempOffsetY = 55;
		   if (!tempSpr)
		   {   
			   tempSpr =CCSprite::createWithTexture(bgCur);
			   tempSpr->setPosition(ccp(0,0));
			   tempSpr->setAnchorPoint(ccp(0,0));
			   tempSpr->setTagbyKey(m_ItemBoxKey);
			   //tempSpr->setPosition(ccp(m_size.width/2+CtrlItemMap[index].nOffsetX+bgsize.width/2-nItemW/2,m_size.height-CtrlItemMap[index].nOffsetY));
			   ParentNode_Team->addChild(tempSpr,2);
			   
			   if (!bgcolorLayer)
			   {
				   ccColor4B color = ccc4(112, 128, 144, 150);
				   bgcolorLayer = CCLayerColor::create(color);//��ɫ��
				   //bgcolorLayer->setPosition(CCPointZero);
				   bgcolorLayer->setAnchorPoint(ccp(0,0));
				  
				   bgcolorLayer->setContentSize(bgsize);//���úʹ��ڵĴ�С
				   bgcolorLayer->setTagbyKey(m_colorbgKey);
				   bgcolorLayer->setPosition(ccp(0+ViewCtrlItemMap[index].nOffsetX,m_size.height-ViewCtrlItemMap[index].nOffsetY-bgsize.height-tempOffsetY));
				   ParentNode_Team->addChild(bgcolorLayer,1);
			   }
			 //  return;
		   }
		   //tempSpr->setContentSize(nTempSize);
		   CCRect nRectZero = CCRectMake(0,0,0,0);
		   nRectZero.size = bgCur->getContentSize();
		   tempSpr->setTexture(bgCur);
		   tempSpr->setTextureRect(nRectZero,false,nRectZero.size);
		   tempSpr->setPosition(ccp(0+ViewCtrlItemMap[index].nOffsetX+bgsize.width/2-nTempSize.width/2,m_size.height-ViewCtrlItemMap[index].nOffsetY-(bgsize.height+nTempSize.height)/2-tempOffsetY));
		  
		   //��ʼ������ñ�����ɫ
		   KUiObjAtContRegion	Obj;
		   Obj.Obj.uGenre = CGOG_ITEM;
		   Obj.Obj.uId    = nItemIndx;
		   Obj.Region.h     = Obj.Region.v = 0;
		   Obj.Region.Width = Obj.Region.Height = 0;
		   Obj.nContainer   = UOC_EQUIPTMENT;

		   ITEM_IN_ENVIRO_PROP eProp = (ITEM_IN_ENVIRO_PROP)g_pCoreShell->
			   GetGameData(GDI_ITEM_IN_ENVIRO_PROP, (unsigned int)&Obj, 0);
		   //if (eProp == IIEP_NORMAL) //����
		   //else 
		   if (eProp == IIEP_NOT_USEABLE)  //������
		   {
			   ccColor3B color_ = ccc3(128, 0, 0);
			   if (bgcolorLayer)
			      bgcolorLayer->setColor(color_);
		   }
		}
	}
	else
	{//ɾ��
		if (ParentNode_Team)
		{
			m_itemIndexBox[index]=0;
			ParentNode_Team->removeChildByTagKey(m_ItemBoxKey);
			ParentNode_Team->removeChildByTagKey(m_colorbgKey);
		}
	}
}
//--------------------------------------------------------------------------
//	���ܣ�װ���仯����
//--------------------------------------------------------------------------
void KuiViewRoleState::UpdateEquip(KUiObjAtRegion* pEquip, int bAdd)
{
	if (Item && pEquip)
	{
		for (int i = 0; i < _ITEM_COUNT; ++i)
		{
			if (ViewCtrlItemMap[i].nPosition == pEquip->Region.v)
			{//�����������
				if (bAdd)//����
					//m_EquipBox[i].HoldObject(pEquip->Obj.uGenre, pEquip->Obj.uId,
					//pEquip->Region.Width, pEquip->Region.Height);
					//CCMessageBox(Item[pEquip->Obj.uId].GetImagePath(),"����");
					addpicBox(i,pEquip->Obj.uId,true);
				else//�Ƴ�
					addpicBox(i,pEquip->Obj.uId,false);
					//CCMessageBox(Item[pEquip->Obj.uId].GetImagePath(),"�Ƴ�");
					//m_EquipBox[i].HoldObject(CGOG_NOTHING, 0, 0, 0);
				
				break;
			}
		}
	}
}

CCRect KuiViewRoleState::getRect(CCNode* pNode,int i)
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
bool KuiViewRoleState::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	//CCSetIterator it = pTouch->begin();
	//CCTouch* touch = (CCTouch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	CCPoint location = pTouch->getLocation();//Ŀ�ĵ�����?��Ļ�����
	CCSprite * itemBoxSpr =NULL;
	bool isthis=false;
	for (int i = 0; i < _ITEM_COUNT; ++i)
	{
		char nKey[32];
		t_sprintf(nKey,"%s",ViewCtrlItemMap[i].pIniSection);
		std::string m_ItemBoxKey = nKey;
		t_sprintf(nKey,"%s_color",ViewCtrlItemMap[i].pIniSection);
		std::string m_colorbgKey = nKey;//��ɫ���key
		itemBoxSpr = (CCSprite *)ParentNode_Team->getChildByTagKey(m_ItemBoxKey);
		CCLayerColor *colorBoxbg = (CCLayerColor *)ParentNode_Team->getChildByTagKey(m_colorbgKey);
		//CCMessageBox("�����ʼ","�����ʼ");
		if  (itemBoxSpr && colorBoxbg)
		{
			if (getRect(colorBoxbg,i+1).containsPoint(location))
			{//������
				//restartAction();
				itemBoxSpr->runAction(red);  
				//ccColor3B color_ = ccc3(128, 0, 0);
				//if (colorBoxbg)
					//colorBoxbg->setColor(color_);
				KUiItemdObject second;
				second.uId    = m_itemIndexBox[i];
				second.uGenre = CGOG_ITEM;
				second.nRect  = getRect(colorBoxbg,0);
				second.nGenkind  =  UOC_EQUIPTMENT;
				CCLayer * sDescLayer = KuiItemdesc::create(second,this,callfuncN_selector(KuiViewRoleState::closedescPadCallback),0);
				m_descPadIsOpen = true;
				m_CurSelItemIndex = m_itemIndexBox[i];//m_ItemData[i].uId;
				//nstrCurSelItemKey =nItemKey;
				isthis = true;
				this->addChild(sDescLayer,2,1001);
			}
			else
			{
				if (!isthis)
				  m_CurSelItemIndex =0;

				itemBoxSpr->setColor(ccWHITE);           //Ĭ����ɫ
			}
		}
	}
	return true;
}

void KuiViewRoleState::closedescPadCallback(CCNode *pNode)
{
	m_descPadIsOpen = false;
}

void KuiViewRoleState::UpdateData(KUiPlayerItem* pDest)
{
	if (pDest == NULL)
		return;

	UpdateBaseData(pDest);

	UpdateAllEquips();
}

void KuiViewRoleState::UpdateBaseData(KUiPlayerItem* pDest)
{
	KUiPlayerBaseInfo	Info;
	memset(&Info, 0, sizeof(KUiPlayerBaseInfo));
	if (g_pCoreShell)
	    g_pCoreShell->GetGameData(GDI_PLAYER_BASE_INFO, (int)&Info,pDest->uId);

	pRoleName->setString(UTEXT(Info.Name,1).c_str());
	pRoleTitle->setString(UTEXT(Info.Title,1).c_str());
	//pRoleTitle_1->setString(UTEXT(Info.Agname,1).c_str());
	//m_Agname.SetText(Info.Agname);  //�º�
	//m_Name  .SetText(Info.Name);    //����
	//m_Title .SetText(Info.Title);   //�ƺ�
}

void KuiViewRoleState::BreatheUpdateInfo()
{
	/*
	KUiPlayerRuntimeInfo	Info;
	memset(&Info, 0, sizeof(KUiPlayerRuntimeInfo));
	if (g_pCoreShell)
	    g_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (int)&Info, 0);

	char nTempS4tr[64];
	ZeroMemory(nTempS4tr,sizeof(nTempS4tr));
	t_sprintf(nTempS4tr,"%d/%d",Info.nLife,Info.nLifeFull);
	pLife->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%d/%d",Info.nMana,Info.nManaFull);
	pMana->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%d/%d",Info.nStamina,Info.nStaminaFull);
	pTili->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%d/%d",Info.nExperience,Info.nExperienceFull);
	pExp->setString(nTempS4tr);
	*/
	//UiNewStatus* pStatus = UiNewStatus::GetIfVisible();
	//if (pStatus)
	//	UpdateRuntimeInfo(&Info);
	//pStatus=NULL;
}


void  KuiViewRoleState::BreathAttributeUpdate()
{
	KUiPlayerAttribute	Info;
	memset(&Info, 0, sizeof(KUiPlayerAttribute));
	if (g_pCoreShell)
	   g_pCoreShell->GetGameData(GDI_PLAYER_RT_ATTRIBUTE, (unsigned int)&Info, 0);

	char nTempS4tr[64];
	ZeroMemory(nTempS4tr,sizeof(nTempS4tr));
	t_sprintf(nTempS4tr,"%d",Info.nPKValue);
	pPk->setString(nTempS4tr);

	t_sprintf(nTempS4tr,"ת������:%d",Info.nReBorn);
	pZhuanSheng->setString(UTEXT(nTempS4tr,1).c_str());

	//t_sprintf(nTempS4tr,"ת������:%d",Info.);
	//pZhuanSheng->setString(UTEXT(nTempS4tr,1).c_str());

}
void KuiViewRoleState::update(float delta)
{
	if (isOpen && g_pCoreShell)
	{
	   //UpdateAllEquips();
	   //BreatheUpdateInfo();
	   //BreathAttributeUpdate();
	}
}

void KuiViewRoleState::draw()
{
	//CCMessageBox("����ѭ��","draw");
}