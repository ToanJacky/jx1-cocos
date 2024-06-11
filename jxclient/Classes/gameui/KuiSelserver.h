#ifndef __GAME_KuiSelserver_H__
#define __GAME_KuiSelserver_H__
/*
//��½���桡�����ʺ�����
*/
#include "cocos2d.h"
#include "cocos-ext.h"
#include <gameui/Uiglobaldata.h>
#include<list>
#include "ImageStore/KImageStore2.h"
#include "engine/KIniFile.h"
#include "engine/Kgetinidata.h"
#include "physics_nodes/CCPhysicsSprite.h" 

#include "engine/KGbktoUtf8.h"
#include "engine/KTimer.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;
using namespace ui;

class KuiSelserver : public CCLayer//,public CCEditBoxDelegate
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	KuiSelserver();
	~KuiSelserver();
	virtual bool init();  
	static cocos2d::CCScene* scene();
	
	CREATE_FUNC(KuiSelserver);
	//��дdraw()����
	virtual void draw();
	virtual void update(float delta); 
private: 
	LOGIN_LOGIC_STATUS			m_Status;  //���ͱ�ʶ����
	LOGIN_LOGIC_RESULT_INFO		m_Result; //���ܱ�ʶ����

	CCAnimate* animate;

	CCSprite *testSprite;
	int       m_nWidth;
	int       m_nHeight;
	int       m_sprCount;
	void  copyData(const char* pFileName);
	bool  isFileExist(const char* pFileName);
	void  selServerCallBack(CCObject* menuItem);
	// a selector callback
	void mianLoginCallback(CCObject* pSender);
	void mianCancelCallback(CCObject* pSender);

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
	virtual void didAccelerate(CCAcceleration* pAccelerationValue);
	void    finish();//������ɺ� ִ��ɾ������
private:
	int     nserCount;
	int     nCurSelIndex;
	KIniFile nserlist;
};

#endif // __GAMEWORLD_SCENE_H__
