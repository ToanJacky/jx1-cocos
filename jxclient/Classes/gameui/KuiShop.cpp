#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiShop.h"
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
#include "gamecore/KBuySell.h"
#include "gameui/KuiItemdesc.h" 
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_CC;

KuiShop::KuiShop()
{
	isOpen = false;
	//memset(Skills,0,sizeof(KUiSkillData) * 50);
	pMoneyLabel       = NULL;
	pXuLabel          = NULL;
	colorLayer        = NULL;
	red               = NULL;
	m_selItemIndex    = -1;
	//m_nMoney = 0;
	//m_nXu = 0;
	//m_nNumObjects = 0;
	m_CurShopIdx  = 0;
	m_nPageCount  = 0;
	m_nObjCount = 0;
	m_pObjsList = NULL;
	m_StartPos.x=0;
	m_StartPos.y=0;
	m_nCurrentCount = 0;
	m_nCurrentPage  = 0;
	ItemNode = NULL;
	pstrEditBox_count = NULL;
	m_descPadIsOpen = false;
}

KuiShop::~KuiShop()
{
	isOpen = false;
	pMoneyLabel  = NULL;
	colorLayer   = NULL;
	Clear();
	if (red)
	   red->release();
	//red               = NULL;
}

//����ȷ��������
KuiShop * KuiShop::create(int nshopIdx,CCObject * callbackListener,SEL_CallFuncN callfun)
{
	KuiShop * popLayer = KuiShop::create();
	//popLayer->setTitle(title);
	popLayer->m_CurShopIdx = nshopIdx;
	popLayer->addDialoaData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//��ʼ��
bool KuiShop::init()
{
	if ( !CCLayer::init() ){
		return false;
	}

	//����֡����
	//CCSpriteFrameCache * sfCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	//sfCache->addSpriteFramesWithFile("p_tips.plist");

	//���ģ̬����
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\����\\�������.spr");
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
	this->setTouchMode(kCCTouchesOneByOne);   //��ֹ��㴥��
	return true;
}

void KuiShop::addDialoaData()
{
	if  (!g_pCoreShell) return;

	//Left=24
	//Top=72
	if (g_pCoreShell)
		g_pCoreShell->GetGameData(GDI_LAOHUJI_INFO,192,0);
	m_StartPos.x =24;//+ITEM_CELL_SIZE/2;
	m_StartPos.y =72;//+ITEM_CELL_SIZE/2;

    pMoneyLabel = CCLabelTTF::create("","fonts/gb_song.ttf", 12);//Arial
	pMoneyLabel->setAnchorPoint(ccp(0,0));
	pMoneyLabel->setPosition(ccp(60,128));
	ParentNode_ItemEx->addChild(pMoneyLabel,1);

	pXuLabel = CCLabelTTF::create("","fonts/gb_song.ttf", 12);//Arial
	pXuLabel->setAnchorPoint(ccp(0,0));
	pXuLabel->setPosition(ccp(60,105));
	ParentNode_ItemEx->addChild(pXuLabel,1);

	ItemNode = CCNode::create();
	ItemNode->setAnchorPoint(ccp(0,0));
	ItemNode->setPosition(ccp(0,0));
	ParentNode_ItemEx->addChild(ItemNode,1);
	UpdateData();	
	ccColor4B color = ccc4(112, 128, 144, 150);
	colorLayer = CCLayerColor::create(color);//��ɫ��
	colorLayer->setPosition(CCPointZero);
	colorLayer->setContentSize(CCSizeZero);//���úʹ��ڵĴ�С
	ParentNode_ItemEx->addChild(colorLayer,1000);
	//����һ������ �ı侫����ɫ
	red = CCTintBy::create(0.2,0,-255,-255);//����ʱ��+��ɫ 
	red->retain();
}

//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void KuiShop::setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;
	if (g_pCoreShell)
		g_pCoreShell->GetGameData(GDI_LAOHUJI_INFO,192,0);
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\����\\���������.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	CCTexture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	CCSprite * sprite_buy_normal = CCSprite::createWithTexture(bgCur);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	CCSprite * sprite_buy_select = CCSprite::createWithTexture(bgCur);
	CCMenuItemSprite * buyConfirm   = CCMenuItemSprite::create(sprite_buy_normal, sprite_buy_select, this, menu_selector(KuiShop::buyCallBackFunc));
	buyConfirm->setPosition(ccp(-70,1));

	CCScale9Sprite *gBg = CCScale9Sprite::create("ui/editbg.png");
	pstrEditBox_count = CCEditBox::create(CCSize(50,24),gBg);
	if (pstrEditBox_count)
	{
		//pEditBox->setContentSize()
		pstrEditBox_count->setText("1");
		pstrEditBox_count->setPlaceHolder(UTEXT("��������",1).c_str());
		pstrEditBox_count->setFontColor(ccYELLOW);
		pstrEditBox_count->setFontSize(24);
		pstrEditBox_count->setMaxLength(3);
		pstrEditBox_count->setPosition(ccp(m_size.width/2-10,47));
		pstrEditBox_count->setInputMode(kEditBoxInputModeNumeric);
		pstrEditBox_count->setReturnType(kKeyboardReturnTypeDone);
		//pMoneyEditBox->setDelegate(this);
		//pMoneyEditBox->setDefaultTouchPriority(-128);
		ParentNode_ItemEx->addChild(pstrEditBox_count);
	}

	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\����\\���ߣ��ر�.spr");
	g_StrLower(nSprName);
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	CCSprite * sprite_close_normal = CCSprite::createWithTexture(bgCur);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	CCSprite * sprite_close_select = CCSprite::createWithTexture(bgCur);
	CCMenuItemSprite * closeConfirm   = CCMenuItemSprite::create(sprite_close_normal, sprite_close_select, this, menu_selector(KuiShop::closeCallBackFunc));
	closeConfirm->setPosition(ccp(69,1));

	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\����\\������壭��.spr");
	g_StrLower(nSprName);
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	CCSprite * sprite_back_normal = CCSprite::createWithTexture(bgCur);
	//sprite_back_normal->setScaleX(2.0);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	CCSprite * sprite_back_select = CCSprite::createWithTexture(bgCur);
	//sprite_back_select->setScaleX(2.0);
	CCMenuItemSprite * backConfirm   = CCMenuItemSprite::create(sprite_back_normal, sprite_back_select, this, menu_selector(KuiShop::backCallBackFunc));
	backConfirm->setPosition(ccp(-50,25));
	backConfirm->setScaleX(3.0);


	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\����\\������壭��.spr");
	g_StrLower(nSprName);
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	CCSprite * sprite_next_normal = CCSprite::createWithTexture(bgCur);
	//sprite_next_normal->setScaleX(2.0);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	CCSprite * sprite_next_select = CCSprite::createWithTexture(bgCur);
	//sprite_next_select->setScaleX(2.0);
	CCMenuItemSprite * nextConfirm   = CCMenuItemSprite::create(sprite_next_normal, sprite_next_select, this, menu_selector(KuiShop::nextCallBackFunc));
	nextConfirm->setPosition(ccp(50,25));
	nextConfirm->setScaleX(3.0);

	CCPoint bgSpritePoint = ParentNode_ItemEx->getPosition();
	CCMenu * menu = CCMenu::create(buyConfirm,closeConfirm,backConfirm,nextConfirm,NULL);
	menu->setPosition(ccp(m_size.width/2,45));//�˵�λ������Ϊ����������
	ParentNode_ItemEx->addChild(menu);         //�ӵ�������
	if (g_GameWorld)//�����������Ĵ���
		g_GameWorld->setchatDelegate(false);
}
//��һҳ
void KuiShop::backCallBackFunc(CCObject * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen =false;
		ParentNode_ItemEx->removeChildByTag(1001,true);
	}

	int index = m_nCurrentPage - 1;
	if (index<0)
		return;
	SetPage(index);
}
//��һҳ
void KuiShop::nextCallBackFunc(CCObject * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen =false;
		ParentNode_ItemEx->removeChildByTag(1001,true);
	}

	int index = m_nCurrentPage + 1;
	if (index<0 || index >=m_nPageCount)
		return;

	SetPage(index);

	/*if  (nIndex == (m_nPageCount-1))
			//m_NextBtn.Enable(FALSE);
		else if  (nIndex <= 0 )
		{
			//m_PreBtn.Enable(FALSE);
		}
		else if  (nIndex > 0 && nIndex < (m_nPageCount-1))
		{
			//m_PreBtn.Enable(TRUE);
			//m_NextBtn.Enable(TRUE);
		}*/

}
//ִ���ϲ����Ļص��������رյ�����
void KuiShop::buyCallBackFunc(CCObject * pSender)
{
	if  (m_selItemIndex<0)
	{
#ifdef WIN32
		CCMessageBox("��ѡ��һ����Ʒ","��ʾ");
#else
		CCMessageBox(UTEXT("��ѡ��һ����Ʒ",1).c_str(),UTEXT("��ʾ",1).c_str());
#endif
		return;
	}
	//��⹺�������
	int  nCount =1;
	char nTempStr[32];
	if (pstrEditBox_count)
	{
		std::string nTempString = pstrEditBox_count->getText();
		if (nTempString.empty())
			nTempString = "1";
		//����ַ�����
		//��ʼ����ַ��ĺϷ���
		std::string strcheck="1";
		strcheck+=nTempString;
		int count = 0,spaceNum=0,chineseNum=0,other=0,letter=0,xiaoxie=0;
		for(int i = 0; i <strcheck.length();i++)
		{ 
			//isalnum
			if(isdigit(strcheck[i]))     //�ж��Ƿ�������0-9
				count++;
			else if(isspace(strcheck[i]))//�ж��Ƿ��ǿո�
				++spaceNum;
			else if(isalpha(strcheck[i])==1)//�ж��Ƿ�����ĸ	 1 Ϊ��д 2 Ϊ Сд  0 Ϊ������ĸ
			{
				++letter;
			}
			else if(isalpha(strcheck[i])==2)//�ж��Ƿ�����ĸ	 1 Ϊ��д 2 Ϊ Сд  0 Ϊ������ĸ
			{
				++xiaoxie;
			}
			else if(strcheck[i]&0x80)//if(strcheck[0]>=0xB0)
				++chineseNum;        //�ж��Ƿ�������
			else
			{
				++other;
			}
		} 

		if (spaceNum>0 || chineseNum>0 || xiaoxie >0 || other>0 || letter>0)
		{ 	//system("pause");
#ifdef WIN32
			CCMessageBox("ֻ����д����!","����");
#else
			CCMessageBox(G2U("ֻ����д����!").c_str(),G2U("����").c_str());
#endif
			return;
		} 
		t_sprintf(nTempStr,pstrEditBox_count->getText());
		nCount = g_Atoui(nTempStr);
	}
	if  (nCount<=0)
		nCount = 1;

	if  (nCount>150)
	{
		nCount = 150;
		pstrEditBox_count->setText("150");
	}

	KUiDraggedObject* pItem  = (KUiDraggedObject*)&m_pObjsList[m_selItemIndex];
	if (pItem)
	{
		KUiObjAtContRegion	Obj;
		Obj.Obj.uGenre    = pItem->uGenre;
		Obj.Obj.uId       = pItem->uId;
		Obj.Region.h      = pItem->DataX;
		Obj.Region.v      = pItem->DataY;
		Obj.Region.Width  = pItem->DataW;
		Obj.Region.Height = pItem->DataH;
		Obj.eContainer    = UOC_NPC_SHOP;
		if  (g_pCoreShell)
			g_pCoreShell->OperationRequest(GOI_TRADE_NPC_BUY,(unsigned int)(&Obj),nCount);
	}
}

