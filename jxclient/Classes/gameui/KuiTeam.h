//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiTeam__
#define __KuiTeam__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

//#define FIGHT_SKILL_COUNT_PER_PAGE	25
//#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��

class KuiTeam:public CCLayer{
public: 
	KuiTeam();
	~KuiTeam();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiTeam);

	static KuiTeam * create(char * title,CCObject * callbackListener,SEL_CallFuncN callfun);
	
	virtual void update(float delta); 
	virtual void draw();

	bool    isOpen;
	void	UpdateData(KUiPlayerTeam* pInfo);
	void    setCanMove(bool ver){__isCanMove =ver;};
private:
	//���Ӽ���ͼ��
	void addDialogData();
	void setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun);
	void selectedItemEvent(CCObject* pSender, ListViewEventType type);
	void selectedPlayersListItemEvent(CCObject* pSender, ListViewEventType type);
	void selectedBoxStateEvent(CCObject* pSender, CheckBoxEventType type);
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

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
	CCSprite *ParentNode_Team;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	CCRect getRect(CCNode* pNode,int i);
	CCLabelTTF* pMoneyLabel;
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
	UIListView* pPlayersListView;
	UICheckBox* checkBox;
	CCSize btnSize;
	KUiPlayerTeam	m_Info;
	KUiPlayerItem*	m_pPlayersList;
	KUiPlayerItem*	m_pNearbyPlayersList;
	int   __nSelIndex;
	int   __playrSelIndex;
	int   __playerListCount;
	int   __NearbyListCount;
	bool m_bScrolling;
	CCPoint m_lastPoint;
	bool __isCanMove;
};

#endif /* defined(__MyGame__PopLayer__) */