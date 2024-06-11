//
//  KuiShowSprite.h
//  KuiShowSprite
//
//  Created by �ᵶ����Ц QQ:25557432
//
//
#ifndef __KuiShowSprite__
#define __KuiShowSprite__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "gamescene/KScenePlaceC.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
//#include "Kuiplayerfun.h"
//#define FIGHT_SKILL_COUNT_PER_PAGE	25

class KuiShowSprite:public CCLayer{
public: 
	KuiShowSprite();
	~KuiShowSprite();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiShowSprite);

	static KuiShowSprite * create(char * nFilePath,int nXpos,int nYpos,CCObject * callbackListener,SEL_CallFuncN callfun,bool nTouchesModel=true,int nRenderFlag=0);
	//���Ӽ���ͼ��
	void addDialogData();
	void mianDialogSet(char * nFilePath,int nXpos,int nYpos,bool nTouchesModel=true,int nRenderFlag=0);
	void setcoloseButton(CCObject * callbackListener,SEL_CallFuncN callfun);
	virtual void update(float delta); 
	virtual void draw();
	bool    isOpen;
	CCSize getShowSize();
	void   removeShowSprite();
	void   setShowPosition(int nPosX,int nPosY);
	void   setRegisterWithTouchDispatcher(int nVer);
	//void _setPlayerFun(Kuiplayerfun * ver){nPlayerFun=ver;};
private:
	//��д����ע�ắ�������¸�����������
	//void registerWithTouchDispatcher();
	//��������ccTouchBegan������true
	CCTexture2D * ccgetTxtData_s(char *sprPath,int nFramIndex,int nMpsX,int nMpsY,int nMpsZ,int *mScreeX,int *mScreeY,int bRenderFlag,int bSinglePlaneCoord,int *mHeadw,int *mHeadh);
	bool ccTouchBegan(CCTouch *pTouch, CCEvent * pevent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	
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
	CCSize  winSize;
	//�Ի��򱳾�����
	CCLayerColor *ParentNode_Team;
	CCRect getRect(CCNode* pNode,int i);
	CCLabelTTF* ptestLabel;
	CCAction* red;
	
	//UILayer *m_pUiLayer;
	//CCEditBox *pstrEditBox_d;
	CCSize bgSize;
	CCSize nTextureSize;
	bool m_bScrolling;
    float nRoleDisX;
    float nRoleDisY;
	CCPoint m_lastPoint;
	CCSprite *sFarmsSprite;
	CCTexture2D *sFarmsTexture; 
	unsigned int __loopTime;
	CCLabelBMFont *pPointLabel;
	int    _nFarms;
	int    _curFarms;
	int    _nModel;
	char   nSprNamePath[128];
	int		_nInterval;		        //֡���
	unsigned int _nFlipTime;		//��ǰ֡�л�������ʱ��
	int   _nXpos;
	int   _nYpos;
	int   _pHwidth;
	int   _pHheight;
	int   _bRenderFlag;
	//KUiImageRef m_Image;
	
};

#endif /* defined(__MyGame__PopLayer__) */