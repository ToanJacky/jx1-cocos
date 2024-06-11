//
//  PopupLayer.h
//  PopupDemo
//  ��Ʒ˵����
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiSkilldesc__
#define __KuiSkilldesc__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class KuiSkilldesc:public CCLayer{
public: 
	KuiSkilldesc();
	~KuiSkilldesc();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiSkilldesc);

	static KuiSkilldesc * create(int nskillId,int nLevel,KUiItemdObject nKillData,CCObject * callbackListener,SEL_CallFuncN callfun,int nKind=0);

	virtual void update(float delta); 
	virtual void draw();

	bool    isOpen;
private:
	//���Ӽ���ͼ��
	void addDialogData();
	//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
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
	CCLayerColor  *ParentNode_Team;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	CCRect getRect(CCNode* pNode,int i);
	CCLabelTTF* ptitleLabel;//����
	CCLabelTTF* potherLabel;//������Ҫ����ɫ���õ�
	CCNode* pdescLabel;     //��Ҫ������ɫ��
	CCLabelTTF* pbaseLabel; //��������
	CCLabelTTF* pScriptLabel; //�ű�˵��
	CCLabelTTF* pScriptLabel_n;
	CCNode* prqLabel;       //��������
	CCNode* pmagicLabel;//ħ������
	CCLayerColor * colorLayer;
	CCAction* red;

	//int  m_ItemIndex;
	KUiItemdObject skillData;
	int _nKind;
	//int  m_nMoney;
	//int  m_nXu;
	//int  m_nNumObjects;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	
};

#endif /* defined(__MyGame__PopLayer__) */