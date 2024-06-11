#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiMyMenu.h"
#include "KuiItem.h"
#include "engine/KIniFile.h"
#include "gamecore/CoreShell.h"
#include "gamecore/KPlayer.h"
#include "engine/KCodec.h"
#include "engine/KCodecLzo.h"
#include "engine/KFile.h"
#include "engine/KFilePath.h"
#include "engine/KList.h"
#include "engine/KMemBase.h"
#include "engine/KMemClass.h"
#include "engine/KMemStack.h"
#include "engine/KNode.h"
#include "engine/KPakFile.h"
#include "engine/KStrBase.h"
#include "engine/KPakList.h"
#include "engine/XPackFile.h"
#include "gamecore/KSkillManager.h"
#include "gameui/KuiSkilldesc.h"
#include "gameui/KCheckbox.h"
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_CC;

KuiMyMenu::KuiMyMenu()
{
	isFollowRole =false;
	active = false;
	isRun  = false;
	radius = 60;
	nRolePoint=ccp(0,0);
	nRoleIndex = 0;

	jsSprite=NULL;
	bgSprite=NULL;
	_BackAttackRadius=0;
	_CurSkillId=0;
	_Beilv=0;
	_useSkillModel=0;
}

KuiMyMenu::~KuiMyMenu()
{
	/*if  (bgSprite)
	    bgSprite->release();
	if (jsSprite)
	    jsSprite->release();*/
}

void KuiMyMenu::updatePos(float dt){
	if  (jsSprite)
	    jsSprite->setPosition(ccpAdd(jsSprite->getPosition(),ccpMult(ccpSub(currentPoint, jsSprite->getPosition()),0.5)));
}
//����ҡ��
void KuiMyMenu::Active()
{
	if (!active)
	{
		active=true;
		this->schedule(schedule_selector(KuiMyMenu::updatePos));                       //���ˢ�º���
		//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,false);
	}else {
	}
}
//���ҡ��
void   KuiMyMenu::Inactive()
{
	if (active) {
		active=false;
		this->unschedule(schedule_selector(KuiMyMenu::updatePos));                 //ɾ��ˢ��
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);//ɾ��ί��
	}else {
	}
}


//�ڶ������������������֮��ļн�
//��Ϊ����ĽǶ����Ԧ�Ϊ��λ����������Ҫת�����Զ�Ϊ��λ
float KuiMyMenu::getAngleSigned()
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

float KuiMyMenu::getRad(CCPoint p1,CCPoint p2)
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
CCPoint KuiMyMenu::getDirection()
{
	return ccpNormalize(ccpSub(centerPoint, currentPoint));
}
//ҡ�˺����ĵ�ľ���
float KuiMyMenu::getVelocity()
{
	return ccpDistance(centerPoint, currentPoint);
}

KuiMyMenu * KuiMyMenu::create(CCMenuItem* item, ...)
{
	va_list args;
	va_start(args,item);
	KuiMyMenu *pRet = new KuiMyMenu();
	if (pRet && pRet->createWithItems(item, args))
	{
		pRet->autorelease();
		va_end(args);
		//��ʼ����һ������ң��
		pRet->setTouchPriority(kCCMenuHandlerPriority);
		pRet->setTouchMode(kCCTouchesOneByOne);
		pRet->setTouchEnabled(true);
		pRet->setEnabled(true);
		pRet->ignoreAnchorPointForPosition(true);
		pRet->setAnchorPoint(ccp(0.5f, 0.5f));
		pRet->m_pSelectedItem = NULL;
		pRet->m_eState = kCCMenuStateWaiting;

		pRet->initWithCenter();
		return pRet;
	}
	va_end(args);
	CC_SAFE_DELETE(pRet);
	return NULL;
}


