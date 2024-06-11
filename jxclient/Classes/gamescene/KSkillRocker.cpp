//#include "gamecore/KCore.h"
#include "KSkillRocker.h"
#include "gamecore/GameDataDef.h"
#include "gameui/Uiglobaldata.h"
using namespace cocos2d;

void KSkillRocker::updatePos(float dt){
	jsSprite->setPosition(ccpAdd(jsSprite->getPosition(),ccpMult(ccpSub(currentPoint, jsSprite->getPosition()),0.5)));
}
//����ҡ��
void KSkillRocker::Active()
{
	if (!active)
	{
		active=true;
		this->schedule(schedule_selector(KSkillRocker::updatePos));                       //���ˢ�º���
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-129,false);
	}else {
	}
}
//���ҡ��
void   KSkillRocker::Inactive()
{
	if (active) {
		active=false;
		this->unschedule(schedule_selector(KSkillRocker::updatePos));                 //ɾ��ˢ��
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);//ɾ��ί��
	}else {
	}
}

//�ڶ������������������֮��ļн�
//��Ϊ����ĽǶ����Ԧ�Ϊ��λ����������Ҫת�����Զ�Ϊ��λ
float KSkillRocker::getAngleSigned()
{//ccpSub(centerPoint, currentPoint),CCPointMake(0, 1))
	//return CC_RADIANS_TO_DEGREES(ccpAngleSigned(ccpSub(centerPoint, currentPoint),CCPointMake(0, 1)));
	/*  //#define PI 3.141592654    float angle = 180.f / PI * rad; ==���ڽǶ�
	float dx = currentPoint.x - centerPoint.x;  
	float dy = currentPoint.y - centerPoint.y;  
	float t = dx/dy;  

	float  at = atan(t);  
	return at/3.1415926*180; 
	
	// ��ȡ�Ƕ�
	Point p1 = this->getPosition(); //ҡ�˵����ĵ�λ��
	Point p2 = touch->getLocation();//�������λ��
	
	*/  
    float rad = getRad(centerPoint,currentPoint);

	return 180.f/3.141592654*rad;

}

float KSkillRocker::getRad(CCPoint p1,CCPoint p2)
{
	float xx = p2.x - p1.x;
	float yy = p2.y - p1.y;
	// б��
	float xie = sqrt(pow(xx, 2) + pow(yy, 2));
	// yy >= 0 �������� 0 �� �� ֮�䡣(0~180��)
	// yy < 0 �������� �� �� 2�� ֮�䡣(180��~360��)
	float rad = yy >= 0 ? (acos(xx / xie)) : (3.141592654 * 2 - acos(xx / xie));
	return rad;
}
//ҡ�˷�λ
CCPoint KSkillRocker::getDirection()
{
	return ccpNormalize(ccpSub(centerPoint, currentPoint));
}
//ҡ�˺����ĵ�ľ���
float KSkillRocker::getVelocity()
{
	return ccpDistance(centerPoint, currentPoint);
}
KSkillRocker* KSkillRocker:: KSkillRockerWithCenter(CCPoint aPoint ,float aRadius ,CCSprite* aJsSprite,CCSprite* aJsBg,bool _isFollowRole){
	KSkillRocker *jstick=KSkillRocker::create();
	jstick->initWithCenter(aPoint,aRadius,aJsSprite,aJsBg,_isFollowRole);
	return jstick;
}

