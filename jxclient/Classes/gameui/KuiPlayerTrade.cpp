//�а�ť�ġ������롡�Ի���
//
#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiPlayerTrade.h"
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
#include "gameui/KuiItemdesc.h"
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_CC;

#define	STR_WAIT_TRADING "�ȴ�����ִ��"
#define	STR_OTHER_OK	 "�Է�������"
#define	STR_OTHER_NOT_OK "�ȴ��Է�����"
#define	SEND_MONEY_CHANGE_MSG_DELAY		16

KuiPlayerTrade::KuiPlayerTrade()
{
	isOpen = false;
	colorLayer        = NULL;
	//red               = NULL;
	ParentNode_Item   = NULL;
	ParentNode_Give   = NULL; 
	ParentNode_tarGive= NULL;
	ptradeInfo        = NULL;
	m_StartPos.x=0;
	m_StartPos.y=0;
	m_StartPos_g.x = 0;
	m_StartPos_g.y = 0;
	m_StartPos_t.x = 0;
	m_StartPos_t.y = 0;
	//btnBackPic = NULL;
	m_nItemNumObjects = 0;
	m_ItemData.clear();
	m_giveItemData.clear();
	m_targiveItemData.clear();
	m_nIstrue = false;
	nstrCurSelItemKey = "";
	inCurSelItemKey   = -1;
	m_descPadIsOpen = false;
	m_targiveNum = 0;
	m_giveNum = 0;
	m_itemNum = 0;
	m_nNeed   = 0;
	nBakChickTime = 0;
	m_nMoney =0;
	//m_nXu    =0;
	m_bAdjustingMoney= 0;
	m_nTakewithMoney= 0;
	m_nSelfTradeMoney = 0;
	m_OtherMoney = NULL;
	m_SelfMoneyBox = NULL;
	okConfirm = NULL;
	tradeConfirm= NULL;
	__IsLock = false;
}

KuiPlayerTrade::~KuiPlayerTrade()
{
	isOpen = false;
	colorLayer = NULL;
	//if (red)
	//	red->release();

	m_ItemData.clear();
	m_giveItemData.clear();
	m_targiveItemData.clear();
}

//����ȷ��������
KuiPlayerTrade * KuiPlayerTrade::create(KUiPlayerItem* pOther,CCObject * callbackListener,SEL_CallFuncN callfun)
{
	KuiPlayerTrade * popLayer = KuiPlayerTrade::create();

	if (pOther)
	   popLayer->m_OtherData = *pOther;

	popLayer->addDialogData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;

	//popLayer->m_nNeed = nNum;  //��ִ�нű����� kind  ���ǲ�ִ�нű�
	//t_sprintf(popLayer->m_Callback,"%s",pszCallback);
	//t_sprintf(popLayer->m_Error,"%s",pszError);
	return popLayer;
}


//��ʼ��
bool KuiPlayerTrade::init()
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
	t_sprintf(nSprName,"\\Spr\\Ui3\\����\\��ҽ���.spr");
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
	//��ñ���ͼƬ�Ĵ�С
	CCSize contentSize = spriteBox->getContentSize();
	m_size   = contentSize;
	m_origin = spriteBox->getPosition();
	spriteBox->setPosition(ccp(winSize.width/2,winSize.height/2+20)); //-m_size.width/2+50
	this->addChild(spriteBox);
	
	//colorLayer->setContentSize(contentSize);//���úʹ��ڵĴ�С
	//����������Ӧ
	this->setTouchEnabled(true);
	this->scheduleUpdate();                   //���ú���
	this->setTouchMode(kCCTouchesOneByOne);   //��ֹ��㴥��
	return true;
}

void KuiPlayerTrade::addDialogData()
{
	if  (!g_pCoreShell) return;
	CCSize nSize;
	nSize.width =150;
	nSize.height=100;

	m_StartPos.x =m_size.width-28*6-10;    //+ITEM_CELL_SIZE/2;
	m_StartPos.y =40;//+ITEM_CELL_SIZE/2;

	m_StartPos_g.x =9;
	m_StartPos_g.y =210;

	m_StartPos_t.x =9;
	m_StartPos_t.y =55;
	
	char nTempStr[64];
	ParentNode_Give =CCNode::create();
	ParentNode_Item =CCNode::create();
	ParentNode_tarGive =CCNode::create();
	ParentNode_ItemEx->addChild(ParentNode_Item);
	ParentNode_ItemEx->addChild(ParentNode_Give);
	ParentNode_ItemEx->addChild(ParentNode_tarGive);
	
	t_sprintf(nTempStr,m_OtherData.Name);
	CCLabelTTF *ptextLabel = CCLabelTTF::create(UTEXT(nTempStr,1).c_str(),UI_GAME_FONT_DEFAULT,12);//Arial
	ptextLabel->setColor(ccYELLOW);
	ptextLabel->setPosition(ccp(50,m_size.height-48));
	ParentNode_ItemEx->addChild(ptextLabel);
	/*t_sprintf(nTempStr,pContent);
	CCLabelTTF *pstrLabel = CCLabelTTF::create(UTEXT(nTempStr,1).c_str(),UI_GAME_FONT_DEFAULT,12,nSize,kCCTextAlignmentLeft);//Arial
	pstrLabel->setColor(ccYELLOW);
	pstrLabel->setPosition(ccp(128/2+40,m_size.height-143));
	ParentNode_ItemEx->addChild(pstrLabel);

	char ntitleStr[64];
	t_sprintf(ntitleStr,strTitel);
	CCLabelTTF *ptitLabel = CCLabelTTF::create(UTEXT(ntitleStr,1).c_str(),UI_GAME_FONT_DEFAULT,12);//Arial
	ptitLabel->setPosition(ccp(m_size.width/2,m_size.height-80));
	ParentNode_ItemEx->addChild(ptitLabel);
	*/
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	int m_nWidth,m_nHeight,nFrams;
	CCTexture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));

	pMoneyLabel = CCLabelTTF::create("","fonts/gb_song.ttf", 12);//Arial
	pMoneyLabel->setAnchorPoint(ccp(0,0));
	pMoneyLabel->setPosition(ccp(m_size.width/2+70,43));
	ParentNode_ItemEx->addChild(pMoneyLabel,1);

	char nPointCount[32];
	m_nMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
	m_nTakewithMoney = m_nMoney;
	//m_nXu    = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_XU, 0, 0);
	if (pMoneyLabel)
	{
		t_sprintf(nPointCount,"%d",m_nMoney);
		pMoneyLabel->setString(nPointCount);
	}

	KUiObjAtRegion* pObjs = NULL;
	int nCount   = g_pCoreShell->GetGameData(GDI_ITEM_TAKEN_WITH, 0, 0); //�õ��������Ʒ������

	m_nItemNumObjects = nCount;

	if (nCount == 0)
		return;

	if (pObjs = (KUiObjAtRegion*)malloc(sizeof(KUiObjAtRegion) * nCount))
	{
		g_pCoreShell->GetGameData(GDI_ITEM_TAKEN_WITH, (unsigned int)pObjs, nCount);//���߳�ִ�У�nCountֵ����
		for (int i = 0; i < nCount; ++i)
		{
			KUiDraggedObject no;
			no.uGenre = pObjs[i].Obj.uGenre;
			no.uId    = pObjs[i].Obj.uId;
			no.DataX  = pObjs[i].Region.h;     //�����е����Xλ�� �ڼ������� 25x25
			no.DataY  = pObjs[i].Region.v;     //�����е����Yλ��
			no.DataW  = pObjs[i].Region.Width; //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
			no.DataH  = pObjs[i].Region.Height;//��Ʒ�Ŀ��(ռ���˶��ٸ�����)
			if (Item && pObjs[i].Obj.uId>0 && pObjs[i].Obj.uId<MAX_ITEM)
			{
				/*m_ItemData[i].uGenre = pObjs[i].Obj.uGenre;
				m_ItemData[i].uId    = pObjs[i].Obj.uId;
				m_ItemData[i].nindex = i;
				m_ItemData[i].DataX  = pObjs[i].Region.h;     //�����е����Xλ�� �ڼ������� 25x25
				m_ItemData[i].DataY  = pObjs[i].Region.v;     //�����е����Yλ��
				m_ItemData[i].DataW  = pObjs[i].Region.Width; //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
				m_ItemData[i].DataH  = pObjs[i].Region.Height;//��Ʒ�Ŀ��(ռ���˶��ٸ�����)
				m_ItemData[i].nGenkind = UOC_ITEM_TAKE_WITH;  //����Я��
				*/
				char nItemSprPath[256];
				ZeroMemory(nItemSprPath,sizeof(nItemSprPath));
				if  (Item[pObjs[i].Obj.uId].GetImagePath())
					t_sprintf(nItemSprPath,Item[pObjs[i].Obj.uId].GetImagePath());
				else
					t_sprintf(nItemSprPath,"\\spr\\others\\�ʺ�.spr");

				if  (nItemSprPath[0])
				{
					//t_sprintf(nItemSprPath,Item[pObjs[i].Obj.uId].GetImagePath());
					g_StrLower(nItemSprPath);
					int m_nWidth,m_nHeight,nFrams;
					CCTexture2D *bgCur = NULL;
					SPRFRAMSINFO nSprInfo;
					ZeroMemory(&nSprInfo,sizeof(nSprInfo));
					bgCur = _getinidata.getinidata_one(nItemSprPath,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
					if (bgCur==NULL)
						return;
					char nItemKey[32];
					t_sprintf(nItemKey,"item_%u",Item[pObjs[i].Obj.uId].GetID());
					std::string nKey =nItemKey;
					CCSprite *nItemSpr = (CCSprite *)ParentNode_Item->getChildByTagKey(nKey);
					if (!nItemSpr)
					{
						nItemSpr = CCSprite::createWithTexture(bgCur);
						//nItemSpr->setAnchorPoint(ccp(0,0));
						nItemSpr->setTagbyKey(nKey);
						ParentNode_Item->addChild(nItemSpr,1);

						if  (Item[pObjs[i].Obj.uId].GetGenre()!=item_equip && Item[pObjs[i].Obj.uId].IsStack())
						{//����װ�� �ɵ�����Ʒ
							char stack[32];
							t_sprintf(stack,"%d",Item[pObjs[i].Obj.uId].GetStackNum());
							CCLabelTTF *stuckCountlabel = CCLabelTTF::create(stack,UI_GAME_FONT_DEFAULT,14);
							stuckCountlabel->setColor(ccYELLOW);
							stuckCountlabel->setAnchorPoint(ccp(0,0));
							stuckCountlabel->setPosition(ccp(0,0));
							stuckCountlabel->setTagbyKey("stack");
							nItemSpr->addChild(stuckCountlabel,1);
						}
					}
					//��ʼ������
					m_ItemData[nKey].uGenre = pObjs[i].Obj.uGenre;
					m_ItemData[nKey].uId    = pObjs[i].Obj.uId;
					m_ItemData[nKey].nindex = i;
					m_ItemData[nKey].DataX  = pObjs[i].Region.h;     //�����е����Xλ�� �ڼ������� 25x25
					m_ItemData[nKey].DataY  = pObjs[i].Region.v;     //�����е����Yλ��
					m_ItemData[nKey].DataW  = pObjs[i].Region.Width; //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
					m_ItemData[nKey].DataH  = pObjs[i].Region.Height;//��Ʒ�Ŀ��(ռ���˶��ٸ�����)
					m_ItemData[nKey].nGenkind = UOC_ITEM_TAKE_WITH;  //����Я��

					ccColor4B color = ccc4(112, 128, 144, 150); //112, 128, 144
					//t_sprintf(nItemKey,"color_%d",i+1);
					strcat(nItemKey,"_color");
					nKey = nItemKey;
					CCLayerColor *bgcolorLayer = (CCLayerColor *)ParentNode_Item->getChildByTagKey(nKey);
					if (!bgcolorLayer)
					{
						bgcolorLayer = CCLayerColor::create(color);//��ɫ��
					    //bgcolorLayer->setPosition(CCPointZero);
						bgcolorLayer->setAnchorPoint(ccp(0,0));
						CCSize bgsize;
						bgsize.width  = no.DataW*ITEM_CELL_SIZE-2;
						bgsize.height = no.DataH*ITEM_CELL_SIZE-2;
					    bgcolorLayer->setContentSize(bgsize);//���úʹ��ڵĴ�С
						bgcolorLayer->setTagbyKey(nKey);
						ParentNode_Item->addChild(bgcolorLayer,0);
					}

					int nCurX = m_StartPos.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2;
					int nCurY = m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2;
					
					nItemSpr->setPosition(ccp(nCurX,m_size.height-nCurY));
					//��ɫ��
					nCurX = m_StartPos.x+no.DataX*ITEM_CELL_SIZE+1;
					nCurY = m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE-1;
					bgcolorLayer->setPosition(ccp(nCurX,m_size.height-nCurY));

					KUiObjAtContRegion	Obj;
					Obj.Obj.uGenre = no.uGenre;
					Obj.Obj.uId    = no.uId;
					Obj.Region.h   = Obj.Region.v = 0;
					Obj.Region.Width = Obj.Region.Height = 0;
					Obj.nContainer = UOC_ITEM_TAKE_WITH;

					ITEM_IN_ENVIRO_PROP eProp = (ITEM_IN_ENVIRO_PROP)g_pCoreShell->
					    GetGameData(GDI_ITEM_IN_ENVIRO_PROP, (unsigned int)&Obj, 0);
					//if (eProp == IIEP_NORMAL) //����
					//else 
					if (eProp == IIEP_NOT_USEABLE)  //������
					{
						ccColor3B color_ = ccc3(128, 0, 0);
						bgcolorLayer->setColor(color_);
					}
					//else if (eProp == IIEP_SPECIAL) //����������Ʒ
				}
			}
		}
		free(pObjs); //�ͷ��ڴ�
		pObjs = NULL; //���
	}

	ccColor4B color = ccc4(112, 128, 144, 150);
	colorLayer = CCLayerColor::create(color);//��ɫ��
	colorLayer->setPosition(CCPointZero);
	colorLayer->setContentSize(CCSizeZero);//���úʹ��ڵĴ�С
	ParentNode_Item->addChild(colorLayer,1000);
	//����һ������ �ı侫����ɫ
	//red = CCTintBy::create(0.2,0,-255,-255);//����ʱ��+��ɫ 
	//red->retain();
}


