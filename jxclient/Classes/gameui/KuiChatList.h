//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiChatList__
#define __KuiChatList__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
#include "Kuiplayerfun.h"
//#define FIGHT_SKILL_COUNT_PER_PAGE	25

class KuiChatList:public CCLayer{
public: 
	KuiChatList();
	~KuiChatList();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiChatList);

	static KuiChatList * create(CCObject * callbackListener,SEL_CallFuncN callfun);
	//���Ӽ���ͼ��
	void addDialogData();
	void addmsg(char * sendName,char * contentt,int nKind=-1,int nColor=0,int nPackage=-1);
	int	 NewChannelMessageArrival(DWORD nChannelID, char* szSendName,char* pMsgBuff, unsigned short nMsgLength,DWORD nItemDwidx=0,char *scrSendName=NULL,int nPackage=-1);
	void setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun);

	virtual void update(float delta); 
	virtual void draw();
	void _removeDelegate(bool isThis);
	bool    isOpen;
	void _setPlayerFun(Kuiplayerfun * ver){nPlayerFun=ver;};
private:
	Kuiplayerfun *nPlayerFun;
	void selectedItemEvent(CCObject* pSender, ListViewEventType type);
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();
	//��������ccTouchBegan������true
	bool ccTouchBegan(CCTouch * touch,CCEvent * pevent);

	void buttonCallBackFunc(CCObject * pSender);
	//�رյ�����
	void closePopLayer(CCObject * pSender);

	//�ϲ����
	CCObject * m_callbackListener;

	//�ص�����
	SEL_CallFuncN m_callfun;
	//�Ի��򱳾���С
	CCPoint m_origin;
	CCSize  m_size;
	CCSize winSize;
	//�Ի��򱳾�����
	//CCSprite * m_bgSprite;
	CCLayerColor  *ParentNode_Team;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	CCRect getRect(CCNode* pNode,int i);
	//CCLabelTTF* pMoneyLabel;
	//CCLabelTTF* pXuLabel;
	//CCLayerColor * colorLayer;
	CCAction* red;

	//int  m_nMoney;
	//int  m_nXu;
	//int  m_nNumObjects;
	//POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	UILayer *m_pUiLayer;
	//Layout  *m_pWidget;  
	//CCArray* m_array;
	UIListView* listView;
	CCSize btnSize;
	CCSize bgSize;
	//int    msgCount;
	
};

#endif /* defined(__MyGame__PopLayer__) */