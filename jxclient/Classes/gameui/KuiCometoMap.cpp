//�а�ť�ġ������롡�Ի���
//
#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiCometoMap.h"
#include "engine/Text.h"
#include "engine/KIniFile.h"
#include "gamecore/CoreShell.h"
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
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_CC;
#define _CHESS_COUNT 100
#define _ROW_MAP_COUNT 7
KuiCometoMap::KuiCometoMap()
{
	isOpen = false;
	colorLayer        = NULL;
	red               = NULL;
	m_StartPos.x=0;
	m_StartPos.y=0;
    m_MaxCount = 0;
	m_MapCount = 0;

	for(int i=0;i<_MAPMAX_COUNT;i++)
	m_recommend[i] = NULL;

	m_pMainUiLayer = NULL;
	btnPageBox_1= NULL;
	btnPageBox_2= NULL;
	btnPageBox_3= NULL;
	btnPageBox_4= NULL;
	btnPageBox_5= NULL;
	m_TimeInfo  = NULL;
	m_AwardButton_yl= NULL;;//����/����
	m_AwardButton_fs= NULL;;//����/��Ե
	m_AwardButton_jb= NULL;;//���
	m_AwardButton_jf= NULL;;//����

	m_timeProcessSprite= NULL;;

	_nCurTime =0; //(��λ������)
	_nMoneyExpTime =0; //��������� 0
	_nFuYuanTime =0;   //��Ե������ 1
	_nJiFenTime =0;    //����       3
	_nJinBiTime =0;    //���       2
	_curPage =AUTOBTN_PAGE_BASE;
	nShowLayer = NULL;
}

KuiCometoMap::~KuiCometoMap()
{
	isOpen = false;
	colorLayer        = NULL;
}

//����ȷ��������
KuiCometoMap * KuiCometoMap::create(CCObject * callbackListener,SEL_CallFuncN callfun)
{
	KuiCometoMap * popLayer = KuiCometoMap::create();
	popLayer->addDialogData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}


//��ʼ��
bool KuiCometoMap::init()
{
	if ( !CCLayer::init() ){
		return false;
	}
	//����������Ӧ
	this->setTouchEnabled(true);
	this->scheduleUpdate();                   //���ú���
	this->setTouchMode(kCCTouchesOneByOne);   //��ֹ��㴥��
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
	t_sprintf(nSprName,"\\spr\\Ui3\\activityguide\\activityguideui.spr");
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
	return true;
}


