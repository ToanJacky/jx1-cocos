#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiTeam.h"
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

KuiTeam::KuiTeam()
{
	isOpen = false;
	__isCanMove=true;
	m_bScrolling=false;
	//memset(Skills,0,sizeof(KUiSkillData) * 50);
	pMoneyLabel       = NULL;
	colorLayer        = NULL;
	red               = NULL;
	m_pUiLayer = NULL;
	//m_pWidget  = NULL;
	//m_array    = NULL;
	pNearbyPlayerslistView = NULL;
	pPlayersListView       = NULL;
	m_pNearbyPlayersList   = NULL;
	m_pPlayersList       = NULL;
	__nSelIndex          = -1;
	__playrSelIndex      = -1;
	__playerListCount    = 0;
	checkBox =NULL;
}

KuiTeam::~KuiTeam()
{
	isOpen = false;
	pMoneyLabel  = NULL;
	colorLayer   = NULL;
	__nSelIndex  = -1;
	__NearbyListCount = 0;
	if (m_pNearbyPlayersList)
	{
		free(m_pNearbyPlayersList);
		m_pNearbyPlayersList =NULL;
	}

	if (m_pPlayersList)
	{
		free(m_pPlayersList);
		m_pPlayersList =NULL;
	}

	//CC_SAFE_RELEASE(m_array);
	if (red)
	    red->release();

}

