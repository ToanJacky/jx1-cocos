#ifndef __GAME_KSELPLAYER_H__
#define __GAME_KSELPLAYER_H__
/*
//��½���� ѡ������ ���� ɾ����
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
//using namespace cocos2d::extension;
using namespace std;
struct KRoleInfo
{
	char				Name[32];		//����
	unsigned	char	Gender;			//�Ա�	be one of the SSC_CHARACTER_GENDER value
	unsigned	char	Attribute;		//��������
	unsigned short	NativePlaceId;	//������ID
	short			nLevel;			//�ȼ�		
};
class KSelPlayer : public cocos2d::CCLayer,public CCRect//,public CCEditBoxDelegate
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	
	virtual bool init();  
	/*virtual void onEnter();
    virtual void onExit();
    virtual void onEnterTransitionDidFinish();
   */
	virtual void update(float delta); 
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::CCScene* scene();
	// implement the "static node()" method manually
	CREATE_FUNC(KSelPlayer);
	//��дdraw()����
	virtual void draw();
	KImageStore2 m_ImageStore;
	//CCSize visibleSize;
	//CCPoint origin;
private: 
	//CCLabelTTF*pMainLabel;
	//CCLabelTTF* pBugLabel;
    CCLabelTTF* pRoela;
	CCLabelTTF* pRoelb;
	CCLabelTTF* pRoelc;
	//====iKNetMsgTargetObject�ӿں���====
	void	AcceptNetMsg(void* pMsgData);	//����������Ϣ
	bool clicked;  //�Ƿ�˫��
	LOGIN_LOGIC_STATUS			m_Status;  //���ͱ�ʶ����
	LOGIN_LOGIC_RESULT_INFO		m_Result; //���ܱ�ʶ����
	KRoleInfo  nCurRoleList[MAX_PLAYER_PER_ACCOUNT];
	//CCSprite *testSprite;
	//int       nTestCount;
	//UINT      nTimeCount;
	int       m_nWidth;
	int       m_nHeight;
	int       m_sprCount;
	CCRect getRect(CCNode* pNode);
	void  copyData(const char* pFileName);
	bool  isFileExist(const char* pFileName);
	void  buttonCallback(CCNode *pNode);
	void  EnterGame();
	int   GetRoleCount(int& nAdviceChoice);
	// a selector callback
	void mainEnterCallback(CCObject* pSender);
	void mainNewCallback(CCObject* pSender);
	void mainDelCallback(CCObject* pSender);
	void CheckClick(float tt);
	void GetRoleImageName(char* pszName, const char* pszPrefix, unsigned char bGender, unsigned char bAttribute, int nIndex);

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

	void ProcessToLoginGameServResponse(tagNotifyPlayerLogin* pResponse);//׼����½��Ϸ����������Ӧ

	void finish();
	void mExit(CCObject* pSender);

	unsigned char	m_Gender[MAX_PLAYER_PER_ACCOUNT];
	unsigned char	m_Attribute[MAX_PLAYER_PER_ACCOUNT];
	char		    m_szLoginBg[32];
	char		    m_szPlayerImgPrefix[128];
	int             m_CurSelIndex;
	int   m_NpcIndex; //�ͻ������ǵ�NPC����
	char nBugInfo[128];
	CCNode *RoleNodea;
	CCNode *RoleNodeb;
	CCNode *RoleNodec;
	CCActionManager* actionManager;
	CCSprite *SelbgSprite;
};

#endif // __GAMEWORLD_SCENE_H__
