//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiTalkDialog1__
#define __KuiTalkDialog1__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
//#define FIGHT_SKILL_COUNT_PER_PAGE	25
//#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��

class KuiTalkDialog1:public CCLayer{
public: 
	KuiTalkDialog1();
	~KuiTalkDialog1();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiTalkDialog1);

	static KuiTalkDialog1 * create(char * pContent, char *OkbtnStr,char *NobtnStr,int nKind,CCObject * callbackListener,SEL_CallFuncN callfun,unsigned int nParam=0,char *strParam=NULL);

	//���Ӽ���ͼ��
	void addDialogData(char * pContent,int nKind);
	void setcoloseButton(char *OkbtnStr,char *NobtnStr,CCObject * callbackListener,SEL_CallFuncN callfun);
	int  getDialogKind(){return __Kind ;};
	virtual void update(float delta); 
	virtual void draw();

	bool    isOpen;
private:
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	//��������ccTouchBegan������true
	bool ccTouchBegan(CCTouch * touch,CCEvent * pevent);
	void touchEvent(CCObject *pSender, TouchEventType type);
	//�رյ�����
	void closePopLayer(CCObject * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(CCObject * pSender);

	//�ϲ����
	CCObject * m_callbackListener;

	//�ص�����
	SEL_CallFuncN m_callfun;

	int  FilterTextColor(char* pMsgBuff, unsigned short nMsgLength);
	//�Ի��򱳾���С
	CCPoint m_origin;
	CCSize  m_size;
	//�Ի��򱳾�����
	CCSprite *ParentNode_ItemEx;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	CCRect getRect(CCNode* pNode,int i);
	//CCLabelTTF* pMoneyLabel;
	//CCLabelTTF* pXuLabel;
	CCLayerColor * colorLayer;
	CCAction* red;
	UILayer *m_pUiLayer;
	int  __Kind;
	unsigned int  __nParam;
	char __strParam[32];
	
};

#endif /* defined(__MyGame__PopLayer__) */