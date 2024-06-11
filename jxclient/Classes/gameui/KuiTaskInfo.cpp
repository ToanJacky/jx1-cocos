#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiTaskInfo.h"
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
KuiTaskInfo::KuiTaskInfo()
{
	isOpen = false;
	__isCanMove=true;
	m_bScrolling=false;
	pmainListView = NULL;
	ptaskMsglistView = NULL;
	colorLayer    = NULL;
	red           = NULL;
	m_pMainUiLayer = NULL;
	__nSelIndex    = -1;
	__nCount     = 0;
	//ZeroMemory(nFindPath,sizeof(nFindPath));
	nFindPath.clear();
}

KuiTaskInfo::~KuiTaskInfo()
{
	m_IndexIDIni.Clear();
	m_IndexData.Clear();
	nFindPath.clear();
	isOpen = false;
	colorLayer   = NULL;
	//CC_SAFE_RELEASE(m_array);
	if (red)
	    red->release();

}

//����ȷ��������
KuiTaskInfo * KuiTaskInfo::create(char * title,CCObject * callbackListener,SEL_CallFuncN callfun)
{
	KuiTaskInfo * popLayer = KuiTaskInfo::create();
	popLayer->addDialogData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//��ʼ��
bool KuiTaskInfo::init()
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
	t_sprintf(nSprName,"\\Spr\\Ui3\\����\\���Ӱ���2.spr");
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
	this->setTouchMode(kCCTouchesOneByOne); //��ֹ��㴥��
	return true;
}

