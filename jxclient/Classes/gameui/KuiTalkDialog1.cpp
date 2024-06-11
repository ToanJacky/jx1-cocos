//�а�ť�ġ������롡�Ի���
//
#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiTalkDialog1.h"
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
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_CC;

KuiTalkDialog1::KuiTalkDialog1()
{
	isOpen = false;
	colorLayer        = NULL;
	red               = NULL;
	__Kind      =-1;
	__nParam    =0;
	ZeroMemory(__strParam,sizeof(__strParam));
}

KuiTalkDialog1::~KuiTalkDialog1()
{
	isOpen = false;
	colorLayer = NULL;
	__nParam    =0;
}

//����ȷ��������
KuiTalkDialog1 * KuiTalkDialog1::create(char * pContent, char *OkbtnStr,char *NobtnStr,int nKind,CCObject * callbackListener,SEL_CallFuncN callfun,unsigned int nParam,char *strParam)
{
	KuiTalkDialog1 * popLayer = KuiTalkDialog1::create();
	popLayer->__nParam = nParam;
	if (strParam)
	   t_sprintf(popLayer->__strParam,strParam);
	//popLayer->setTitle(title);
	popLayer->addDialogData(pContent,nKind);
	//���ð�ť
	popLayer->setcoloseButton(OkbtnStr,NobtnStr,callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}


//��ʼ��
bool KuiTalkDialog1::init()
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
	t_sprintf(nSprName,"\\Spr\\Ui3\\NPC�Ի���\\�Ի���2.spr");
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
	this->setTouchMode(kCCTouchesOneByOne); //��ֹ��㴥��
	return true;
}


void KuiTalkDialog1::addDialogData(char * pContent,int nKind)
{
	if  (!g_pCoreShell) return;
	CCSize nSize;
	nSize.width =256;
	nSize.height=24;
	char nTempStr[128];
	t_sprintf(nTempStr,pContent);
	__Kind = nKind;
	CCLabelTTF *pstrLabel = CCLabelTTF::create(UTEXT(nTempStr,1).c_str(),UI_GAME_FONT_DEFAULT,14,nSize,kCCTextAlignmentLeft);//Arial
	pstrLabel->setColor(ccYELLOW);
	pstrLabel->setAnchorPoint(ccp(0,0));
	pstrLabel->setPosition(ccp(30,m_size.height-50));
	ParentNode_ItemEx->addChild(pstrLabel);

	switch(nKind)
	{
	case SMCT_UI_TONG_JOIN_APPLY://�������
	case SMCT_UI_RENASCENCE:
		{
			/*CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-128, true);
			CCLabelTTF *ptextLabel = CCLabelTTF::create(UTEXT("�����Ļ�س�",1).c_str(),UI_GAME_FONT_DEFAULT,14,nSize,kCCTextAlignmentCenter);//Arial
			ptextLabel->setColor(ccYELLOW);
			ptextLabel->setPosition(ccp(m_size.width/2,20));
			ParentNode_ItemEx->addChild(ptextLabel);*/
		}
	break;
	case SMCT_UI_TEAM_INVITE:
		{//Ӧ���������

		}
		break;
	case SMCT_UI_TEAM_APPLY:
		{//ͬ���ܾ����˵��������
		}
		break;
	default:
		break;
	}
	
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
void KuiTalkDialog1::setcoloseButton(char *OkbtnStr,char *NobtnStr,CCObject * callbackListener,SEL_CallFuncN callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;

	m_pUiLayer = UILayer::create();
	ParentNode_ItemEx->addChild(m_pUiLayer);

	switch(__Kind)
	{
	case SMCT_UI_TONG_JOIN_APPLY://�������
	case SMCT_UI_TRADE:
	case SMCT_UI_TEAM_APPLY:
	case SMCT_UI_TEAM_INVITE:
		{
			//ͬ��
			char nOkStr[64],nNoStr[64];
			t_sprintf(nOkStr,OkbtnStr);
			t_sprintf(nNoStr,NobtnStr);
			
			UIButton* button = UIButton::create();
			button->setTouchEnabled(true);
			button->loadTextures("ui/textbg.png", "ui/textbg.png", "");
			button->setTitleText(UTEXT(nOkStr,1).c_str());
			button->setPosition(ccp(m_size.width/2-70,30));
			button->setTag(__Kind);
			button->setTagbyKey("1");
			button->addTouchEventListener(this,toucheventselector(KuiTalkDialog1::touchEvent));
			m_pUiLayer->addWidget(button);
			//ȡ��
			UIButton* _button = UIButton::create();
			_button->setTouchEnabled(true);
			_button->loadTextures("ui/textbg.png", "ui/textbg.png", "");
			_button->setTitleText(UTEXT(nNoStr,1).c_str());
			_button->setPosition(ccp(m_size.width/2+70,30));
			_button->setTag(__Kind);
			_button->setTagbyKey("0");
			_button->addTouchEventListener(this,toucheventselector(KuiTalkDialog1::touchEvent));
			m_pUiLayer->addWidget(_button);
		}
		break;
	case SMCT_UI_RENASCENCE:
		{
			//�س�����
			UIButton* button = UIButton::create();
			button->setTouchEnabled(true);
			button->loadTextures("ui/textbg.png", "ui/textbg.png", "");
			button->setTitleText(UTEXT("�س�����",1).c_str());
			button->setPosition(ccp(m_size.width/2,30));
			button->setTag(SMCT_UI_RENASCENCE);
			button->setTagbyKey("1");
			button->addTouchEventListener(this,toucheventselector(KuiTalkDialog1::touchEvent));
			m_pUiLayer->addWidget(button);
		}
		break;
	default:
		break;
	}

}
//ִ���ϲ����Ļص��������رյ�����
void KuiTalkDialog1::buttonCallBackFunc(CCObject * pSender)
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
void KuiTalkDialog1::closePopLayer(CCObject * pSender)
{
	this->removeFromParentAndCleanup(true);
}

//��д����ע�ắ�������¸�����������
void KuiTalkDialog1::registerWithTouchDispatcher()
{
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-128, true);
}