void KuiCometoMap::addDialogData()
{
	if  (!g_pCoreShell) return;
	CCSize nSize;
	nSize.width =128;
	nSize.height=24;
	m_StartPos.x =10;//+ITEM_CELL_SIZE/2;
	m_StartPos.y =140-24;//+ITEM_CELL_SIZE/2;


	m_pMainUiLayer = UILayer::create();
	ParentNode_ItemEx->addChild(m_pMainUiLayer);


	char nSprName[128]={0};
	int m_nWidth,m_nHeight,nFrams;
	CCTexture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));

	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\activityguide\\pagebutton.spr");
	g_StrLower(nSprName);

	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCTexture2D * check_btn_normal = bgCur;
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCTexture2D * check_btn_active = bgCur;

	float nTempFontSize =14;
	float nOffsetY = 45;

	btnPageBox_1 = UIButton::create();
	btnPageBox_1->setTouchEnabled(true);
	btnPageBox_1->loadTexturesByData(check_btn_normal,check_btn_active,check_btn_normal);
	btnPageBox_1->setTitleFontSize(nTempFontSize);
	btnPageBox_1->setTitleText(UTEXT("��������",1).c_str());
	//btnPageBox_1->setAnchorPoint(ccp(0,0));
	btnPageBox_1->setPosition(ccp(70,m_size.height-nOffsetY));
	btnPageBox_1->setTag(AUTOBTN_PAGE_BASE);
	btnPageBox_1->setTagbyKey("1");
	//btnPageBox_1->ignoreContentAdaptWithSize(true);
	btnPageBox_1->addTouchEventListener(this,toucheventselector(KuiCometoMap::touchPageBtnEvent));
	m_pMainUiLayer->addWidget(btnPageBox_1);

	btnPageBox_2 = UIButton::create();
	btnPageBox_2->setTouchEnabled(true);
	btnPageBox_2->loadTexturesByData(check_btn_normal,check_btn_active,check_btn_normal);
	btnPageBox_2->setTitleFontSize(nTempFontSize);
	btnPageBox_2->setTitleText(UTEXT("�Ƽ�����",1).c_str());
	//btnPageBox_1->setAnchorPoint(ccp(0,0));
	btnPageBox_2->setPosition(ccp(175,m_size.height-nOffsetY));
	btnPageBox_2->setTag(AUTOBTN_PAGE_ART);
	btnPageBox_2->setTagbyKey("2");
	//btnPageBox_1->ignoreContentAdaptWithSize(true);
	btnPageBox_2->addTouchEventListener(this,toucheventselector(KuiCometoMap::touchPageBtnEvent));
	m_pMainUiLayer->addWidget(btnPageBox_2);

	btnPageBox_3 = UIButton::create();
	btnPageBox_3->setTouchEnabled(true);
	btnPageBox_3->loadTexturesByData(check_btn_normal,check_btn_active,check_btn_normal);
	btnPageBox_3->setTitleFontSize(nTempFontSize);
	btnPageBox_3->setTitleText(UTEXT("�ƺŽ���",1).c_str());
	//btnPageBox_1->setAnchorPoint(ccp(0,0));
	btnPageBox_3->setPosition(ccp(280,m_size.height-nOffsetY));
	btnPageBox_3->setTag(AUTOBTN_PAGE_ITEM_1);
	btnPageBox_3->setTagbyKey("3");
	//btnPageBox_1->ignoreContentAdaptWithSize(true);
	btnPageBox_3->addTouchEventListener(this,toucheventselector(KuiCometoMap::touchPageBtnEvent));
	m_pMainUiLayer->addWidget(btnPageBox_3);

	btnPageBox_4 = UIButton::create();
	btnPageBox_4->setTouchEnabled(true);
	btnPageBox_4->loadTexturesByData(check_btn_normal,check_btn_active,check_btn_normal);
	btnPageBox_4->setTitleFontSize(nTempFontSize);
	btnPageBox_4->setTitleText(UTEXT("��������",1).c_str());
	//btnPageBox_1->setAnchorPoint(ccp(0,0));
	btnPageBox_4->setPosition(ccp(385,m_size.height-nOffsetY));
	btnPageBox_4->setTag(AUTOBTN_PAGE_ITEM_2);
	btnPageBox_4->setTagbyKey("4");
	//btnPageBox_1->ignoreContentAdaptWithSize(true);
	btnPageBox_4->addTouchEventListener(this,toucheventselector(KuiCometoMap::touchPageBtnEvent));
	m_pMainUiLayer->addWidget(btnPageBox_4);

	btnPageBox_5 = UIButton::create();
	btnPageBox_5->setTouchEnabled(true);
	btnPageBox_5->loadTexturesByData(check_btn_normal,check_btn_active,check_btn_normal);
	btnPageBox_5->setTitleFontSize(nTempFontSize);
	btnPageBox_5->setTitleText(UTEXT("��������",1).c_str());
	//btnPageBox_1->setAnchorPoint(ccp(0,0));
	btnPageBox_5->setPosition(ccp(490,m_size.height-nOffsetY));
	btnPageBox_5->setTag(AUTOBTN_PAGE_OTHER);
	btnPageBox_5->setTagbyKey("5");
	//btnPageBox_1->ignoreContentAdaptWithSize(true);
	btnPageBox_5->addTouchEventListener(this,toucheventselector(KuiCometoMap::touchPageBtnEvent));
	m_pMainUiLayer->addWidget(btnPageBox_5);
	//char nTempStr[128];
	//t_t_sprintf(nTempStr,pContent);
	/*CCLabelTTF *pstrLabel = CCLabelTTF::create(UTEXT(nTempStr,1).c_str(),UI_GAME_FONT_DEFAULT,14,nSize,kCCTextAlignmentCenter);//Arial
	pstrLabel->setColor(ccYELLOW);
	pstrLabel->setPosition(ccp(128/2+30,m_size.height-50));
	ParentNode_ItemEx->addChild(pstrLabel);

	CCLabelTTF *ptextLabel = CCLabelTTF::create(UTEXT("�����Ļ�س�",1).c_str(),UI_GAME_FONT_DEFAULT,14,nSize,kCCTextAlignmentCenter);//Arial
	ptextLabel->setColor(ccYELLOW);
	ptextLabel->setPosition(ccp(m_size.width/2,20));
	ParentNode_ItemEx->addChild(ptextLabel);
	*/
	//ͼ��
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\activityguide\\gameicon.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	CCSprite *iconSprite=NULL;
	if (bgCur)
	{
		iconSprite = CCSprite::createWithTexture(bgCur);
		iconSprite->setAnchorPoint(ccp(0,0));
		iconSprite->setPosition(ccp(30,m_size.height-90));
		ParentNode_ItemEx->addChild(iconSprite,1);
	}
	//�����������
	ccColor4B color = ccc4(112, 128, 144,150);
	CCSize nPageSize    = CCSizeMake(m_size.width-35,m_size.height-63);//���Ĵ�С
	CCPoint CCPointPage = CCPointMake(0,0);//��ʼλ��
	int nisOpen=0,nsaveMonye=0;
	_PageLayer_1 = UILayer::create();//UILayer::create();//��������
	//_PageLayer_1->setContentSize(nPageSize);
	_PageLayer_1->setPosition(CCPointPage);
	//_PageLayer_1->setVisible(false);
	ParentNode_ItemEx->addChild(_PageLayer_1);

	ZeroMemory(nSprName,sizeof(nSprName));
	//�������汳����� \spr\Ui3\activityguide\calendarpage.spr
	t_sprintf(nSprName,"\\spr\\Ui3\\activityguide\\calendarpage.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	CCSprite *pageSprite_a = CCSprite::createWithTexture(bgCur);
	pageSprite_a->setAnchorPoint(ccp(0,0));
	pageSprite_a->setPosition(ccp(19,23));
	_PageLayer_1->addChild(pageSprite_a,-1);

	/*KIniFile	Ini;
	if (Ini.Load("\\Ui\\Ui3\\uiactivityguide\\activitycalendar.ini"))
	{

	}*/
	m_TimeInfo = CCLabelTTF::create(UTEXT("����ʱ��:0Сʱ0����",1).c_str(),UI_GAME_FONT_DEFAULT,14);//Arial
	m_TimeInfo->setColor(ccYELLOW);
	m_TimeInfo->setAnchorPoint(ccp(0,0));
	m_TimeInfo->setPosition(ccp(90,m_size.height-80));
	_PageLayer_1->addChild(m_TimeInfo,1);


	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\activityguide\\progressbarback.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur)
	{
		UIImageView* ProcessBg=UIImageView::create();
		ProcessBg->loadTextureByData(bgCur);
		ProcessBg->setAnchorPoint(ccp(0,0));
		ProcessBg->setPosition(ccp(270,m_size.height-75));
		_PageLayer_1->addChild(ProcessBg,1);
	}
	
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\activityguide\\progressbar.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);

	if (bgCur)
	{
		m_timeProcessSprite=CCSprite::createWithTexture(bgCur);
		m_timeProcessSprite->setAnchorPoint(ccp(0,0));
		m_timeProcessSprite->setPosition(ccp(270,m_size.height-75));
		_PageLayer_1->addChild(m_timeProcessSprite,2);
	}
	//\\spr\\npcres\\passerby\\passerby239\\passerby239.spr
	//������\spr\skill\����\������ը.spr
	 nShowLayer = KuiShowSprite::create("\\spr\\npcres\\passerby\\passerby239\\passerby239.spr",19,80,this,callfuncN_selector(KuiCometoMap::closeShowPad));
	 nShowLayer->setRegisterWithTouchDispatcher(128);
	 _PageLayer_1->addChild(nShowLayer,1);

	
	KIniFile	Ini;
	if (Ini.Load("\\Ui\\Ui3\\uiactivityguide\\activitycalendar.ini"))
	{
	  char nTempInfo[128];
	  ZeroMemory(nTempInfo,sizeof(nTempInfo));
	  Ini.GetString("DescText","decsText","��ȡע��:\n\n  ����ʱ�䲻�ۼ�,�������߹���!\n���п���������ľ�ϲ��",nTempInfo,sizeof(nTempInfo));
	  CCLabelTTF * m_DescInfo = CCLabelTTF::create(UTEXT("��ȡע��\n  ����ʱ�䲻�ۼ�,�������߹���!\n���п���������ľ�ϲ��",1).c_str(),UI_GAME_FONT_DEFAULT,14);//Arial
	  m_DescInfo->setColor(ccYELLOW);
	  m_DescInfo->setAnchorPoint(ccp(0,0));
	  m_DescInfo->setPosition(ccp(430,m_size.height-180));
	  _PageLayer_1->addChild(m_DescInfo,1);

	  CCSize temp_size =CCSizeMake(150,16);

	  m_AwardButton_yl = UIButton::create();
	  m_AwardButton_yl->setTouchEnabled(true);
	  m_AwardButton_yl->loadTextures("ui/btn_sel.png","ui/statelan.png","ui/btn_sel.png");
	  m_AwardButton_yl->setTitleFontSize(nTempFontSize);
	  m_AwardButton_yl->setScale9Enabled(true);
	 
	  Ini.GetString("AwardKind0","Text","ÿ���ӻ�ȡ����:0",nTempInfo,sizeof(nTempInfo));
	 
	  m_AwardButton_yl->setSize(temp_size);
	  m_AwardButton_yl->setTitleText(UTEXT(nTempInfo,1).c_str());
	  m_AwardButton_yl->setTitleColor(ccYELLOW);
	//btnPageBox_1->setAnchorPoint(ccp(0,0));
	  m_AwardButton_yl->setPosition(ccp(520,m_size.height-285));
	  m_AwardButton_yl->setTag(1);
	  m_AwardButton_yl->setTagbyKey("1");
	//btnPageBox_1->ignoreContentAdaptWithSize(true);
	  m_AwardButton_yl->addTouchEventListener(this,toucheventselector(KuiCometoMap::touchAwardBtnEvent));
	  _PageLayer_1->addWidget(m_AwardButton_yl);

	  m_AwardButton_fs = UIButton::create();
	  m_AwardButton_fs->setTouchEnabled(true);
	  m_AwardButton_fs->loadTextures("ui/btn_sel.png","ui/statelan.png","ui/btn_sel.png");
	  m_AwardButton_fs->setTitleFontSize(nTempFontSize);
	  m_AwardButton_fs->setScale9Enabled(true);
	  Ini.GetString("AwardKind1","Text","ÿ���ӻ�ȡ����:0",nTempInfo,sizeof(nTempInfo));
	  m_AwardButton_fs->setSize(temp_size);
	  m_AwardButton_fs->setTitleText(UTEXT(nTempInfo,1).c_str());
	  m_AwardButton_fs->setTitleColor(ccYELLOW);
	  //btnPageBox_1->setAnchorPoint(ccp(0,0));
	  m_AwardButton_fs->setPosition(ccp(520,m_size.height-310));
	  m_AwardButton_fs->setTag(2);
	  m_AwardButton_fs->setTagbyKey("2");
	  //btnPageBox_1->ignoreContentAdaptWithSize(true);
	  m_AwardButton_fs->addTouchEventListener(this,toucheventselector(KuiCometoMap::touchAwardBtnEvent));
	  _PageLayer_1->addWidget(m_AwardButton_fs);

	  m_AwardButton_jb = UIButton::create();
	  m_AwardButton_jb->setTouchEnabled(true);
	  m_AwardButton_jb->loadTextures("ui/btn_sel.png","ui/statelan.png","ui/btn_sel.png");
	  m_AwardButton_jb->setTitleFontSize(nTempFontSize);
	  m_AwardButton_jb->setScale9Enabled(true);
	  Ini.GetString("AwardKind2","Text","ÿ���ӻ�ȡ����:0",nTempInfo,sizeof(nTempInfo));
	  m_AwardButton_jb->setSize(temp_size);
	  m_AwardButton_jb->setTitleText(UTEXT(nTempInfo,1).c_str());
	  m_AwardButton_jb->setTitleColor(ccYELLOW);
	  //btnPageBox_1->setAnchorPoint(ccp(0,0));
	  m_AwardButton_jb->setPosition(ccp(520,m_size.height-340));
	  m_AwardButton_jb->setTag(3);
	  m_AwardButton_jb->setTagbyKey("3");
	  //btnPageBox_1->ignoreContentAdaptWithSize(true);
	  m_AwardButton_jb->addTouchEventListener(this,toucheventselector(KuiCometoMap::touchAwardBtnEvent));
	  _PageLayer_1->addWidget(m_AwardButton_jb);

	  m_AwardButton_jf = UIButton::create();
	  m_AwardButton_jf->setTouchEnabled(true);
	  m_AwardButton_jf->loadTextures("ui/btn_sel.png","ui/statelan.png","ui/btn_sel.png");
	  m_AwardButton_jf->setTitleFontSize(nTempFontSize);
	  m_AwardButton_jf->setScale9Enabled(true);
	  Ini.GetString("AwardKind3","Text","ÿ���ӻ�ȡ����:0",nTempInfo,sizeof(nTempInfo));
	  m_AwardButton_jf->setSize(temp_size);
	  m_AwardButton_jf->setTitleText(UTEXT(nTempInfo,1).c_str());
	  m_AwardButton_jf->setTitleColor(ccYELLOW);
	  //btnPageBox_1->setAnchorPoint(ccp(0,0));
	  m_AwardButton_jf->setPosition(ccp(520,m_size.height-365));
	  m_AwardButton_jf->setTag(4);
	  m_AwardButton_jf->setTagbyKey("4");
	  //btnPageBox_1->ignoreContentAdaptWithSize(true);
	  m_AwardButton_jf->addTouchEventListener(this,toucheventselector(KuiCometoMap::touchAwardBtnEvent));
	  _PageLayer_1->addWidget(m_AwardButton_jf);

	}
	Ini.Clear();
	//�Ƽ��������
	CCSprite *pageSprite;
	_PageLayer_2 = UILayer::create();//��������
	_PageLayer_2->setPosition(CCPointPage);
	ParentNode_ItemEx->addChild(_PageLayer_2);
	//�Ƽ����� �����һ���ҳ����
	pageSprite = CCSprite::create("ui/calendarpage.png");
	pageSprite->setAnchorPoint(ccp(0,0));
	pageSprite->setPosition(ccp(19,23));
	_PageLayer_2->addChild(pageSprite,-1);

	//��ʼ�������
	KIniFile mapData;
	char nTmepStr[256];
	ZeroMemory(nTmepStr,sizeof(nTmepStr));
	if (_clientlanguage!=1)
		t_sprintf(nTmepStr,"\\Ui\\Ui3\\uiactivityguide\\recommendmap_vn.ini");
	else
		t_sprintf(nTmepStr,"\\Ui\\Ui3\\uiactivityguide\\recommendmap.ini");

	if (mapData.Load(nTmepStr))
	{
		mapData.GetInteger("ValueConfig","lvlCount",_ROW_MAP_COUNT+1,&m_MaxCount);
		mapData.GetInteger("ValueConfig","AllMapCount",_ROW_MAP_COUNT+1,&m_MapCount);
		char nKeyName[32];
		ZeroMemory(nKeyName,sizeof(nKeyName[32]));
		int i,j;
		int inMapIdx[_ROW_MAP_COUNT];
		ZeroMemory(inMapIdx,sizeof(inMapIdx));
		//for (i = 0; i < _ROW_MAP_COUNT; ++i)
		//inMapIdx[i]=0;

		float offsetX=19,offsetY=pageSprite->getContentSize().height+pageSprite->getPosition().y-iconSprite->getContentSize().height-20;
		float startPosY = offsetY-23;

		CCMenu* maplistMenu = CCMenu::create(NULL);
		maplistMenu->setAnchorPoint(ccp(0,0));
		maplistMenu->setPosition(ccp(19,23));
		_PageLayer_2->addChild(maplistMenu,1);

		for (i = 0; i < m_MaxCount; ++i)
		{//8���ȼ��ֶ�  
			t_sprintf(nKeyName,"recommend_%d",i+1);
			char nstrName[32];
			mapData.GetString(nKeyName,"Text","",nstrName,sizeof(nstrName)); //kCCTextAlignmentLeft
			m_recommend[i] = CCLabelTTF::create(UTEXT(nstrName,1).c_str(),UI_GAME_FONT_DEFAULT,14);//Arial
			m_recommend[i]->setAnchorPoint(ccp(0,0));
			m_recommend[i]->setPosition(ccp(offsetX,offsetY));
			offsetY -=30;
			_PageLayer_2->addChild(m_recommend[i]);
			ZeroMemory(inMapIdx,sizeof(inMapIdx));
			mapData.GetInt8(nKeyName,"maps",inMapIdx); //ÿ�����ж��ٸ���ͼ

			float nLeft,nWidht;
			nLeft  = m_recommend[i]->getPosition().x;
			//nTop   = m_recommend[i]->getPosition().y;
			nWidht = m_recommend[i]->getContentSize().width;
			//nHight = m_recommend[i]->getContentSize().height;
		    float backBtnW = nLeft+nWidht + 20; //������һ����ť�ĳ���
			for (j = 0; j < _ROW_MAP_COUNT; ++j)
			{//ÿ���ȼ������¡��ж��ٸ���ͼ
				if (inMapIdx[j]>0)
				{
					char nNameMame[32]={0};
					ZeroMemory(nNameMame,sizeof(nNameMame));
					int _MapId=0;
					t_sprintf(nKeyName,"map_%d",inMapIdx[j]);
					mapData.GetString(nKeyName,"MapName","����",nNameMame,sizeof(nNameMame));
					mapData.GetInteger(nKeyName,"MapId",0,&_MapId);
					int nFonSize = 12;
					CCSize tempSize;
					tempSize.width  = 80;
					tempSize.height = 26; 
					CCLabelTTF* mapNameLabel = CCLabelTTF::create(UTEXT(nNameMame,1).c_str(),UI_GAME_FONT_DEFAULT,nFonSize,tempSize,kCCTextAlignmentLeft);
					mapNameLabel->setColor(ccc3(0,255,0));
					mapNameLabel->setAnchorPoint(ccp(0,0));
					//mapNameLabel->setHorizontalAlignment(kCCTextAlignmentLeft);        //����λ��
					//mapNameLabel->setVerticalAlignment(kCCVerticalTextAlignmentCenter);//���¾���
					CCMenuItemLabel* mserLabel1=CCMenuItemLabel::create(mapNameLabel,this,menu_selector(KuiCometoMap::movemapCallBack));
					mserLabel1->setTag(_MapId);
					t_sprintf(nKeyName,"%d",inMapIdx[j]+1);
					std::string mapKey = nKeyName;
					mserLabel1->setTagbyKey(mapKey);
					mserLabel1->setPosition(ccp(backBtnW+2,startPosY));
					backBtnW += mapNameLabel->getContentSize().width;
					maplistMenu->addChild_(mserLabel1,_MapId);
				}
			}
			startPosY-=30; //�����е�ƫ��
		} 
		mapData.Clear();
	}
	//��������
	_PageLayer_3 = UILayer::create();
	_PageLayer_3->setPosition(CCPointPage);
	ParentNode_ItemEx->addChild(_PageLayer_3);
	//�Ƽ����� �����һ���ҳ����
	pageSprite = CCSprite::create("ui/calendarpage.png");
	pageSprite->setAnchorPoint(ccp(0,0));
	pageSprite->setPosition(ccp(19,23));
	_PageLayer_3->addChild(pageSprite,-1);

	//��������
	_PageLayer_4 = UILayer::create();
	_PageLayer_4->setPosition(CCPointPage);
	ParentNode_ItemEx->addChild(_PageLayer_4);
	//�Ƽ����� �����һ���ҳ����
	pageSprite = CCSprite::create("ui/calendarpage.png");
	pageSprite->setAnchorPoint(ccp(0,0));
	pageSprite->setPosition(ccp(19,23));
	_PageLayer_4->addChild(pageSprite,-1);

	//��������
	_PageLayer_5 = UILayer::create();
	_PageLayer_5->setPosition(CCPointPage);
	ParentNode_ItemEx->addChild(_PageLayer_5);
	//�Ƽ����� �����һ���ҳ����
	pageSprite = CCSprite::create("ui/calendarpage.png");
	pageSprite->setAnchorPoint(ccp(0,0));
	pageSprite->setPosition(ccp(19,23));
	_PageLayer_5->addChild(pageSprite,-1);


	_PageLayer_1->setVisible(true);
	_PageLayer_2->setVisible(false);
	_PageLayer_3->setVisible(false);
	_PageLayer_4->setVisible(false);
	_PageLayer_5->setVisible(false);
	setBtnTouchEnble(1,true);
	setBtnTouchEnble(2,false);
	setBtnTouchEnble(3,false);
	setBtnTouchEnble(4,false);
	setBtnTouchEnble(5,false);
}