void KuiTaskInfo::addDialogData()
{
	if  (!g_pCoreShell) return;

	/*pMoneyLabel = CCLabelTTF::create("","fonts/gb_song.ttf", 12);//ͳ˧
	pMoneyLabel->setAnchorPoint(ccp(0,0));
	pMoneyLabel->setPosition(ccp(70,35));
	ParentNode_Task->addChild(pMoneyLabel,1);
*/
	m_pMainUiLayer = UILayer::create();
	ParentNode_Task->addChild(m_pMainUiLayer);

	m_IndexIDIni.Load("\\Ui\\uitasklist.ini");
	AddTaskIndex();
	// Create the checkbox
	char nSprName[128]={0};
	int m_nWidth,m_nHeight,nFrams;
	CCTexture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\���\\��ӿ���.spr");
	g_StrLower(nSprName);
  

	pmainListView = UIListView::create();
	// set list view direction
	pmainListView->setDirection(SCROLLVIEW_DIR_VERTICAL);
	pmainListView->setTouchEnabled(true);
	pmainListView->setBounceEnabled(true);
	pmainListView->setBackGroundImage("loadingStart.png");
	pmainListView->setBackGroundImageScale9Enabled(true);
	//listView->setBackGroundColorOpacity(0);
	pmainListView->setBackGroundImageOpacity(50);
	pmainListView->setSize(CCSizeMake(120, 250));
	//listView->setInnerContainerSize(CCSizeMake(110, 110));
	pmainListView->setPosition(ccp(20,26));
	pmainListView->addEventListenerListView(this, listvieweventselector(KuiTaskInfo::selectedMainListItemEvent));
	m_pMainUiLayer->addWidget(pmainListView);
	pmainListView->setGravity(LISTVIEW_GRAVITY_CENTER_HORIZONTAL);

	btnMsgSize.width  =245;
    btnMsgSize.height =16;

	ptaskMsglistView = UIListView::create();
	// set list view direction
	ptaskMsglistView->setDirection(SCROLLVIEW_DIR_VERTICAL);
	ptaskMsglistView->setTouchEnabled(true);
	ptaskMsglistView->setBounceEnabled(true);
	ptaskMsglistView->setBackGroundImage("loadingStart.png");
	ptaskMsglistView->setBackGroundImageScale9Enabled(true);
	//listView->setBackGroundColorOpacity(0);
	ptaskMsglistView->setBackGroundImageOpacity(50);
	ptaskMsglistView->setSize(CCSizeMake(265, 250));
	//listView->setInnerContainerSize(CCSizeMake(110, 110));
	ptaskMsglistView->setPosition(ccp(150,26));
	ptaskMsglistView->addEventListenerListView(this, listvieweventselector(KuiTaskInfo::selectedMsgListItemEvent));
	m_pMainUiLayer->addWidget(ptaskMsglistView);
	ptaskMsglistView->setGravity(LISTVIEW_GRAVITY_CENTER_HORIZONTAL);

	btnNewSize.width = 100; //120
	btnNewSize.height= 28;
	btnCurSize.width = 100;
	btnCurSize.height= 16;
	/*UIButton* custom_button = UIButton::create();
	custom_button->setName("TextButton");
	custom_button->setTouchEnabled(true);
	custom_button->loadTextures("ui/state/life_normal.png", "ui/state/life_pk.png", "");
	custom_button->setScale9Enabled(true);
	custom_button->setSize(btnNewSize);

	Layout *custom_item = Layout::create();
	custom_item->setSize(btnNewSize);
	custom_button->setPosition(ccp(btnNewSize.width/2,btnNewSize.height/2));
	custom_item->addChild(custom_button);
	pmainListView->pushBackCustomItem(custom_item);
	custom_button->setTitleText(UTEXT("���Բ��Բ���",1));
	*/
	for (int i=0;i<__nCount;i++)
	{
		KTaskIndexNode *SelNode = m_IndexData.GetItem(i);
		if (!SelNode) continue;

		if (SelNode->nTaskId==-1)
		{//�Ǹ��ڵ�
			UIButton* custom_button = UIButton::create();
			custom_button->setName("TextButton");
			custom_button->setTouchEnabled(true);
			custom_button->loadTextures("ui/textbg_1.png", "ui/state/life_pk.png", "");
			custom_button->setScale9Enabled(true);
			custom_button->setSize(btnNewSize);

			Layout *custom_item = Layout::create();
			custom_item->setSize(btnNewSize);
			custom_button->setPosition(ccp(btnNewSize.width/2,btnNewSize.height/2));
			custom_item->addChild(custom_button);
			pmainListView->pushBackCustomItem(custom_item);
			custom_button->setTag(i+1);
			custom_button->setTagbyKey("-1");//�Ǹ��ڵ�
			custom_button->setUserData(SelNode);
			//pmainListView->insertCustomItem(custom_item,SelNode->nIndex+1);
			char nTempInfo[128];
			t_sprintf(nTempInfo,"%s",SelNode->szMsg);
			custom_button->setTitleText(UTEXT(nTempInfo,1));
			custom_button->setTitleColor(ccYELLOW);
		}
	}
	pmainListView->refreshView();
	pmainListView->jumpToBottom();

	/*ccColor4B color = ccc4(112, 128, 144, 150);
	colorLayer = CCLayerColor::create(color);//��ɫ��
	colorLayer->setPosition(CCPointZero);
	colorLayer->setContentSize(CCSizeZero);  //���úʹ��ڵĴ�С
	ParentNode_Task->addChild(colorLayer,1000);
	*/
	//����һ������ �ı侫����ɫ
	//red = CCTintBy::create(0.2,0,-255,-255);//����ʱ��+��ɫ 
	//red->retain();
}

