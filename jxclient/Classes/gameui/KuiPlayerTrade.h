//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiPlayerTrade__
#define __KuiPlayerTrade__
#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��
#define MAX_ITEM_COUNT  60
#define MAX_TRADE_COUNT  32
#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include <map>
USING_NS_CC;
USING_NS_CC_EXT;

//#define FIGHT_SKILL_COUNT_PER_PAGE	25
//#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��

class KuiPlayerTrade:public CCLayer{
public: 
	KuiPlayerTrade();
	~KuiPlayerTrade();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiPlayerTrade);

	static KuiPlayerTrade * create(KUiPlayerItem* pOther,CCObject * callbackListener,SEL_CallFuncN callfun);
	//char* pszTitle, char* pszInitString,const int nNum,const char* pszCallback,const char* pszError

	void	UpdateItem(KUiObjAtRegion* pItem, int bAdd);//��Ʒ�仯
	void	UpdateGiveItem(KUiObjAtRegion* pItem, int bAdd);
	void	UpdateTarGiveItem(KUiObjAtRegion* pObj, int bAdd);
	void	UpdateTradeOperData();
	virtual void update(float delta); 
	virtual void draw();
	void	__ClosePad();
	bool    isOpen;
private:
	//���Ӽ���ͼ��
	void addDialogData();
	void setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun);

	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	//��������ccTouchBegan������true
	bool ccTouchBegan(CCTouch * touch,CCEvent * pevent);

	void  closeItemPadCallback(CCNode *pNode);
	//�رյ�����
	void closePopLayer(CCObject * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(CCObject * pSender);
	void btnCallBackFunc_fangzhi(CCObject * pSender);
	void btnCallBackFunc_quxia(CCObject * pSender);
	void okCallBackFunc(CCObject * pSender);
	void tradeCallBackFunc(CCObject * pSender);
	void zhengCallBackFunc(CCObject * pSender);

	//�ϲ����
	CCObject * m_callbackListener;

	//�ص�����
	SEL_CallFuncN m_callfun;

	int  FilterTextColor(char* pMsgBuff, unsigned short nMsgLength);
	typedef std::map<std::string,KUiItemdObject> _ItemData;                 //SkillId,Echance
	_ItemData m_ItemData;

	typedef std::map<unsigned int,KUiItemdObject> _giveItemData;                 //SkillId,Echance
	_giveItemData m_giveItemData;

	typedef std::map<unsigned int,KUiItemdObject> _targiveItemData;
	_targiveItemData m_targiveItemData;
	
	bool  m_nIstrue;
	int AddObject(KUiDraggedObject* pObject, int nCount);     //�ɲ���
	int AddGiveObject(KUiDraggedObject* pObject, int nindex); //�Լ���� �ɲ���
	int AddtarGiveObject(KUiDraggedObject* pObject, int nindex); //�Լ���� �ɲ���
	void  UpdateItemData();
	//�Ի��򱳾���С
	CCPoint m_origin;
	CCSize  m_size;
	CCSize  winSize;
	//CCSize  itemBoxSize;
	//CCSize  giveBoxSize;
	//�Ի��򱳾�����
	//CCSprite * m_bgSprite;
	CCSprite *ParentNode_ItemEx;

	CCNode *ParentNode_Item;
	CCNode *ParentNode_Give;
	CCNode *ParentNode_tarGive;

	CCMenuItemSprite * okConfirm;
	CCMenuItemSprite * tradeConfirm;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	CCRect getRectItem(CCNode* pNode,int i);
	CCRect getGiveItem(CCNode* pNode,int i);
	CCRect gettarGiveItem(CCNode* pNode,int i);
	void   OnAdjustedMoney();
	void   OnModififyMoney();
	CCLabelTTF* pMoneyLabel;
	CCLabelTTF* ptradeInfo;
	CCLabelTTF* m_OtherMoney;

	CCEditBox *m_SelfMoneyBox;
	//CCLabelTTF* pXuLabel;
	CCLayerColor * colorLayer;
	CCAction* red;
	//CCSprite *btnBackPic;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	POINT m_StartPos_g; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	POINT m_StartPos_t; //�Է���Ʒ��
	int  m_nItemNumObjects;
	std::string nstrCurSelItemKey;
	int inCurSelItemKey;
	bool m_descPadIsOpen;
	int m_itemNum,m_giveNum,m_nNeed,m_targiveNum;
	//char   m_Callback[64];
	//char   m_Error[16];
	unsigned int nBakChickTime;
	int  m_nMoney;
	//int  m_nXu;
	KUiPlayerItem m_OtherData;
	int	 m_bAdjustingMoney;
	int	 m_nTakewithMoney;
	int	 m_nSelfTradeMoney;
	bool __IsLock;
};

#endif /* defined(__MyGame__PopLayer__) */