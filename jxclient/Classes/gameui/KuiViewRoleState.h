//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiViewRoleState__
#define __KuiViewRoleState__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

#define FIGHT_SKILL_COUNT_PER_PAGE	25
#define ITEM_CELL_SIZE	27   //ÿ�����ӵĸ߶ȺͿ��
#define _ITEM_COUNT 15
class KuiViewRoleState:public CCLayer{
public: 
	KuiViewRoleState();
	~KuiViewRoleState();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiViewRoleState);

	static KuiViewRoleState * create(KUiPlayerItem* pDest,CCObject * callbackListener,SEL_CallFuncN callfun);
	bool    isOpen;
private:
	virtual void update(float delta); 
	virtual void draw();

	void  UpdateData(KUiPlayerItem* pDest);
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
	//�Ի��򱳾���С
	CCPoint m_origin;
	CCSize  m_size;
	//�Ի��򱳾�����
	CCSprite * m_bgSprite;
	CCSprite *ParentNode_Team;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	CCRect getRect(CCNode* pNode,int i);
	void closedescPadCallback(CCNode *pNode);

	CCLabelTTF* pRoleName;
	CCLabelTTF* pRoleTitle;
	//CCLabelTTF* pRoleTitle_1;
	
	CCLabelBMFont* pPk;

	CCLabelTTF* pZhuanSheng;
	CCLabelTTF* pZhuanShengLevel;

	CCLayerColor * colorLayer;
	CCAction* red;
	CCNode * ItemBoxNode;
	int  m_nMoney;
	int  m_nXu;
	int  m_nNumObjects;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	void UpdateAllEquips();
	void UpdateEquip(KUiObjAtRegion* pEquip, int bAdd);
	void addpicBox(int index,int nItemIndx,bool isAdd);
	bool m_descPadIsOpen;
	int  m_itemIndexBox[_ITEM_COUNT];
	int  m_CurSelItemIndex;
	void UpdateBaseData(KUiPlayerItem* pDest);
	void BreatheUpdateInfo();
	void BreathAttributeUpdate();
	int  __RemainPoint;
	KUiPlayerItem* ncurDest;
	
};

#endif /* defined(__MyGame__PopLayer__) */