void KuiCometoMap::closeShowPad(CCNode *pNode)
{

}
void KuiCometoMap::touchAwardBtnEvent(CCObject *pSender, TouchEventType type)
{
	if  (!g_pCoreShell || !g_GameWorld) return;
	KUiPlayerSetTimeInfo	Info;
	memset(&Info, 0, sizeof(KUiPlayerSetTimeInfo));
	switch (type)
	{
	case TOUCH_EVENT_BEGAN://Touch Down
		{
			if (pSender)
			{
				UIButton* nTbtn = (UIButton*)pSender;
				nTbtn->setTitleColor(ccWHITE);
				int  nTag = nTbtn->getTag();
				switch(nTag)
				{
				case 1:
					Info.nYinLiangTime = _nCurTime;
					break;
				case 2:
					 Info.nExpTime = _nCurTime;
					break;
				case 3:
					Info.nJinBiTime = _nCurTime;
					break;
				case 4:
					Info.nExpPiontTime = _nCurTime;
					break;
				default:
					break;
				}
				g_pCoreShell->GetGameData(GDI_PLAYER_SETTIME_INFO,(int)&Info,nTag-1);
				//nTbtn->setTouchEnabled(false);
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
				nTbtn->setTitleColor(ccYELLOW);
			}
		}
		break;
	case TOUCH_EVENT_CANCELED:
		break;
	default:
		break;
	}
}

