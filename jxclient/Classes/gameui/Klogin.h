#ifndef __GAME_KLOGIN_H__
#define __GAME_KLOGIN_H__
/*
//��½���桡�����ʺ�����
*/
#include "cocos2d.h"
#include "cocos-ext.h"
#include <gameui/Uiglobaldata.h>
#include<list>
#include "ImageStore/KImageStore2.h"
#include "engine/Kgetinidata.h"
#include "physics_nodes/CCPhysicsSprite.h" 

#include "engine/KGbktoUtf8.h"
#include "engine/KTimer.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;

class Klogin : public CCLayer//,public CCEditBoxDelegate
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	
	virtual bool init();  
	/*virtual void onEnter();
    virtual void onExit();
    virtual void onEnterTransitionDidFinish();
   */
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::CCScene* scene();
	
	//CCLabelTTF* GetTestLabel(CCObject* pSender){return pMainLabel;};
	// implement the "static node()" method manually
	CREATE_FUNC(Klogin);
	//��дdraw()����
	virtual void draw();
	virtual void update(float delta); 
	//KImageStore2 m_ImageStore;
private: 
	//====iKNetMsgTargetObject�ӿں���====
	void	AcceptNetMsg(void* pMsgData);	//����������Ϣ
	LOGIN_LOGIC_STATUS			m_Status;  //���ͱ�ʶ����
	LOGIN_LOGIC_RESULT_INFO		m_Result; //���ܱ�ʶ����

	CCAnimate* animate;
	CCLabelTTF* pMainLabel;
	
	CCSprite *testSprite;
	int       nTestCount;
	UINT      nTimeCount;
	int       m_nWidth;
	int       m_nHeight;
	int       m_sprCount;
	void  copyData(const char* pFileName);
	bool  isFileExist(const char* pFileName);
	// a selector callback
	void mianSkillCallback(CCObject* pSender);
	void mianLoginCallback(CCObject* pSender);
	void mianCancelCallback(CCObject* pSender);
	// default implements are used to call script callback if exist
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	// default implements are used to call script callback if exist
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
	virtual void didAccelerate(CCAcceleration* pAccelerationValue);

	void   finish();//������ɺ� ִ��ɾ������
	void   mExit(CCObject* pSender);
	void	GetAccountPassword(char* pszAccount, KSG_PASSWORD* pPassword);
	void	SetAccountPassword(const char* pszAccount, const KSG_PASSWORD* pcPassword);
	void	ClearAccountPassword(bool bAccount, bool bPassword);
private:
	//====��������������Ӧ�ķ��ش���====
	void	ProcessAccountLoginResponse(KLoginStructHead* pResponse);		//�����˺ŵ�½����Ӧ
	void	ProcessRoleListResponse(TProcessData* pResponse);				//��ɫ�б���
};

#endif // __GAMEWORLD_SCENE_H__
