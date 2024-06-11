//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuigetMoney__
#define __KuigetMoney__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

//#define FIGHT_SKILL_COUNT_PER_PAGE	25
//#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��

class KuigetMoney:public CCLayer{
public: 
	KuigetMoney();
	~KuigetMoney();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuigetMoney); //��ʼ��

	static KuigetMoney * create(int nMoney,int nMaxMoney,int nKind,CCObject * callbackListener,SEL_CallFuncN callfun);

	virtual void update(float delta); 
	virtual void draw();

	bool    isOpen;
private:
	//���Ӽ���ͼ��
	void addDialogData();
	//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
	void setConfirmCancelButton(CCObject * callbackListener,SEL_CallFuncN callfun);
	void setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun);

	void editBoxEditingDidBegin(CCEditBox *editBox); //��ʼʱ�ĺ���
	void editBoxEditingDidEnd(CCEditBox *editBox); //����ʱ�ĺ���
	void editBoxReturn(CCEditBox *editBox); //����������س�ʱ�Ĵ���
	void editBoxTextChanged(CCEditBox *editBox, const std::string &text); //���ָ��ĺ���
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	//��������ccTouchBegan������true
	bool ccTouchBegan(CCTouch * touch,CCEvent * pevent);

	//�رյ�����
	void closePopLayer(CCObject * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(CCObject * pSender);
	void getmoneyCallBackFunc(CCObject * pSender);

	//�ϲ����
	CCObject * m_callbackListener;

	//�ص�����
	SEL_CallFuncN m_callfun;
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
	//CCAction* red;

	//int  m_nMoney;
	//int  m_nXu;
	//int  m_nNumObjects;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	CCEditBox *pMoneyEditBox;
    int  nKind;
	unsigned int seveMoney;
};

#endif /* defined(__MyGame__PopLayer__) */