//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiItemKuo__
#define __KuiItemKuo__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��

class KuiItemKuo:public CCLayer{
public: 
	KuiItemKuo();
	~KuiItemKuo();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiItemKuo);

	static KuiItemKuo * create(CCObject * callbackListener,SEL_CallFuncN callfun);
	
	virtual void update(float delta); 
	virtual void draw();

	bool    isOpen;
	void    UpdateData(int nExboxIndex);
	void	UpdateItem(KUiObjAtRegion* pItem, int bAdd,int nExBoxIndex);//��Ʒ�仯
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
	void buttonCallBackFunc_1(CCObject * pSender);
	void buttonCallBackFunc_2(CCObject * pSender);
	void buttonCallBackFunc_3(CCObject * pSender);
	void qiCallBackFunc(CCObject * pSender);
	void quCallBackFunc(CCObject * pSender);
	void zenliCallBackFunc(CCObject * pSender);
	void closeItemPadCallback(CCNode *pNode);
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
	CCSprite *ParentNode_ItemEx;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	CCRect getRect(CCNode* pNode,int i);
	//CCLabelTTF* pMoneyLabel;
	//CCLabelTTF* pXuLabel;
	CCLayerColor * colorLayer;
	CCAction* red;

	typedef std::map<std::string,KUiItemdObject> _ItemData;                 //SkillId,Echance
	_ItemData m_ItemData;

	int  m_nIstrue;
	int  m_nNumObjects;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	std::string nstrCurSelItemKey;
	bool m_descPadIsOpen;
	int AddObject(KUiDraggedObject* pObject, int nCount,int ExBoxIndex);
	CCSprite *btnBackPic;
	unsigned int nBakChickTime;
	bool m_bScrolling;
	CCPoint m_lastPoint;
	int nCurExPadIndex;
};

#endif /* defined(__MyGame__PopLayer__) */