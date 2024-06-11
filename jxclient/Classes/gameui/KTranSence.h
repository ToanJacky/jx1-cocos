#ifndef __GAME_KTRANSENCE_H__
#define __GAME_KTRANSENCE_H__
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
#include "network/Thread.h"
#include "engine/KGbktoUtf8.h"
#include "engine/KTimer.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;

class KTranSence : public CCLayer,protected CThread//,public CCEditBoxDelegate
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	KTranSence();
	~KTranSence();
	virtual bool init();  
	/*virtual void onEnter();
    virtual void onExit();
    virtual void onEnterTransitionDidFinish();
   */
	using CThread::Start;
	virtual void update(float delta); 
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::CCScene* scene();
	CREATE_FUNC(KTranSence);
	//��дdraw()����
	virtual void draw();
	virtual void  Run();
	//KImageStore2 m_ImageStore;
private: 
	void loadSuccess();
	//��ȡ�Ļص�����
	void loadingCallBack(CCObject *obj);
	void updateProgress(float dt);
	// a selector callback
	void mianCancelCallback(CCObject* pSender);
	// implement the "static node()" method manually
	//��ȡ��ʼʱ��Ľ�����
	CCSprite *m_pLoadBarStart;
	//��ȡ���ʱ��Ľ�����
	CCProgressTimer *m_pLoadBarEnd;
	//�߳���صĺ���
	//�ܵļ���ͼƬ��
	int m_nAllLoadResources;
	//��ǰ����ͼƬ��
	int m_nLoadedResources;
	//��ȡ����
	float m_fProgressIndex;
	CCLabelTTF *m_pLabelLoading;
	CCLabelTTF *m_pLabelPercent;
	//	��Ҫ������m_fWaitTime��Ч����ʱ�򣬽���һ����update()��init()�����ʼ����ע��ȡ��������
	//	float m_fWaitTime
	//====iKNetMsgTargetObject�ӿں���====
	void	AcceptNetMsg(void* pMsgData);	//����������Ϣ
	LOGIN_LOGIC_STATUS			m_Status;  //���ͱ�ʶ����
	LOGIN_LOGIC_RESULT_INFO		m_Result; //���ܱ�ʶ����
	CCLabelTTF* pMainLabel;
	//CCLabelTTF* pBugLabel;
	int       m_nWidth;
	int       m_nHeight;
	int       m_sprCount;
	void  copyData(const char* pFileName);
	bool  isFileExist(const char* pFileName);
	//virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

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

	void    startload();//������ɺ� ִ��ɾ������
	void	ClearAccountPassword(bool bAccount, bool bPassword);
	void	ClearCachedata();
	void    mExit(CCObject* pSender);
private:			//��ɫ�б���
	float m_ScaleX;
	float m_ScaleY;
	int   m_NpcIndex; //�ͻ������ǵ�NPC����
	//char nBugInfo[128];
};

#endif // __GAMEWORLD_SCENE_H__
