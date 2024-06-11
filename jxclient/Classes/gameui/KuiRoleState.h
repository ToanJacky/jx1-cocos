//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiRoleState__
#define __KuiRoleState__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

#define FIGHT_SKILL_COUNT_PER_PAGE	25
#define ITEM_CELL_SIZE	74   //ÿ�����ӵĸ߶ȺͿ��
#define ITEM_CELL_SIZE_SMALL	55   //ÿ�����ӵĸ߶ȺͿ��
#define _ITEM_COUNT 15
class KuiRoleState:public CCLayer{
public: 
	KuiRoleState();
	~KuiRoleState();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiRoleState);

	static KuiRoleState * create(char * title,CCObject * callbackListener,SEL_CallFuncN callfun);

	virtual void update(float delta); 
	virtual void draw();

	bool    isOpen;
	void    setCanMove(bool ver){__isCanMove =ver;};
private:
	//���Ӽ���ͼ��
	void addDialogData();
	void setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun);
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	//��������ccTouchBegan������true
	bool ccTouchBegan(CCTouch * touch,CCEvent * pevent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	//�رյ�����
	void closePopLayer(CCObject * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(CCObject * pSender);

	void buttonEqCallBackFunc(CCObject * pSender);
	void buttonxiuCallBackFunc(CCObject * pSender);
	void addCallBackFunc(CCObject * pSender);

	//�ϲ����
	CCObject * m_callbackListener;
	KTabFile nVipInfo;
	//�ص�����
	SEL_CallFuncN m_callfun;
	//�Ի��򱳾���С
	CCPoint m_origin;
	//CCPoint diff;
	CCSize  m_size;
	CCSize winSize;
	//�Ի��򱳾�����
	CCSprite * m_bgSprite;
	CCSprite *ParentNode_Team;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	CCRect getRect(CCNode* pNode,int i);
	void closedescPadCallback(CCNode *pNode);

	CCLabelTTF* pRoleName;
	CCLabelTTF* pRoleTitle;
	CCLabelTTF* pRoleTitle_1;
	CCLabelTTF* pRoleVip;
	CCLabelTTF* pFuYuan;
	CCLabelTTF* pShenWang;
	CCLabelTTF* proelLevel;
	CCLabelTTF* pPaiMing;

	CCLabelTTF* pLife;
	CCLabelTTF* pMana;
	CCLabelTTF* pTili;

	CCLabelTTF* pSex;
	CCLabelTTF* pExp;
	CCLabelTTF* pLeftAct;
	CCLabelTTF* pRigthAct;

	CCLabelTTF* pMingZhong;
	CCLabelTTF* pShanBi;
	CCLabelTTF* pRunSpeed;
	CCLabelTTF* pActpeed;
	CCLabelTTF* pMicCount;

	CCLabelTTF* pLiLiang;
	CCLabelTTF* pWauGong;
	CCLabelTTF* pNeiGong;
	CCLabelTTF* pShenFa;

	CCLabelTTF* pPuFang;
	CCLabelTTF* pBingFang;
	CCLabelTTF* pLeiFang;
	CCLabelTTF* pHuoFang;
	CCLabelTTF* pDuFang;

	CCLabelTTF* pLucky;
	CCLabelTTF* pPk;

	CCLabelTTF* pZhuanSheng;
	CCLabelTTF* pZhuanShengLevel;

	CCLabelTTF* pTemDescItem;

	CCEditBox * liEditBox;
	CCEditBox * waiEditBox;
	CCEditBox * shenEditBox;
	CCEditBox * neiEditBox;
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
	void UpdateBaseData();
	void BreatheUpdateInfo();
	void BreathAttributeUpdate();
	int  __RemainPoint;
	bool m_bScrolling;
	CCPoint m_lastPoint;
	unsigned int nBakChickTime;
	bool __isCanMove;
	CCSprite * m_ExeProcessSprite;
	int _VipLevel;
	bool nIsOpenTxt;
};

#endif /* defined(__MyGame__PopLayer__) */