void KuiTaskInfo::addmsgInfo(char *nMsgInfo,int nIsBtn,int nMsgIndex)
{
	if  (!ptaskMsglistView || !nMsgInfo) return;
	char nTempInfo[512];
	t_sprintf(nTempInfo,"%s",nMsgInfo);


	/*ccFontDefinition strokeTextDef;
	strokeTextDef.m_fontSize = 12;
	strokeTextDef.m_fontName = std::string(UI_GAME_FONT_SONG);//UI_GAME_FONT_DEFAULT
	strokeTextDef.m_stroke.m_strokeEnabled = true;
	strokeTextDef.m_stroke.m_strokeColor   = ccBLACK;
	strokeTextDef.m_stroke.m_strokeSize    = 1.5;
	strokeTextDef.m_fontFillColor          = ccWHITE;
	strokeTextDef.m_alignment              = kCCTextAlignmentCenter;
	// stroke only label
	CCLabelTTF* custom_button = CCLabelTTF::createWithFontDefinition(UTEXT(nTempInfo,1).c_str(),strokeTextDef);
	*/
	//CCLabelTTF* custom_button = CCLabelTTF::create(UTEXT(nTempInfo,1).c_str(),UI_GAME_FONT_DEFAULT,12);
	//custom_button->setTagbyKey("TextLabel");
	//custom_button->setTouchEnabled(true);
	//custom_button->loadTextures("ui/textbg_1.png", "ui/state/life_pk.png", "");
	//custom_button->setScale9Enabled(true);
	//custom_button->setSize(btnMsgSize);


	CCSize nRenderSize;
	nRenderSize.width  = btnMsgSize.width-5;
	nRenderSize.height = 12;
	RichText  * chatRickText = RichText::create();//WithParentSize(nParentSize);
	chatRickText->setSize(nRenderSize);
	chatRickText->ignoreContentAdaptWithSize(false);//�Ƿ����������Ӧ�߿��  ��һ��
	chatRickText->setAnchorPoint(ccp(0,0));

	RichElementText* nCurMsg = RichElementText::create(1,ccYELLOW,255,UTEXT(nTempInfo,1).c_str(),UI_GAME_FONT_DEFAULT,12); 
	chatRickText->pushBackElement(nCurMsg);

	nRenderSize = chatRickText->getRenderSize();
	chatRickText->setSize(nRenderSize);

	Layout *custom_item = Layout::create();
	custom_item->setSize(nRenderSize);
	//custom_button->setPosition(ccp(btnMsgSize.width/2,btnMsgSize.height/2));
	chatRickText->setPosition(ccp(0,nRenderSize.height-20));
	custom_item->addChild(chatRickText,2,2);

	if (nIsBtn)
	{//�ǰ�Ť
		UIButton* custom_button = UIButton::create();
		custom_button->setName("TextButton");
		//custom_button->setItemDwidx(nItemDwidx);
		//custom_button->setItemName("");
		custom_button->loadTextures("ui/textbg.png", "ui/state/life_pk.png","");
		custom_button->setOpacity(0); //255Ϊ��ȫ��͸�� 0Ϊ��ȫ͸��
		custom_button->setAnchorPoint(ccp(0,0));
		custom_button->setScale9Enabled(true);
		CCSize nBtnSize;
		nBtnSize.width = nRenderSize.width - 50;
		nBtnSize.height= nRenderSize.height;
		custom_button->setSize(nBtnSize);
		custom_button->setTitleFontSize(12);

		//һ�еľͶ� ���г������� 
		int nRowS = chatRickText->getRenderRows();
		if  (nRowS>1)
		{
			float nh = nRenderSize.height/nRowS; //ÿ�еĸ߶�
			custom_button->setPosition(ccp(0,chatRickText->getPosition().y-3-(nRowS-1)*nh));
		}
		else
			custom_button->setPosition(ccp(0,chatRickText->getPosition().y-3));

		//��ʼ������
		//nFindPath[nMsgIndex]
		if (nMsgIndex>=0 && nMsgIndex < MAX_TASK_MESSAGE_MUM)
		     custom_button->setUserData(&nFindPath[nMsgIndex]);
		
		custom_item->addChild(custom_button,1,1);
	}
	
	ptaskMsglistView->pushBackCustomItem(custom_item);
	ptaskMsglistView->refreshView();
	ptaskMsglistView->jumpToBottom();

}

void KuiTaskInfo::addmsgBtnInfo(KTaskPathInfo* pMsg)
{
	if  (!ptaskMsglistView || !pMsg) return;

	int nCurMsgidex=pMsg->nMsgIndex;

	if (nCurMsgidex>-1 && nCurMsgidex < MAX_TASK_MESSAGE_MUM)
	{
		nFindPath[nCurMsgidex].nMsgidx=pMsg->nMsgIndex;
		nFindPath[nCurMsgidex].nTsakType=pMsg->nTaskType;
		nFindPath[nCurMsgidex].nMapIdx=pMsg->nMapIdx;
		nFindPath[nCurMsgidex].nXpos=pMsg->nMapXpos;
		nFindPath[nCurMsgidex].nYpos=pMsg->nMapYpos;
	}
	else
		CCMessageBox("�������������,����ϵGM����!","����");

	/*UIButton* custom_button = UIButton::create();
	custom_button->setName("TextButton");
	custom_button->setTouchEnabled(true);
	custom_button->loadTextures("ui/textbg_1.png", "ui/state/life_pk.png", "");
	custom_button->setScale9Enabled(true);
	custom_button->setSize(btnMsgSize);

	Layout *custom_item = Layout::create();
	custom_item->setSize(btnMsgSize);
	custom_button->setPosition(ccp(btnMsgSize.width/2,btnMsgSize.height/2));
	custom_item->addChild(custom_button);
	pmainListView->pushBackCustomItem(custom_item);
	//custom_button->setTag(i+1);
	//custom_button->setTagbyKey("-1");//�Ǹ��ڵ�
	//custom_button->setUserData(SelNode);
	//pmainListView->insertCustomItem(custom_item,SelNode->nIndex+1);
	char nTempInfo[128];
	t_sprintf(nTempInfo,"%s",nMsgInfo);
	custom_button->setTitleText(UTEXT(nTempInfo,1));
	custom_button->setTitleColor(ccYELLOW);
	*/

}

