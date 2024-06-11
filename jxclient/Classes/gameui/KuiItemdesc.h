//
//  PopupLayer.h
//  PopupDemo
//  ��Ʒ˵����
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiItemdesc__
#define __KuiItemdesc__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
#define	BUY_SELL_SCALE_DE	1
class KuiItemdesc:public CCLayer{
public: 
	KuiItemdesc();
	~KuiItemdesc();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiItemdesc);

	static KuiItemdesc * create(KUiItemdObject nItemData,CCObject * callbackListener,SEL_CallFuncN callfun,int nShowKind);
	static KuiItemdesc * create(int nshopIndex,KUiItemdObject nItemData,CCObject * callbackListener,SEL_CallFuncN callfun);

	virtual void update(float delta); 
	virtual void draw();

	bool    isOpen;
private:
	//���Ӽ���ͼ��
	void addDialogData();
	void addshopdesc(int nshopIndex);
	void setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun);
	void setcoloseButtonInfo(CCObject * callbackListener,SEL_CallFuncN callfun);
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	//��������ccTouchBegan������true
	bool ccTouchBegan(CCTouch * touch,CCEvent * pevent);

	//�رյ�����
	void closePopLayer(CCObject * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(CCObject * pSender);
	void chaiCallBackFunc(CCObject * pSender);
	void kuaiCallBackFunc(CCObject * pSender);
	void buttonSuDungFunc(CCObject * pSender);
	void buttonThaoFunc(CCObject * pSender);
	void buttonSellItemFunc(CCObject * pSender);
	void buttonVutBoFunc(CCObject * pSender);
	void buttonSendChatFunc(CCObject * pSender);
	void buttonDinhFunc(CCObject * pSender);
	void buttonDinhGiaFunc(CCObject * pSender);
	//�ϲ����
	CCObject * m_callbackListener;
	typedef enum
	{
		LAYOUT_ABSOLUTE,
		LAYOUT_LINEAR_VERTICAL,
		LAYOUT_LINEAR_HORIZONTAL,
		LAYOUT_RELATIVE
	}LayoutType;
	//�ص�����
	SEL_CallFuncN m_callfun;
	//�Ի��򱳾���С
	CCPoint m_origin;
	CCSize  m_size;
	//�Ի��򱳾�����
	CCSprite * m_bgSprite;
	CCLayerColor  *ParentNode_Team;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	CCRect getRect(CCNode* pNode,int i);
	CCLabelTTF* ptitleLabel;//����
	CCLabelTTF* potherLabel;//khong co mau
	CCNode* pdescLabel;     //can set mau
	CCNode* pnameLabel;     //can set mau
	CCLabelTTF* pbaseLabel; //thuoc tinh co ban
	CCNode* prqLabel;   //thuoc tinh yeu cau
	CCNode* sprItem;   //thuoc tinh yeu cau
	CCNode* pmagicLabel;//thuoc tinh phep thuat
	CCLayerColor * colorLayer;
	// UIListView* listView;
	// UILayer *m_pUiLayer;
	UILayer *m_pUiLayer;
	UIListView* listView;
	CCAction* red;
	unsigned int nBakChickTime;
	//int  m_ItemIndex;
	KUiItemdObject m_ItemData;
	//int  m_nMoney;
	//int  m_nXu;
	//int  m_nNumObjects;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	int   m_ShowKind;
	
};

#endif /* defined(__MyGame__PopLayer__) */