void KuiShop::closeCallBackFunc(CCObject * pSender)
{
	CCNode * node = dynamic_cast<CCNode *>(pSender);
	//node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��
	if (m_callfun &&  m_callbackListener)
	{
		//ִ�е��ò�ص����������ݲ���CCNode
		(m_callbackListener->*m_callfun)(node);
	}

	this->removeFromParentAndCleanup(true);
}

//�رյ�����
void KuiShop::closePopLayer(CCObject * pSender)
{
	this->removeFromParentAndCleanup(true);
}


//��д����ע�ắ�������¸�����������
void KuiShop::registerWithTouchDispatcher()
{
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,128,true);
}

CCRect KuiShop::getRect(CCNode* pNode,int i)
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
bool KuiShop::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	//CCSetIterator it = pTouch->begin();
	//CCTouch* touch = (CCTouch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	CCPoint location = pTouch->getLocation();//Ŀ�ĵ�����?��Ļ�����
	bool isThis=false;
	for (int i = 0; i < m_nCurrentCount; ++i)
	{
		if (m_ItemIndex[i]>=0)
		{
			 int nidx     = m_pObjsList[m_ItemIndex[i]].Obj.uId;
			 int nIndex   = BuySell.GetItemIndex(m_CurShopIdx,nidx);
			 char nItemKey[32];
			 ZeroMemory(&nItemKey,sizeof(nItemKey));
			 t_sprintf(nItemKey,"item_%u",nIndex+1);
			 std::string nKey =nItemKey;
			 CCSprite *nItemSpr = (CCSprite *)ItemNode->getChildByTagKey(nKey);
			 t_sprintf(nItemKey,"color_%d",nIndex+1);
			 nKey =nItemKey;
			 CCLayerColor *ncolorlable = (CCLayerColor *)ItemNode->getChildByTagKey(nKey);
			 if  (nItemSpr && ncolorlable)
			 {
				 if (getRect(ncolorlable,i+1).containsPoint(location))
				 {//������
					 //restartAction();
					 nItemSpr->runAction(red);  
					 m_selItemIndex = m_ItemIndex[i];
					 KUiItemdObject second;
					 second.uId    = m_pObjsList[m_ItemIndex[i]].Obj.uId;
					 second.uGenre = UOC_NPC_SHOP;
					 second.nRect  = getRect(ncolorlable,0);
					// CCLayer * sDescLayer = KuiItemdesc::create("",second,this,callfuncN_selector(KuiShop::closedescPadCallback));
					 CCLayer * sDescLayer = KuiItemdesc::create(m_CurShopIdx,second,this,callfuncN_selector(KuiShop::closedescPadCallback));
					 m_descPadIsOpen = true;
					 isThis = true;
					 this->addChild(sDescLayer,2,1001);
				 }
				 else
				 {
					 if (!isThis)
						 m_selItemIndex = -1;
					 nItemSpr->setColor(ccWHITE);           //Ĭ����ɫ
				 }
			 }
		}
	}
	return true;
}

