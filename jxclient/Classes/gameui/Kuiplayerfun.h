//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __Kuiplayerfun__
#define __Kuiplayerfun__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

//#define FIGHT_SKILL_COUNT_PER_PAGE	25
//#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��
enum  SelfChannel   //����Ƶ��
{
	ch_Team = 0,
	ch_Faction,
	ch_Tong,
	ch_Screen,//ȫ��
	ch_Msgr,
	ch_GM
};

enum CHANNELRESOURCE
{
	CH_NEARBY,
	CH_TEAM,
	CH_WORLD,
	CH_FACTION,
	CH_TONG,
	CH_SYSTEM,
	CH_CITY,
	CH_MSGR,
	CH_CHATROOM,
};

enum LISTAG_CHAT
{
	CH_ALL_LIST=1,
	CH_SHIJIE_LIST,
	CH_CHENGSHI_LIST,
	CH_DUIWU_LIST,
	CH_SILIAO_LIST,
	CH_BANGPAI_LIST,
	CH_COUNT_LIST,
};

#define MAX_CHANNELRESOURCE 8  //Ƶ��������

struct KChannelResourceInfo
{
	char cTitle[32];
	char cShortTitle[MAX_CHANNELRESOURCE][32];
	KRColor uTextColor;
	KRColor uTextBorderColor;
	WORD nMenuPicIndex;
	WORD nMenuPicHeight;
	char cMenuText[32];
	WORD nMenuDeactivatePicIndex;
	WORD nMenuDeactivatePicHeight;
	char cMenuDeactivateText[32];
	KRColor uMenuBkColor;
	WORD nTextPicIndex;
	char cFormatName[32];	        //����Ƶ������
	int nNeverClose;
	char szSoundFileName[80];
	unsigned int uSendMsgInterval;	//���ڷ�����Ϣ��ʱ����(����)
	unsigned int uSendMsgNum;	    //���ڻ����������Ϣ�����Ŀ,���10��

};

struct KChannelActivateInfo
{
	DWORD nChannelID;               //Ƶ��ID
	BYTE cost;
	bool bSubscribe;
	int ResourceIndex;	            //-1��ʾ����Դ
	unsigned int uLastSendMsgTime;	//�ϴη���Ϣ��ʱ��
	unsigned int uLeftSendMsgNum;	//�Ѿ�����Ĵ�����Ϣ
	unsigned int uBufferOffset;	    //���ÿռ����ʼλ��
	char Buffer[1024 * 10];         //�����Ϣ��
};

struct KMSN_ChannelInfo
{
	KRColor uTextColorSelf;
	KRColor uTextBorderColorSelf;
	WORD nSelfTextPicIndex;
	KRColor uTextBKColorSelf;

	KRColor uTextFriendColor;
	KRColor uTextBorderFriendColor;
	WORD nFriendMenuPicIndex;
	WORD nFriendMenuPicHeight;
	KRColor uFriendMenuBkColor;
	WORD nFriendTextPicIndex;
	char szFriendSoundFileName[80];

	KRColor uTextColorUnknown;
	KRColor uTextBorderColorUnknown;
	WORD nStrangerMenuPicIndex;
	WORD nStrangerMenuPicHeight;
	KRColor uStrangerMenuBkColor;
	WORD nStrangerTextPicIndex;
};
struct KPlayer_Chat_Tab
{
	char	szChatTabName[32];	 //Ƶ�����֣�
	int		nId;
};

#define MAX_CHAT_TAB 9   //����Ƶ������

class Kuiplayerfun:public CCLayer{
public: 
	Kuiplayerfun();
	~Kuiplayerfun();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(Kuiplayerfun);

	static Kuiplayerfun * create(CCObject * callbackListener,SEL_CallFuncN callfun);
	
	virtual void update(float delta); 
	virtual void draw();

	bool    isOpen;
	void	UpdateData(KUiPlayerTeam* pInfo);

	void	QueryAllChannel();
	void	ReplaceChannelName(char* szDest, size_t nDestSize, char* szSrc);
	int	    ReleaseActivateChannelAll();
	void	OpenChannel(char* channelName, DWORD nChannelID, BYTE cost);
	int	    PushChannelData(DWORD dwID, const char* Buffer, int nLen);
	int	    GetChannelData(DWORD& dwID, BYTE& cost, char*& Buffer, int& nLen,int& nIiemDwIdx);
	int   	PopChannelData(DWORD dwID);	
	void    _removeDelegate(bool isThis);
	DWORD   GetChannelCount(); // ȡ��Ƶ��������
	int	    GetChannelIndex(DWORD dwID);
	int	    GetChannelIndex(char* channelName);
	bool    IsChannelType(int nChannelIndex, SelfChannel type);
	char*   GetChannelTitle(int nChannelIndex);
	DWORD   GetChannelID(int nChannelIndex);
	int     GetChannelResourceIndex(int nChannelIndex);
	int     CheckChannel(int nChannelIndex, bool b);
	bool    GetChannelSubscribe(int nChannelIndex);
	int		NewChannelMessageArrival(DWORD nChannelID, char* szSendName,char* pMsgBuff, unsigned short nMsgLength,DWORD nItemDwidx=0,char *scrSendName=NULL,int nPackage=-1);
	void    ShowMSNMessage(char* szName, const char* pMsgBuff, unsigned short nMsgLength,DWORD nItemDwidx,bool isClient=false);
	int	    FindActivateChannelIndex(DWORD nChannelID);

