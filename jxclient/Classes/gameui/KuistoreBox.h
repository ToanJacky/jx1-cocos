//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuistoreBox__
#define __KuistoreBox__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include <map>
USING_NS_CC;
USING_NS_CC_EXT;

//#define FIGHT_SKILL_COUNT_PER_PAGE	25
#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��
//#define MAX_ITEM_COUNT  60

class KuistoreBox:public CCLayer{
public: 
	KuistoreBox();
	~KuistoreBox();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuistoreBox);

	static KuistoreBox * create(char * title,CCObject * callbackListener,SEL_CallFuncN callfun);
	
	virtual void update(float delta); 
	virtual void draw();
	void	UpdateItem(KUiObjAtRegion* pItem, int bAdd);//��Ʒ�仯
	void	UpdateExItem(KUiObjAtRegion* pItem, int bAdd,int exBoxIndex);
	bool    isOpen;
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
	void getmoneyCallBackFunc(CCObject * pSender);
	void buttonCallBackFunc(CCObject * pSender);
	void openboxCallBackFunc(CCObject * pSender);

	void closeItemKuoPadCallback(CCNode *pNode);

	void zenliCallBackFunc(CCObject * pSender);
	void diuCallBackFunc(CCObject * pSender);
	void chuCallBackFunc(CCObject * pSender);
	void ziCallBackFunc(CCObject * pSender);
	void kuo1CallBackFunc(CCObject * pSender);
	void kuo2CallBackFunc(CCObject * pSender);
	void kuo3CallBackFunc(CCObject * pSender);

	//�ϲ����
	CCObject * m_callbackListener;

	//�ص�����
	SEL_CallFuncN m_callfun;
	//�Ի��򱳾���С
	CCPoint m_origin;
	CCSize  m_size;
	CCSize winSize;
	//�Ի��򱳾�����
	CCSprite * m_bgSprite;
	CCSprite *ParentNode_Item;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	CCRect getRect(CCNode* pNode,int i);
	CCLabelTTF* pMoneyLabel;
	//CCLabelTTF* pXuLabel;
	CCLayerColor * colorLayer;
	//CCAction* red;
	//CCMenu * savemenu;

	bool m_descPadIsOpen;
	bool m_getmoneyPadIsOpen;
	int  m_nMoney;
	//int  m_nXu;
	int  m_nNumObjects;
	POINT m_StartPos;//���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	//KuistoreBoxdObject m_ItemData[MAX_ITEM_COUNT];//KuistoreBoxObject
	int AddObject(KUiDraggedObject* pObject, int nCount);
	typedef std::map<std::string,KUiItemdObject> _ItemData;                 //SkillId,Echance
	_ItemData m_ItemData;
	CCSprite *btnBackPic;
	//int   m_CurSelItemIndex;
	void  closeItemPadCallback(CCNode *pNode);
	void  closegetmonyPadCallback(CCNode *pNode);
	void  UpdateData();
	std::string  nstrCurSelItemKey;
	float m_MovePosX;
	unsigned int nBakChickTime;
	bool m_bScrolling;
	CCPoint m_lastPoint;
	bool _m_itemExPadIsOpen;
	CCLayer * sItemExLayer;
	
};

#endif /* defined(__MyGame__PopLayer__) */