void KuiCometoMap::touchPageBtnEvent(CCObject *pSender, TouchEventType type)
{
	if  (!g_pCoreShell || !g_GameWorld) return;

	switch (type)
	{
	case TOUCH_EVENT_BEGAN://Touch Down
		{
			if (pSender)
			{
				UIButton* nTbtn = (UIButton*)pSender;
				nTbtn->setTitleColor(ccYELLOW);
				switch(nTbtn->getTag())
				{
				case AUTOBTN_PAGE_BASE:
					{//��������
						_curPage = AUTOBTN_PAGE_BASE;
						_PageLayer_1->setVisible(true);
						_PageLayer_2->setVisible(false);
						_PageLayer_3->setVisible(false);
						_PageLayer_4->setVisible(false);
						_PageLayer_5->setVisible(false);
						setBtnTouchEnble(1,true);
						setBtnTouchEnble(2,false);
						setBtnTouchEnble(3,false);
						setBtnTouchEnble(4,false);
						setBtnTouchEnble(5,false);

					}
					break;
				case AUTOBTN_PAGE_ART:
					{//�Ƽ�����
						_curPage = AUTOBTN_PAGE_ART;
						_PageLayer_1->setVisible(false);
						_PageLayer_2->setVisible(true);
						_PageLayer_3->setVisible(false);
						_PageLayer_4->setVisible(false);
						_PageLayer_5->setVisible(false);
						setBtnTouchEnble(1,false);
						setBtnTouchEnble(2,true);
						setBtnTouchEnble(3,false);
						setBtnTouchEnble(4,false);
						setBtnTouchEnble(5,false);

					}
					break;
				case AUTOBTN_PAGE_ITEM_1:
					{//��Ʒ1����
						_curPage = AUTOBTN_PAGE_ITEM_1;
						_PageLayer_1->setVisible(false);
						_PageLayer_2->setVisible(false);
						_PageLayer_3->setVisible(true);
						_PageLayer_4->setVisible(false);
						_PageLayer_5->setVisible(false);
						setBtnTouchEnble(1,false);
						setBtnTouchEnble(2,false);
						setBtnTouchEnble(3,true);
						setBtnTouchEnble(4,false);
						setBtnTouchEnble(5,false);
					}
					break;
				case AUTOBTN_PAGE_ITEM_2:
					{//��Ʒ2����
						_curPage = AUTOBTN_PAGE_ITEM_2;
						_PageLayer_1->setVisible(false);
						_PageLayer_2->setVisible(false);
						_PageLayer_3->setVisible(false);
						_PageLayer_4->setVisible(true);
						_PageLayer_5->setVisible(false);
						setBtnTouchEnble(1,false);
						setBtnTouchEnble(2,false);
						setBtnTouchEnble(3,false);
						setBtnTouchEnble(4,true);
						setBtnTouchEnble(5,false);
					}
					break;
				case AUTOBTN_PAGE_OTHER:
					{//��������
						_curPage = AUTOBTN_PAGE_OTHER;
						_PageLayer_1->setVisible(false);
						_PageLayer_2->setVisible(false);
						_PageLayer_3->setVisible(false);
						_PageLayer_4->setVisible(false);
						_PageLayer_5->setVisible(true);

						setBtnTouchEnble(1,false);
						setBtnTouchEnble(2,false);
						setBtnTouchEnble(3,false);
						setBtnTouchEnble(4,false);
						setBtnTouchEnble(5,true);

					}
					break;
				default:
					break;
				}
			}
		}
		//m_pDisplayValueLabel->setText(CCString::createWithFormat("Touch Down")->getCString());
		break;
	case TOUCH_EVENT_MOVED://Touch Move
		break;

	case TOUCH_EVENT_ENDED://Touch Up
		{
			if (pSender)
			{
				UIButton* nTbtn = (UIButton*)pSender;
				nTbtn->setTitleColor(ccWHITE);
				if  (nTbtn->getTag()==AUTOBTN_PAGE_BASE)
				{
					//buttonCallBackFunc(NULL);
					//CCMessageBox(nTbtn->GetTagbyKey().c_str(),"���°�ť");
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

void KuiCometoMap::setBtnTouchEnble(int index,bool enble)
{
	switch(index)
	{
	case 1:
		{
			if (m_AwardButton_yl)
			    m_AwardButton_yl->setTouchEnabled(enble);//����/����
			if (m_AwardButton_fs)
			    m_AwardButton_fs->setTouchEnabled(enble);//����/��Ե
			if (m_AwardButton_jb)
			    m_AwardButton_jb->setTouchEnabled(enble);//���
			if (m_AwardButton_jf)
			    m_AwardButton_jf->setTouchEnabled(enble);//����
		}
		break;
	case 3:
		{
			if (m_AwardButton_yl)
				m_AwardButton_yl->setTouchEnabled(enble);//����/����
			if (m_AwardButton_fs)
				m_AwardButton_fs->setTouchEnabled(enble);//����/��Ե
			if (m_AwardButton_jb)
				m_AwardButton_jb->setTouchEnabled(enble);//���
			if (m_AwardButton_jf)
				m_AwardButton_jf->setTouchEnabled(enble);//����
		}
		break;
	case 2:
		{
			if (m_AwardButton_yl)
				m_AwardButton_yl->setTouchEnabled(enble);//����/����
			if (m_AwardButton_fs)
				m_AwardButton_fs->setTouchEnabled(enble);//����/��Ե
			if (m_AwardButton_jb)
				m_AwardButton_jb->setTouchEnabled(enble);//���
			if (m_AwardButton_jf)
				m_AwardButton_jf->setTouchEnabled(enble);//����
		}
		break;
	case 4:
		{
			if (m_AwardButton_yl)
				m_AwardButton_yl->setTouchEnabled(enble);//����/����
			if (m_AwardButton_fs)
				m_AwardButton_fs->setTouchEnabled(enble);//����/��Ե
			if (m_AwardButton_jb)
				m_AwardButton_jb->setTouchEnabled(enble);//���
			if (m_AwardButton_jf)
				m_AwardButton_jf->setTouchEnabled(enble);//����
		}
		break;
	case 5:
		{
			if (m_AwardButton_yl)
				m_AwardButton_yl->setTouchEnabled(enble);//����/����
			if (m_AwardButton_fs)
				m_AwardButton_fs->setTouchEnabled(enble);//����/��Ե
			if (m_AwardButton_jb)
				m_AwardButton_jb->setTouchEnabled(enble);//���
			if (m_AwardButton_jf)
				m_AwardButton_jf->setTouchEnabled(enble);//����
		}
		break;
	default:
		break;
	}
}

void KuiCometoMap::movemapCallBack(CCObject* menuItem)
{
	if (menuItem)
	{
		CCMenuItemLabel *nCerBox  = (CCMenuItemLabel * )menuItem;
		if (nCerBox)
		{
            //char msg[64];
			//t_sprintf(msg,"map:%d,index:%s",nCerBox->getTag(),nCerBox->GetTagbyKey().c_str());
			//CCMessageBox(msg,"1111");
			//����ʼִ�пͻ��˴��ͽű�
			if (g_pCoreShell)
			{
				char nTempstr[32];
				t_sprintf(nTempstr,nCerBox->GetTagbyKey().c_str());
				int nIndex =g_Atoui(nTempstr);
			    g_pCoreShell->OperationRequest(GOI_PLAYER_SYSSHOP,0,nIndex-1,3);
			}
		}
	}
}
// -------------------------------------------------------------------------
// ����	: ��������
// -------------------------------------------------------------------------
void KuiCometoMap::UpdateData()
{
	//KUiSkillData m_Skills[SKILLTREE_MAX_SKILL_COUNT]
	if (!g_GameWorld || !g_pCoreShell) return;
}

//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void KuiCometoMap::setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;

	//���һ���رհ�ť��\spr\Ui3\activityguide\closebutton.spr
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
	CCSprite * sprite_close_normal =  CCSprite::createWithTexture(bgCur);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	CCSprite * sprite_close_select =  CCSprite::createWithTexture(bgCur);
	CCMenuItemSprite * closeConfirm = CCMenuItemSprite::create(sprite_close_normal, sprite_close_select, this, menu_selector(KuiCometoMap::buttonCallBackFunc));
	closeConfirm->setPosition(ccp(0,0));

	CCMenu * menu = CCMenu::create(closeConfirm,NULL);
	menu->setPosition(ccp(m_size.width/2,13));//�˵�λ������Ϊ����������
	ParentNode_ItemEx->addChild(menu);         //�ӵ�������
}
//ִ���ϲ����Ļص��������رյ�����
void KuiCometoMap::buttonCallBackFunc(CCObject * pSender)
{
	CCNode * node = NULL;
	if (pSender)
		node = dynamic_cast<CCNode *>(pSender);
	//node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��
    if (nShowLayer)
		nShowLayer->removeShowSprite();

	if (m_callfun &&  m_callbackListener)
		(m_callbackListener->*m_callfun)(node);

	this->removeFromParentAndCleanup(true);
}

//�رյ�����
void KuiCometoMap::closePopLayer(CCObject * pSender)
{
	if (nShowLayer)
		nShowLayer->removeShowSprite();

	this->removeFromParentAndCleanup(true);
}

//��д����ע�ắ�������¸�����������
void KuiCometoMap::registerWithTouchDispatcher()
{
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,128,true);
}

CCRect KuiCometoMap::getRect(CCNode* pNode,int i)
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
bool KuiCometoMap::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	//CCSetIterator it = pTouch->begin();
	//CCTouch* touch = (CCTouch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	CCPoint location = pTouch->getLocation();//Ŀ�ĵ�����?��Ļ�����
	//case SMCT_UI_RENASCENCE:	//ѡ������	
	//	if (g_pCoreShell)
	//	   g_pCoreShell->OperationRequest(GOI_PLAYER_RENASCENCE,0,true);//nSelAction=0��ԭ������, nSelAction=1���س�	
    //buttonCallBackFunc(NULL);
	return true;
}

void KuiCometoMap::update(float delta)
{
	//CCMessageBox("����ѭ��","update");
  if (isOpen && g_pCoreShell)
  {
	  switch(_curPage)
	  {
	  case AUTOBTN_PAGE_BASE:{
		KUiPlayerActivityInfo	Info;
		memset(&Info, 0, sizeof(KUiPlayerActivityInfo));
		ZeroMemory(&Info,sizeof(Info));
		g_pCoreShell->GetGameData(GDI_PLAYER_TIME_INFO, (int)&Info, 0);

		if (m_TimeInfo)
		   m_TimeInfo->setString(UTEXT(Info.nTimeInfo,1).c_str());

		float ver = (60.00*Info.nHour+Info.nMin)/1400;
		if (m_timeProcessSprite)
		{
			if (ver>=0 && ver<=1)
			   m_timeProcessSprite->setScaleX(ver);
			else
			   m_timeProcessSprite->setScaleX(1);
		}
		//UIMessageBox2(Info.nTimeInfo);
		/*if (Info.nHour*60+Info.nMin<=1440)
			n_ProcessFg.Set2IntValue(Info.nHour*60+Info.nMin,1440);   //������	  24Сʱ
		else 
			n_ProcessFg.Set2IntValue(1440,1440);*/
		_nCurTime      = Info.nHour*60+Info.nMin; //(��λ������)
		_nMoneyExpTime = Info.nYinLiangTime; //��������� 0
		_nFuYuanTime   = Info.nExpTime;   //��Ե������ 1
		_nJiFenTime    = Info.nExpPiontTime;    //����       3
		_nJinBiTime    = Info.nJinBiTime;    //���       2


		if (_nCurTime-Info.nExpTime>=1)
		{//��Ե������ 1
			m_AwardButton_fs->setTitleColor(ccYELLOW);
			m_AwardButton_fs->setTouchEnabled(true);
		}
		else
		{
			m_AwardButton_fs->setTitleColor(ccGRAY);
			m_AwardButton_fs->setTouchEnabled(false);
		}


		if (_nCurTime-Info.nYinLiangTime>=1)
		{//��������� 0
			m_AwardButton_yl->setTitleColor(ccYELLOW);
			m_AwardButton_yl->setTouchEnabled(true);
		}
		else
		{
			m_AwardButton_yl->setTitleColor(ccGRAY);
			m_AwardButton_yl->setTouchEnabled(false);
		}

		if (_nCurTime-Info.nExpPiontTime>=60)
		{//����
			m_AwardButton_jf->setTitleColor(ccYELLOW);
			m_AwardButton_jf->setTouchEnabled(true);
		}
		else
		{
			m_AwardButton_jf->setTitleColor(ccGRAY);
			m_AwardButton_jf->setTouchEnabled(false);
		}

		if (_nCurTime-Info.nJinBiTime>=60)
		{ //��� 
			m_AwardButton_jb->setTitleColor(ccYELLOW);
			m_AwardButton_jb->setTouchEnabled(true);
		}
		else
		{
			m_AwardButton_jb->setTitleColor(ccGRAY);
			m_AwardButton_jb->setTouchEnabled(false);
		}
			 }
			 break;
	  default:
		  setBtnTouchEnble(AUTOBTN_PAGE_BASE,false);
		  break;
	  }
		
  }
}

void KuiCometoMap::draw()
{
	//CCMessageBox("����ѭ��","draw");
}


int KuiCometoMap::FilterTextColor(char* pMsgBuff, unsigned short nMsgLength)
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