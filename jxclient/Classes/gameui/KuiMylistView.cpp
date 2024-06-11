#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiMylistView.h"
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

KuiMylistView::KuiMylistView()
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

KuiMylistView::~KuiMylistView()
{
}

//�ڶ������������������֮��ļн�
//��Ϊ����ĽǶ����Ԧ�Ϊ��λ����������Ҫת�����Զ�Ϊ��λ
float KuiMylistView::getAngleSigned()
{//ccpSub(centerPoint, currentPoint),CCPointMake(0, 1)) 
    float rad = getRad(centerPoint,currentPoint);

	return 180.f/3.141592654*rad;

}

float KuiMylistView::getRad(CCPoint p1,CCPoint p2)
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


KuiMylistView * KuiMylistView::create()
{
	KuiMylistView *widget = new KuiMylistView();
	if (widget && widget->init())
	{
		widget->autorelease();

		//widget->setTouchMode(kCCTouchesOneByOne);
		widget->setTouchEnabled(true);
		widget->setEnabled(true);
		widget->ignoreAnchorPointForPosition(true);
		widget->setAnchorPoint(ccp(0.5f, 0.5f));
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return NULL;
}


KuiMylistView* KuiMylistView::initWithCenter(){
	
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

    jsSprite = CCSprite::create("useskill_z.png");
	jsSprite->setPosition(centerPoint);//����spr
	bgSprite = CCSprite::create("useskill_b.png");
	bgSprite->setPosition(centerPoint);   //����spr
	//bgSprite->setTag(1000);
	//bgSprite->retain();
	//jsSprite->retain();
	this->addChild(bgSprite);
	this->addChild(jsSprite);
	if(isFollowRole){
		//this->setVisible(false);
		setSpriteVisible(false);
	}
	//this->setTouchMode(kCCTouchesOneByOne); //��ֹ��㴥��
	return this;
}


/*KuiMylistView* KuiMylistView::menuWithItem(CCMenuItem* item)
{
	return create(item, NULL);
}*/


// ��дregisterWithTouchDispatcher
void KuiMylistView::registerWithTouchDispatcher()
{
	//�������ȼ���Ϊ1��ֻҪ��CCScrollView��tableView��CCControlButton�ȣ���Щ���ȼ���Ϊ0���;Ϳ���
	//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-129,true);
	//CCDirector* pDirector = CCDirector::sharedDirector();
	//pDirector->getTouchDispatcher()->addTargetedDelegate(this, this->getTouchPriority(),true);//false
}


bool KuiMylistView::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	if (g_GameWorld && g_pCoreShell && g_pCoreShell->getTradeState()) return false;

	_useSkillModel=g_GameWorld->getUseSkillModel();//0 Ϊ�Զ� 1 Ϊ�ֶ�

	return true;
}

void KuiMylistView::ccTouchMoved(CCTouch* touch, CCEvent* event)
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
}

void KuiMylistView::ccTouchEnded(CCTouch* touch, CCEvent* event)
{

	if (g_GameWorld && g_pCoreShell && g_pCoreShell->getTradeState()) return;
	currentPoint = centerPoint;
	isRun = true;
}

void KuiMylistView::ccTouchCancelled(CCTouch *touch, CCEvent* event)
{

}


void  KuiMylistView::setSpriteOpacity(int ver)
{
	if (jsSprite)
		jsSprite->setOpacity(ver);
	if (bgSprite)
		bgSprite->setOpacity(ver);
}

void  KuiMylistView::setSpriteVisible(bool ver)
{
	if (jsSprite)
		jsSprite->setVisible(ver);
	if (bgSprite)
		bgSprite->setVisible(ver);

}

void  KuiMylistView::setSpritePiont(CCPoint aPoint)
{
	centerPoint  = aPoint;
	currentPoint = centerPoint;              //�ƶ���ǰ������
	if (jsSprite)
		jsSprite->setPosition(centerPoint);   //����spr
	if (bgSprite)
		bgSprite->setPosition(centerPoint);   //����spr
}