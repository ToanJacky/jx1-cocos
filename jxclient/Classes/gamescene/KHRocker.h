
#include "cocos2d.h"
#include "gamecore/CoreShell.h"
//#include "KgameWorld.h"
using namespace cocos2d;

class HRocker :public CCLayer {
public :
	//��ʼ�� aPoint��ҡ������ aRadius��ҡ�˰뾶 aJsSprite��ҡ�˿��Ƶ� aJsBg��ҡ�˱���
	static HRocker*  HRockerWithCenter(CCPoint aPoint ,float aRadius ,CCSprite* aJsSprite,CCSprite* aJsBg,bool _isFollowRole);
	//����ҡ��
	void Active();
	//���ҡ��
	void Inactive();
	CCPoint getDirection();
	float   getAngleSigned();
	float   getRad(CCPoint p1,CCPoint p2);
	bool    getIsRun(){return isRun;};
	void    setIsRun(bool nisRun){isRun=nisRun;};
	void    setRoleIndex(int nIdx){nRoleIndex=nIdx;};
	void    setRolePoint(CCPoint nPoint){nRolePoint=nPoint;};
	void    setCoreShell(iCoreShell * nCoreShell){pCoreShell=nCoreShell;};
	//void    setGameWorld(KgameWorld * nCoreShell){__GameWorld=nCoreShell;};
	CCPoint getMoveEndPos(){return moveEndPoint;};
private:
	HRocker * initWithCenter(CCPoint aPoint ,float aRadius ,CCSprite* aJsSprite,CCSprite* aJsBg,bool _isFollowRole);
	CCPoint centerPoint; //ҡ������
	CCPoint currentPoint;//ҡ�˵�ǰλ��
	bool isRun;
	bool active;//�Ƿ񼤻�ҡ��
	float radius;//ҡ�˰뾶
	CCSprite *jsSprite;
	bool isFollowRole;//�Ƿ�����û����
	float getVelocity();
	void  updatePos(float dt);  //CCTime
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	CCPoint nRolePoint;
	int     nRoleIndex;
	iCoreShell * pCoreShell;
	//KgameWorld *__GameWorld; 
	CCSize  visibleSize;
	CCPoint moveEndPoint;
	CREATE_FUNC(HRocker);
};
