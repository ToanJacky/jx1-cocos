//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiTalkDialog__
#define __KuiTalkDialog__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

//#define FIGHT_SKILL_COUNT_PER_PAGE	25
//#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��

class KuiTalkDialog:public CCLayer{
public: 
	KuiTalkDialog();
	~KuiTalkDialog();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiTalkDialog);
	//SpeakWords(KUiInformationParam* pWordDataList, int nCount);
	static KuiTalkDialog * create(KUiInformationParam* pWordDataList, int nCount,CCObject * callbackListener,SEL_CallFuncN callfun);

	//���Ӽ���ͼ��
	void addskillspr(KUiInformationParam* pWordDataList, int nCount);
	void setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun);

	virtual void update(float delta); 
	virtual void draw();

	bool    isOpen;
	void	Show(char* pInformation, int nInforLen, const char* pBtnLabel, unsigned int uParam = 0);//��ʾ����
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
	CCLabelTTF* pInformationLabel;
	CCLayerColor * colorLayer;
	CCAction* red;

	//int  m_nMoney;
	//int  m_nXu;
	int  m_AnswerCount;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)

	KUiInformationParam*	m_pWordDataList;
	int						m_nNumWordData;
	int						m_nCurrentWord;
	void      changeMsg();
	
};

#endif /* defined(__MyGame__PopLayer__) */