KuiMyMenu* KuiMyMenu::initWithCenter(){
	
	visibleSize = CCDirector::sharedDirector()->getVisibleSize();//�ɼ���
	//pCoreShell = NULL;
	//__GameWorld = NULL;
	moveEndPoint = ccp(0,0);

	if(!isFollowRole){
		centerPoint =ccp(visibleSize.width,-100.0f);
	}else{
		centerPoint =ccp(visibleSize.width,-100.0f);  //��������
	}
	currentPoint = centerPoint; //�ƶ���ǰ������

	/*ccColor4B color = ccc4(112, 128, 144, 150);
	colorLayer = CCLayerColor::create(color);//��ɫ��
	colorLayer->setContentSize(CCSizeMake(200,200));//���úʹ��ڵĴ�С
	colorLayer->setPosition(CCPointMake(visibleSize.width-200,10));
	colorLayer->setAnchorPoint(ccp(0,0));
	//colorLayer->addChild(spriteBox);
	this->addChild(colorLayer);*/

	bgSprite = CCSprite::create("useskill_b.png");
	bgSprite->setPosition(centerPoint);   //����spr
	jsSprite = CCSprite::create("useskill_z.png");
	jsSprite->setPosition(centerPoint);//����spr
	//bgSprite->setTag(1000);
	//bgSprite->retain();
	//jsSprite->retain();
	this->addChild(bgSprite);
	this->addChild(jsSprite);
	if(isFollowRole){
		//this->setVisible(false);
		setSpriteVisible(false);
	}
	this->Active();//����ҡ��
	//this->setTouchMode(kCCTouchesOneByOne); //��ֹ��㴥��
	return this;
}


KuiMyMenu* KuiMyMenu::menuWithItem(CCMenuItem* item)
{
	return create(item, NULL);
}


// ��дregisterWithTouchDispatcher
void KuiMyMenu::registerWithTouchDispatcher()
{
	//�������ȼ���Ϊ1��ֻҪ��CCScrollView��tableView��CCControlButton�ȣ���Щ���ȼ���Ϊ0���;Ϳ���
	//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-129,true);
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, this->getTouchPriority(),true);//false
}


bool KuiMyMenu::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	//��ִ��ң��
	//CCMessageBox("KuiMyMenu::ccTouchBegan","CCTouch");

	if (!active)
		return false;
	if (g_GameWorld && g_pCoreShell && g_pCoreShell->getTradeState()) return false;

	_useSkillModel=g_GameWorld->getUseSkillModel();//0 Ϊ�Զ� 1 Ϊ�ֶ�

	//-------------------------------------------------
	CC_UNUSED_PARAM(event);
	if (m_eState != kCCMenuStateWaiting || !isEnabled())
	{
		return false;
	}

	for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
	{
		if (c->isVisible() == false)
		{
			return false;
		}
	}

	m_pSelectedItem = this->itemForTouch(touch);
	if (m_pSelectedItem)
	{
		//CCMessageBox("m_pSelectedItem Began","m_pSelectedItem");
		if (_useSkillModel==0) //������Զ� ��ִ��
		   m_pSelectedItem->activate();//ִ�лص�����

		m_eState = kCCMenuStateTrackingTouch;
		if (isCheckBox && m_pSelectedItem->GetTagbyKey()=="box")//
		{
			if (m_pSelectedItem->isSelected())
				m_pSelectedItem->unselected();
			else
				m_pSelectedItem->selected();
		}
		else
			m_pSelectedItem->selected();

		setSpriteOrder(0);
		setSpritePiont(m_pSelectedItem->getPosition());
		setSpriteVisible(true);
		setSpriteOpacity(105);
		if (_useSkillModel==1) //��������Զ� ��ִ��
		{
			g_GameWorld->setAttackSprPosition(ccp(visibleSize.width/2,visibleSize.height/2));//nRolePoint
			g_GameWorld->setAttackSprVisible(true);
		}

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
			bgSprite->setPosition(currentPoint);  //this->getChildByTag(1000)
		}
		//��ȡ���ܵķ�Χ
		if (_useSkillModel==1) //��������Զ� ��ִ��
		{
		  _CurSkillId = m_pSelectedItem->getTag();
		  if (_CurSkillId<=0 || _CurSkillId>=MAX_SKILL) return false;
		  ISkill * pOrdinSkill = g_SkillManager.GetSkill(_CurSkillId, 1);
		  if (pOrdinSkill && !pOrdinSkill->IsAura())
		  {
			_BackAttackRadius = pOrdinSkill->getBackAttackRadius();
		  }
		  _Beilv=_BackAttackRadius/radius;
		}
		return true;
	}
	return false;
}