CCRect KuiTalkDialog1::getRect(CCNode* pNode,int i)
{
	CCRect  rc;
	rc.origin     = pNode->getPosition(); //��������ͼ��ƫ��
	//visibleSize
	rc.size       = pNode->getContentSize();

	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //��������
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //���ϱ�    //- rc.origin.y
	
	return rc;
}

void KuiTalkDialog1::touchEvent(CCObject *pSender, TouchEventType type)
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
				switch (nTbtn->getTag())
				{
				   case SMCT_UI_TONG_JOIN_APPLY://�������
					   {
						   /*
						   g_pCoreShell->TongOperation(GTOI_TONG_JOIN_REPLY,
						   (unsigned int)pPlayer, (nSelAction == 0));	//nSelAction=0����Ӧ, nSelAction=1���ܾ�
						   */
						   if (nTbtn->GetTagbyKey()=="1")
						   {//ͬ��
							   g_pCoreShell->TongOperation(GTOI_TONG_JOIN_REPLY,__nParam,true,0,__strParam);	    //nSelAction=0����Ӧ, nSelAction=1���ܾ�
						   }else
							   g_pCoreShell->TongOperation(GTOI_TONG_JOIN_REPLY,__nParam,false,0,__strParam);	//nSelAction=0����Ӧ, nSelAction=1���ܾ�

						   //char nTemp[64];
						   //t_sprintf(nTemp,"%u:%s",__nParam,__strParam);
						   //CCMessageBox(UTEXT(nTemp,1).c_str(),"test");
					   }
					   break;
				   case SMCT_UI_TRADE:
				    {//���뽻�׵�
						if (nTbtn->GetTagbyKey()=="1")
						{//ͬ��
							g_pCoreShell->OperationRequest(GOI_TRADE_INVITE_RESPONSE,__nParam,true);	//nSelAction=0����Ӧ, nSelAction=1���ܾ�
						}else
                            g_pCoreShell->OperationRequest(GOI_TRADE_INVITE_RESPONSE,__nParam,false);	//nSelAction=0����Ӧ, nSelAction=1���ܾ�

				    }
					break;
				   case SMCT_UI_TEAM_APPLY:
					{//ͬ���ܾ����˵��������
						if (nTbtn->GetTagbyKey()=="1")
						{
							//CCMessageBox("dlalog","ͬ�����");
							g_pCoreShell->TeamOperation(TEAM_OI_APPLY_RESPONSE,__nParam,true);
						}
						else
							g_pCoreShell->TeamOperation(TEAM_OI_APPLY_RESPONSE,__nParam,false);	//nSelAction=0����Ӧ, nSelAction=1���ܾ�

					 }
					 break;
					case SMCT_UI_TEAM_INVITE://Touch Down
						{//�Լ��ܾ���ͬ��������ӵ�����
							if (__nParam)
							{
								if (nTbtn->GetTagbyKey()=="1")
								    g_pCoreShell->TeamOperation(TEAM_OI_INVITE_RESPONSE,__nParam,true);
								else
                                    g_pCoreShell->TeamOperation(TEAM_OI_INVITE_RESPONSE,__nParam,false);	//nSelAction=0����Ӧ, nSelAction=1���ܾ�

							}
							//(unsigned int)sPlayer
						}
						break;
					case SMCT_UI_RENASCENCE://Touch Down
						{
							if (g_pCoreShell)
								g_pCoreShell->OperationRequest(GOI_PLAYER_RENASCENCE,0,true);//nSelAction=0��ԭ������, nSelAction=1���س�	
							buttonCallBackFunc(NULL);
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
				switch (nTbtn->getTag())
				{
				case SMCT_UI_TONG_JOIN_APPLY://�������
				case SMCT_UI_TRADE:
				case SMCT_UI_TEAM_APPLY:
				case SMCT_UI_TEAM_INVITE://Touch Down
					{
						buttonCallBackFunc(NULL);
					}
					break;
				case SMCT_UI_RENASCENCE://����
					{
					}
					break;
				default:
					break;
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

//��������ccTouchBegan������true
bool KuiTalkDialog1::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	//CCSetIterator it = pTouch->begin();
	//CCTouch* touch = (CCTouch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	CCPoint location = pTouch->getLocation();//Ŀ�ĵ�����?��Ļ�����	
	switch(__Kind)
	{
	case SMCT_UI_RENASCENCE:
		{//ѡ������
			
		}
		break;
	case SMCT_UI_TEAM_INVITE:
		{
		}
		break;
	default:
		break;
	}
	return true;
}

void KuiTalkDialog1::update(float delta)
{
	//CCMessageBox("����ѭ��","update");
	if (isOpen && g_pCoreShell)
	{
	   // m_nMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
	  //	m_nXu    = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_XU, 0, 0);
	}
}

void KuiTalkDialog1::draw()
{
	//CCMessageBox("����ѭ��","draw");
}

int KuiTalkDialog1::FilterTextColor(char* pMsgBuff, unsigned short nMsgLength)
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