void KuiShop::closedescPadCallback(CCNode *pNode)
{
	m_descPadIsOpen = false;
}

void KuiShop::update(float delta)
{
	//CCMessageBox("����ѭ��","update");
	if (isOpen && g_pCoreShell)
	{
	   /* m_nMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
		m_nXu    = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_XU, 0, 0);
		char nPointCount[32];
	   if (pMoneyLabel)
	   {
		   t_sprintf(nPointCount,"%d",m_nMoney);
		  // pSkillPointLabel->setColor(ccWit)
		   pMoneyLabel->setString(nPointCount);
	   }
	   if  (pXuLabel)
	   {
		   t_sprintf(nPointCount,"%d",m_nXu);
		   // pSkillPointLabel->setColor(ccWit)
		   pXuLabel->setString(nPointCount);
	   }*/
	}
}

void KuiShop::draw()
{
	//CCMessageBox("����ѭ��","draw");
}
void KuiShop::Clear()
{
	m_nObjCount  = 0;
	m_nPageCount = 0;

	if (m_pObjsList)
	{
		free(m_pObjsList);
		m_pObjsList = NULL;
	}
}


//������Ʒ
int KuiShop::AddObject(KUiDraggedObject* pObject, int nCount)
{
	if (!pObject) return false;


	for (int i = 0; i < nCount; ++i)
	{
		    int nIndex   = BuySell.GetItemIndex(m_CurShopIdx,pObject->uId);
		    KItem* pItem = BuySell.GetItemidx(nIndex);

			if (pItem==NULL) return false;

			KUiDraggedObject no;
			no.uGenre = pObject->uGenre;
			no.uId    = pObject->uId;       
			no.DataX  = pObject->DataX;     //�����е����Xλ�� �ڼ������� 25x25
			no.DataY  = pObject->DataY;     //�����е����Yλ��
			no.DataW  = pObject->DataW;     //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
			no.DataH  = pObject->DataH;     //��Ʒ�Ŀ��(ռ���˶��ٸ�����)
			if (pItem)
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
				if  (pItem->GetImagePath())
					t_sprintf(nItemSprPath,pItem->GetImagePath());
				else
					t_sprintf(nItemSprPath,"\\spr\\others\\�ʺ�.spr");

				if  (nItemSprPath[0])
				{
					//t_sprintf(nItemSprPath,pItem->GetImagePath());
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
					t_sprintf(nItemKey,"item_%u",nIndex+1);
					std::string nKey =nItemKey;
					CCSprite *nItemSpr = (CCSprite *)ItemNode->getChildByTagKey(nKey);
					if (!nItemSpr)
					{
						nItemSpr = CCSprite::createWithTexture(bgCur);
						//nItemSpr->setAnchorPoint(ccp(0,0));
						nItemSpr->setTagbyKey(nKey);
						ItemNode->addChild(nItemSpr,1);

						if  (pItem->GetGenre()!=item_equip && pItem->IsStack())
						{//����װ�� �ɵ�����Ʒ
							char stack[32];
							t_sprintf(stack,"%d",pItem->GetStackNum());
							CCLabelTTF *stuckCountlabel = CCLabelTTF::create(stack,UI_GAME_FONT_DEFAULT,14);
							stuckCountlabel->setColor(ccYELLOW);
							stuckCountlabel->setAnchorPoint(ccp(0,0));
							stuckCountlabel->setPosition(ccp(0,0));
							stuckCountlabel->setTagbyKey("stack");
							nItemSpr->addChild(stuckCountlabel,1);
						}
					}
					//��ʼ������
					/*m_ItemData[nKey].uGenre = pObject->uGenre;
					m_ItemData[nKey].uId    = pObject->uId;
					m_ItemData[nKey].nindex = i;
					m_ItemData[nKey].DataX  = pObject->DataX;     //�����е����Xλ�� �ڼ������� 25x25
					m_ItemData[nKey].DataY  = pObject->DataY;     //�����е����Yλ��
					m_ItemData[nKey].DataW  = pObject->DataW; //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
					m_ItemData[nKey].DataH  = pObject->DataH;//��Ʒ�Ŀ��(ռ���˶��ٸ�����)
					m_ItemData[nKey].nGenkind = UOC_ITEM_TAKE_WITH;  //����Я��
					*/
					ccColor4B color = ccc4(112, 128, 144, 150);//112, 128, 144
					t_sprintf(nItemKey,"color_%d",nIndex+1);
					//strcat(nItemKey,"_color");
					nKey =nItemKey;
					CCLayerColor *bgcolorLayer = (CCLayerColor *)ItemNode->getChildByTagKey(nKey);
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
						ItemNode->addChild(bgcolorLayer,0);
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
					Obj.nContainer = UOC_NPC_SHOP;

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
void KuiShop::SetPage(int nIndex)
{
	if  (ItemNode)
		ParentNode_ItemEx->removeChild(ItemNode,true);

	ItemNode = CCNode::create();
	ItemNode->setAnchorPoint(ccp(0,0));
	ItemNode->setPosition(ccp(0,0));
	ParentNode_ItemEx->addChild(ItemNode,1);

	if (nIndex >= 0 && nIndex < m_nPageCount && m_pObjsList)
	{
		//m_ItemsBox.Clear();
		int nTempCount=0;
		int i;
		for (i = 0; i < PAGE_MAX_COUNT_ITEM; i++)
		{//
			m_ItemIndex[i]=-1;
		}

		for (i = 0; i < m_nObjCount; i++)
		{//��ʼ������Ʒ
			if (m_pObjsList[i].nContainer == nIndex)  //�ڼ�ҳ����Ʒ
			{
				AddObject((KUiDraggedObject*)&m_pObjsList[i], 1);
				m_ItemIndex[nTempCount] = i; //ÿҳ���������
				nTempCount++;
			}
			//	m_ItemsBox.AddObject((KUiDraggedObject*)&m_pObjsList[i], 1);
		}
		m_nCurrentCount = nTempCount;
		m_nCurrentPage  = nIndex;                      //��ǰҳ
		//char nPages[16]={0};
		//t_sprintf(nPages,"%d/%d",m_nCurrentPage+1,m_nPageCount);
		//m_CurPageText.SetText(nPages);

		/*if  (nIndex == (m_nPageCount-1))
			//m_NextBtn.Enable(FALSE);
		else if  (nIndex <= 0 )
		{
			//m_PreBtn.Enable(FALSE);
		}
		else if  (nIndex > 0 && nIndex < (m_nPageCount-1))
		{
			//m_PreBtn.Enable(TRUE);
			//m_NextBtn.Enable(TRUE);
		}*/
	}
}
void KuiShop::UpdateData()
{
	Clear();
	m_nObjCount = g_pCoreShell->GetDataNpcShop(m_CurShopIdx, 0, 0);
	if (m_nObjCount == 0)
		return;

	if (m_pObjsList = (KUiObjAtContRegion*)malloc(sizeof(KUiObjAtContRegion) * m_nObjCount))
	{
		g_pCoreShell->GetDataNpcShop(m_CurShopIdx, (unsigned int)m_pObjsList, m_nObjCount);//���߳�ִ�У�nCountֵ����
		m_nPageCount = m_pObjsList[m_nObjCount - 1].nContainer + 1;
		SetPage(0);
		if (m_nPageCount > 1)
		{
			//m_PreBtn.Enable(TRUE);
			//m_NextBtn.Enable(TRUE);
		}
		//char msg[64];
		//t_sprintf(msg,"����:%d,ҳ:%d,%d",m_nObjCount,m_nPageCount,m_CurShopIdx);
		//CCMessageBox(msg,"�̵�");
	}
	else
		m_nObjCount = 0; 
}