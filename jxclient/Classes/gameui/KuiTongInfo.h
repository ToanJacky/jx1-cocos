//
//  KuiTongInfo.h
//  KuiTongInfo
//
//  Created by �ᵶ����Ц QQ:25557432.
//
//
#ifndef __KuiTongInfo__
#define __KuiTongInfo__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class KuiTongInfo:public CCLayer{
public: 
	KuiTongInfo();
	~KuiTongInfo();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiTongInfo);

	static KuiTongInfo * create(char * title,CCObject * callbackListener,SEL_CallFuncN callfun);
	
	virtual void update(float delta); 
	virtual void draw();

	bool    isOpen;
	void    setCanMove(bool ver){__isCanMove =ver;};
private:
	//���Ӽ���ͼ��
	void addDialogData();
	void setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun);
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	//��������ccTouchBegan������true
	bool ccTouchBegan(CCTouch * touch,CCEvent * pevent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	//�رյ�����
	void closePopLayer(CCObject * pSender);
	void oktouchEvent(CCObject *pSender, TouchEventType type);
	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(CCObject * pSender);
	void btnCallBackFunc(CCObject * pSender);
	

	//�ϲ����
	CCObject * m_callbackListener;

	//�ص�����
	SEL_CallFuncN m_callfun;
	//�Ի��򱳾���С
	CCPoint m_origin;
	CCSize  m_size;
	CCSize  winSize;
	//�Ի��򱳾�����
	//CCSprite * m_bgSprite;
	CCSprite *ParentNode_Task;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	CCRect getRect(CCNode* pNode,int i);
	//CCLabelTTF* pMoneyLabel;
	//CCLabelTTF* pXLabel;
	CCLayerColor * colorLayer;
	CCAction* red;
	UILayer *m_pMainUiLayer;
	//Layout  *m_pWidget;  
	//CCArray* m_array;
	//UIListView* ptaskMsglistView;
	//UIListView* pmainListView;
	//UICheckBox* checkBox;
	CCSize btnSize;
	//KUiPlayerTeam	m_Info;
	bool m_bScrolling;
	CCPoint m_lastPoint;
	bool __isCanMove;

    int   __nSelIndex;
};

#endif /* defined(__KuiTongInfo__) */