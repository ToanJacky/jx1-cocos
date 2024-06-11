//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiCometoMap__
#define __KuiCometoMap__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "gameui/KuiShowSprite.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
//#define FIGHT_SKILL_COUNT_PER_PAGE	25
//#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��
#define _MAPMAX_COUNT 20
class KuiCometoMap:public CCLayer{
public: 
	KuiCometoMap();
	~KuiCometoMap();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiCometoMap);

	static KuiCometoMap * create(CCObject * callbackListener,SEL_CallFuncN callfun);

	//���Ӽ���ͼ��
	void addDialogData();
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
	void  closeShowPad(CCNode *pNode);
	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(CCObject * pSender);
	void movemapCallBack(CCObject* menuItem);
	//�ϲ����
	CCObject * m_callbackListener;

	void UpdateData();
	//�ص�����
	SEL_CallFuncN m_callfun;

	void touchPageBtnEvent(CCObject *pSender, TouchEventType type);
	void touchAwardBtnEvent(CCObject *pSender, TouchEventType type);
	int  FilterTextColor(char* pMsgBuff, unsigned short nMsgLength);
	void setBtnTouchEnble(int index,bool enble);
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
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	int  m_MaxCount;
	int  m_MapCount;
	CCLabelTTF *m_recommend[_MAPMAX_COUNT];
	CCLabelTTF *m_TimeInfo;;
	UILayer *m_pMainUiLayer;
	Layout  *m_pWidget;

	UIButton* btnPageBox_1;//��ҳ��ť
	UIButton* btnPageBox_2;
	UIButton* btnPageBox_3;
	UIButton* btnPageBox_4;
	UIButton* btnPageBox_5;

	UILayer * _PageLayer_1;//��ҳ���
	UILayer * _PageLayer_2;
	UILayer * _PageLayer_3; 
	UILayer * _PageLayer_4;
	UILayer * _PageLayer_5;

	UIButton* m_AwardButton_yl;//����/����
	UIButton* m_AwardButton_fs;//����/��Ե
	UIButton* m_AwardButton_jb;//���
	UIButton* m_AwardButton_jf;//����

	CCSprite * m_timeProcessSprite;
	KuiShowSprite * nShowLayer;

	int _nCurTime; //(��λ������)
	int _nMoneyExpTime; //��������� 0
	int _nFuYuanTime;   //��Ե������ 1
	int _nJiFenTime;    //����       3
	int _nJinBiTime;    //���       2
	
	int _curPage;
};

#endif /* defined(__MyGame__PopLayer__) */