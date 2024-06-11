//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiSortSkill_r__
#define __KuiSortSkill_r__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

//#define FIGHT_SKILL_COUNT_PER_PAGE	25
//#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��
#define	SKILLTREE_MAX_SKILL_COUNT		65
#define	SKILLTREE_SHORTCUT_SKILL_COUNT	9

class KuiSortSkill_r:public CCLayer{
public: 
	KuiSortSkill_r();
	~KuiSortSkill_r();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiSortSkill_r);

	static KuiSortSkill_r * create(int nKind,CCObject * callbackListener,SEL_CallFuncN callfun);

	virtual void update(float delta); 
	virtual void draw();

	bool    isOpen;
private:
	//���Ӽ���ͼ��
	void addDialogData(int nKind);
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

	void UpdateData();
	//�ص�����
	SEL_CallFuncN m_callfun;

	int  FilterTextColor(char* pMsgBuff, unsigned short nMsgLength);

	void skillCallback(CCObject* pSender);
	void closeSkilldescPadCallback(CCNode *pNode);
	//�Ի��򱳾���С
	CCPoint m_origin;
	CCSize  m_size;
	//�Ի��򱳾�����
	CCSprite * m_bgSprite;
	CCLayerColor * ParentNode_ItemEx;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	CCRect getRect(CCNode* pNode,int i);
	//CCLabelTTF* pMoneyLabel;
	//CCLabelTTF* pXuLabel;
	CCLayerColor * colorLayer;
	CCAction* red;

	int				m_nNumSkills;
	KUiSkillData	m_Skills[SKILLTREE_MAX_SKILL_COUNT];
	//KUiSkillData::nLevel�ڴ˵ĺ����ʾλ�˼�����ʾ�ڵڼ���
	KUiSkillData	ms_ShortcutSkills[SKILLTREE_SHORTCUT_SKILL_COUNT];
	//KUiSkillData::nLevel�ڴ˵ĺ���0��ʾ���ּ��ܣ�1��ʾ���ּ���
	int   _sortCount_r;
	
};

#endif /* defined(__MyGame__PopLayer__) */