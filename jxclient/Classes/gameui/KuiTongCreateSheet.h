//
//  KuiTongCreateSheet.h
//  KuiTongCreateSheet
//
//  Created by �ᵶ����Ц QQ:25557432.
//
//
#ifndef __KuiTongCreateSheet__
#define __KuiTongCreateSheet__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class KuiTongCreateSheet:public CCLayer{
public: 
	KuiTongCreateSheet();
	~KuiTongCreateSheet();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiTongCreateSheet);

	static KuiTongCreateSheet * create(char * title,CCObject * callbackListener,SEL_CallFuncN callfun);
	
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
	void btnCallBackFunc(CCObject * pSender);
	

	//�ϲ����
	CCObject * m_callbackListener;

	//�ص�����
	SEL_CallFuncN m_callfun;
	//�Ի��򱳾���С
	CCPoint m_origin;
	CCSize  m_size;
	CCSize  winSize;
	//�Ի��򱳾�����
	//CCSprite * m_bgSprite;
	CCSprite *ParentNode_Task;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	CCRect getRect(CCNode* pNode,int i);
	CCLabelTTF* pTongNameLabel;
	CCLabelTTF* pCampTitleLabel;
	CCLabelTTF* pCampInfoLabel;
	CCLayerColor * colorLayer;
	CCAction* red;
	UILayer *m_pMainUiLayer;
	CCEditBox *pNameEditBox;
	UIButton* btnBox_zp;
	UIButton* btnBox_zl;
	UIButton* btnBox_xp;
	CCSize btnSize;
	bool m_bScrolling;
	CCPoint m_lastPoint;
	bool __isCanMove;
    int   __nSelCamp;
};

#endif /* defined(__KuiTongCreateSheet__) */