//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void KuiPlayerTrade::setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;

	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\����\\��ҽ��ף���������.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	CCTexture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	CCPoint nStartPos;
	nStartPos.x = m_size.width/2+20;
	nStartPos.y = m_size.height/2;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	CCSprite * sprite_ok_btn_confirm_normal =  CCSprite::createWithTexture(bgCur);//CCSprite::create("close.png");//CCSprite::createWithSpriteFrame(f_tips_btn_confirm_normal);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	CCSprite * sprite_ok_btn_confirm_select =  CCSprite::createWithTexture(bgCur);
	okConfirm = CCMenuItemSprite::create(sprite_ok_btn_confirm_normal, sprite_ok_btn_confirm_select, this, menu_selector(KuiPlayerTrade::okCallBackFunc));
	okConfirm->setPosition(ccp(-140,0));
	okConfirm->setTagbyKey("box");
	//����ȷ��
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\����\\��ҽ��ף�����ȷ��.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	CCSprite * sprite_trade_normal =  CCSprite::createWithTexture(bgCur);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	CCSprite * sprite_trade_select = CCSprite::createWithTexture(bgCur);
	tradeConfirm = CCMenuItemSprite::create(sprite_trade_normal, sprite_trade_select, this, menu_selector(KuiPlayerTrade::tradeCallBackFunc));
	tradeConfirm->setPosition(ccp(-15,0));
	tradeConfirm->setTagbyKey("box");
	//�ر�
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\����\\��ҽ��ף�ȡ������.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	CCSprite * sprite_close_normal =  CCSprite::createWithTexture(bgCur);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	CCSprite * sprite_close_select = CCSprite::createWithTexture(bgCur);
	CCMenuItemSprite * closeConfirm = CCMenuItemSprite::create(sprite_close_normal, sprite_close_select, this, menu_selector(KuiPlayerTrade::buttonCallBackFunc));
	//closeConfirm->setTagbyKey("box");
	closeConfirm->setPosition(ccp(120,0));

	//��ʼ���Ӳ�����ť
	CCSprite * sprite_fanzhi_normal =  CCSprite::create("ui/btn/fz01.png");
	CCSprite * sprite_fanzhi_select = CCSprite::create("ui/btn/fz02.png");
	CCMenuItemSprite * fanzhiConfirm = CCMenuItemSprite::create(sprite_fanzhi_normal, sprite_fanzhi_select, this, menu_selector(KuiPlayerTrade::btnCallBackFunc_fangzhi));
	fanzhiConfirm->setPosition(ccp(38,-30));

	CCSprite * sprite_quxia_normal =  CCSprite::create("ui/btn/qx01.png");
	CCSprite * sprite_quxia_select = CCSprite::create("ui/btn/qx02.png");
	CCMenuItemSprite * quxiaConfirm = CCMenuItemSprite::create(sprite_quxia_normal, sprite_quxia_select, this, menu_selector(KuiPlayerTrade::btnCallBackFunc_quxia));
	quxiaConfirm->setPosition(ccp(38+sprite_quxia_normal->getContentSize().width,-30));
	//CCPoint bgSpritePoint = ParentNode_ItemEx->getPosition();
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\������\\ͼ��\\zenglibtn.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	CCSprite * sprite_zhengli_normal =  CCSprite::createWithTexture(bgCur);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	CCSprite * sprite_zhengli_select =  CCSprite::createWithTexture(bgCur);
	CCMenuItemSprite * zhengliConfirm = CCMenuItemSprite::create(sprite_zhengli_normal, sprite_zhengli_select, this, menu_selector(KuiPlayerTrade::zhengCallBackFunc));
	zhengliConfirm->setPosition(ccp(35+sprite_quxia_normal->getContentSize().width*2,-30));
	
	tradeConfirm->setEnabled(false);
	CCMenu * menu = CCMenu::create(okConfirm,tradeConfirm,closeConfirm,fanzhiConfirm,quxiaConfirm,zhengliConfirm,NULL);
	menu->setisCheckBox(true);
	menu->setPosition(ccp(m_size.width/2,20)); //�˵�λ������Ϊ����������
	ParentNode_ItemEx->addChild(menu);         //�ӵ�������

	CCScale9Sprite *pBg = CCScale9Sprite::create(); 
	m_SelfMoneyBox = CCEditBox::create(CCSize(112,24),pBg);
	const ccColor3B nfcolor ={255,255,255};
	if (m_SelfMoneyBox)
	{
		//pEditBox->setContentSize()
		//pMoneyEditBox->setText("your money");
		m_SelfMoneyBox->setPlaceHolder("input money");
		m_SelfMoneyBox->setFontColor(nfcolor);
		m_SelfMoneyBox->setFontSize(24);
		m_SelfMoneyBox->setMaxLength(9);
		m_SelfMoneyBox->setPosition(ccp(90,45));
		m_SelfMoneyBox->setInputMode(kEditBoxInputModeNumeric);
		m_SelfMoneyBox->setReturnType(kKeyboardReturnTypeDone);
		//pMoneyEditBox->setDelegate(this);
		//pMoneyEditBox->setDefaultTouchPriority(-128);
		ParentNode_ItemEx->addChild(m_SelfMoneyBox,1,500);
	}

	m_OtherMoney = CCLabelTTF::create("money",UI_GAME_FONT_DEFAULT,12);//Arial
	m_OtherMoney->setColor(ccYELLOW);
	m_OtherMoney->setPosition(ccp(90,m_size.height-170));
	ParentNode_ItemEx->addChild(m_OtherMoney);
}