void KuiTaskInfo::AddTaskIndex()
{
	//KRankIndex  *pIncome;
	KTaskIndexNode Node;
	char szSec[32]={0};
	int i, nValue;
	//if(!m_IndexIDIni.IsSectionExist("Main"))
	__nCount =m_IndexIDIni.GetSectionCount();

	for(i = 0;i < __nCount;i++)
	{
		//memcpy(&Node, pIncome, sizeof(KRankIndex));
		Node.nIndex=i;
		t_sprintf(szSec, "%d", i);
		m_IndexIDIni.GetString(szSec, "Name","", Node.szMsg,MAX_TASK_MESSAGE_LENGTH);

		Node.nMsgLen = strlen(Node.szMsg);

		m_IndexIDIni.GetString(szSec, "Unit","", Node.szUnit, sizeof(Node.szUnit));
		m_IndexIDIni.GetInteger(szSec,"TaskId", -1, &nValue);
		Node.nTaskId = nValue;
		m_IndexIDIni.GetInteger(szSec,"Type", 0, &nValue);
		Node.nType = nValue;
		m_IndexIDIni.GetInteger(szSec,"Class", 0, &nValue);
		Node.nClass = nValue;
		m_IndexIDIni.GetInteger(szSec,"Trace", 0, &nValue);
		Node.nTrace = nValue;
		m_IndexIDIni.GetInteger(szSec,"Count", 0, &nValue);
		Node.nCount = nValue;
		m_IndexData.AddItem(Node);
	}
	//m_bIsIndexDataChange = TRUE;

}

void KuiTaskInfo::removeitems()
{
	if  (!pmainListView) return;
	CCArray *items = pmainListView->getItems();
	if  (!items)  return;

	int items_count = items->count();
	for (int i=0;i<items_count;i++)
	{
		UIWidget *item = pmainListView->getItem(i);
		if  (item)
		{
			UIButton *button = static_cast<UIButton*>(item->getChildByName("TextButton"));
			if  (button)
			{
				KTaskIndexNode *nCurData = (KTaskIndexNode *)button->getUserData();
				if (nCurData && nCurData->nTaskId!=-1)
				    pmainListView->removeItem(i);

				if (nCurData && nCurData->nTaskId==-1)
				{
                   if  (button->getCustomState())
				       button->setCustomState(false);
				}

				if (!nCurData)
					pmainListView->removeItem(i);
			}
			else
				pmainListView->removeItem(i);
		}
		pmainListView->refreshView();
	}
}

void KuiTaskInfo::selectedMsgListItemEvent(CCObject *pSender, ListViewEventType type)
{
	switch (type)
	{
	case LISTVIEW_ONSELECTEDITEM_START:
		{
			UIListView* listViewEx = static_cast<UIListView*>(pSender);
			if (listViewEx)
			{
				int nSelIndex_ = listViewEx->getCurSelectedIndex();

				UIWidget *item = listViewEx->getItem(nSelIndex_);
				if  (item)
				{
					UIButton *button = static_cast<UIButton*>(item->getChildByName("TextButton"));
					if  (button)
					{
						nNewTaskFindPath * nCurPtr =(nNewTaskFindPath *)button->getUserData();
						if (nCurPtr)
						{//��ʼ����
							if (nCurPtr->nMapIdx>0)
							{
								if (g_pCoreShell)
								{
									KUiTongYong nPos;
									ZeroMemory(&nPos,sizeof(nPos));
									nPos.inKind=1;
									nPos.inParma=nCurPtr->nMapIdx;
									nPos.inParmb=nCurPtr->nXpos;
									nPos.inParmc=nCurPtr->nYpos;
									nPos.inParmd=0;
									if (g_pCoreShell)
									   g_pCoreShell->OperationRequest(GOI_PLAYER_SYSSHOP,(unsigned int)&nPos,0);
								}
							}
						}
					}
				}
			}
		}
	    break;
	case LISTVIEW_ONSELECTEDITEM_END:
		break;
	default:
		break;
	}
}

