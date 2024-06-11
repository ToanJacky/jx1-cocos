//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiAccreg__
#define __KuiAccreg__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

//#define FIGHT_SKILL_COUNT_PER_PAGE	25
//#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��
#define TIME_OUT     5           //���峬ʱʱ��
class KuiAccreg:public CCLayer{
public: 
	KuiAccreg();
	~KuiAccreg();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiAccreg);

	static KuiAccreg * create(CCObject * callbackListener,SEL_CallFuncN callfun);
	virtual void update(float delta); 
	virtual void draw();

	virtual void editBoxEditingDidBegin(CCEditBox *editBox); //��ʼʱ�ĺ���
	virtual void editBoxEditingDidEnd(CCEditBox *editBox);   //����ʱ�ĺ���
	virtual void editBoxReturn(CCEditBox *editBox);          //����������س�ʱ�Ĵ���
	virtual void editBoxTextChanged(CCEditBox *editBox, const std::string &text); //���ָ��ĺ���

	bool    isOpen;
private:
	//���Ӽ���ͼ��
	void addDialogData();
	void setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun);
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
	void mianLoginCallback(CCObject* pSender);
	void mianCancelCallback(CCObject* pSender);
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

	void colseSockets();
	int  m_AnswerCount;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	CCEditBox * AccNode;
	CCEditBox * PasNode;
	CCEditBox * ePasNode;
	SOCKET s;
	unsigned int nport;
	std::string  naccIp;
	std::string m_WritablePath;
	
};

#endif /* defined(__MyGame__PopLayer__) */