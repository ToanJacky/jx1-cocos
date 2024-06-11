//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiMaxMap__
#define __KuiMaxMap__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
#include "Kuiplayerfun.h"
//#define FIGHT_SKILL_COUNT_PER_PAGE	25

class KuiMaxMap:public CCLayer{
public: 
	KuiMaxMap();
	~KuiMaxMap();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiMaxMap);

	static KuiMaxMap * create(char * title,CCObject * callbackListener,SEL_CallFuncN callfun);
	//���Ӽ���ͼ��
	void addDialogData();
	void setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun);
	virtual void update(float delta); 
	virtual void draw();
	bool    isOpen;
	//void _setPlayerFun(Kuiplayerfun * ver){nPlayerFun=ver;};
private:
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
	//�Ի��򱳾�����
	//CCSprite * m_bgSprite;
	CCLayerColor *ParentNode_Team;
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
	CCEditBox *pstrEditBox_d;
	//Layout  *m_pWidget;  
	//CCArray* m_array;
	//UIListView* listView;
	//CCSize btnSize;
	CCSize bgSize;
	CCSize nTextureSize;
	CCSprite *nMapPicSpr;
	bool m_bScrolling;
    float nRoleDisX;
    float nRoleDisY;
	CCPoint m_lastPoint;
	CCClippingNode *clipper;
	CCSprite *nPartNode;
	CCSprite *BtnFlag;
	CCSprite *content_Map;
	unsigned int __loopTime;
	CCLabelBMFont *pPointLabel;
	//int    msgCount;
	
};

#endif /* defined(__MyGame__PopLayer__) */