void KuiTaskInfo::selectedMainListItemEvent(CCObject *pSender, ListViewEventType type)
{
	char msg[64];
	switch (type)
	{
	case LISTVIEW_ONSELECTEDITEM_START:
		{
			if (ptaskMsglistView)
				ptaskMsglistView->removeAllItems();

			UIListView* listViewEx = static_cast<UIListView*>(pSender);
			if (listViewEx)
			{
				__nSelIndex = listViewEx->getCurSelectedIndex();
				
				UIWidget *item = listViewEx->getItem(__nSelIndex);
				if  (item)
				{
					UIButton *button = static_cast<UIButton*>(item->getChildByName("TextButton"));
					if  (button)
					{
						KTaskIndexNode *nCurData = (KTaskIndexNode *)button->getUserData();
						if (nCurData)
						{
							if (nCurData->nTaskId==-1)
							{
								/*char mTemp[64];
								t_sprintf(mTemp,"%d",nCurData->nIndex);
								CCMessageBox(mTemp,"test");*/
								if (nCurData->nCount>0 && !button->getCustomState())
								{
									for (int i=0;i<nCurData->nCount;i++)
									{
										KTaskIndexNode *SelNode = m_IndexData.GetItem(nCurData->nIndex+1+i);
										if (!SelNode) continue;
										UIButton* custom_button = UIButton::create();
										custom_button->setName("TextButton");
										custom_button->setTouchEnabled(true);
										custom_button->loadTextures("ui/textbg_1.png", "ui/state/life_pk.png", "");
										custom_button->setScale9Enabled(true);
										custom_button->setSize(btnCurSize);

										Layout *custom_item = Layout::create();
										custom_item->setSize(btnCurSize);
										custom_button->setPosition(ccp(btnCurSize.width/2,btnCurSize.height/2));
										custom_item->addChild(custom_button);
										//custom_item->setPositionX(btnNewSize.width/2);
										//pmainListView->pushBackCustomItem(custom_item);
										//custom_button->setTag(i+1);
										//custom_button->setTagbyKey("-1");//�Ǹ��ڵ�
										custom_button->setUserData(SelNode);
										pmainListView->insertCustomItem(custom_item,__nSelIndex+1+i);
										char nTempInfo[128];
										t_sprintf(nTempInfo,"%s",SelNode->szMsg);
										custom_button->setTitleText(UTEXT(nTempInfo,1));
										custom_button->setTitleColor(ccWHITE);
									}
									pmainListView->refreshView();
									button->setCustomState(true);
								}
								else
								{	//����Ǵ򿪵� �ڶ��ΰ��� ��ɾ��						
									removeitems();
									removeitems();
									removeitems();
									removeitems();
									button->setCustomState(false);
								}
							}
							else
							{//��������
								/*char mTemp[64];
								t_sprintf(mTemp,"%d/%d",nCurData->nIndex,__nSelIndex);
								CCMessageBox(mTemp,"test");*/
								if (nCurData->nTaskId>0) 
								{
									KUiScrptinfo nVal;
									memset(&nVal, 0, sizeof(KUiScrptinfo));
									if (g_pCoreShell)
									   g_pCoreShell->GetGameData(GDI_SCRIPT_VAL_WITH, (int)&nVal,1,nCurData->nTaskId);  //ִ�нű����ص�����
									//UIMessageBox2(SelNode->szMsg);
								}
							}
						}
						/*if (button->GetTagbyKey()=="-1")
						{//������Ǹ���Ŀ
							char mTemp[64];
							t_sprintf(mTemp,"%d",button->getTag());
							CCMessageBox(mTemp,"test");
						}
						else
						{//�����������Ŀ
						}*/
					}
				}

				/*UIButton* custom_button = UIButton::create();
				custom_button->setName("TextButton");
				custom_button->setTouchEnabled(true);
				custom_button->loadTextures("ui/state/life_normal.png", "ui/state/life_pk.png", "");
				custom_button->setScale9Enabled(true);
				custom_button->setSize(btnNewSize);

				Layout *custom_item = Layout::create();
				custom_item->setSize(btnNewSize);
				custom_button->setPosition(ccp(btnNewSize.width/2,btnNewSize.height/2));
				custom_item->addChild(custom_button);
				pmainListView->insertCustomItem(custom_item,__nSelIndex+1);
				char nTempInfo[64];
				t_sprintf(nTempInfo,"���Բ���_%d",__nSelIndex+1);
				custom_button->setTitleText(UTEXT(nTempInfo,1));
				custom_button->setTitleColor(ccYELLOW);*/
				//pmainListView->refreshView();
				//pmainListView->jumpToBottom();
			}	
			break;
		}
	case LISTVIEW_ONSELECTEDITEM_END:
		//removeitems();
		break;
	default:
		break;
	}
}
//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void KuiTaskInfo::setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;
	
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\������\\����\\����ָ��.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	CCTexture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));

	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
    UIImageView *titleImg = UIImageView::create();
	titleImg->loadTextureByData(bgCur);
	titleImg->setPosition(ccp(m_size.width/2-7,m_size.height-12));
	m_pMainUiLayer->addWidget(titleImg);

	//�ر�
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\����\\���Ӱ���2-�ر�.spr");
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

	CCMenuItemSprite * LeaveConfirm = CCMenuItemSprite::create(sprite_Leave_normal, sprite_Leave_select, this, menu_selector(KuiTaskInfo::btnCallBackFunc));
	LeaveConfirm->setPosition(ccp(m_size.width/2-9,13));
	LeaveConfirm->setTag(1);
	//����˵�
	CCMenu * menu = CCMenu::create(LeaveConfirm,NULL);
	menu->setAnchorPoint(ccp(0,0));
	menu->setPosition(ccp(0,0));             //�˵�λ������Ϊ����������
	ParentNode_Task->addChild(menu);         //�ӵ�������


	KUiPlayerTeam mTeam;
	ZeroMemory(&mTeam,sizeof(mTeam));
	if (g_pCoreShell->TeamOperation(TEAM_OI_GD_INFO, (unsigned int)&mTeam, 0)) //û�ж��� �ʹ���һ֧����
	{//�ж���
	}
	else
	{//û�ж���
	}
}