void KuiMyMenu::ccTouchMoved(CCTouch* touch, CCEvent* event)
{

	if (g_GameWorld && g_pCoreShell && g_pCoreShell->getTradeState()) return;
	//if (pCoreShell) pCoreShell->setPadCanMove(PAD_KIND_ITEM,false);
	//int nBeilv = 2;
	CCPoint touchPoint = touch->getLocationInView();                     //��Ļ����ϵ +x ���� +������
	touchPoint = CCDirector:: sharedDirector()->convertToGL(touchPoint); //ת�������½�gl�������(0,0)
	float nDistance = ccpDistance(touchPoint, centerPoint);
	if (nDistance > radius)
	{//������Χ
		currentPoint =ccpAdd(centerPoint,ccpMult(ccpNormalize(ccpSub(touchPoint, centerPoint)), radius));
	}else {
		currentPoint = touchPoint;
	}

	CC_UNUSED_PARAM(event);
	CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchMoved] -- invalid state");
	CCMenuItem *currentItem = this->itemForTouch(touch);
	if (currentItem != m_pSelectedItem) 
	{
		if (m_pSelectedItem)
		{
			m_pSelectedItem->unselected();
		}
		m_pSelectedItem = currentItem;
		if (m_pSelectedItem)
		{
			m_pSelectedItem->selected();
		}
		//---�������ͬ�İ�ť�ͷ���
		return;
	}
	
	//��ʼ��⼼���ƶ���λ�� ������������
	//currentPoint ��ǰ������λ�� centerPoint����Բ���ԭʼλ��
	float nVer = getAngleSigned();  //���ĵ��봥����ĽǶ�
	float nDestPontX,nDestPontY;
	
	if (nVer==0)
	{//����
		nDestPontX = nRolePoint.x+(currentPoint.x-centerPoint.x)*_Beilv;//Ŀ��gl����x y����
		nDestPontY = /*visibleSize.height-*/nRolePoint.y;
	}
	else if(nVer>0 && nVer<90)
	{//����
		nDestPontX = nRolePoint.x+(currentPoint.x-centerPoint.x)*_Beilv;//Ŀ��gl����x y����
		nDestPontY = /*visibleSize.height-*/(nRolePoint.y+(currentPoint.y-centerPoint.y)*_Beilv);
	}
	else if (nVer==90)
	{//����
		nDestPontX = nRolePoint.x;//Ŀ��gl����x y����
		nDestPontY = /*visibleSize.height-*/(nRolePoint.y+(currentPoint.y-centerPoint.y)*_Beilv);
	}
	else if (nVer>90 && nVer<180)
	{//����
		nDestPontX = nRolePoint.x-(centerPoint.x-currentPoint.x)*_Beilv;//Ŀ��gl����x y����
		nDestPontY = /*visibleSize.height-*/(nRolePoint.y+(currentPoint.y-centerPoint.y)*_Beilv);
	}
	else if (nVer==180)
	{//����
		nDestPontX = nRolePoint.x-(centerPoint.x-currentPoint.x)*_Beilv;//Ŀ��gl����x y����
		nDestPontY = /*visibleSize.height-*/nRolePoint.y;
	}
	else if (nVer>180 && nVer<270)
	{//����
		nDestPontX = nRolePoint.x-(centerPoint.x-currentPoint.x)*_Beilv;//Ŀ��gl����x y����
		nDestPontY = /*visibleSize.height-*/(nRolePoint.y-(centerPoint.y-currentPoint.y)*_Beilv);
	}
	else if (nVer==270)
	{//����
		nDestPontX = nRolePoint.x;//Ŀ��gl����x y����
		nDestPontY = /*visibleSize.height-*/(nRolePoint.y-(centerPoint.y-currentPoint.y)*_Beilv);
	}
	else if (nVer>270 && nVer<360)
	{//����
		nDestPontX = nRolePoint.x+(currentPoint.x-centerPoint.x)*_Beilv;//
		nDestPontY = /*visibleSize.height-*/(nRolePoint.y-(centerPoint.y-currentPoint.y)*_Beilv);
	}
	else if (nVer==360)
	{//����
		nDestPontX = nRolePoint.x+(currentPoint.x-centerPoint.x)*_Beilv;//
		nDestPontY = /*visibleSize.height-*/nRolePoint.y;
	}
	isRun  = false;
	moveEndPoint.x = nDestPontX;
	moveEndPoint.y = nDestPontY;
	if (_useSkillModel==1) //��������Զ� ��ִ��
	{
		g_GameWorld->setAttackSprPosition(moveEndPoint);
	}
	
	//int nX = nDestPontX;
	//int nY = nDestPontY;
	//int nZ = 0;

	//SubWorld[0].GetMpsByLocalPosition(nX, nY, nZ);  //����Ļ���� ���� ת����������
	//g_pCoreShell->setNpcDir(nX,nY);
	//g_pCoreShell->GotoWhere(nDestPontX,nDestPontY,0); //������������
}

