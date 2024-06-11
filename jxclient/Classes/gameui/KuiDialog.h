//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiDialog__
#define __KuiDialog__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

//#define FIGHT_SKILL_COUNT_PER_PAGE	25
//#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��

class KuiDialog:public CCLayer{
public: 
	KuiDialog();
	~KuiDialog();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiDialog);

	static KuiDialog * create(KUiQuestionAndAnswer* pContent, KUiNpcSpr* pImage,int nIsTimeClose,CCObject * callbackListener,SEL_CallFuncN callfun);

	//���Ӽ���ͼ��
	void addDialogData(KUiQuestionAndAnswer* pContent, KUiNpcSpr* pImage,int nIsTimeClose);

	void setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun);

	virtual void update(float delta); 
	virtual void draw();

	bool    isOpen;
private:
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	//��������ccTouchBegan������true
	bool ccTouchBegan(CCTouch * touch,CCEvent * pevent);

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
	CCSprite * m_bgSprite;
	CCSprite *ParentNode_ItemEx;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	CCRect getRect(CCNode* pNode,int i);
	//CCLabelTTF* pMoneyLabel;
	//CCLabelTTF* pXuLabel;
	CCLayerColor * colorLayer;
	CCAction* red;

	//int  m_nMoney;
	//int  m_nXu;
	int  m_AnswerCount;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	
};

#endif /* defined(__MyGame__PopLayer__) */