bool KSkillRocker::ccTouchBegan(CCTouch* touch, CCEvent* event)
{//��ʼ�ƶ�
	if (!active)
		return false;
	//if (pCoreShell) pCoreShell->setPadCanMove(PAD_KIND_ITEM,false);

	if (pCoreShell && pCoreShell->getTradeState()) return false;

	this->setVisible(true);

	setSpriteOpacity(255);
	CCPoint touchPoint = touch->getLocationInView();     //��ȡ����������
	touchPoint = CCDirector:: sharedDirector()->convertToGL(touchPoint); //ת����Ļ����
	if(!isFollowRole){
		if (ccpDistance(touchPoint, centerPoint) > radius){
			return false;
		}
	}
	currentPoint = touchPoint;
	if(isFollowRole){
		centerPoint=currentPoint;
		jsSprite->setPosition(currentPoint);
		this->getChildByTag(1000)->setPosition(currentPoint);
	}
	//CCMessageBox("��ʼ�ƶ���","��ʼ�ƶ���");
	return true;
}
void  KSkillRocker::ccTouchMoved(CCTouch* touch, CCEvent* event)
{//�ƶ���
	if (pCoreShell && pCoreShell->getTradeState()) return;
	//if (pCoreShell) pCoreShell->setPadCanMove(PAD_KIND_ITEM,false);
	int nBeilv = 3;
	CCPoint touchPoint = touch->getLocationInView();                     //��Ļ����ϵ +x ���� +������
	touchPoint = CCDirector:: sharedDirector()->convertToGL(touchPoint); //ת�������½�gl�������(0,0)
	float nDistance = ccpDistance(touchPoint, centerPoint);
	if (nDistance > radius)
	{//������Χ
		currentPoint =ccpAdd(centerPoint,ccpMult(ccpNormalize(ccpSub(touchPoint, centerPoint)), radius));
	}else {
		currentPoint = touchPoint;
	}
	//char nPiontinfo[128]={0};
    /*sprintf(nPiontinfo,"CurrentPointX:%f,CurrentPointY:%f\nCenterPointX:%f,CenterPointY:%f",
		    currentPoint.x,currentPoint.y,centerPoint.x,centerPoint.y);
	CCMessageBox(nPiontinfo,"ҡ���ƶ���");*/
	float nVer = getAngleSigned();  //���ĵ��봥����ľ���
    float nDestPontX,nDestPontY;
	if (nVer==0)
	{//����
        nDestPontX = nRolePoint.x+(currentPoint.x-centerPoint.x)*nBeilv;//Ŀ��gl����x y����
		nDestPontY = visibleSize.height-nRolePoint.y;
	}
	else if(nVer>0 && nVer<90)
	{//����
		nDestPontX = nRolePoint.x+(currentPoint.x-centerPoint.x)*nBeilv;//Ŀ��gl����x y����
		nDestPontY = visibleSize.height-(nRolePoint.y+(currentPoint.y-centerPoint.y)*nBeilv);
	}
	else if (nVer==90)
	{//����
		nDestPontX = nRolePoint.x;//Ŀ��gl����x y����
		nDestPontY = visibleSize.height-(nRolePoint.y+(currentPoint.y-centerPoint.y)*nBeilv);
	}
	else if (nVer>90 && nVer<180)
	{//����
		nDestPontX = nRolePoint.x-(centerPoint.x-currentPoint.x)*nBeilv;//Ŀ��gl����x y����
		nDestPontY = visibleSize.height-(nRolePoint.y+(currentPoint.y-centerPoint.y)*nBeilv);
	}
	else if (nVer==180)
	{//����
		nDestPontX = nRolePoint.x-(centerPoint.x-currentPoint.x)*nBeilv;//Ŀ��gl����x y����
		nDestPontY = visibleSize.height-nRolePoint.y;
	}
	else if (nVer>180 && nVer<270)
	{//����
		nDestPontX = nRolePoint.x-(centerPoint.x-currentPoint.x)*nBeilv;//Ŀ��gl����x y����
		nDestPontY = visibleSize.height-(nRolePoint.y-(centerPoint.y-currentPoint.y)*nBeilv);
	}
	else if (nVer==270)
	{//����
		nDestPontX = nRolePoint.x;//Ŀ��gl����x y����
		nDestPontY = visibleSize.height-(nRolePoint.y-(centerPoint.y-currentPoint.y)*nBeilv);
	}
	else if (nVer>270 && nVer<360)
	{//����
		nDestPontX = nRolePoint.x+(currentPoint.x-centerPoint.x)*nBeilv;//
		nDestPontY = visibleSize.height-(nRolePoint.y-(centerPoint.y-currentPoint.y)*nBeilv);
	}
	else if (nVer==360)
	{//����
		nDestPontX = nRolePoint.x+(currentPoint.x-centerPoint.x)*nBeilv;//
		nDestPontY = visibleSize.height-nRolePoint.y;
	}

	if (pCoreShell == NULL)
		return ;
/*
		pCoreShell->LockSomeoneAction(0);

		pCoreShell->LockObjectAction(0);

	if (pCoreShell)
	{
		isRun  = false;
		moveEndPoint.x = nDestPontX;
		moveEndPoint.y = nDestPontY;
		pCoreShell->GotoWhere(nDestPontX,nDestPontY,0); //������������
	}*/
	
}

