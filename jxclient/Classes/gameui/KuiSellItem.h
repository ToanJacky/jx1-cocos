//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiSellItem__
#define __KuiSellItem__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include <map>
USING_NS_CC;
USING_NS_CC_EXT;

#define FIGHT_SKILL_COUNT_PER_PAGE	25
#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��
#define MAX_ITEM_COUNT  60

class KuiSellItem:public CCLayer{
public: 
	KuiSellItem();
	~KuiSellItem();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiSellItem);

	static KuiSellItem * create(CCObject * callbackListener,SEL_CallFuncN callfun);
	virtual void update(float delta); 
	virtual void draw();
	void	UpdateItem(KUiObjAtRegion* pItem, int bAdd);//��Ʒ�仯
	bool    isOpen;
	void    setCanMove(bool ver){__isCanMove =ver;};
private:
	//���Ӽ���ͼ��
	void addDialogData();
	//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
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

	void tuoguanCallBackFunc(CCObject * pSender);
	void kuaiCallBackFunc(CCObject * pSender);
	void zhengCallBackFunc(CCObject * pSender);
	void gaoCallBackFunc(CCObject * pSender);
	void tanCallBackFunc(CCObject * pSender);
	void biaoCallBackFunc(CCObject * pSender);
	void saveCallBackFunc(CCObject * pSender);
	
	void closesavemonyPadCallback(CCNode *pNode);
	void closegaoPadCallback(CCNode *pNode);
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
	//CCLabelTTF* pMoneyLabel;
	//CCLabelTTF* pXuLabel;
	CCLayerColor * colorLayer;
	//CCAction* red;
	CCMenu * savemenu;

	bool m_descPadIsOpen;
	bool m_EqdescPadIsOpen;
	bool m_gaoPadIsOpen;
	int  m_nMoney;
	int  m_nXu;
	int  m_nNumObjects;
	POINT m_StartPos;//���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	//KuiSellItemdObject m_ItemData[MAX_ITEM_COUNT];//KuiSellItemObject
	int AddObject(KUiDraggedObject* pObject, int nCount);
	typedef std::map<std::string,KUiItemdObject> _ItemData;                 //SkillId,Echance
	_ItemData m_ItemData;
	void closelockPadCallback(CCNode *pNode);
	//int   m_CurSelItemIndex;
	void  closeItemPadCallback(CCNode *pNode);
	void  UpdateData();
	std::string  nstrCurSelItemKey;
	float m_MovePosX;
	bool  m_nIstrue;
	bool  m_savemoneyPadIsOpen;
	unsigned int nBakChickTime;
	char m_ShopName[64];
	int   m_IsSetPriceCount;
	CCMenu * menu;
	bool isDoIt;
	bool m_bScrolling;
	CCPoint m_lastPoint;
	bool __isCanMove;
};

#endif /* defined(__MyGame__PopLayer__) */