	void    setCanMove(bool ver){__isCanMove =ver;};
private:
	//���Ӽ���ͼ��
	void setBtnTouchEnabled(UIListView*listView,bool mEnabled);
	void addDialogData();
	void setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun);
	void selectedItemEvent(CCObject* pSender, ListViewEventType type);
	void selectedPlayersListItemEvent(CCObject* pSender, ListViewEventType type);
	void selectedBoxStateEvent(CCObject* pSender, CheckBoxEventType type);
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	void oktouchEvent(CCObject *pSender, TouchEventType type);
	//��������ccTouchBegan������true
	bool ccTouchBegan(CCTouch * touch,CCEvent * pevent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	//�رյ�����
	void closePopLayer(CCObject * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(CCObject * pSender);
	void btnCallBackFunc(CCObject * pSender);
	
	void Clear();
	void OnRefresh();					//ˢ��
	void touchPageBtnEvent(CCObject *pSender, TouchEventType type);
	//�ϲ����
	CCObject * m_callbackListener;
	UIButton* btnPageBox_q;//ȫ��
	UIButton* btnPageBox_s;//����
	UIButton* btnPageBox_c;//����
	UIButton* btnPageBox_d;//����
	UIButton* btnPageBox_sl; //˽��
	UIButton* btnPageBox_m;//����
	UIButton* btnPageBox_b;//����
	//�ص�����
	SEL_CallFuncN m_callfun;
	//�Ի��򱳾���С
	CCPoint m_origin;
	CCSize  m_size;
	CCSize winSize;
	CCSize  bgSize;
	//�Ի��򱳾�����
	//CCSprite * m_bgSprite;
	CCSprite *ParentNode_Team;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	CCRect getRect(CCNode* pNode,int i);
	//CCLabelTTF* pMoneyLabel;
	//CCLabelTTF* pXLabel;
	CCLayerColor * colorLayer;
	CCAction* red;

	//int  m_nMoney;
	//int  m_nXu;
	//int  m_nNumObjects;
	//POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	UILayer *m_pUiLayer;
	//Layout  *m_pWidget;  
	//CCArray* m_array;
	UIListView* pNearbyPlayerslistView;
	UIListView* pListView_all;
	UIListView* pListView_shijie;
	UIListView* pListView_chengshi;
	UIListView* pListView_duiwu;
	UIListView* pListView_siliao;
    UIListView* pListView_bangpai;

	UICheckBox* checkBox;
	CCSize btnSize;
	KUiPlayerTeam	m_Info;
	KUiPlayerItem*	m_pPlayersList;
	KUiPlayerItem*	m_pNearbyPlayersList;
	int   __nSelIndex;
	int   __playrSelIndex;
	int   __playerListCount;
	int   __NearbyListCount;

	CCEditBox *pfindEditBox;
	//����ϵͳ
	KChannelResourceInfo m_ChannelsResource[MAX_CHANNELRESOURCE];
	int m_nChannelsResource;
	char m_DefaultChannelSendName[32];
	int m_nDefaultChannelResource;
	KChannelActivateInfo* m_pActivateChannel;
	int m_nActivateChannels;  //�Ѿ������Ƶ���˵�
	unsigned int m_uLastDelMsgTime;
	int	 IsNeverCloseActivateChannel(int nChannelIndex);
	

	bool ReplaceSpecialField(char* szDest, char* szSrc);

	int	FindActivateChannelResourceIndex(char* cTitle);
	int	FindChannelResourceIndex(char* cTitle);
	int	FindActivateChannelIndexByKey(char* cKey);
	BYTE FindActivateChannelCost(DWORD nChannelID);
	void SendQueryChannelID(int nChannelResourceIndex);
	void SendChannelSubscribe(int nChannelIndex, bool b);
	int	 AddActivateChannel(const KChannelActivateInfo& Item);	//��������Item��Index
	int	 FindActivateChannelIndexById(DWORD nChannelID);
	int	 FindActivateChannelIndex(char* cTitle);
	
	void addchatmsg(char * typeName,char * sendName,char * contentt,UIListView*listView,int nKind,int nColor=0,unsigned int nItemDwidx=0,char*scrSendName=NULL,int nPackage=0);

	bool m_bScrolling;
	CCPoint m_lastPoint;
	bool __isCanMove;
};

#endif /* defined(__MyGame__PopLayer__) */