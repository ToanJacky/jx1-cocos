//  ɽ����ͼ����
//  KuiMoveToMap.h
//  KuiMoveToMap
//
#ifndef __KuiMoveToMap__
#define __KuiMoveToMap__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
#include "Kuiplayerfun.h"
//#define FIGHT_SKILL_COUNT_PER_PAGE	25
#define MAX_COUNT_MAPS	120
struct mapListData
{
	UIButton* m_MoveMapBtn;
	int       m_SubwordId;
	int       m_PosX;
	int       m_PosY;
};

class KuiMoveToMap:public CCLayer{
public: 
	KuiMoveToMap();
	~KuiMoveToMap();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiMoveToMap);
	static KuiMoveToMap * create(char * title,CCObject * callbackListener,SEL_CallFuncN callfun);
	virtual void update(float delta); 
	virtual void draw();
	bool    isOpen;
	//void _setPlayerFun(Kuiplayerfun * ver){nPlayerFun=ver;};
private:
	//���Ӽ���ͼ��
	void addDialogData();
	void setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun);

	void oktouchEvent(CCObject *pSender, TouchEventType type);
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();
	//��������ccTouchBegan������true
	bool ccTouchBegan(CCTouch *pTouch, CCEvent * pevent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

	//�رյ�����
	void closePopLayer(CCObject * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(CCObject * pSender);
	//�ϲ����
	CCObject * m_callbackListener;

	//�ص�����
	SEL_CallFuncN m_callfun;
	//�Ի��򱳾���С
	CCPoint m_origin;
	CCSize  m_size;
	CCSize  winSize;

	CCLayerColor *ParentNode_Team;
	CCRect getRect(CCNode* pNode,int i);

	CCAction* red;
	UILayer *m_pUiLayer;
	CCSize bgSize;
	CCSize nTextureSize;
	CCSprite *nMapPicSpr;
	bool m_bScrolling;
	CCPoint m_lastPoint;
	CCClippingNode *clipper;
	CCSprite *content_Map;
	//int    msgCount;
	int m_PosCount;
	mapListData m_MoveMapBtn[MAX_COUNT_MAPS];
};

#endif /* defined(__MyGame__PopLayer__) */