void KuiPlayerTrade::zhengCallBackFunc(CCObject * pSender)
{
	if (m_descPadIsOpen)
	{  
	}

	if  (m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		CCMessageBox("��ѡ��һ����Ʒ!","��ʾ:");
#else
		CCMessageBox(UTEXT("��ѡ��һ����Ʒ!",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		return;
	}
	if  (m_ItemData[nstrCurSelItemKey].uId<=0 || m_ItemData[nstrCurSelItemKey].uId>=MAX_ITEM)
	{
#ifdef WIN32
		CCMessageBox("����Ʒ��������","��ʾ:");
#else
		CCMessageBox(UTEXT("����Ʒ��������",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif	
		return;
	}

	KUiPlayerGameTime	pInfo;
	memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
	ZeroMemory(&pInfo,sizeof(pInfo));
	if (g_pCoreShell)
		g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (int)&pInfo, 0);

	unsigned int nCurChicktime = pInfo.npTime;  //��¼�򿪴��ڵ�����

	if (nBakChickTime>0 && nCurChicktime-nBakChickTime<=2)
	{//�������
		//nBakChickTime = nCurChicktime;
#ifdef WIN32
		CCMessageBox("�����̫Ƶ����","��ʾ:");
#else
		CCMessageBox(UTEXT("�����̫Ƶ����",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		return;
	}

	nBakChickTime = nCurChicktime;

	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre    = m_ItemData[nstrCurSelItemKey].uGenre;
	Obj.Obj.uId       = m_ItemData[nstrCurSelItemKey].uId;
	Obj.Region.h      = m_ItemData[nstrCurSelItemKey].DataX;
	Obj.Region.v      = m_ItemData[nstrCurSelItemKey].DataY;
	Obj.Region.Width  = m_ItemData[nstrCurSelItemKey].DataW;
	Obj.Region.Height = m_ItemData[nstrCurSelItemKey].DataH;
	Obj.eContainer    = UOC_ITEM_TAKE_WITH;
	if (g_pCoreShell)
		g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(int)(&Obj),room_equipment); 
}
//����ȷ�� ��ʼ����
void KuiPlayerTrade::tradeCallBackFunc(CCObject * pSender)
{
	g_pCoreShell->OperationRequest(GOI_TRADE,0,0);
	UpdateTradeOperData();
}
//��������
void KuiPlayerTrade::okCallBackFunc(CCObject * pSender)
{
	if (pSender)
	{
		CCNode *  nCurSelNode = ((CCMenuItemImage *)pSender)->getSelectedSprite();
		if (nCurSelNode)
		{
			OnAdjustedMoney();
			if  (nCurSelNode->isVisible()) 
			{//�Ѿ���ѡ��״̬ δѡ��---ѡ��
				g_pCoreShell->OperationRequest(GOI_TRADE_LOCK,0,true);
				__IsLock = true;
			}
			else
			{//����Ϊѡ��״̬ :ѡ��---δѡ��
				g_pCoreShell->OperationRequest(GOI_TRADE_LOCK,0,false);
				__IsLock = false;
			}
			UpdateTradeOperData();
		}
		//((CCMenuItemImage *)pSender)->setEnabled(false);
	}

	/*CCNode * node = NULL;
	if (pSender)
		node = dynamic_cast<CCNode *>(pSender);
	//node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��
	if (m_callfun &&  m_callbackListener)
		(m_callbackListener->*m_callfun)(node);

	this->removeFromParentAndCleanup(true);*/
}

void KuiPlayerTrade::btnCallBackFunc_fangzhi(CCObject * pSender)
{//�����Լ��Ķ���
	if (m_descPadIsOpen)
	{  
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}

	if  (nstrCurSelItemKey.empty() || m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		CCMessageBox("��ѡ��һ����Ʒ!","��ʾ");
#else
		CCMessageBox(UTEXT("��ѡ��һ����Ʒ!",1).c_str(),UTEXT("��ʾ",1).c_str());
#endif
		return;
	}
	if  (m_ItemData[nstrCurSelItemKey].uId<=0 || m_ItemData[nstrCurSelItemKey].uId>=MAX_ITEM)
	{
#ifdef WIN32
		CCMessageBox("����Ʒ���ܷ���","��ʾ");
#else
		CCMessageBox(UTEXT("����Ʒ���ܷ���",1).c_str(),UTEXT("��ʾ",1).c_str());
#endif	
		return;
	}

	if (__IsLock)
	{
#ifdef WIN32
		CCMessageBox("�������ײ��ܷ���","��ʾ");
#else
		CCMessageBox(UTEXT("�������ײ��ܷ���",1).c_str(),UTEXT("��ʾ",1).c_str());
#endif	
		return;
	}
	KUiPlayerGameTime	pInfo;
	memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
	ZeroMemory(&pInfo,sizeof(pInfo));
	if (g_pCoreShell)
		g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (int)&pInfo, 0);

	unsigned int nCurChicktime = pInfo.npTime;  //��¼�򿪴��ڵ�����

	if (nBakChickTime>0 && nCurChicktime-nBakChickTime<=2)
	{//�������
		return;
	}
	nBakChickTime = nCurChicktime;

	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre    = m_ItemData[nstrCurSelItemKey].uGenre;
	Obj.Obj.uId       = m_ItemData[nstrCurSelItemKey].uId;
	Obj.Region.h      = m_ItemData[nstrCurSelItemKey].DataX;
	Obj.Region.v      = m_ItemData[nstrCurSelItemKey].DataY;
	Obj.Region.Width  = m_ItemData[nstrCurSelItemKey].DataW;
	Obj.Region.Height = m_ItemData[nstrCurSelItemKey].DataH;
	Obj.eContainer    = UOC_ITEM_TAKE_WITH;
	nstrCurSelItemKey="";
	if (g_pCoreShell)
		g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(int)(&Obj),room_trade); 
	//g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(int)(&Obj),room_immediacy); 
}

void KuiPlayerTrade::btnCallBackFunc_quxia(CCObject * pSender)
{
	if (m_descPadIsOpen)
	{  
	}

	if  (inCurSelItemKey<=-1 || m_giveItemData.count(inCurSelItemKey)<=0)
	{
#ifdef WIN32
		CCMessageBox("��ѡ��һ����Ʒ!","��ʾ:");
#else
		CCMessageBox(UTEXT("��ѡ��һ����Ʒ!",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		return;
	}
	if  (m_giveItemData[inCurSelItemKey].uId<=0 || m_giveItemData[inCurSelItemKey].uId>=MAX_ITEM)
	{
#ifdef WIN32
		CCMessageBox("����Ʒ����ȡ��","��ʾ:");
#else
		CCMessageBox(UTEXT("����Ʒ����ȡ��",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif	
		return;
	}

	if (__IsLock)
	{
#ifdef WIN32
		CCMessageBox("�������ײ���ȡ��","��ʾ");
#else
		CCMessageBox(UTEXT("�������ײ���ȡ��",1).c_str(),UTEXT("��ʾ",1).c_str());
#endif	
		return;
	}

	KUiPlayerGameTime	pInfo;
	memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
	ZeroMemory(&pInfo,sizeof(pInfo));
	if (g_pCoreShell)
		g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (int)&pInfo, 0);

	unsigned int nCurChicktime = pInfo.npTime;  //��¼�򿪴��ڵ�����

	if (nBakChickTime>0 && nCurChicktime-nBakChickTime<=2)
	{//�������
		return;
	}
	nBakChickTime = nCurChicktime;

	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre    = m_giveItemData[inCurSelItemKey].uGenre;
	Obj.Obj.uId       = m_giveItemData[inCurSelItemKey].uId;
	Obj.Region.h      = m_giveItemData[inCurSelItemKey].DataX;
	Obj.Region.v      = m_giveItemData[inCurSelItemKey].DataY;
	Obj.Region.Width  = m_giveItemData[inCurSelItemKey].DataW;
	Obj.Region.Height = m_giveItemData[inCurSelItemKey].DataH;
	Obj.eContainer    = UOC_TO_BE_TRADE;
	inCurSelItemKey=-1;
	if (g_pCoreShell)
		g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(int)(&Obj),room_equipment);
}

void KuiPlayerTrade::__ClosePad()
{
	CCNode * node = NULL;
	if (m_callfun &&  m_callbackListener)
		(m_callbackListener->*m_callfun)(node);

	this->removeFromParentAndCleanup(true);
}
//�ر����
void KuiPlayerTrade::buttonCallBackFunc(CCObject * pSender)
{
	//����ȡ��
	g_pCoreShell->OperationRequest(GOI_TRADE_CANCEL,0,0);
	
	CCNode * node = NULL;
	if (pSender)
		node = dynamic_cast<CCNode *>(pSender);
	//node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��
	if (m_callfun &&  m_callbackListener)
		(m_callbackListener->*m_callfun)(node);

	this->removeFromParentAndCleanup(true);
}

//�رյ�����
void KuiPlayerTrade::closePopLayer(CCObject * pSender)
{
	this->removeFromParentAndCleanup(true);
}

//��д����ע�ắ�������¸�����������
void KuiPlayerTrade::registerWithTouchDispatcher()
{
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,2, true);
}

void KuiPlayerTrade::closeItemPadCallback(CCNode *pNode)
{//��������б�Ǻţ���ȷ�����ĸ���ť
	m_descPadIsOpen = false;
}

CCRect KuiPlayerTrade::gettarGiveItem(CCNode* pNode,int i)
{
	CCRect  rc;
	rc.origin     = pNode->getPosition();                      //��������ͼ��ƫ��
	rc.size       = pNode->getContentSize();                   //�����Ʒ�Ŀ�Ⱥ͸߶�
	rc.origin.x   = m_origin.x + m_size.width/2-28+rc.origin.x;      //�������� + m_size.width-30
	rc.origin.y   = m_origin.y+rc.origin.y+m_size.height/2-115;    //���ϱ�    //- rc.origin.y
	return rc;
}

CCRect KuiPlayerTrade::getGiveItem(CCNode* pNode,int i)
{
	CCRect  rc;
	rc.origin     = pNode->getPosition();                      //��������ͼ��ƫ��
	rc.size       = pNode->getContentSize();                   //�����Ʒ�Ŀ�Ⱥ͸߶�
	rc.origin.x   = m_origin.x + m_size.width/2-28+rc.origin.x;      //�������� + m_size.width-30
	rc.origin.y   = m_origin.y+ rc.origin.y + 70;              //���ϱ�
	return rc;
}


CCRect KuiPlayerTrade::getRectItem(CCNode* pNode,int i)
{
	CCRect  rc;
	rc.origin     = pNode->getPosition(); //��������ͼ��ƫ��
	rc.size       = pNode->getContentSize();
	rc.origin.x   = m_origin.x+ rc.origin.x+20+28*6; //�������� m_size.width/2-30
	rc.origin.y   = m_origin.y+ rc.origin.y+70; //���ϱ�    //- rc.origin.y
	
	return rc;
}
//��������ccTouchBegan������true
bool KuiPlayerTrade::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	//CCSetIterator it = pTouch->begin();
	//CCTouch* touch = (CCTouch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	CCPoint location = pTouch->getLocation();//Ŀ�ĵ�����?��Ļ�����
	CCSprite * SkillNode =NULL;
	CCLayerColor *nCurColorlayer = NULL;
	bool isthis = false;

	_ItemData::iterator it;
	for( it = m_ItemData.begin(); it != m_ItemData.end(); ++it)
	{
		if (it->second.uId>0 && it->second.uId<MAX_ITEM)
		{////����ĳ���ܵļӳ�
			char nKey[32];
			t_sprintf(nKey,"item_%u_color",Item[it->second.uId].GetID());
			std::string nItemKey = nKey;
			nCurColorlayer = (CCLayerColor *)ParentNode_Item->getChildByTagKey(nItemKey);
			t_sprintf(nKey,"item_%u",Item[it->second.uId].GetID());
			nItemKey = nKey;
			SkillNode = (CCSprite *)ParentNode_Item->getChildByTagKey(nItemKey);
			if  (nCurColorlayer)
			{
				if (getRectItem(nCurColorlayer,0).containsPoint(location))
				{//������
					if (SkillNode)
					{
						inCurSelItemKey   = -1;
						//SkillNode->runAction(red);
						SkillNode->setColor(ccRED);
						m_ItemData[nItemKey].nRect  = getRectItem(nCurColorlayer,0);
						m_ItemData[nItemKey].nindex = -1;
						CCLayer * sDescLayer = KuiItemdesc::create(it->second,this,callfuncN_selector(KuiPlayerTrade::closeItemPadCallback),1);
						m_descPadIsOpen = true;
						//m_CurSelItemIndex = -1;//m_ItemData[i].uId;
						nstrCurSelItemKey =nItemKey;

						isthis = true;
						this->addChild(sDescLayer,2,1001);
					}
				}
				else
				{
					if (!isthis) //������,�Ͳ�����
					{
						nstrCurSelItemKey ="";
						inCurSelItemKey   = -1;
						//m_CurSelItemIndex = -1;
					}
					if (SkillNode)
						SkillNode->setColor(ccWHITE);           //Ĭ����ɫ
				}
			}
		}
	}
	//give���
	isthis = false;
	_giveItemData::iterator its;
	for( its = m_giveItemData.begin(); its != m_giveItemData.end(); ++its)
	{
		if (its->second.uId>0 && its->second.uId<MAX_ITEM)
		{////����ĳ���ܵļӳ�
			char nKey[32];
			t_sprintf(nKey,"item_%u_color",Item[its->second.uId].GetID());
			std::string nItemKey = nKey;
			nCurColorlayer = (CCLayerColor *)ParentNode_Give->getChildByTagKey(nItemKey);
			t_sprintf(nKey,"item_%u",Item[its->second.uId].GetID());
			nItemKey = nKey;
			SkillNode = (CCSprite *)ParentNode_Give->getChildByTagKey(nItemKey);
			if  (nCurColorlayer)
			{
				//CCMessageBox("��ʼ���","��ʼ���");
				if (getGiveItem(nCurColorlayer,0).containsPoint(location))
				{//������
					//CCMessageBox("������","������");
					if (SkillNode)
					{
						//SkillNode->runAction(red);
						SkillNode->setColor(ccRED);
						its->second.nRect  = getGiveItem(nCurColorlayer,0);
						inCurSelItemKey    = its->second.nindex;
						//its->second.nindex = -1;
						CCLayer * sDescLayer = KuiItemdesc::create(its->second,this,callfuncN_selector(KuiPlayerTrade::closeItemPadCallback),1);
						m_descPadIsOpen = true;
						//m_CurSelItemIndex = -1;//m_ItemData[i].uId;
						//nstrCurSelItemKey =nItemKey;
						isthis = true;
						this->addChild(sDescLayer,2,1001);
					}
				}
				else
				{
					if (!isthis) //������,�Ͳ�����
					{
						//nstrCurSelItemKey ="";
						inCurSelItemKey   = -1;
						//m_CurSelItemIndex = -1;
					}
					if (SkillNode)
						SkillNode->setColor(ccWHITE);           //Ĭ����ɫ
				}
			}
		}
	}

	//targive���
	isthis = false;
	_targiveItemData::iterator itt;
	for( itt = m_targiveItemData.begin(); itt != m_targiveItemData.end(); ++itt)
	{
		if (itt->second.uId>0 && itt->second.uId<MAX_ITEM)
		{////����ĳ���ܵļӳ�
			char nKey[32];
			t_sprintf(nKey,"item_%u_color",Item[itt->second.uId].GetID());
			std::string nItemKey = nKey;
			nCurColorlayer = (CCLayerColor *)ParentNode_tarGive->getChildByTagKey(nItemKey);
			t_sprintf(nKey,"item_%u",Item[itt->second.uId].GetID());
			nItemKey = nKey;
			SkillNode = (CCSprite *)ParentNode_tarGive->getChildByTagKey(nItemKey);
			if  (nCurColorlayer)
			{
				//CCMessageBox("��ʼ���","��ʼ���");
				if (gettarGiveItem(nCurColorlayer,0).containsPoint(location))
				{//������
					//CCMessageBox("������","������");
					if (SkillNode)
					{
						//SkillNode->runAction(red);
						SkillNode->setColor(ccRED);
						itt->second.nRect  = gettarGiveItem(nCurColorlayer,0);
						//inCurSelItemKey    = itt->second.nindex;
						//its->second.nindex = -1;
						CCLayer * sDescLayer = KuiItemdesc::create(itt->second,this,callfuncN_selector(KuiPlayerTrade::closeItemPadCallback),1);
						m_descPadIsOpen = true;
						//m_CurSelItemIndex = -1;//m_ItemData[i].uId;
						//nstrCurSelItemKey =nItemKey;

						isthis = true;
						this->addChild(sDescLayer,2,1001);
					}
				}
				else
				{
					if (!isthis) //������,�Ͳ�����
					{
						//nstrCurSelItemKey ="";
						//inCurSelItemKey   = -1;
						//m_CurSelItemIndex = -1;
					}
					if (SkillNode)
						SkillNode->setColor(ccWHITE);           //Ĭ����ɫ
				}
			}
		}
	}
	return true;
}

void KuiPlayerTrade::UpdateTradeOperData()
{
	int bLock = g_pCoreShell->GetGameData(GDI_TRADE_OPER_DATA, UTOD_IS_LOCKED, 0);
	//m_Ok.CheckButton(bLock);
	if(okConfirm)
	{
		if (bLock)
	       okConfirm->selected();
		else
			okConfirm->unselected();
	}
	if (!ptradeInfo)
	{
		ptradeInfo = CCLabelTTF::create("",UI_GAME_FONT_DEFAULT,12);//Arial
		ptradeInfo->setColor(ccYELLOW);
		ptradeInfo->setPosition(ccp(120,m_size.height-190));
		ParentNode_ItemEx->addChild(ptradeInfo);
	}

	char nTempStr[64];
	ZeroMemory(nTempStr,sizeof(nTempStr));
	if (g_pCoreShell->GetGameData(GDI_TRADE_OPER_DATA, UTOD_IS_TRADING, 0))
	{//�ȴ��Է�����
		if (tradeConfirm)
		{
			tradeConfirm->selected();
			tradeConfirm->setEnabled(true);
		}
		//m_Trade.Enable(true);
		//m_Trade.CheckButton(true);
		//m_Info.SetText(STR_WAIT_TRADING, strlen(STR_WAIT_TRADING));
		//m_Info.SetTextColor(m_uWaitTradeMsgColor);
		t_sprintf(nTempStr,STR_WAIT_TRADING);
		if (ptradeInfo)
			ptradeInfo->setString(UTEXT(nTempStr,1).c_str());
	}
	else
	{
		int bOtherLock = g_pCoreShell->GetGameData(GDI_TRADE_OPER_DATA, UTOD_IS_OTHER_LOCKED, 0);
		//m_Trade.CheckButton(false);
		//m_Trade.Enable(bLock && bOtherLock);
		if (tradeConfirm)
		{
			tradeConfirm->unselected();
			tradeConfirm->setEnabled(bLock && bOtherLock);
		}
		//�Է��Ƿ�����
		t_sprintf(nTempStr,bOtherLock ? STR_OTHER_OK:STR_OTHER_NOT_OK);
		if (ptradeInfo)
			ptradeInfo->setString(UTEXT(nTempStr,1).c_str());
		//m_Info.SetText(bOtherLock ? STR_OTHER_OK : STR_OTHER_NOT_OK, -1);
		//m_Info.SetTextColor(bOtherLock ? m_uOtherLockMsgColor : m_uOtherNotLockMsgColor);
	}
	if (m_SelfMoneyBox)
	   m_SelfMoneyBox->setEnabled(!bLock);
	/*m_SelfItemsBox.Enable(!bLock);
	m_SelfMoney.Enable(!bLock);
	m_TakewithItemsBox.Enable(!bLock);
	m_AddMoney.Enable(!bLock);
	m_ReduceMoney.Enable(!bLock);
	m_SelfMoney.Enable(!bLock);*/
}

//�Է��Ķ���
void KuiPlayerTrade::UpdateTarGiveItem(KUiObjAtRegion* pObj, int bAdd)
{
	if (!pObj) return;
	if (pObj->Obj.uGenre == CGOG_MONEY)
	{//�Է���Ǯ�ı���
		if (m_OtherMoney)
		{
		  char nTempStr[64];
		  ZeroMemory(nTempStr,sizeof(nTempStr));
		  t_sprintf(nTempStr,"%u",pObj->Obj.uId);
		  m_OtherMoney->setString(nTempStr);
		}
	}
	else
	{//��Ʒ
			KUiDraggedObject Obj;
			Obj.uGenre = pObj->Obj.uGenre;
			Obj.uId    = pObj->Obj.uId;
			Obj.DataX  = pObj->Region.h;
			Obj.DataY  = pObj->Region.v;
			Obj.DataW  = pObj->Region.Width;
			Obj.DataH  = pObj->Region.Height;

			if  (Obj.uId<=0 || Obj.uId>=MAX_ITEM)
			{
				//CCMessageBox("��������","��Ʒ");
				return;
			}

			if (bAdd) //����
			{
				//m_ItemBox.AddObject(&Obj,1);
				if (pObj->Obj.uId)
				{
					m_targiveNum++;
				}

				AddtarGiveObject(&Obj,m_targiveNum);

			}
			else      //����
			{
				if (pObj->Obj.uId)
				{
					m_targiveNum--;
				}
				char nKey[32];
				t_sprintf(nKey,"item_%u_color",Item[Obj.uId].GetID());
				std::string nItemKey = nKey;
				ParentNode_tarGive->removeChildByTagKey(nItemKey,true);
				t_sprintf(nKey,"item_%u",Item[Obj.uId].GetID());
				nItemKey = nKey;
				ParentNode_tarGive->removeChildByTagKey(nItemKey,true);
				_targiveItemData::iterator it;
				for( it = m_targiveItemData.begin(); it != m_targiveItemData.end(); it++)
				{
					if (it->second.uId>0 && it->second.uId<MAX_ITEM && it->second.uId ==Obj.uId)
					{
						m_targiveItemData.erase(it->second.nindex);//
						break;
					}
				}
			}
	}
}

//���˵Ķ���
void KuiPlayerTrade::UpdateGiveItem(KUiObjAtRegion* pItem, int bAdd)
{
	if (pItem)
	{

		if (pItem->Obj.uGenre == CGOG_MONEY)
		{//�Լ���Ǯ
			m_nSelfTradeMoney = pItem->Obj.uId;
			//m_SelfMoney.SetIntText(pObj->Obj.uId);
			if (m_SelfMoneyBox)
			{
				char nTempStr[64];
				ZeroMemory(nTempStr,sizeof(nTempStr));
				t_sprintf(nTempStr,"%u",pItem->Obj.uId);
				m_SelfMoneyBox->setText(nTempStr);
			}
			return;
		}

		KUiDraggedObject Obj;
		Obj.uGenre = pItem->Obj.uGenre;
		Obj.uId    = pItem->Obj.uId;
		Obj.DataX  = pItem->Region.h;
		Obj.DataY  = pItem->Region.v;
		Obj.DataW  = pItem->Region.Width;
		Obj.DataH  = pItem->Region.Height;

		if  (Obj.uId<=0 || Obj.uId>=MAX_ITEM)
		{
			//CCMessageBox("��������","��Ʒ");
			return;
		}

		if (bAdd) //����
		{
			//m_ItemBox.AddObject(&Obj,1);
			if (pItem->Obj.uId)
			{
				m_giveNum++;
			}

			AddGiveObject(&Obj,m_giveNum);

		}
		else      //����
		{
			if (pItem->Obj.uId)
			{
				m_giveNum--;
			}
			char nKey[32];
			t_sprintf(nKey,"item_%u_color",Item[Obj.uId].GetID());
			std::string nItemKey = nKey;
			ParentNode_Give->removeChildByTagKey(nItemKey,true);
			t_sprintf(nKey,"item_%u",Item[Obj.uId].GetID());
			nItemKey = nKey;
			ParentNode_Give->removeChildByTagKey(nItemKey,true);
			_giveItemData::iterator it;
			for( it = m_giveItemData.begin(); it != m_giveItemData.end(); it++)
			{
				if (it->second.uId>0 && it->second.uId<MAX_ITEM && it->second.uId ==Obj.uId)
				{
					m_giveItemData.erase(it->second.nindex);
					break;
				}
			}
			//m_giveItemData.erase(nItemKey);
		}
	}
}

void KuiPlayerTrade::UpdateItem(KUiObjAtRegion* pItem, int bAdd)
{

	if (pItem)
	{
		KUiDraggedObject Obj;
		Obj.uGenre = pItem->Obj.uGenre;
		Obj.uId    = pItem->Obj.uId;
		Obj.DataX  = pItem->Region.h;
		Obj.DataY  = pItem->Region.v;
		Obj.DataW  = pItem->Region.Width;
		Obj.DataH  = pItem->Region.Height;

		if  (Obj.uId<=0 || Obj.uId>=MAX_ITEM)
		{
			//CCMessageBox("��������","��Ʒ");
			return;
		}

		if (bAdd) //����
		{
			//m_ItemBox.AddObject(&Obj,1);
			if (pItem->Obj.uId)
			{
				m_itemNum ++;
			}
			
			AddObject(&Obj,1);

		}
		else      //����
		{
			if (pItem->Obj.uId)
			{
				m_itemNum--;
			}
			char nKey[32];
			t_sprintf(nKey,"item_%u_color",Item[Obj.uId].GetID());
			std::string nItemKey = nKey;
			ParentNode_Item->removeChildByTagKey(nItemKey,true);
			t_sprintf(nKey,"item_%u",Item[Obj.uId].GetID());
			nItemKey = nKey;
			ParentNode_Item->removeChildByTagKey(nItemKey,true);
			m_ItemData.erase(nItemKey);
		}
		//m_ItemBox.RemoveObject(&Obj);
		//UiSoundPlay(UI_SI_PICKPUT_ITEM);
		//UpdateData();
	}
	else //����ȫ������Ʒ
		UpdateItemData();
}

void KuiPlayerTrade::UpdateItemData()
{
	char nTempStr[64];
	ZeroMemory(nTempStr,sizeof(nTempStr));
	m_nTakewithMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
	t_sprintf(nTempStr,"&d",m_nTakewithMoney);
	pMoneyLabel->setString(nTempStr);
// m_CurSelItemIndex = -1;
   nstrCurSelItemKey ="";
  // memset(m_ItemData,0,sizeof(KuiSellItemdObject) * 60);
   _ItemData::iterator it;
   for( it = m_ItemData.begin(); it != m_ItemData.end(); it++)
   {
	   if (it->second.uId>0 && it->second.uId<MAX_ITEM)
	   {////����ĳ���ܵļӳ�
		   char nKey[32];
		   t_sprintf(nKey,"item_%u_color",Item[it->second.uId].GetID());
		   std::string nItemKey = nKey;
		   ParentNode_Item->removeChildByTagKey(nItemKey,true);
		   //nCurColorlayer = (CCLayerColor *)ParentNode_Item->getChildByTagKey(nItemKey);
		   t_sprintf(nKey,"item_%u",Item[it->second.uId].GetID());
		   nItemKey = nKey;
		   //SkillNode = (CCSprite *)ParentNode_Item->getChildByTagKey(nItemKey);
		   ParentNode_Item->removeChildByTagKey(nItemKey,true);
	   }
   }
   m_ItemData.clear();

	KUiObjAtRegion* pObjs = NULL;
	int nCount    = g_pCoreShell->GetGameData(GDI_ITEM_TAKEN_WITH, 0, 0); //�õ��������Ʒ������

	m_nItemNumObjects = nCount;

	if (nCount == 0)
		return;

	if (pObjs = (KUiObjAtRegion*)malloc(sizeof(KUiObjAtRegion) * nCount))
	{
		g_pCoreShell->GetGameData(GDI_ITEM_TAKEN_WITH, (unsigned int)pObjs, nCount);//���߳�ִ�У�nCountֵ����
		for (int i = 0; i < nCount; ++i)
		{
			KUiDraggedObject no;
			no.uGenre = pObjs[i].Obj.uGenre;
			no.uId    = pObjs[i].Obj.uId;
			no.DataX  = pObjs[i].Region.h;     //�����е����Xλ�� �ڼ������� 25x25
			no.DataY  = pObjs[i].Region.v;     //�����е����Yλ��
			no.DataW  = pObjs[i].Region.Width; //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
			no.DataH  = pObjs[i].Region.Height;//��Ʒ�Ŀ��(ռ���˶��ٸ�����)
			if (Item && pObjs[i].Obj.uId>0 && pObjs[i].Obj.uId<MAX_ITEM)
			{
				char nItemSprPath[256];
				ZeroMemory(nItemSprPath,sizeof(nItemSprPath));
				if  (Item[pObjs[i].Obj.uId].GetImagePath())
					t_sprintf(nItemSprPath,Item[pObjs[i].Obj.uId].GetImagePath());
				else
					t_sprintf(nItemSprPath,"\\spr\\others\\�ʺ�.spr");

				if  (nItemSprPath[0])
				{
					//t_sprintf(nItemSprPath,Item[pObjs[i].Obj.uId].GetImagePath());
					g_StrLower(nItemSprPath);
					int m_nWidth,m_nHeight,nFrams;
					CCTexture2D *bgCur = NULL;
					SPRFRAMSINFO nSprInfo;
					ZeroMemory(&nSprInfo,sizeof(nSprInfo));
					bgCur = _getinidata.getinidata_one(nItemSprPath,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
					if (bgCur==NULL)
						return;
					char nItemKey[32];
					ZeroMemory(&nItemKey,sizeof(nItemKey));
					t_sprintf(nItemKey,"item_%u",Item[pObjs[i].Obj.uId].GetID());
					std::string nKey =nItemKey;
					CCSprite *nItemSpr = (CCSprite *)ParentNode_Item->getChildByTagKey(nKey);
					if (!nItemSpr)
					{
						nItemSpr = CCSprite::createWithTexture(bgCur);
						//nItemSpr->setAnchorPoint(ccp(0,0));
						nItemSpr->setTagbyKey(nKey);
						ParentNode_Item->addChild(nItemSpr,1);

						if  (Item[pObjs[i].Obj.uId].GetGenre()!=item_equip && Item[pObjs[i].Obj.uId].IsStack())
						{//����װ�� �ɵ�����Ʒ
							char stack[32];
							t_sprintf(stack,"%d",Item[pObjs[i].Obj.uId].GetStackNum());
							CCLabelTTF *stuckCountlabel = CCLabelTTF::create(stack,UI_GAME_FONT_DEFAULT,14);
							stuckCountlabel->setColor(ccYELLOW);
							stuckCountlabel->setAnchorPoint(ccp(0,0));
							stuckCountlabel->setPosition(ccp(0,0));
							stuckCountlabel->setTagbyKey("stack");
							nItemSpr->addChild(stuckCountlabel,1);
						}
					}
					//��ʼ������
					m_ItemData[nKey].uGenre = pObjs[i].Obj.uGenre;
					m_ItemData[nKey].uId    = pObjs[i].Obj.uId;
					m_ItemData[nKey].nindex = i;
					m_ItemData[nKey].DataX  = pObjs[i].Region.h;     //�����е����Xλ�� �ڼ������� 25x25
					m_ItemData[nKey].DataY  = pObjs[i].Region.v;     //�����е����Yλ��
					m_ItemData[nKey].DataW  = pObjs[i].Region.Width; //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
					m_ItemData[nKey].DataH  = pObjs[i].Region.Height;//��Ʒ�Ŀ��(ռ���˶��ٸ�����)
					m_ItemData[nKey].nGenkind = UOC_ITEM_TAKE_WITH;  //����Я��

					ccColor4B color = ccc4(112, 128, 144, 150);//112, 128, 144
					//t_sprintf(nItemKey,"color_%d",i+1);
					strcat(nItemKey,"_color");
					nKey =nItemKey;
					CCLayerColor *bgcolorLayer = (CCLayerColor *)ParentNode_Item->getChildByTagKey(nKey);
					if (!bgcolorLayer)
					{
						bgcolorLayer = CCLayerColor::create(color);//��ɫ��
					    //bgcolorLayer->setPosition(CCPointZero);
						bgcolorLayer->setAnchorPoint(ccp(0,0));
						CCSize bgsize;
						bgsize.width  = no.DataW*ITEM_CELL_SIZE-2;
						bgsize.height = no.DataH*ITEM_CELL_SIZE-2;
					    bgcolorLayer->setContentSize(bgsize);//���úʹ��ڵĴ�С
						bgcolorLayer->setTagbyKey(nKey);
						ParentNode_Item->addChild(bgcolorLayer,0);
					}

					int nCurX = m_StartPos.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2;
					int nCurY = m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2;
					
					nItemSpr->setPosition(ccp(nCurX,m_size.height-nCurY));
					//��ɫ��
					nCurX = m_StartPos.x+no.DataX*ITEM_CELL_SIZE+1;
					nCurY = m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE-1;
					bgcolorLayer->setPosition(ccp(nCurX,m_size.height-nCurY));

					KUiObjAtContRegion	Obj;
					Obj.Obj.uGenre = no.uGenre;
					Obj.Obj.uId    = no.uId;
					Obj.Region.h   = Obj.Region.v = 0;
					Obj.Region.Width = Obj.Region.Height = 0;
					Obj.nContainer = UOC_ITEM_TAKE_WITH;

					ITEM_IN_ENVIRO_PROP eProp = (ITEM_IN_ENVIRO_PROP)g_pCoreShell->
					    GetGameData(GDI_ITEM_IN_ENVIRO_PROP, (unsigned int)&Obj, 0);
					//if (eProp == IIEP_NORMAL) //����
					//else 
					if (eProp == IIEP_NOT_USEABLE)  //������
					{
						ccColor3B color_ = ccc3(128, 0, 0);
						bgcolorLayer->setColor(color_);
					}
					//else if (eProp == IIEP_SPECIAL) //����������Ʒ
				}
			}
		}
		free(pObjs); //�ͷ��ڴ�
		pObjs = NULL; //���
	}
}

void KuiPlayerTrade::OnModififyMoney()
{
	m_bAdjustingMoney = 1;
	char nTempStr[64];
	ZeroMemory(nTempStr,sizeof(nTempStr));
	t_sprintf(nTempStr,m_SelfMoneyBox->getText());
	
	int nNewSelfTradeMoney = g_Atoui(nTempStr);//m_SelfMoney.GetIntNumber();
	if (nNewSelfTradeMoney < 0)
		nNewSelfTradeMoney = 0;
	if (nNewSelfTradeMoney > m_nTakewithMoney + m_nSelfTradeMoney)
	{
		nNewSelfTradeMoney = m_nTakewithMoney + m_nSelfTradeMoney;
		//m_SelfMoney.SetIntText(nNewSelfTradeMoney);
		t_sprintf(nTempStr,"%u",nNewSelfTradeMoney);
		m_SelfMoneyBox->setText(nTempStr);
	}

	m_nTakewithMoney -= nNewSelfTradeMoney - m_nSelfTradeMoney;
	m_nSelfTradeMoney = nNewSelfTradeMoney;
	//m_TakewithMoney.SetMoneyText(m_nTakewithMoney);
	t_sprintf(nTempStr,"%u",m_nTakewithMoney);
	pMoneyLabel->setString(nTempStr);
}

void KuiPlayerTrade::OnAdjustedMoney()
{
	OnModififyMoney();
	m_bAdjustingMoney = 0;
	if (g_pCoreShell)
	{
		KUiObjAtRegion	Obj = {0};
		Obj.Obj.uGenre = CGOG_MONEY;
		Obj.Obj.uId = m_nSelfTradeMoney;
		g_pCoreShell->OperationRequest(GOI_TRADE_DESIRE_ITEM,(unsigned int)&Obj, 0);
	}
}

void KuiPlayerTrade::update(float delta)
{
	//CCMessageBox("����ѭ��","update");
	if (isOpen && g_pCoreShell)
	{
		if (m_bAdjustingMoney)
		{
			if ((++m_bAdjustingMoney) == SEND_MONEY_CHANGE_MSG_DELAY)
				OnAdjustedMoney();
		}

	   if (g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, GET_BOXEX_OPEN, 0))
		   m_nIstrue = true;
		   //	m_ItemEX.SetOpenClose(TRUE);
		   else
			   m_nIstrue = false;

	  //���µ�������
		   _ItemData::iterator it;
		   for( it = m_ItemData.begin(); it != m_ItemData.end(); ++it)
		   {
			   if (it->second.uId>0 && it->second.uId<MAX_ITEM)
			   {
				   if  (Item[it->second.uId].GetGenre()==0)
					   continue;

				   char nItemKey[32];
				   t_sprintf(nItemKey,"item_%u",Item[it->second.uId].GetID());
				   std::string nKey =nItemKey;
				   CCSprite *nTempSpr = (CCSprite *)ParentNode_Item->getChildByTagKey(nKey);
				   if  (nTempSpr)
				   {
					   CCLabelTTF *stuckCountlabel = (CCLabelTTF *)nTempSpr->getChildByTagKey("stack");
					   if  (stuckCountlabel)
					   {
						   char nCount[32];
						   t_sprintf(nCount,"%d",Item[it->second.uId].GetStackNum());
						   stuckCountlabel->setString(nCount);
					   }
				   }
			   }
		   }
	}
}

void KuiPlayerTrade::draw()
{
	//CCMessageBox("����ѭ��","draw");
}
//�Է������
int KuiPlayerTrade::AddtarGiveObject(KUiDraggedObject* pObject, int nindex)
{
	if (!pObject) return false;
	for (int i = 0; i < 1; ++i)
	{
		KUiDraggedObject no;
		no.uGenre = pObject->uGenre;
		no.uId    = pObject->uId;
		no.DataX  = pObject->DataX;     //�����е����Xλ�� �ڼ������� 25x25
		no.DataY  = pObject->DataY;     //�����е����Yλ��
		no.DataW  = pObject->DataW; //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
		no.DataH  = pObject->DataH;//��Ʒ�Ŀ��(ռ���˶��ٸ�����)
		if (Item && pObject->uId>0 && pObject->uId<MAX_ITEM)
		{
			char nItemSprPath[256];
			ZeroMemory(nItemSprPath,sizeof(nItemSprPath));
			if  (Item[pObject->uId].GetImagePath())
				t_sprintf(nItemSprPath,Item[pObject->uId].GetImagePath());
			else
				t_sprintf(nItemSprPath,"\\spr\\others\\�ʺ�.spr");
			if (nItemSprPath[0])
			{
				//t_sprintf(nItemSprPath,Item[pObject->uId].GetImagePath());
				g_StrLower(nItemSprPath);
				int m_nWidth,m_nHeight,nFrams;
				CCTexture2D *bgCur = NULL;
				SPRFRAMSINFO nSprInfo;
				ZeroMemory(&nSprInfo,sizeof(nSprInfo));
				bgCur = _getinidata.getinidata_one(nItemSprPath,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
				if (bgCur==NULL)
					return false;
				char nItemKey[32];
				ZeroMemory(&nItemKey,sizeof(nItemKey));
				t_sprintf(nItemKey,"item_%u",Item[pObject->uId].GetID());
				std::string nKey =nItemKey;
				CCSprite *nItemSpr = (CCSprite *)ParentNode_tarGive->getChildByTagKey(nKey);
				if (!nItemSpr)
				{
					nItemSpr = CCSprite::createWithTexture(bgCur);
					//nItemSpr->setAnchorPoint(ccp(0,0));
					nItemSpr->setTagbyKey(nKey);
					ParentNode_tarGive->addChild(nItemSpr,1);

					if  (Item[pObject->uId].GetGenre()!=item_equip && Item[pObject->uId].IsStack())
					{//����װ�� �ɵ�����Ʒ
						char stack[32];
						t_sprintf(stack,"%d",Item[pObject->uId].GetStackNum());
						CCLabelTTF *stuckCountlabel = CCLabelTTF::create(stack,UI_GAME_FONT_DEFAULT,14);
						stuckCountlabel->setColor(ccYELLOW);
						stuckCountlabel->setAnchorPoint(ccp(0,0));
						stuckCountlabel->setPosition(ccp(0,0));
						stuckCountlabel->setTagbyKey("stack");
						nItemSpr->addChild(stuckCountlabel,1);
					}
				}
				//��ʼ������
				m_targiveItemData[nindex].uGenre = pObject->uGenre;
				m_targiveItemData[nindex].uId    = pObject->uId;
				m_targiveItemData[nindex].nindex = nindex;
				m_targiveItemData[nindex].DataX  = pObject->DataX;     //�����е����Xλ�� �ڼ������� 25x25
				m_targiveItemData[nindex].DataY  = pObject->DataY;     //�����е����Yλ��
				m_targiveItemData[nindex].DataW  = pObject->DataW;     //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
				m_targiveItemData[nindex].DataH  = pObject->DataH;     //��Ʒ�Ŀ��(ռ���˶��ٸ�����)
				m_targiveItemData[nindex].nGenkind = UOC_OTHER_TO_BE_TRADE;  //�Լ����

				ccColor4B color = ccc4(112, 128, 144, 150);//112, 128, 144
				//t_sprintf(nItemKey,"color_%d",i+1);
				strcat(nItemKey,"_color");
				nKey =nItemKey;
				CCLayerColor *bgcolorLayer = (CCLayerColor *)ParentNode_tarGive->getChildByTagKey(nKey);
				if (!bgcolorLayer)
				{
					bgcolorLayer = CCLayerColor::create(color);//��ɫ��
					//bgcolorLayer->setPosition(CCPointZero);
					bgcolorLayer->setAnchorPoint(ccp(0,0));
					CCSize bgsize;
					bgsize.width  = no.DataW*ITEM_CELL_SIZE-2;
					bgsize.height = no.DataH*ITEM_CELL_SIZE-2;
					bgcolorLayer->setContentSize(bgsize);//���úʹ��ڵĴ�С
					bgcolorLayer->setTagbyKey(nKey);
					ParentNode_tarGive->addChild(bgcolorLayer,0);
				}

				int nCurX = m_StartPos_t.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2;
				int nCurY = m_StartPos_t.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2;

				nItemSpr->setPosition(ccp(nCurX,m_size.height-nCurY));
				//��ɫ��
				nCurX = m_StartPos_t.x+no.DataX*ITEM_CELL_SIZE+1;
				nCurY = m_StartPos_t.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE-1;
				bgcolorLayer->setPosition(ccp(nCurX,m_size.height-nCurY));

				KUiObjAtContRegion	Obj;
				Obj.Obj.uGenre = no.uGenre;
				Obj.Obj.uId    = no.uId;
				Obj.Region.h   = Obj.Region.v = 0;
				Obj.Region.Width = Obj.Region.Height = 0;
				Obj.nContainer = UOC_ITEM_GIVE;

				ITEM_IN_ENVIRO_PROP eProp = (ITEM_IN_ENVIRO_PROP)g_pCoreShell->
					GetGameData(GDI_ITEM_IN_ENVIRO_PROP, (unsigned int)&Obj, 0);
				//if (eProp == IIEP_NORMAL) //����
				//else 
				if (eProp == IIEP_NOT_USEABLE)  //������
				{
					ccColor3B color_ = ccc3(128, 0, 0);
					bgcolorLayer->setColor(color_);
				}
				//else if (eProp == IIEP_SPECIAL) //����������Ʒ
			}
		}
	}
	return true;
}

//�Լ����
int KuiPlayerTrade::AddGiveObject(KUiDraggedObject* pObject, int nindex)
{
	if (!pObject) return false;
	for (int i = 0; i < 1; ++i)
	{
		KUiDraggedObject no;
		no.uGenre = pObject->uGenre;
		no.uId    = pObject->uId;
		no.DataX  = pObject->DataX;     //�����е����Xλ�� �ڼ������� 25x25
		no.DataY  = pObject->DataY;     //�����е����Yλ��
		no.DataW  = pObject->DataW; //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
		no.DataH  = pObject->DataH;//��Ʒ�Ŀ��(ռ���˶��ٸ�����)
		if (Item && pObject->uId>0 && pObject->uId<MAX_ITEM)
		{
			char nItemSprPath[256];
			ZeroMemory(nItemSprPath,sizeof(nItemSprPath));
			if  (Item[pObject->uId].GetImagePath())
				t_sprintf(nItemSprPath,Item[pObject->uId].GetImagePath());
			else
				t_sprintf(nItemSprPath,"\\spr\\others\\�ʺ�.spr");
			if (nItemSprPath[0])
			{
				//t_sprintf(nItemSprPath,Item[pObject->uId].GetImagePath());
				g_StrLower(nItemSprPath);
				int m_nWidth,m_nHeight,nFrams;
				CCTexture2D *bgCur = NULL;
				SPRFRAMSINFO nSprInfo;
				ZeroMemory(&nSprInfo,sizeof(nSprInfo));
				bgCur = _getinidata.getinidata_one(nItemSprPath,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
				if (bgCur==NULL)
					return false;
				char nItemKey[32];
				ZeroMemory(&nItemKey,sizeof(nItemKey));
				t_sprintf(nItemKey,"item_%u",Item[pObject->uId].GetID());
				std::string nKey =nItemKey;
				CCSprite *nItemSpr = (CCSprite *)ParentNode_Give->getChildByTagKey(nKey);
				if (!nItemSpr)
				{
					nItemSpr = CCSprite::createWithTexture(bgCur);
					//nItemSpr->setAnchorPoint(ccp(0,0));
					nItemSpr->setTagbyKey(nKey);
					ParentNode_Give->addChild(nItemSpr,1);

					if  (Item[pObject->uId].GetGenre()!=item_equip && Item[pObject->uId].IsStack())
					{//����װ�� �ɵ�����Ʒ
						char stack[32];
						t_sprintf(stack,"%d",Item[pObject->uId].GetStackNum());
						CCLabelTTF *stuckCountlabel = CCLabelTTF::create(stack,UI_GAME_FONT_DEFAULT,14);
						stuckCountlabel->setColor(ccYELLOW);
						stuckCountlabel->setAnchorPoint(ccp(0,0));
						stuckCountlabel->setPosition(ccp(0,0));
						stuckCountlabel->setTagbyKey("stack");
						nItemSpr->addChild(stuckCountlabel,1);
					}
				}
				//��ʼ������
				m_giveItemData[nindex].uGenre = pObject->uGenre;
				m_giveItemData[nindex].uId    = pObject->uId;
				m_giveItemData[nindex].nindex = nindex;
				m_giveItemData[nindex].DataX  = pObject->DataX;     //�����е����Xλ�� �ڼ������� 25x25
				m_giveItemData[nindex].DataY  = pObject->DataY;     //�����е����Yλ��
				m_giveItemData[nindex].DataW  = pObject->DataW;     //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
				m_giveItemData[nindex].DataH  = pObject->DataH;     //��Ʒ�Ŀ��(ռ���˶��ٸ�����)
				m_giveItemData[nindex].nGenkind = UOC_TO_BE_TRADE;  //�Լ����

				ccColor4B color = ccc4(112, 128, 144, 150);//112, 128, 144
				//t_sprintf(nItemKey,"color_%d",i+1);
				strcat(nItemKey,"_color");
				nKey =nItemKey;
				CCLayerColor *bgcolorLayer = (CCLayerColor *)ParentNode_Give->getChildByTagKey(nKey);
				if (!bgcolorLayer)
				{
					bgcolorLayer = CCLayerColor::create(color);//��ɫ��
					//bgcolorLayer->setPosition(CCPointZero);
					bgcolorLayer->setAnchorPoint(ccp(0,0));
					CCSize bgsize;
					bgsize.width  = no.DataW*ITEM_CELL_SIZE-2;
					bgsize.height = no.DataH*ITEM_CELL_SIZE-2;
					bgcolorLayer->setContentSize(bgsize);//���úʹ��ڵĴ�С
					bgcolorLayer->setTagbyKey(nKey);
					ParentNode_Give->addChild(bgcolorLayer,0);
				}

				int nCurX = m_StartPos_g.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2;
				int nCurY = m_StartPos_g.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2;

				nItemSpr->setPosition(ccp(nCurX,m_size.height-nCurY));
				//��ɫ��
				nCurX = m_StartPos_g.x+no.DataX*ITEM_CELL_SIZE+1;
				nCurY = m_StartPos_g.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE-1;
				bgcolorLayer->setPosition(ccp(nCurX,m_size.height-nCurY));

				KUiObjAtContRegion	Obj;
				Obj.Obj.uGenre = no.uGenre;
				Obj.Obj.uId    = no.uId;
				Obj.Region.h   = Obj.Region.v = 0;
				Obj.Region.Width = Obj.Region.Height = 0;
				Obj.nContainer = UOC_ITEM_GIVE;

				ITEM_IN_ENVIRO_PROP eProp = (ITEM_IN_ENVIRO_PROP)g_pCoreShell->
					GetGameData(GDI_ITEM_IN_ENVIRO_PROP, (unsigned int)&Obj, 0);
				//if (eProp == IIEP_NORMAL) //����
				//else 
				if (eProp == IIEP_NOT_USEABLE)  //������
				{
					ccColor3B color_ = ccc3(128, 0, 0);
					bgcolorLayer->setColor(color_);
				}
				//else if (eProp == IIEP_SPECIAL) //����������Ʒ
			}
		}
	}
	return true;
}
//������Ʒ
int KuiPlayerTrade::AddObject(KUiDraggedObject* pObject, int nCount)
{
	if (!pObject) return false;

	for (int i = 0; i < nCount; ++i)
	{
			KUiDraggedObject no;
			no.uGenre = pObject->uGenre;
			no.uId    = pObject->uId;
			no.DataX  = pObject->DataX;     //�����е����Xλ�� �ڼ������� 25x25
			no.DataY  = pObject->DataY;     //�����е����Yλ��
			no.DataW  = pObject->DataW; //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
			no.DataH  = pObject->DataH;//��Ʒ�Ŀ��(ռ���˶��ٸ�����)
			if (Item && pObject->uId>0 && pObject->uId<MAX_ITEM)
			{
				char nItemSprPath[256];
				ZeroMemory(nItemSprPath,sizeof(nItemSprPath));
				if  (Item[pObject->uId].GetImagePath())
					t_sprintf(nItemSprPath,Item[pObject->uId].GetImagePath());
				else
					t_sprintf(nItemSprPath,"\\spr\\others\\�ʺ�.spr");
				if (nItemSprPath[0])
				{
					//t_sprintf(nItemSprPath,Item[pObject->uId].GetImagePath());
					g_StrLower(nItemSprPath);
					int m_nWidth,m_nHeight,nFrams;
					CCTexture2D *bgCur = NULL;
					SPRFRAMSINFO nSprInfo;
					ZeroMemory(&nSprInfo,sizeof(nSprInfo));
					bgCur = _getinidata.getinidata_one(nItemSprPath,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
					if (bgCur==NULL)
						return false;
					char nItemKey[32];
					ZeroMemory(&nItemKey,sizeof(nItemKey));
					t_sprintf(nItemKey,"item_%u",Item[pObject->uId].GetID());
					std::string nKey =nItemKey;
					CCSprite *nItemSpr = (CCSprite *)ParentNode_Item->getChildByTagKey(nKey);
					if (!nItemSpr)
					{
						nItemSpr = CCSprite::createWithTexture(bgCur);
						//nItemSpr->setAnchorPoint(ccp(0,0));
						nItemSpr->setTagbyKey(nKey);
						ParentNode_Item->addChild(nItemSpr,1);

						if  (Item[pObject->uId].GetGenre()!=item_equip && Item[pObject->uId].IsStack())
						{//����װ�� �ɵ�����Ʒ
							char stack[32];
							t_sprintf(stack,"%d",Item[pObject->uId].GetStackNum());
							CCLabelTTF *stuckCountlabel = CCLabelTTF::create(stack,UI_GAME_FONT_DEFAULT,14);
							stuckCountlabel->setColor(ccYELLOW);
							stuckCountlabel->setAnchorPoint(ccp(0,0));
							stuckCountlabel->setPosition(ccp(0,0));
							stuckCountlabel->setTagbyKey("stack");
							nItemSpr->addChild(stuckCountlabel,1);
						}
					}
					//��ʼ������
					m_ItemData[nKey].uGenre = pObject->uGenre;
					m_ItemData[nKey].uId    = pObject->uId;
					m_ItemData[nKey].nindex = i;
					m_ItemData[nKey].DataX  = pObject->DataX;     //�����е����Xλ�� �ڼ������� 25x25
					m_ItemData[nKey].DataY  = pObject->DataY;     //�����е����Yλ��
					m_ItemData[nKey].DataW  = pObject->DataW; //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
					m_ItemData[nKey].DataH  = pObject->DataH;//��Ʒ�Ŀ��(ռ���˶��ٸ�����)
					m_ItemData[nKey].nGenkind = UOC_ITEM_TAKE_WITH;  //����Я��

					ccColor4B color = ccc4(112, 128, 144, 150);//112, 128, 144
					//t_sprintf(nItemKey,"color_%d",i+1);
					strcat(nItemKey,"_color");
					nKey =nItemKey;
					CCLayerColor *bgcolorLayer = (CCLayerColor *)ParentNode_Item->getChildByTagKey(nKey);
					if (!bgcolorLayer)
					{
						bgcolorLayer = CCLayerColor::create(color);//��ɫ��
					    //bgcolorLayer->setPosition(CCPointZero);
						bgcolorLayer->setAnchorPoint(ccp(0,0));
						CCSize bgsize;
						bgsize.width  = no.DataW*ITEM_CELL_SIZE-2;
						bgsize.height = no.DataH*ITEM_CELL_SIZE-2;
					    bgcolorLayer->setContentSize(bgsize);//���úʹ��ڵĴ�С
						bgcolorLayer->setTagbyKey(nKey);
						ParentNode_Item->addChild(bgcolorLayer,0);
					}

					int nCurX = m_StartPos.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2;
					int nCurY = m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2;
					
					nItemSpr->setPosition(ccp(nCurX,m_size.height-nCurY));
					//��ɫ��
					nCurX = m_StartPos.x+no.DataX*ITEM_CELL_SIZE+1;
					nCurY = m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE-1;
					bgcolorLayer->setPosition(ccp(nCurX,m_size.height-nCurY));

					KUiObjAtContRegion	Obj;
					Obj.Obj.uGenre = no.uGenre;
					Obj.Obj.uId    = no.uId;
					Obj.Region.h   = Obj.Region.v = 0;
					Obj.Region.Width = Obj.Region.Height = 0;
					Obj.nContainer = UOC_ITEM_TAKE_WITH;

					ITEM_IN_ENVIRO_PROP eProp = (ITEM_IN_ENVIRO_PROP)g_pCoreShell->
					    GetGameData(GDI_ITEM_IN_ENVIRO_PROP, (unsigned int)&Obj, 0);
					//if (eProp == IIEP_NORMAL) //����
					//else 
					if (eProp == IIEP_NOT_USEABLE)  //������
					{
						ccColor3B color_ = ccc3(128, 0, 0);
						bgcolorLayer->setColor(color_);
					}
					//else if (eProp == IIEP_SPECIAL) //����������Ʒ
				}
			}
	}
	return true;
}

int KuiPlayerTrade::FilterTextColor(char* pMsgBuff, unsigned short nMsgLength)
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