void KuiMyMenu::ccTouchEnded(CCTouch* touch, CCEvent* event)
{

	if (g_GameWorld && g_pCoreShell && g_pCoreShell->getTradeState()) return;

	//if (pCoreShell) pCoreShell->setPadCanMove(PAD_KIND_ITEM,true);
	currentPoint = centerPoint;
	if(isFollowRole){
	//this->setVisible(false);
	   setSpriteVisible(false);
	}
	isRun = true;
	//this->setVisible(false);
	//setSpriteOpacity(100);
	setSpritePiont(ccp(visibleSize.width,-100.0f));

	g_GameWorld->setAttackSprVisible(false);
	//this->Inactive();
	//CCMessageBox("�ƶ�����","�ƶ�����");
	
	CC_UNUSED_PARAM(touch);
	CC_UNUSED_PARAM(event);
	CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchEnded] -- invalid state");
	if (m_pSelectedItem)
	{
		if (isCheckBox)
		{//�����ʹbox
			if  (m_pSelectedItem->GetTagbyKey()!="box")
				m_pSelectedItem->unselected();
		}
		else
			m_pSelectedItem->unselected();

		//m_pSelectedItem->activate();//ִ�лص�����
	}
	m_eState = kCCMenuStateWaiting;

	//�ֶ����ƿ�ʼ����
	if (_useSkillModel==1) //������ֶ� ��ִ��
	{
		g_GameWorld->customSkillAttack(_CurSkillId,moveEndPoint.x,visibleSize.height-moveEndPoint.y);
		_Beilv=0;
		_BackAttackRadius=0;
		_useSkillModel=0;
		_CurSkillId=0;
	}
}

void KuiMyMenu::ccTouchCancelled(CCTouch *touch, CCEvent* event)
{
	CC_UNUSED_PARAM(touch);
	CC_UNUSED_PARAM(event);
	CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchCancelled] -- invalid state");
	if (m_pSelectedItem)
	{
		m_pSelectedItem->unselected();
	}
	m_eState = kCCMenuStateWaiting;
}


void  KuiMyMenu::setSpriteOpacity(int ver)
{
	if (jsSprite)
		jsSprite->setOpacity(ver);
	if (bgSprite)
		bgSprite->setOpacity(ver);
}

void  KuiMyMenu::setSpriteVisible(bool ver)
{
	if (jsSprite)
		jsSprite->setVisible(ver);
	if (bgSprite)
		bgSprite->setVisible(ver);

}

void  KuiMyMenu::setSpriteOrder(int ver)
{
	if  (ver<=0)
	{
		if (bgSprite)
			bgSprite->setZOrder(zOrder);
		if (jsSprite)
			jsSprite->setZOrder(zOrder);
		
	}
	else
	{
		if (bgSprite)
			bgSprite->setZOrder(ver);
		if (jsSprite)
			jsSprite->setZOrder(ver);
	}
}

void  KuiMyMenu::setSpritePiont(CCPoint aPoint)
{
	this->Active();
	centerPoint  = aPoint;
	currentPoint = centerPoint;              //�ƶ���ǰ������
	if (jsSprite)
		jsSprite->setPosition(centerPoint);   //����spr
	if (bgSprite)
		bgSprite->setPosition(centerPoint);   //����spr
}