//
//  KuiMylistView.h
//  KuiMylistView
//
//  Created by �ᵶ����Ц QQ:25557432
//
//
#ifndef __KuiMylistView__
#define __KuiMylistView__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

using namespace ui;
class KuiMylistView:public ListView{
public: 
	KuiMylistView();
	~KuiMylistView();
	//��ʼ��
	static KuiMylistView* create();
	
	CCPoint getMoveEndPos(){return moveEndPoint;};
	void    setSpritePiont(CCPoint aPoint);
	void    setSpriteVisible(bool ver);
	void    setSpriteOpacity(int ver);

	bool    getIsRun(){return isRun;};
	void    setIsRun(bool nisRun){isRun=nisRun;};
	void    setRoleIndex(int nIdx){nRoleIndex=nIdx;};
	void    setRolePoint(CCPoint nPoint){nRolePoint=nPoint;};
private:
	//��д����ע�ắ�������¸�����������
	virtual void registerWithTouchDispatcher();
	float getAngleSigned();
	float getRad(CCPoint p1,CCPoint p2);
	KuiMylistView * initWithCenter();
	//��������ccTouchBegan������true
	bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	void ccTouchEnded(CCTouch* touch, CCEvent* event);
	void ccTouchCancelled(CCTouch *touch, CCEvent* event);
	void ccTouchMoved(CCTouch* touch, CCEvent* event);

	CCPoint nRolePoint;
	int     nRoleIndex;

	CCPoint moveEndPoint;
	CCPoint centerPoint; //ҡ������
	CCPoint currentPoint;//ҡ�˵�ǰλ��
	bool    isRun;
	bool    active;//�Ƿ񼤻�ҡ��
	float   radius;//ҡ�˰뾶
	CCSprite *jsSprite;
	CCSprite *bgSprite;
	bool    isFollowRole;//�Ƿ�����û����

	int     _BackAttackRadius;
	int     _CurSkillId;
	float   _Beilv;
	int     _useSkillModel;
};

#endif /* defined(__KuiMylistView__) */