void  KSkillRocker::ccTouchEnded(CCTouch* touch, CCEvent* event)
{//�ƶ�����
	if (pCoreShell && pCoreShell->getTradeState()) return;

	//if (pCoreShell) pCoreShell->setPadCanMove(PAD_KIND_ITEM,true);

	currentPoint = centerPoint;
	//if(isFollowRole){
	//this->setVisible(false);
	//}
	//if (pCoreShell)
	//   pCoreShell->setSand();
	isRun = true;
	//this->setVisible(false);
	//setSpriteOpacity(100);
	setSpritePiont(ccp(visibleSize.width,-100.0f));
	//this->Inactive();
	//CCMessageBox("�ƶ�����","�ƶ�����");
}

KSkillRocker* KSkillRocker::initWithCenter(CCPoint aPoint ,float aRadius ,CCSprite* aJsSprite,CCSprite* aJsBg,bool _isFollowRole){
	isFollowRole =_isFollowRole;
	active = false;
	isRun  = false;
	radius = aRadius;
	visibleSize = CCDirector::sharedDirector()->getVisibleSize();//�ɼ���
	pCoreShell = NULL;
	//__GameWorld = NULL;
	moveEndPoint = ccp(0,0);
	if(!_isFollowRole){
		centerPoint =aPoint;
	}else{
		centerPoint =ccp(0,0);  //��������
	}
	currentPoint = centerPoint; //�ƶ���ǰ������

	/*ccColor4B color = ccc4(112, 128, 144, 150);
	colorLayer = CCLayerColor::create(color);//��ɫ��
	colorLayer->setContentSize(CCSizeMake(200,200));//���úʹ��ڵĴ�С
	colorLayer->setPosition(CCPointMake(visibleSize.width-200,10));
	colorLayer->setAnchorPoint(ccp(0,0));
	//colorLayer->addChild(spriteBox);
	this->addChild(colorLayer);
	*/
	jsSprite = aJsSprite;
	jsSprite->setPosition(centerPoint);//����spr
	bgSprite = aJsBg;
	aJsBg->setPosition(centerPoint);   //����spr
	aJsBg->setTag(1000);
	this->addChild(aJsBg);
	this->addChild(jsSprite);
	if(isFollowRole){
		this->setVisible(false);
	}
	//this->Active();//����ҡ��
	//this->setTouchMode(kCCTouchesOneByOne); //��ֹ��㴥��
	return this;
}


void  KSkillRocker::setSpriteOpacity(int ver)
{
	if (jsSprite)
	  jsSprite->setOpacity(ver);
	if (bgSprite)
	  bgSprite->setOpacity(ver);
}

void  KSkillRocker::setSpriteVisible(bool ver)
{
	this->setVisible(ver);

}

void  KSkillRocker::setSpritePiont(CCPoint aPoint)
{
	this->Active();
	centerPoint  = aPoint;
	currentPoint = centerPoint;              //�ƶ���ǰ������
    if (jsSprite)
	   jsSprite->setPosition(centerPoint);   //����spr
	if (bgSprite)
	   bgSprite->setPosition(centerPoint);   //����spr
}