//�����ť�ص�����
void KuiTaskInfo::oktouchEvent(CCObject *pSender, TouchEventType type)
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

void KuiTaskInfo::btnCallBackFunc(CCObject * pSender)
{
	if (!g_pCoreShell|| !g_GameWorld) return;

	CCNode * node = NULL;
	if (pSender)
	{
		node= dynamic_cast<CCNode *>(pSender);
		switch(node->getTag())
		{
		case 1:
			 buttonCallBackFunc(NULL);
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		default:
			break;
		}
	}
}
//ִ���ϲ����Ļص��������رյ�����
void KuiTaskInfo::buttonCallBackFunc(CCObject * pSender)
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
void KuiTaskInfo::closePopLayer(CCObject * pSender)
{
	this->removeFromParentAndCleanup(true);
}

//��д����ע�ắ�������¸�����������
void KuiTaskInfo::registerWithTouchDispatcher()
{
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,128, true);
}

CCRect KuiTaskInfo::getRect(CCNode* pNode,int i)
{
	CCRect  rc;
	rc.origin     = pNode->getPosition(); //��������ͼ��ƫ��
	//visibleSize
	rc.size       = pNode->getContentSize();

	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //��������
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //���ϱ�    //- rc.origin.y
	
	return rc;
}

void KuiTaskInfo::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if (!m_bScrolling) return;
	m_bScrolling = false;

	//__loopTime = MAX_LOOP_TIME;
}

//�ƶ���
void KuiTaskInfo::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if (!m_bScrolling || !__isCanMove) return;
	CCPoint point   = ParentNode_Task->convertToNodeSpace(CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView()));
	CCPoint diff    = ccpSub(point,m_lastPoint);                  //����ͼƬ��λ��
	ParentNode_Task->setPosition(ccpAdd(ParentNode_Task->getPosition(),diff));
	m_origin      = ParentNode_Task->getPosition();
	m_lastPoint = point;
}
//��������ccTouchBegan������true
bool KuiTaskInfo::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
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
void KuiTaskInfo::update(float delta)
{
	//CCMessageBox("����ѭ��","update");
	if (isOpen && g_pCoreShell)
	{
	}
}

void KuiTaskInfo::draw()
{
	//CCMessageBox("����ѭ��","draw");
}