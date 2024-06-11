#ifndef __KuiNewPlayer__
#define __KuiNewPlayer__
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
struct KRoleInfonew
{
	char				Name[32];		//����
	unsigned	char	Gender;			//�Ա�	be one of the SSC_CHARACTER_GENDER value
	unsigned	char	Attribute;		//��������
	unsigned short	NativePlaceId;	//������ID
	short			nLevel;			//�ȼ�		
};
class KuiNewPlayer : public cocos2d::CCLayer,public CCRect//,public CCEditBoxDelegate
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	KuiNewPlayer();
	~KuiNewPlayer();
	virtual bool init();  
	virtual void update(float delta); 
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::CCScene* scene();
	CREATE_FUNC(KuiNewPlayer);
	//��дdraw()����
	virtual void draw();
	KImageStore2 m_ImageStore;
	//CCSize visibleSize;
	//CCPoint origin;
private: 
    /*CCLabelTTF* pRoela;
	CCLabelTTF* pRoelb;
	CCLabelTTF* pRoelc;*/
	//====iKNetMsgTargetObject�ӿں���====
	void	AcceptNetMsg(void* pMsgData);	//����������Ϣ
	bool clicked;  //�Ƿ�˫��
	LOGIN_LOGIC_STATUS			m_Status;  //���ͱ�ʶ����
	LOGIN_LOGIC_RESULT_INFO		m_Result; //���ܱ�ʶ����
	KRoleInfonew  m_Info;
	//CCSprite *testSprite;
	//int       nTestCount;
	//UINT      nTimeCount;
	CCMenuItemImage *pLoginItem;
	int       m_nWidth;
	int       m_nHeight;
	int       m_sprCount;
	CCRect getRect(CCNode* pNode);
	void  copyData(const char* pFileName);
	bool  isFileExist(const char* pFileName);
	void  buttonCallback(CCNode *pNode);
	int   GetRoleCount(int& nAdviceChoice);
	// a selector callback
	void mainEnterCallback(CCObject* pSender);
	void CheckClick(float tt);
	void GetRoleImageName(char* pszName, const char* pszPrefix, unsigned char bGender, unsigned char bAttribute, int nIndex);
	long _GetBig5Count(char *str);
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
	void ProcessCreateRoleResponse(tagNewDelRoleResponse* pResponse);	//�½���ɫ�б����Ӧ
	void ProcessToLoginGameServResponse(tagNotifyPlayerLogin* pResponse);//׼����½��Ϸ����������Ӧ
	CCAnimation* creatSpranimation(int nindex,int isLoop=-1,bool isforever=true);
	void finish();
	void mExit(CCObject* pSender);
	void mSellistCallBack(CCObject* pSender);
	int  CreateRole();

	//unsigned char	m_Gender[MAX_PLAYER_PER_ACCOUNT];
	//unsigned char	m_Attribute[MAX_PLAYER_PER_ACCOUNT];
	char		    m_szLoginBg[32];
	char		    m_szPlayerImgPrefix[128];
	int             m_CurSelIndex;
	CCNode *RoleNodeman;   //�нڵ�
	CCNode *RoleNodewoman; //Ů�ڵ�
	CCActionManager* actionManager;
	CCSprite *SelbgSprite;
	CCEditBox *proleNameEditBox;
	struct PROPTYPEINFO
	{
		int					nShowTextLen;
		char				propertyShow[256];		//����˵��
		char				szMaleImg[128];
		char				szFemaleImg[128];
		char				szMaleSound[128];
		char				szSeriesImg[128];
		char				szFemaleSound[128];
	}m_propTypeInfoTable[series_num];
};

#endif // __GAMEWORLD_SCENE_H__
