//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiMyMenu__
#define __KuiMyMenu__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class KuiMyMenu:public CCMenu{
public: 
	KuiMyMenu();
	~KuiMyMenu();
	//��ʼ��
	//virtual bool init();
	//����
	//CREATE_FUNC(KuiMyMenu);
	/** creates a CCMenu with it's items */
	static KuiMyMenu* create(CCMenuItem* item, ...);
	static KuiMyMenu* menuWithItem(CCMenuItem* item);
	// ��дregisterWithTouchDispatcher
	virtual void registerWithTouchDispatcher();

	CCPoint getMoveEndPos(){return moveEndPoint;};
	void    setSpritePiont(CCPoint aPoint);
	void    setSpriteVisible(bool ver);
	void    setSpriteOpacity(int ver);

	bool    getIsRun(){return isRun;};
	void    setIsRun(bool nisRun){isRun=nisRun;};
	void    setRoleIndex(int nIdx){nRoleIndex=nIdx;};
	void    setRolePoint(CCPoint nPoint){nRolePoint=nPoint;};
	void    setSpriteOrder(int ver);
private:
	//����ҡ��
	void Active();
	//���ҡ��
	void Inactive();
	void updatePos(float dt);  //CCTime

	CCPoint getDirection();
	float   getAngleSigned();
	float   getRad(CCPoint p1,CCPoint p2);
	float   getVelocity();
	//��д����ע�ắ�������¸�����������
	//void registerWithTouchDispatcher();
	KuiMyMenu * initWithCenter();
	//��������ccTouchBegan������true
	/*bool ccTouchBegan(CCTouch * touch,CCEvent * pevent);
	void ccTouchEnded(CCTouch* touch, CCEvent* pevent);
	void ccTouchCancelled(CCTouch *touch, CCEvent* pevent);
	void ccTouchMoved(CCTouch* touch, CCEvent* pevent);*/
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

#endif /* defined(__MyGame__PopLayer__) */
