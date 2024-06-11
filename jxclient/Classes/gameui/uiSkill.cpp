#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "uiskill.h"
#include "engine/CXmlStream.h"
#include "engine/KIniFile.h"
#include "gamecore/CoreShell.h"
//#include "engine/KPalette.h"
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
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_CC;

uiskill::uiskill():
m__pMenu(NULL)
,m_contentPadding(0)
,m_contentPaddingTop(0)
,m_callbackListener(NULL)
,m_callback(NULL)
,m__sfBackGround(NULL)
,m__s9BackGround(NULL)
,m__ltContentText(NULL)
,m__ltTitle(NULL)
,m_ParentNode_Fight(NULL)
{
    //memset(Skills,0,sizeof(KUiSkillData) * 50);
}

uiskill::~uiskill()
{
    CC_SAFE_RELEASE(m__pMenu);
    CC_SAFE_RELEASE(m__sfBackGround);
    CC_SAFE_RELEASE(m__ltContentText);
    CC_SAFE_RELEASE(m__ltTitle);
    CC_SAFE_RELEASE(m__s9BackGround);
	CC_SAFE_RELEASE(m_ParentNode_Fight);
}

bool uiskill::init()
{
    bool bRef = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());

        this->setContentSize(CCSizeZero);
        // ��ʼ����Ҫ�� Menu
        CCMenu* menu = CCMenu::create();
        menu->setPosition(CCPointZero);
        setMenuButton(menu);
        setTouchEnabled(true);
        bRef = true;
    } while (0);
    return bRef;
}


void uiskill::registerWithTouchDispatcher()
{
    // ����Ĵ������ȼ�����Ϊ -128 �Ᵽ֤�ˣ������·��Ĵ���
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-128, true);
}

bool uiskill::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{//����������
   // CCLog("uiskill touch");
    return true;
}


uiskill* uiskill::create(const char *backgroundImage,int nKind)
{
    uiskill* ml = uiskill::create();
	if  (!nKind)
	{
      ml->setSpriteBackGround(CCSprite::create(backgroundImage));//�����ľ���
      ml->setSprite9BackGround(CCScale9Sprite::create(backgroundImage));
	}
	else
	{//������أ���������
		char nSprName[128]={0};
		ZeroMemory(nSprName,sizeof(nSprName));
		t_sprintf(nSprName,backgroundImage);
		g_StrLower(nSprName);
		int m_nWidth,m_nHeight,nFrams;
		CCTexture2D *bgCur = NULL;
		SPRFRAMSINFO nSprInfo;
		ZeroMemory(&nSprInfo,sizeof(nSprInfo));
		bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
		if (bgCur==NULL)
			return NULL;
		ml->setSpriteBackGround(CCSprite::createWithTexture(bgCur));//�����ľ���
	}

	  CCSprite *nFightNode =  CCSprite::create();
	  nFightNode->setPosition(ccp(0,0));
	  nFightNode->setAnchorPoint(ccp(0,0));
	  

	  KUiSkillData	Skills[FIGHT_SKILL_COUNT];

	  if (g_pCoreShell)
		  g_pCoreShell->GetGameData(GDI_FIGHT_SKILLS, (unsigned int)Skills, 0); //��ȡ��ҳ��������

	  for (int i = 0; i < FIGHT_SKILL_COUNT_PER_PAGE; ++i)
	  {//ս������
		  //m_FightSkills[i].HoldObject(Skills[i].uGenre, Skills[i].uId, Skills[i].nLevel, 0);//��ʾ����������Ʒ�������������ɶ��������
		  ISkill *pSkill = g_SkillManager.GetSkill(Skills[i].uId,1);
		  if (pSkill)
		  {//�滭����ͼ��
			  //pSkill->m_szSkillIcon;
			  //pSkill->DrawSkillIcon(x, y, Width, Height,inParam);  //���Ƽ���ͼ��
			  //pSkill->getIconPath()
			  //char msg[64];
			  //t_sprintf(msg,"idx:%d,Skillid:%d",i,Skills[i].uId);
			  //CCMessageBox(pSkill->getIconPath().c_str(),"�滭����ͼ��");
			  //CCSprite *nSkillSpr =CCSprite::create();
			  //nSkillSpr->setAnchorPoint(ccp(0,0));
			  //nFightNode->addChild(nSkillSpr,1,i+1);

			  char nSprName[128]={0};
			  ZeroMemory(nSprName,sizeof(nSprName));
			  t_sprintf(nSprName,pSkill->getIconPath().c_str());
			  g_StrLower(nSprName);
			  int m_nWidth,m_nHeight,nFrams;
			  CCTexture2D *bgCur = NULL;
			  SPRFRAMSINFO nSprInfo;
			  ZeroMemory(&nSprInfo,sizeof(nSprInfo));
			  bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
			  if (bgCur==NULL)
				  return ml;

			  CCRect CCRectZero = CCRectMake(0,0,5,5);
			  CCRectZero.size = bgCur->getContentSize();
			  nFightNode->setTextureRect(CCRectZero,false,CCRectZero.size);
			  nFightNode->setTexture(bgCur);
			  nFightNode->setPosition(ccp(0,0));
			  //this->setSpriteBackGround(nSkillSpr);
			  //getSpriteBackGround()->setTexture(bgCur);
			   ml->setParentNode_Fight(nFightNode); 
		  }
	  }
	  

    return ml;
}


void uiskill::setTitle(const char*title,int fontsize)
{
    CCLabelTTF* ltfTitle = CCLabelTTF::create(title,UI_GAME_FONT_DEFAULT,fontsize);
    setLabelTitle(ltfTitle);
}