//����ȷ��������
KuiTeam * KuiTeam::create(char * title,CCObject * callbackListener,SEL_CallFuncN callfun)
{
	KuiTeam * popLayer = KuiTeam::create();
	popLayer->addDialogData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//��ʼ��
bool KuiTeam::init()
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
	t_sprintf(nSprName,"\\Spr\\Ui3\\���\\���2.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	CCTexture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return false;
	CCSprite * spriteBox = CCSprite::createWithTexture(bgCur);
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

void KuiTeam::addDialogData()
{
	if  (!g_pCoreShell) return;

	btnSize.width = 70;
	btnSize.height= 14;

	pMoneyLabel = CCLabelTTF::create("","fonts/gb_song.ttf", 12);//ͳ˧
	pMoneyLabel->setAnchorPoint(ccp(0,0));
	pMoneyLabel->setPosition(ccp(70,35));
	ParentNode_Team->addChild(pMoneyLabel,1);

	m_pUiLayer = UILayer::create();
	ParentNode_Team->addChild(m_pUiLayer);

	// Create the checkbox
	char nSprName[128]={0};
	int m_nWidth,m_nHeight,nFrams;
	CCTexture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\���\\��ӿ���.spr");
	g_StrLower(nSprName);

	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCTexture2D * check_box_normal = bgCur;

	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCTexture2D * check_box_active = bgCur;

	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCTexture2D * check_box_active_disable = bgCur;
	checkBox = UICheckBox::create();
	checkBox->setTouchEnabled(true);
	checkBox->loadTexturesBydata(check_box_normal,
		check_box_active_disable,
		check_box_active_disable,check_box_normal,check_box_active);
	checkBox->setPosition(ccp(90,13));
	checkBox->addEventListenerCheckBox(this, checkboxselectedeventselector(KuiTeam::selectedBoxStateEvent));
	m_pUiLayer->addWidget(checkBox);
	/*m_pWidget = Layout::create();
	m_pWidget->setBackGroundImage("loadingEnd.png");
	m_pWidget->setBackGroundImageScale9Enabled(true);
	m_pWidget->setBackGroundColorOpacity(105);
	m_pWidget->setContentSize(m_size);
	m_pUiLayer->addWidget(m_pWidget);
	*/
	// create list view data
	/*m_array = CCArray::create();
	CC_SAFE_RETAIN(m_array);
	for (int i = 0; i < 16; ++i)
	{
		CCString* ccstr = CCString::createWithFormat("����б�_%d", i);
		m_array->addObject(ccstr);
	}*/
	//CCSize widgetSize = m_pWidget->getSize();
	// Create the list view

	
	pPlayersListView = UIListView::create();
	// set list view direction
	pPlayersListView->setDirection(SCROLLVIEW_DIR_VERTICAL);
	pPlayersListView->setTouchEnabled(true);
	pPlayersListView->setBounceEnabled(true);
	pPlayersListView->setBackGroundImage("loadingStart.png");
	pPlayersListView->setBackGroundImageScale9Enabled(true);
	//listView->setBackGroundColorOpacity(0);
	pPlayersListView->setBackGroundImageOpacity(50);
	pPlayersListView->setSize(CCSizeMake(110, 110));
	//listView->setInnerContainerSize(CCSizeMake(110, 110));
	pPlayersListView->setPosition(ccp(10,m_size.height/2-50));
	pPlayersListView->addEventListenerListView(this, listvieweventselector(KuiTeam::selectedPlayersListItemEvent));
	m_pUiLayer->addWidget(pPlayersListView);
	UpdateData(NULL);
	pPlayersListView->setGravity(LISTVIEW_GRAVITY_CENTER_VERTICAL);
	pPlayersListView->refreshView();
	pPlayersListView->jumpToBottom();
	//��Χ���
	pNearbyPlayerslistView = UIListView::create();
	// set list view direction
	pNearbyPlayerslistView->setDirection(SCROLLVIEW_DIR_VERTICAL);
	pNearbyPlayerslistView->setTouchEnabled(true);
	pNearbyPlayerslistView->setBounceEnabled(true);
	pNearbyPlayerslistView->setBackGroundImage("loadingStart.png");
	pNearbyPlayerslistView->setBackGroundImageScale9Enabled(true);
	//listView->setBackGroundColorOpacity(0);
	pNearbyPlayerslistView->setBackGroundImageOpacity(50);
	pNearbyPlayerslistView->setSize(CCSizeMake(110, 110));
	//listView->setInnerContainerSize(CCSizeMake(110, 110));
	pNearbyPlayerslistView->setPosition(ccp(m_size.width/2+40,m_size.height/2-50));
	pNearbyPlayerslistView->addEventListenerListView(this, listvieweventselector(KuiTeam::selectedItemEvent));
	m_pUiLayer->addWidget(pNearbyPlayerslistView);
	OnRefresh();
	/*int count = m_array->count();
	// add custom item
	for (int i = 0; i < count/2; ++i)
	{
		UIButton* custom_button = UIButton::create();
		custom_button->setName("TextButton");
		custom_button->setTouchEnabled(true);
		custom_button->loadTextures("ui/state/life_normal.png", "ui/state/life_pk.png", "");
		custom_button->setScale9Enabled(true);
		custom_button->setSize(btnSize);

		Layout *custom_item = Layout::create();
		custom_item->setSize(custom_button->getSize());
		custom_button->setPosition(ccp(custom_item->getSize().width / 2, custom_item->getSize().height / 2));
		custom_item->addChild(custom_button);

		listView->pushBackCustomItem(custom_item);
	}
	// insert custom item
	CCArray *items = listView->getItems();
	int items_count = items->count();
	for (int i = 0; i < items_count; ++i)
	{
		UIWidget *item = listView->getItem(i);
		UIButton *button = static_cast<UIButton*>(item->getChildByName("TextButton"));
		int index = listView->getIndex(item);
		button->setTitleFontSize(12);
		//static_cast<CCString*>(m_array->objectAtIndex(index))->getCString()
		char nTempstr[32];   //static_cast<CCString*>(
		t_sprintf(nTempstr,static_cast<CCString*>(m_array->objectAtIndex(index))->getCString());
		button->setTitleText(UTEXT(nTempstr,1));
	}
	*/
	pNearbyPlayerslistView->setGravity(LISTVIEW_GRAVITY_CENTER_VERTICAL);
	pNearbyPlayerslistView->refreshView();
	pNearbyPlayerslistView->jumpToBottom();

  
	ccColor4B color = ccc4(112, 128, 144, 150);
	colorLayer = CCLayerColor::create(color);//��ɫ��
	colorLayer->setPosition(CCPointZero);
	colorLayer->setContentSize(CCSizeZero);  //���úʹ��ڵĴ�С
	ParentNode_Team->addChild(colorLayer,1000);
	//����һ������ �ı侫����ɫ
	//red = CCTintBy::create(0.2,0,-255,-255);//����ʱ��+��ɫ 
	//red->retain();
}

//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void KuiTeam::setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;
	
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\���\\�ر�.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	CCTexture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));

	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCSprite * sprite_colse_normal = CCSprite::createWithTexture(bgCur);
	sprite_colse_normal->setAnchorPoint(ccp(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCSprite * sprite_colse_select = CCSprite::createWithTexture(bgCur);
	sprite_colse_select->setAnchorPoint(ccp(0,0));

	CCMenuItemSprite * colseConfirm = CCMenuItemSprite::create(sprite_colse_normal, sprite_colse_select, this, menu_selector(KuiTeam::buttonCallBackFunc));
	colseConfirm->setPosition(ccp(177+177/2,m_size.height-207-sprite_colse_select->getContentSize().height/2));
	//�������
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\���\\�������.spr");
	g_StrLower(nSprName);

	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCSprite * sprite_Invite_normal = CCSprite::createWithTexture(bgCur);
	sprite_Invite_normal->setAnchorPoint(ccp(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	CCSprite * sprite_Invite_select = CCSprite::createWithTexture(bgCur);
	sprite_Invite_select->setAnchorPoint(ccp(0,0));

	CCMenuItemSprite * InviteConfirm = CCMenuItemSprite::create(sprite_Invite_normal, sprite_Invite_select, this, menu_selector(KuiTeam::btnCallBackFunc));
	InviteConfirm->setPosition(ccp(m_size.width/2,163));
	InviteConfirm->setTag(1);

	//�߳����� \Spr\Ui3\���\�������.spr
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\���\\�߳�����.spr");
	g_StrLower(nSprName);

	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCSprite * sprite_Kick_normal = CCSprite::createWithTexture(bgCur);
	sprite_Kick_normal->setAnchorPoint(ccp(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	CCSprite * sprite_Kick_select = CCSprite::createWithTexture(bgCur);
	sprite_Kick_select->setAnchorPoint(ccp(0,0));

	CCMenuItemSprite * KickConfirm = CCMenuItemSprite::create(sprite_Kick_normal, sprite_Kick_select, this, menu_selector(KuiTeam::btnCallBackFunc));
	KickConfirm->setPosition(ccp(m_size.width/2,143));
	KickConfirm->setTag(2);
	//�ӳ��ƽ� \Spr\Ui3\���\�ӳ��ƽ�.spr
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\���\\�ӳ��ƽ�.spr");
	g_StrLower(nSprName);

	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCSprite * sprite_Appoint_normal = CCSprite::createWithTexture(bgCur);
	sprite_Appoint_normal->setAnchorPoint(ccp(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	CCSprite * sprite_Appoint_select = CCSprite::createWithTexture(bgCur);
	sprite_Appoint_select->setAnchorPoint(ccp(0,0));

	CCMenuItemSprite * AppointConfirm = CCMenuItemSprite::create(sprite_Appoint_normal, sprite_Appoint_select, this, menu_selector(KuiTeam::btnCallBackFunc));
	AppointConfirm->setPosition(ccp(m_size.width/2,123));
	AppointConfirm->setTag(3);
	//ˢ��
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\���\\ˢ���б�.spr");
	g_StrLower(nSprName);

	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCSprite * sprite_Refresh_normal = CCSprite::createWithTexture(bgCur);
	sprite_Refresh_normal->setAnchorPoint(ccp(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCSprite * sprite_Refresh_select = CCSprite::createWithTexture(bgCur);
	sprite_Refresh_select->setAnchorPoint(ccp(0,0));

	CCMenuItemSprite * RefreshConfirm = CCMenuItemSprite::create(sprite_Refresh_normal, sprite_Refresh_select, this, menu_selector(KuiTeam::btnCallBackFunc));
	RefreshConfirm->setPosition(ccp(m_size.width/2,103));
	RefreshConfirm->setTag(4);

	//�뿪����
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\���\\�뿪����.spr");
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

	CCMenuItemSprite * LeaveConfirm = CCMenuItemSprite::create(sprite_Leave_normal, sprite_Leave_select, this, menu_selector(KuiTeam::btnCallBackFunc));
	LeaveConfirm->setPosition(ccp(m_size.width/2,83));
	LeaveConfirm->setTag(5);
	//����˵�
	CCMenu * menu = CCMenu::create(colseConfirm,InviteConfirm,KickConfirm,AppointConfirm,RefreshConfirm,LeaveConfirm,NULL);
	//menu->setPosition(ccp(bgSpritePoint.x-m_size.width/2,bgSpritePoint.y-m_size.height/2));//�˵�λ������Ϊ����������
	menu->setAnchorPoint(ccp(0,0));
	menu->setPosition(ccp(0,0));             //�˵�λ������Ϊ����������
	ParentNode_Team->addChild(menu);         //�ӵ�������

	KUiPlayerTeam mTeam;
	ZeroMemory(&mTeam,sizeof(mTeam));
	if (g_pCoreShell->TeamOperation(TEAM_OI_GD_INFO, (unsigned int)&mTeam, 0)) //û�ж��� �ʹ���һ֧����
	{//�ж���
		if (mTeam.bTeamLeader)
		{//�ӳ�
			//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("��ʾ:�Ѿ��Ƕӳ�,����ѡ���Ա��");
			KickConfirm->setEnabled(true);
			AppointConfirm->setEnabled(true);
			LeaveConfirm->setEnabled(true);
		}
		else
		{//��Ա
			LeaveConfirm->setEnabled(true);
			KickConfirm->setEnabled(false);
			AppointConfirm->setEnabled(false);
		}
	}
	else
	{//û�ж���
		KickConfirm->setEnabled(false);
		AppointConfirm->setEnabled(false);
		LeaveConfirm->setEnabled(false);
	}
}

void KuiTeam::btnCallBackFunc(CCObject * pSender)
{
	if (!g_pCoreShell|| !g_GameWorld || !pNearbyPlayerslistView) return;

	CCNode * node = NULL;
	if (pSender)
	{
		node= dynamic_cast<CCNode *>(pSender);
		switch(node->getTag())
		{
		case 1:
			{//�������
				if (__nSelIndex >= 0 && __nSelIndex<__NearbyListCount)
				{		
					if (m_Info.cNumMember == 0)
						g_pCoreShell->TeamOperation(TEAM_OI_CREATE, 0, 0);

					g_pCoreShell->TeamOperation(TEAM_OI_INVITE,	(unsigned int)(&m_pNearbyPlayersList[__nSelIndex]),0);
				}
			}
			break;
		case 2:
			{//�߳�����
				if (m_pPlayersList && __playrSelIndex >= 0 && __playrSelIndex<__playerListCount)	
					g_pCoreShell->TeamOperation(TEAM_OI_KICK, (unsigned int)&m_pPlayersList[__playrSelIndex],0);
			}
			break;
		case 3:
			{//�ӳ��ƽ�
				if (m_pPlayersList && __playrSelIndex >= 0 && __playrSelIndex<__playerListCount)	
					g_pCoreShell->TeamOperation(TEAM_OI_APPOINT, (unsigned int)&m_pPlayersList[__playrSelIndex], 0);
			}
			break;
		case 4:
			{//ˢ��
               pNearbyPlayerslistView->removeAllItems();
			   pNearbyPlayerslistView->setPosition(ccp(m_size.width/2+40,m_size.height/2-50));
			   OnRefresh();
			   pNearbyPlayerslistView->setGravity(LISTVIEW_GRAVITY_CENTER_VERTICAL);//�ж���
			   pNearbyPlayerslistView->refreshView();
			}
			break;
		case 5:
			{//�˳�����
				g_pCoreShell->TeamOperation(TEAM_OI_LEAVE, 0, 0);
				break;

			   pNearbyPlayerslistView->removeItem(0);//ɾ����һ��
			   //listView->sortAllChildren();
			   pNearbyPlayerslistView->refreshView();
			   //CCArray *items = listView->getItems();
			   //int items_count = items->count();
			   CCSize btnNewSize;
			   btnNewSize.width = 140;
			   btnNewSize.height= 28;

			   UIButton* custom_button = UIButton::create();
			   custom_button->setName("TextButton");
			   custom_button->setTouchEnabled(true);
			   custom_button->loadTextures("ui/state/life_normal.png", "ui/state/life_pk.png", "");
			   custom_button->setScale9Enabled(true);
			   custom_button->setSize(btnNewSize);

			   Layout *custom_item = Layout::create();
			   custom_item->setSize(btnNewSize);
			   custom_button->setPosition(ccp(btnNewSize.width / 2, btnNewSize.height / 2));
			   custom_item->addChild(custom_button);
			   pNearbyPlayerslistView->pushBackCustomItem(custom_item);
			   custom_button->setTitleText(UTEXT("���Բ���",1));

			   pNearbyPlayerslistView->refreshView();
			   pNearbyPlayerslistView->jumpToBottom();
			}
			break;
		default:
			break;
		}
	}
	// remove last item
	// remove item by index
	//items_count = items->count();
	//listView->removeItem(items_count - 1);
}
//ִ���ϲ����Ļص��������رյ�����
void KuiTeam::buttonCallBackFunc(CCObject * pSender)
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
void KuiTeam::closePopLayer(CCObject * pSender)
{
	this->removeFromParentAndCleanup(true);
}

void KuiTeam::selectedBoxStateEvent(CCObject* pSender, CheckBoxEventType type)
{
	if  (!g_pCoreShell || !g_GameWorld) return;
	switch (type)
	{
	case CHECKBOX_STATE_EVENT_UNSELECTED:
		{
		if (!g_pCoreShell->TeamOperation(TEAM_OI_CLOSE, 0,0))
		    break;

		if  (checkBox)
			checkBox->setSelectedState(false);
		}
		break;
	case CHECKBOX_STATE_EVENT_SELECTED:
		//m_pDisplayValueLabel->setText(CCString::createWithFormat("Selected")->getCString());
		//CCMessageBox("ѡ��","ѡ��");
		if (!g_pCoreShell->TeamOperation(TEAM_OI_CLOSE,0,1))
		{//��ʧ��
		   if  (checkBox)
			   checkBox->setSelectedState(false);
		}
		break;
	default:
		break;
	}
}
void KuiTeam::selectedPlayersListItemEvent(CCObject *pSender, ListViewEventType type)
{
	char msg[64];
	switch (type)
	{
	case LISTVIEW_ONSELECTEDITEM_START:
		{
			UIListView* listViewEx = static_cast<UIListView*>(pSender);
			if (listViewEx)
			   __playrSelIndex = listViewEx->getCurSelectedIndex();
				//CCLOG("select child start index = %d", listViewEx->getCurSelectedIndex());
			break;
		}
	case LISTVIEW_ONSELECTEDITEM_END:
		break;
	default:
		break;
	}
}
void KuiTeam::selectedItemEvent(CCObject *pSender, ListViewEventType type)
{
	switch (type)
	{
	case LISTVIEW_ONSELECTEDITEM_START:
		{
			UIListView* listViewEx = static_cast<UIListView*>(pSender);
			if (listViewEx)
				__nSelIndex = listViewEx->getCurSelectedIndex();
				//CCLOG("select child start index = %d", listViewEx->getCurSelectedIndex());
			break;
		}
	case LISTVIEW_ONSELECTEDITEM_END:
		{
			break;
		}
	default:
		break;
	}

	//CCMessageBox(msg,"����¼�");
}


//��д����ע�ắ�������¸�����������
void KuiTeam::registerWithTouchDispatcher()
{
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,128, true);
}

CCRect KuiTeam::getRect(CCNode* pNode,int i)
{
	CCRect  rc;
	rc.origin     = pNode->getPosition(); //��������ͼ��ƫ��
	//visibleSize
	rc.size       = pNode->getContentSize();

	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //��������
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //���ϱ�    //- rc.origin.y
	
	return rc;
}

void KuiTeam::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if (!m_bScrolling) return;
	m_bScrolling = false;

	//__loopTime = MAX_LOOP_TIME;
}

//�ƶ���
void KuiTeam::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if (!m_bScrolling || !__isCanMove) return;
	CCPoint point   = ParentNode_Team->convertToNodeSpace(CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView()));
	CCPoint diff    = ccpSub(point,m_lastPoint);                  //����ͼƬ��λ��
	ParentNode_Team->setPosition(ccpAdd(ParentNode_Team->getPosition(),diff));
	m_origin      = ParentNode_Team->getPosition();
	m_lastPoint = point;
}
//��������ccTouchBegan������true
bool KuiTeam::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint point = ParentNode_Team->convertToNodeSpace(CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView()));
	CCRect rect = CCRectMake(0, 0, winSize.width/*ParentNode_Team->getContentSize().width*/,winSize.height/* ParentNode_Team->getContentSize().height*/);
	m_bScrolling = rect.containsPoint(point); //����ڷ�Χ�� �Ϳ����϶�
	m_lastPoint = point;
	return true;
	//CCSetIterator it = pTouch->begin();
	//CCTouch* touch = (CCTouch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	/*CCPoint location = pTouch->getLocation();//Ŀ�ĵ�����?��Ļ�����
	CCSprite * SkillNode =NULL;
	for (int i = 0; i < FIGHT_SKILL_COUNT_PER_PAGE; ++i)
	{
		SkillNode = (CCSprite *)ParentNode_Team->getChildByTag(i+1);
		if  (SkillNode)
		{
			if (getRect(SkillNode,i+1).containsPoint(location))
			{//������
				SkillNode->runAction(red);  
			}
			else
			{
				SkillNode->setColor(ccWHITE);           //Ĭ����ɫ
			}
		}
	}
	return true;*/
}

void KuiTeam::OnRefresh()
{
	if (!g_pCoreShell || !g_GameWorld ||!pNearbyPlayerslistView) return;
	//��ʼ��Ӹ�������б�
	if (m_pNearbyPlayersList)
	{
		free(m_pNearbyPlayersList);
		m_pNearbyPlayersList = NULL;
	}

	int nCount = g_pCoreShell->GetGameData(GDI_NEARBY_IDLE_PLAYER_LIST, 0, 0);  //��ȡ��������
	__NearbyListCount = nCount;
	if (nCount)
	{
		m_pNearbyPlayersList = (KUiPlayerItem*)malloc(sizeof(KUiPlayerItem) * nCount);
		if (m_pNearbyPlayersList)
		{
			g_pCoreShell->GetGameData(GDI_NEARBY_IDLE_PLAYER_LIST, (unsigned int)m_pNearbyPlayersList,nCount);
			for (int i = 0; i < nCount; ++i)
			{
				UIButton* custom_button = UIButton::create();
				custom_button->setName("TextButton");
				custom_button->setTouchEnabled(true);
				//_button_2->loadTextures("ui/textbg.png", "ui/textbg.png", "");
				custom_button->loadTextures("ui/textbg.png", "ui/state/life_pk.png","");
				custom_button->setScale9Enabled(true);
				custom_button->setSize(btnSize);
				custom_button->setTitleFontSize(12);
				char nTempstr[32];
				t_sprintf(nTempstr,m_pNearbyPlayersList[i].Name);
				custom_button->setTitleText(UTEXT(nTempstr,1));

				Layout *custom_item = Layout::create();
				custom_item->setSize(btnSize);
				custom_button->setPosition(ccp(btnSize.width/2, btnSize.height/2));
				custom_item->addChild(custom_button);
				custom_item->setTag(m_pNearbyPlayersList[i].nIndex);//NpcIndex
				pNearbyPlayerslistView->pushBackCustomItem(custom_item);
				//m_pNearbyPlayersList++;
			}
		}
	}
	else
	{
		UIButton* custom_button = UIButton::create();
		custom_button->setName("TextButton");
		custom_button->setTouchEnabled(true);
		custom_button->loadTextures("ui/textbg.png", "ui/state/life_pk.png", "");
		custom_button->setScale9Enabled(true);
		custom_button->setSize(btnSize);
		custom_button->setTitleFontSize(12);
		char nTempstr[32];
		t_sprintf(nTempstr,"��������ڸ���");
		custom_button->setTitleText(UTEXT(nTempstr,1));

		Layout *custom_item = Layout::create();
		custom_item->setSize(btnSize);
		custom_button->setPosition(ccp(btnSize.width/2, btnSize.height/2));
		custom_item->addChild(custom_button);
		//custom_item->setTag(m_pNearbyPlayersList->nIndex);//NpcIndex
		pNearbyPlayerslistView->pushBackCustomItem(custom_item);
	}
}

//--------------------------------------------------------------------------
//	���ܣ����¶�����Ϣ
//--------------------------------------------------------------------------
void KuiTeam::UpdateData(KUiPlayerTeam* pInfo)
{
	if (!g_pCoreShell || !g_GameWorld) return;
	Clear();
	KUiPlayerLeaderShip	LeaderShip;
	memset(&LeaderShip, 0, sizeof(KUiPlayerLeaderShip));
	g_pCoreShell->GetGameData(GDI_PLAYER_LEADERSHIP,(unsigned int)&LeaderShip, 0);  //����ͳ˧������ص�����
	char msg[64];
	t_sprintf(msg,"%d",LeaderShip.nLeaderShipLevel);
	pMoneyLabel->setString(msg);//m_LeaderAbility.SetIntText(LeaderShip.nLeaderShipLevel);  //ͳ˧

	if (pInfo)
	{
		m_Info = *pInfo;
	}
	else if (!g_pCoreShell->TeamOperation(TEAM_OI_GD_INFO, (unsigned int)&m_Info, 0))//�������ڵĶ�����Ϣ
		memset(&m_Info,0,sizeof(m_Info));

	if (m_Info.bTeamLeader)  //����Ƕӳ�
	{//��ť����
		if  (checkBox && m_Info.bOpened)
			checkBox->setSelectedState(true);

	}
	else
	{
		if  (checkBox)
			checkBox->setSelectedState(false);
	}

	if (m_Info.cNumMember > 0)
	{//����ж���
		m_pPlayersList = (KUiPlayerItem*)malloc(sizeof(KUiPlayerItem) * (m_Info.cNumMember));
		if (m_pPlayersList)
		{
			int nCount = g_pCoreShell->TeamOperation(TEAM_OI_GD_MEMBER_LIST,(unsigned int)m_pPlayersList, m_Info.cNumMember);
			if (nCount == m_Info.cNumMember)
			{//��ʼ�����б�����
                __playerListCount =nCount;
				for (int i = 0; i < nCount; ++i)
				{
					UIButton* custom_button = UIButton::create();
					custom_button->setName("TextButton");
					custom_button->setTouchEnabled(true);
					custom_button->loadTextures("ui/textbg.png", "ui/state/life_pk.png","");
					custom_button->setScale9Enabled(true);
					custom_button->setSize(btnSize);
					custom_button->setTitleFontSize(12);
					char nTempstr[32];
					t_sprintf(nTempstr,m_pPlayersList[i].Name);
					custom_button->setTitleText(UTEXT(nTempstr,1));

					Layout *custom_item = Layout::create();
					custom_item->setSize(btnSize);
					custom_button->setPosition(ccp(btnSize.width/2, btnSize.height/2));
					custom_item->addChild(custom_button);
					custom_item->setTag(m_pPlayersList[i].nIndex);//NpcIndex
					pPlayersListView->pushBackCustomItem(custom_item);
				}
			}
		}
		else
		{
			m_Info.cNumMember = 0;
			__playerListCount = 0;
		}
	}
	OnRefresh();
}

void KuiTeam::Clear()
{
   //���ð�ť״̬
	memset(&m_Info, 0, sizeof(KUiPlayerTeam));
	if (m_pPlayersList)
	{
		free(m_pPlayersList);
		m_pPlayersList = NULL;
	}

	if (m_pNearbyPlayersList)
	{
		free(m_pNearbyPlayersList);
		m_pNearbyPlayersList = NULL;
	}

	if (pPlayersListView)
	{
		pPlayersListView->removeAllItems();
		pPlayersListView->refreshView();
	}
	if (pNearbyPlayerslistView)
	{
		pNearbyPlayerslistView->removeAllItems();
		pNearbyPlayerslistView->refreshView();
	}
}
void KuiTeam::update(float delta)
{
	//CCMessageBox("����ѭ��","update");
	if (isOpen && g_pCoreShell)
	{
	}
}

void KuiTeam::draw()
{
	//CCMessageBox("����ѭ��","draw");
}