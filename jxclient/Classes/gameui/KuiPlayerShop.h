//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiPlayerShop__
#define __KuiPlayerShop__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

#define FIGHT_SKILL_COUNT_PER_PAGE	25
#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��
#define PAGE_MAX_COUNT_ITEM	60
class KuiPlayerShop:public CCLayer{
public: 
	KuiPlayerShop();
	~KuiPlayerShop();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiPlayerShop);

	static KuiPlayerShop * create(KUiPlayerItem* pDest,CCObject * callbackListener,SEL_CallFuncN callfun);
	void UpdateItem();
	virtual void update(float delta); 
	virtual void draw();

	bool    isOpen;
private:
	void UpdateData(KUiPlayerItem* pDest);
	//���Ӽ���ͼ��
	void addDialoaData();
	void setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun);
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	//��������ccTouchBegan������true
	bool ccTouchBegan(CCTouch * touch,CCEvent * pevent);

	//�رյ�����
	void closePopLayer(CCObject * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buyCallBackFunc(CCObject * pSender);
	//��һҳ
	void backCallBackFunc(CCObject * pSender);
	//��һҳ
	void nextCallBackFunc(CCObject * pSender);
	void closeCallBackFunc(CCObject * pSender);
	void closedescPadCallback(CCNode *pNode);
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
	void SetPage(int nIndex);
	CCRect getRect(CCNode* pNode,int i);
    int    AddObject(KUiDraggedObject* pObject, int nCount);
	void	Clear();
	CCLabelTTF* pNameLabel;
	//CCLabelTTF* pXuLabel;
	CCLayerColor * colorLayer;
	//CCAction* red;
	CCNode * ItemNode;//һ����
	//int  m_nMoney;
	//int  m_nXu;
	//int  m_nNumObjects;
	int m_nObjCount;
	//int m_CurShopIdx;
	int m_nPageCount;
	int	m_nCurrentPage;  //��ǰҳ
	int	m_nCurrentCount;
	int m_ItemIndex[PAGE_MAX_COUNT_ITEM];
	KUiObjAtContRegion* m_pObjsList;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	bool m_descPadIsOpen;
	int m_selItemIndex;
	CCEditBox *pstrEditBox_count;
	KUiPlayerItem m_Dest;
	int	m_nMark;
	unsigned int nBakChickTime;
};

#endif /* defined(__MyGame__PopLayer__) */