void uiskill::setContentText(const char *text, int fontsize, int padding, int paddingTop){
    CCLabelTTF* ltf = CCLabelTTF::create(text,UI_GAME_FONT_DEFAULT,fontsize);
    setLabelContentText(ltf);
    m_contentPadding    = padding;
    m_contentPaddingTop = paddingTop;
}

void uiskill::setCallbackFunc(cocos2d::CCObject *target, SEL_CallFuncN callfun)
{
    m_callbackListener = target;
    m_callback         = callfun;
}


bool uiskill::addButton(const char *normalImage, const char *selectedImage, const char *title, int tag,int isSpr){
    
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();//��ǰ���ڵĸ߶ȺͿ��
    CCPoint pCenter = ccp(winSize.width/2, winSize.height/2);
    
    // ����ͼƬ�˵���ť
    CCMenuItemImage* menuImage = CCMenuItemImage::create(normalImage, selectedImage, this, menu_selector(uiskill::buttonCallback));
    menuImage->setTag(tag);
    menuImage->setPosition(pCenter);
    // �������˵��������λ�� ���ְ�ť
    CCSize imenu = menuImage->getContentSize();
    CCLabelTTF* ttf = CCLabelTTF::create(title,"",20);
    ttf->setColor(ccc3(0,0,0));
    ttf->setPosition(ccp(imenu.width/2,imenu.height/2));
    menuImage->addChild(ttf); 
    getMenuButton()->addChild(menuImage);
    return true;
}


void uiskill::buttonCallback(cocos2d::CCObject *pSender){
    CCNode* node = dynamic_cast<CCNode*>(pSender);
    //CCLog("touch tag: %d", node->getTag());
    if (m_callback && m_callbackListener){
        (m_callbackListener->*m_callback)(node);
    }
	//�ͷ���
    this->removeFromParentAndCleanup(true);
}

void uiskill::onEnter()
{
    CCLayer::onEnter();
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint pCenter = ccp(winSize.width / 2, winSize.height / 2);
    
    CCSize contentSize;
     // �趨�ò�����������ʱ����
    if (getContentSize().equals(CCSizeZero))
    {//���û�����ô�С��
        getSpriteBackGround()->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        this->addChild(getSpriteBackGround(),0,0);
        contentSize = getSpriteBackGround()->getTexture()->getContentSize();
    }else
    {//��������� �Ͱ��� 9������
        CCScale9Sprite *background = getSprite9BackGround();
        background->setContentSize(getContentSize());
        background->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        this->addChild(background,0);
        contentSize = getContentSize();
    }
    
     // ��Ӱ�ť����������λ��
    this->addChild(getMenuButton());
    //float btnWidth = contentSize.width/(getMenuButton()->getChildrenCount()+1);
    
    CCArray* array = getMenuButton()->getChildren();
    CCObject* pObj = NULL;
	
	CCMenuItemImage* itemImage = dynamic_cast<CCMenuItemImage*>(array->objectAtIndex(0));
    int count = getMenuButton()->getChildrenCount();
    int btnWidth = itemImage->getContentSize().width;
    int padingWidth = (contentSize.width - btnWidth * count) / (count + 1);
	int i = 0;
    CCARRAY_FOREACH(array, pObj)
    {//���ð�ť��λ�� 
        CCNode* node = dynamic_cast<CCNode*>(pObj);
        node->setAnchorPoint(ccp(0, 0.5f)); //������㣿
        node->setPosition(ccp(winSize.width * 0.5f - contentSize.width * 0.5f + padingWidth * (i + 1) + btnWidth * i, winSize.height * 0.5f - contentSize.height / 3));
        i++;     
    }
	
    /*int i = 0; ԭ����
    CCARRAY_FOREACH(array, pObj)
    {
        CCNode* node = dynamic_cast<CCNode*>(pObj);
        node->setPosition(ccp(winSize.width / 2 - contentSize.width / 2 + btnWidth * (i + 1), winSize.height / 2 - contentSize.height / 3));
        i++;
    }*/
    
    // ��ʾ�Ի������
    if (getLabelTitle())
    {//����б���Ļ�
        getLabelTitle()->setPosition(ccpAdd(pCenter, ccp(0, contentSize.height / 2 - 35.0f)));
        this->addChild(getLabelTitle());
    }
    
    // ��ʾ�ı�����
    if (getLabelContentText())
    {
        CCLabelTTF* ltf = getLabelContentText();
        ltf->setPosition(ccp(winSize.width / 2, winSize.height / 2));
          ltf->setDimensions(CCSizeMake(contentSize.width - m_contentPadding * 2, contentSize.height - m_contentPaddingTop));
         ltf->setHorizontalAlignment(kCCTextAlignmentLeft);
        this->addChild(ltf);
    }
	this->setPosition(ccp(0,0));   //�����������λ��
    
	//ParentNode_Fight = CCNode::create();
	//ParentNode_Fight->setAnchorPoint(ccp(0,0));
	//ParentNode_Fight->setPosition(ccp(0,0));
	this->addChild(getParentNode_Fight(),100);
	getParentNode_Fight()->setPosition(ccp(winSize.width/2, winSize.height/2));
	
	if  (!getParentNode_Fight())
		return;

}


void uiskill::onExit()
{
    //CCLog("popup on exit.");
    CCLayer::onExit();
}

void uiskill::update(float delta)
{
	//CCMessageBox("����ѭ��","update");
}

void uiskill::draw()
{
	//CCMessageBox("����ѭ��","draw");
}