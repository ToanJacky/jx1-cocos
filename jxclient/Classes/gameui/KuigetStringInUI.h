//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuigetStringInUI__
#define __KuigetStringInUI__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
#define FIGHT_SKILL_COUNT_PER_PAGE	25
#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��

class KuigetStringInUI:public CCLayer, public cocos2d::extension::CCEditBoxDelegate
{
public: 
	KuigetStringInUI();
	~KuigetStringInUI();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuigetStringInUI);
	static KuigetStringInUI * create(char* pszTitle,int nMinLen, int nMaxLen,const char* pszCallback,const char* pszError,int bNum,CCObject * callbackListener,SEL_CallFuncN callfun,int outKind=0);

	virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox);
	virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox);
	virtual void editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text);
	virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox);

	virtual void update(float delta); 
	virtual void draw();

	bool    isOpen;
private:
	//���Ӽ���ͼ��
	void addDialogData(char* pszTitle);
	//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
	void setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun);
	/*void editBoxEditingDidBegin(CCEditBox *editBox); //��ʼʱ�ĺ���
	void editBoxEditingDidEnd(CCEditBox *editBox); //����ʱ�ĺ���
	void editBoxReturn(CCEditBox *editBox); //����������س�ʱ�Ĵ���
	void editBoxTextChanged(CCEditBox *editBox, const std::string &text); //���ָ��ĺ���
	*/
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	//��������ccTouchBegan������true
	bool ccTouchBegan(CCTouch * touch,CCEvent * pevent);

	//�رյ�����
	void closePopLayer(CCObject * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(CCObject * pSender);
	void getstringgCallBackFunc(CCObject * pSender);
	void _getstringgCallBackFunc(CCObject * pSender);

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
	CCLabelTTF* ptitleLabel;
	//CCLabelTTF* pXuLabel;
	CCLayerColor * colorLayer;
	CCAction* red;

	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	CCEditBox *pMoneyEditBox;
	//char strTitle[64];
	char strCallBackfun[64];
	char strErrorfun[16];
	int  minMsgLen;
	int  maxMsgLen;
	int  inPutKind;
	int  outPutKind;
};

#endif /* defined(__MyGame__PopLayer__) */