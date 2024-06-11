#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "gameui/Uiglobaldata.h"
#include "Klogin.h"
#include "Klogin_f.h"
#include "KSelPlayer.h"
#include "engine/CXmlStream.h"
#include "engine/KIniFile.h"
#include "gamecore/CoreShell.h"
#include "gameui/PopupLayer.h"
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
#include "gamecore/KSubWorldSet.h"
#include "gamescene/KUpdateLayer.h"
#ifndef WIN32
#include <ctype.h>
#endif
extern iCoreShell*	g_pCoreShell; //ȫ�ֵ���
extern KImageStore2 m_ImageStore; //ȫ�ֵ���
//extern KPakList     g_pPakList;
//extern KPakList * g_pPakList;
#include <list>
USING_NS_CC;

void CcharToDouble(const char* ch, double& res)
{
	sscanf(ch,"%lf",&res);
}

CCScene* Klogin::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	Klogin *layer = Klogin::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Klogin::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

	m_sprCount         =0;
	nTimeCount = 0;
	m_nWidth   = 0;
	m_nHeight  = 0;
	//pWW        =NULL;
	animate    =NULL;
	m_bIsPlayerInGame =false;
	setTouchEnabled(true);
	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	//CCMenuItemImage *pCloseItem = CCMenuItemImage::createWithTexture();
	m_Status = LL_S_WAIT_INPUT_ACCOUNT;
	char nSprName[128]={0};
	int nFrams=0;
	t_sprintf(nSprName,"\\spr\\Ui3\\loginui\\okbtn\\loginui_6_ok.spr");
	g_StrLower(nSprName);
	CCMenuItemImage *pCancelItem=NULL;
	CCMenuItemImage *pLoginItem=NULL;
	CCTexture2D *Login = _getinidata.getinidata_new(nSprName,0,&m_nWidth,&m_nHeight,&nFrams);
	if  (Login)
	{	
		char nSprFilePath[64]={0};
		DWORD nFielpahtdwid = g_FileName2Id(nSprName);
		t_sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,0); 
		CCTexture2D *pNormalTexture = CCTextureCache::sharedTextureCache()->isFileExist(nSprFilePath);
		t_sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,1);
		CCTexture2D *pSelectedTexture = CCTextureCache::sharedTextureCache()->isFileExist(nSprFilePath);
		t_sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,2);
		CCTexture2D *pDisabledTexture = CCTextureCache::sharedTextureCache()->isFileExist(nSprFilePath);
		pLoginItem = CCMenuItemImage::createWithTexture(
			pNormalTexture,
			pDisabledTexture,
			pSelectedTexture,this,menu_selector(Klogin::mianLoginCallback));
		pLoginItem->setPosition(ccp(origin.x + visibleSize.width - pLoginItem->getContentSize().width/2-200 ,
			origin.y + pLoginItem->getContentSize().height/2+20));//һ����ť
	}

	nFrams=0;
	t_sprintf(nSprName,"\\spr\\Ui3\\loginui\\okbtn\\loginui_6_cancel.spr");
	g_StrLower(nSprName);
	CCTexture2D *Cancel = _getinidata.getinidata_new(nSprName,0,&m_nWidth,&m_nHeight,&nFrams);
	if (Cancel)
	{
		char nSprFilePath[64]={0};
		DWORD nFielpahtdwid = g_FileName2Id(nSprName);
		t_sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,0); 
		CCTexture2D *pNormalTexture = CCTextureCache::sharedTextureCache()->isFileExist(nSprFilePath);
		t_sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,1);
		CCTexture2D *pSelectedTexture = CCTextureCache::sharedTextureCache()->isFileExist(nSprFilePath);
		t_sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,2);
		CCTexture2D *pDisabledTexture = CCTextureCache::sharedTextureCache()->isFileExist(nSprFilePath);
		pCancelItem = CCMenuItemImage::createWithTexture(
			pNormalTexture,
			pDisabledTexture,
			pSelectedTexture,this,menu_selector(Klogin::mianCancelCallback));
		pCancelItem->setPosition(ccp(origin.x + visibleSize.width - pCancelItem->getContentSize().width/2 ,
			origin.y + pCancelItem->getContentSize().height/2+20));//һ����ť
	}
	/*CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
		"mainskillmix.png",
		"mianskillbig.png",
		this,
		menu_selector(Klogin::mianSkillCallback)); //�ص�����
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2-20 ,
		origin.y + pCloseItem->getContentSize().height/2+20));//һ����ť
	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition(CCPointZero);
	m_sprCount ++;
	this->addChild(pMenu,1,m_sprCount);*/
	/////////////////////////////
	// 3. add your codes below...
	// add a label shows "Hello World"
	// create and initialize a label
	const char * strTiele = CXmlStream::GetStringByKeyFromFile("string.xml","title"); //Resources ��Դ����Ŀ¼��,���û������ļ���������
	//KPakList g_pPakList;
	//CCFileUtils::getPathForFilename()
	//std::string nPath = CCFileUtils::sharedFileUtils()->getWritablePath();

	//CCMessageBox(nPath.c_str(),"getWritablePath");//�ɶ�д·�� /data/data/����/files/
	//nPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("package.ini");
	//nPath = CCFileUtils::sharedFileUtils()->getWritablePath();//getWriteablePath();
	//nPath += "download";
	int nCount = 0;
	//tolower() //��дתСд
	//toupper() //Сдת��д
	//getExtSDCardPath();
	//Environment.getExternalStorageDirectory();
	//Context::getCacheDir();
	//Environment.getExternalStorageDirectory().toString() + "/" + "Tianchaoxiong/useso";
	//AssetManager::InputStream.open();
	//pContext.getAssets();
	//getAssetManager();
	//nCount = g_pPakList.Open(fielDir.c_str());  //����PAK
	//CCFileUtils::getFullPathForDirectoryAndFilename("Resources","test.ini");
	//fullPathForFilename(filename.c_str());
	pMainLabel = CCLabelTTF::create(strTiele,"Arial", 20);
	pMainLabel->setString(UTEXT("",1).c_str());//������ʾ������
	//CCMessageBox(nPath.c_str(),"title");
	// position the label on the center of the screen
	pMainLabel->setPosition(ccp(origin.x + visibleSize.width/2,
		origin.y + visibleSize.height - pMainLabel->getContentSize().height-10));
	// add the label as a child to this layer
	//m_sprCount++;
	this->addChild(pMainLabel, 1);

	CCSprite *bgSprite=CCSprite::create("loginbg.jpg");//��½����
	bgSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	float winw = visibleSize.width;   //��ȡ��Ļ���
	float winh = visibleSize.height;  //��ȡ��Ļ�߶�
	float spx  = bgSprite->getTextureRect().getMaxX();
	float spy  = bgSprite->getTextureRect().getMaxY();
	bgSprite->setScaleX(winw/spx); //���þ��������ű���
	bgSprite->setScaleY(winh/spy);
	m_sprCount++;
	this->addChild(bgSprite,0,m_sprCount);
	//CCNode * d = newmenu->getChildByTag(1);//��ȡ���ڵ��µ��ӽڵ�
	//newmenu->removeChild(d,true);          //ɾ�����ڵ��µ��ӽڵ�
	//newmenu->removeChildByTag(1);
	//itemaddmaop->removeFromParentAndCleanup(true);  //
	//d->removeFromParentAndCleanup(true);
	//d->release();//�ͷ����ڴ档������
	//if (itemaddmaop)
	//   newmenu->addChild(itemaddmaop,true);  //���ڵ�������һ���ӽڵ�
	//����һ�������
	//CCComponent::create();
	//newmenu->setUserData();//Ϊĳ���ڵ���Զ�������
	//�����￪ʼ��ȡͼ�����ݵ�ָ��data
		//memset()
		//\\Spr\\Ui3\\�������\\��½����_6_�߿�.spr  \Spr\Ui3\�������\����С����\Ҷ��Ʈ��.spr  \\spr\\Ui3\\loginui\\uidonghua\\yezi.spr
	    
		 t_sprintf(nSprName,"\\spr\\Ui3\\loginui\\uidonghua\\yezi.spr");
		 g_StrLower(nSprName);
		 CCTexture2D *bgCur = _getinidata.getinidata_new(nSprName,0,&m_nWidth,&m_nHeight,&nFrams);
		 if  (bgCur)
		 {		
			CCSprite *ySprite=CCSprite::createWithTexture(bgCur);
			ySprite->setPosition(ccp(visibleSize.width/2 + origin.x-100, visibleSize.height/2 + origin.y));
			/*float winw = visibleSize.width;   //��ȡ��Ļ���
			float winh = visibleSize.height;  //��ȡ��Ļ�߶�
			float spx  = bgSprite->getTextureRect().getMaxX();
			float spy  = bgSprite->getTextureRect().getMaxY();
			ySprite->setScaleX(winw/spx); //���þ��������ű���
			ySprite->setScaleY(winh/spy);*/
			m_sprCount++;
			this->addChild(ySprite,2,m_sprCount);
			char nySprFilePath[64]={0};
			CCAnimation*animation = CCAnimation::create();//����һ������
			DWORD nFielpahtdwid = g_FileName2Id(nSprName);
			for (int i=0;i<nFrams;i++)
			{   
				t_sprintf(nySprFilePath,"%u-%d",nFielpahtdwid,i);  
				//CCMessageBox(nSprFilePath,"animation");
				animation->addSpriteFrameWithSprName(nySprFilePath);//�����������������֡
			}
			//���ö���֡��ʱ����
			animation->setDelayPerUnit(0.10f);
			//���ò���ѭ�� һֱ���� Ϊ-1
		    animation->setLoops(-1);
			//���ö���������ָ�����һ֡ setRestoreOriginalFrame
			animation->setRestoreOriginalFrame(TRUE);
			// ����һ����Զѭ���Ķ���
			animate = CCAnimate::create(animation);
			//CCRepeatForever* forever = CCRepeatForever::create(animate);
			if (ySprite)
			   ySprite->runAction(CCSequence::create(animate,CCCallFunc::create(this, callfunc_selector(Klogin::finish)),NULL));
		 }
		 //�����ʺš����� "\\Spr\\Ui3\\�������\\��½����_6_�߿�.spr"
		 t_sprintf(nSprName,"\\spr\\Ui3\\loginui\\longi_6_b.spr");//"\\spr\\Ui3\\loginui\\longi_6_b.spr"
		 g_StrLower(nSprName);
		 nFrams=0;
		 CCTexture2D *Cur = _getinidata.getinidata_new(nSprName,0,&m_nWidth,&m_nHeight,&nFrams);
		testSprite = NULL;
		if (Cur)
		{ 
			//��½����ͼ
			testSprite=CCSprite::createWithTexture(Cur);

		  if (testSprite)
		  {
			char nTempStr[64];
			ZeroMemory(nTempStr,sizeof(nTempStr));
			KIniFile accinfo;
			accinfo.Load("accinfo.ini",true);
			 
			testSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y-50));
			m_sprCount++;
			this->addChild(testSprite,2,m_sprCount);
			//�ʺ������
			CCScale9Sprite *pAccBg = CCScale9Sprite::create();  //"boxgb.png"
			CCEditBox *pAccEditBox = CCEditBox::create(CCSize(200,24),pAccBg);
			const ccColor3B nfcolor ={255,255,255};
			if (pAccEditBox)
			{
				//pEditBox->setContentSize()
				//pAccEditBox->setText("input your account");
				pAccEditBox->setFontColor(nfcolor);
				pAccEditBox->setFontSize(24);
				pAccEditBox->setMaxLength(8);
				//pEditBox->setPosition(ccp(testSprite->getPositionX(), testSprite->getPositionX()));
				pAccEditBox->setPositionX(testSprite->getPositionX()+20);
				pAccEditBox->setPositionY(testSprite->getPositionY()+38);
				///m_sprCount++;
				accinfo.GetString("info","acc","",nTempStr,sizeof(nTempStr));
				pAccEditBox->setText(nTempStr);
				this->addChild(pAccEditBox,2,1000);
			}
		
			//���������
			CCScale9Sprite *pPassBg = CCScale9Sprite::create(); //"boxgb.png"
			CCEditBox *pPassEditBox = CCEditBox::create(CCSize(200,24),pPassBg);
			if (pPassEditBox)
			{
				//pEditBox->setContentSize()
				//pPassEditBox->setText("input your password");
				pPassEditBox->setFontColor(nfcolor);
				pPassEditBox->setFontSize(24);
				pPassEditBox->setInputFlag(kEditBoxInputFlagPassword);
				pPassEditBox->setMaxLength(16);
				//pEditBox->setPosition(ccp(testSprite->getPositionX(), testSprite->getPositionX()));
				pPassEditBox->setPositionX(testSprite->getPositionX()+20);
				pPassEditBox->setPositionY(testSprite->getPositionY());
				//m_sprCount++;
				accinfo.GetString("info","pas","",nTempStr,sizeof(nTempStr));
				pPassEditBox->setText(nTempStr);
				this->addChild(pPassEditBox,2,999);
			}
			accinfo.Clear();

			/*char nSprFilePath[64]={0};
			CCAnimation*animation = CCAnimation::create();//����һ������
            //std:string nFileName ="\\spr\\skill\\1502\\kl\\kl_150_jiankun_fu.spr";
			DWORD nFielpahtdwid = g_FileName2Id(nSprName);
			for (int i=0;i<nFrams;i++)
			{   
				t_sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,i);  
				//CCMessageBox(nSprFilePath,"animation");
				animation->addSpriteFrameWithSprName(nSprFilePath);//�����������������֡
			}
			//���ö���֡��ʱ����
			animation->setDelayPerUnit(0.02f);
			//���ò���ѭ�� һֱ���� Ϊ-1
		    animation->setLoops(-1);
			//���ö���������ָ�����һ֡ setRestoreOriginalFrame
			animation->setRestoreOriginalFrame(TRUE);
			// ����һ����Զѭ���Ķ���
			animate = CCAnimate::create(animation);
			//CCRepeatForever* forever = CCRepeatForever::create(animate);
			if (testSprite)
			   //testSprite->runAction(animate);
			   testSprite->runAction(CCSequence::create(animate,CCCallFunc::create(this, callfunc_selector(Klogin::finish)),NULL));
			*/
			//if(testSprite->isActionDone(animate))
			 // removeChild(testSprite,true);
			//CCMoveTo* move = CCMoveTo::create(30, ccp(-20, 200));
			//testSprite->runAction(move); 
			//removeChild(testSprite,true);
			// create menu, it's an autorelease object
			CCMenu* pMenu=NULL;
			if (Login && Cancel)
			{
				pLoginItem->setPositionX(testSprite->getPositionX()-75);
				pLoginItem->setPositionY(testSprite->getPositionY()-80); //ȷ�ϰ�ť
				pCancelItem->setPositionX(testSprite->getPositionX()+75);//�ı䰴ť
				pCancelItem->setPositionY(testSprite->getPositionY()-80);

				pMenu = CCMenu::create(pLoginItem,pCancelItem,NULL);
				pMenu->setPosition(CCPointZero);//
				m_sprCount ++;
				this->addChild(pMenu,2,m_sprCount);
			}
		  }
		}

		//CCSpriteFrameCache
	    //CCTextureCache ������
		/*
		
		void addSpriteFramesWithFile(const char *pszPlist)
		����һ��.plist�ļ���Ӷ������֡�� һ���������Զ����ء��������ƽ���.plist��׺���滻Ϊ.png����ɡ�
		void addSpriteFramesWithFile(const char* plist, const char* textureFileName)
		��ͨ��һ��.plist�ļ���Ӷ������֡�������뱻�����ľ���֡��ϡ�
		void addSpriteFramesWithFile(const char *pszPlist, CCTexture2D *pobTexture)
		��ͨ��һ��.plist�ļ���Ӷ������֡�������뱻�����ľ���֡��ϡ�
		void addSpriteFrame(CCSpriteFrame *pobFrame, const char *pszFrameName)
		��ͨ���������������һ������֡�� ��������Ѿ����ڣ���ôԭ�����Ƶ����ݽ����µ������
		*/
		/*CCSprite *lpSprite=CCSprite::create("jin.png");
		lpSprite->setPosition(ccp(visibleSize.width/2 +origin.x, visibleSize.height/2 + origin.y));
		m_sprCount++;
		this->addChild(lpSprite,1,m_sprCount);*/
		//
		//CCFileUtils::sharedFileUtils()->CloseFilePtr("laohuji.spr",FALSE);

		//fclose(fp);
		/*
		float winw = visibleSize.width;   //��ȡ��Ļ���
		float winh = visibleSize.height;  //��ȡ��Ļ�߶�
		float spx = testSprite->getTextureRect().getMaxX();
		float spy = testSprite->getTextureRect().getMaxY();
		testSprite->setScaleX(winw/spx);  //���þ��������ű���
		testSprite->setScaleY(winh/spy);
		
		*/
	//this->setRotation(CC_RADIANS_TO_DEGREES(angle));
	// add "HelloWorld" splash screen"
	//CCSprite* pSprite = CCSprite::create("HelloWorld.png");
	// position the sprite on the center of the screen
	//pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	// add the sprite as a child to this layer
	//this->addChild(pSprite, 0);
	//���Զ����ǵ�ǰһ������
	//CCSprite* sprite = CCSprite::create("bg.jpg");
	//ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y)
	//sprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));//(ccp(visibleSize.width/2,visibleSize.height/2));
	//sprite->setScaleX(1.5f);
	//sprite->setScaleY(2.0f);
	//Size mywinsize=Director::getInstance()->getWinSize();
	/*float winw = visibleSize.width; //��ȡ��Ļ���
	float winh = visibleSize.height;//��ȡ��Ļ�߶�
	float spx = sprite->getTextureRect().getMaxX();
	float spy = sprite->getTextureRect().getMaxY();
	sprite->setScaleX(winw/spx); //���þ��������ű���
	sprite->setScaleY(winh/spy);*/
	//this->addChild(sprite,0);
	nTestCount = 0;
	//----------������Դ�ļ����ɶ�д�ļ���
	/*if (!isFileExist("laohuji.spr"))
	{//�����ھͿ�ʼ����
		copyData("laohuji.spr");
	}
	//��ʼ���� �Ƿ��Ƴɹ�....
	if (isFileExist("laohuji.spr"))
	{//�����ھͿ�ʼ����
		CCMessageBox("suee","isFileExist");
	}*/
	//this->draw();
	this->scheduleUpdate();  //ң�˸��µ��ú���
	//this->setTouchMode(kCCTouchesOneByOne); //��ֹ��㴥��
	return true;
}

void Klogin::mianCancelCallback(CCObject* pSender)
{
	if (nAcclient)
	{
		m_bIsClientConnecting=false;
		m_bIsPlayerInGame =false;
		nAcclient->Cleanup();
		
	}
	CCDirector::sharedDirector()->replaceScene(Klogin_f::scene());
}

void Klogin::mianLoginCallback(CCObject* pSender)
{
	CCEditBox * AccNode =  (CCEditBox *)this->getChildByTag(1000);
	CCEditBox * PasNode =  (CCEditBox *)this->getChildByTag(999);
	std::string strAcc;
	std::string strPas;
	//CCEditBox *pPassEditBox
	if (AccNode && PasNode)
	{
		strAcc = AccNode->getText();
		strPas = PasNode->getText();

		if (strAcc.length()>16 || strAcc.length()<=0 || strPas.length()>16 || strPas.length()<=0)
		{
#ifdef WIN32
			CCMessageBox("Sai t��i kho?n ho?c m?t kh?u","Ch�� y");
#else
			CCMessageBox(G2U("Sai t��i kho?n ho?c m?t kh?u��").c_str(),G2U("Ch�� y").c_str());
#endif
			return;
		}
		//��ʼ����ַ��ĺϷ���
		std::string strcheck="check";
		strcheck+=strAcc;
		strcheck+=strPas;
		int count = 0,spaceNum=0,chineseNum=0,other=0,letter=0,xiaoxie=0;
		for(int i = 0; i <strcheck.length();i++)
		{ 
			if(isdigit(strcheck[i])) //�ж��Ƿ�������
				count++;
			else if(isspace(strcheck[i]))//�ж��Ƿ��ǿո�
				++spaceNum;
			else if(isalpha(strcheck[i])==1)//�ж��Ƿ�����ĸ	 1 Ϊ��д 2 Ϊ Сд  0 Ϊ������ĸ
			{
				++letter;
			}
			else if(isalpha(strcheck[i])==2)//�ж��Ƿ�����ĸ	 1 Ϊ��д 2 Ϊ Сд  0 Ϊ������ĸ
			{
				++xiaoxie;
			}
			else if(strcheck[i]&0x80)//if(strcheck[0]>=0xB0)
				++chineseNum;    //�ж��Ƿ�������
			else
			{//���ź���ע�� ������������
				++other;
			}
		} 
if (_clientlanguage==1)
{//���İ汾
		if (spaceNum>0 || chineseNum >0 || other>0 || letter>0)
		{ 	//system("pause");
#ifdef WIN32
			CCMessageBox("���зǷ��ַ�,�����ʺ�,����\n���ܺ��пո�,��д��ĸ���������!","����");
#else
			CCMessageBox(G2U("���зǷ��ַ�,�����ʺ�,����\n���ܺ��пո�,��д��ĸ���������!").c_str(),G2U("����").c_str());
#endif
			return;
		} 
}
else
{//�������İ汾
	if (spaceNum>0 || chineseNum >0 || other>0)
	{ 	//system("pause");
#ifdef WIN32
		CCMessageBox("���зǷ��ַ�,�����ʺ�,����\n���ܺ��пո�,��д��ĸ���������!","����");
#else
		CCMessageBox("Invalid character,please check your name!\n Cannot contain special symbols such as chinese, spaces, etc.!","Warning");
#endif
		return;
	} 
}

	}
	else
	{
#ifdef WIN32
		CCMessageBox("�������󣮣�","����");
#else
		CCMessageBox(G2U("�������󣮣�").c_str(),G2U("����").c_str());
#endif
		return;
	}
	KIniFile accinfo;
	if (accinfo.Load("accinfo.ini",true))
	{
		char nTempStr[64];
		t_sprintf(nTempStr,strAcc.c_str());
		accinfo.WriteString("info","acc",nTempStr);

		t_sprintf(nTempStr,strPas.c_str());
		accinfo.WriteString("info","pas",nTempStr);
		accinfo.Save("accinfo.ini");
		accinfo.Clear();
	}

	if (m_bIsClientConnecting && nAcclient)
	{
		if  (m_ClientChoices.nServerRegionIndex<=0)
		{
#ifdef WIN32
			CCMessageBox("��ѡ���½��������","����");
#else
			CCMessageBox(G2U("��ѡ���½��������").c_str(),G2U("����").c_str());
#endif
			return;
		}
		//ģ���ʺŵ�½
		m_nNumRole = 0;                          //��ɫ��������Ϊ0
		memset(&m_RoleList,0,sizeof(m_RoleList));//��ս�ɫ�嵥����
		ClearAccountPassword(true, true);        //��ձ��ݵ��ʺ�����
		
		BYTE Buff[sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE];
		RandMemSet(sizeof(Buff),(BYTE*)Buff);		// random memory for make a cipher
		char pszAccount[32]={0};
		t_sprintf(pszAccount,strAcc.c_str());

		(*(PROTOCOL_MSG_TYPE*)Buff) = c2s_login_fhgyrinhkmvnsheypo;
		KLoginAccountInfo* pInfo = (KLoginAccountInfo*)&Buff[PROTOCOL_MSG_SIZE];

		pInfo->Size  = sizeof(KLoginAccountInfo);
		pInfo->Param = LOGIN_A_LOGIN | LOGIN_R_REQUEST;
		strncpy(pInfo->Account,pszAccount,sizeof(pInfo->Account));
		
		pInfo->Account[sizeof(pInfo->Account) - 1] = '\0';
		//KSG_PASSWORD pcPassword;
		KSG_StringToMD5String(pInfo->Password.szPassword,strPas.c_str());
		//pInfo->Password      = pcPassword;
		pInfo->nSelSererIdx    = m_ClientChoices.nServerRegionIndex;//KUiSelServer::GetCurServer();
		pInfo->ProtocolVersion = KPROTOCOL_VERSION;       //  ����Э��汾���Ա�У���Ƿ����
		m_Status               = LL_S_ACCOUNT_CONFIRMING; //  �ȴ���֤�ʺ�����
		SetAccountPassword(pszAccount, &pInfo->Password);
	    if (!nAcclient->SendMsg(Buff, sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE))
		//if (!nAcclient->SendPackToServer(Buff,sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE))  //sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE
		{
			if (nAcclient)
			{
				nAcclient->Cleanup();
				m_bIsClientConnecting=false;
			}
			CCDirector::sharedDirector()->replaceScene(Klogin_f::scene());
		}
		//CCDirector::sharedDirector()->replaceScene(KSelPlayer::scene());
		//CCMessageBox("send suss","send suss");
	}
}
//�رհ�ť�ص�����
void Klogin::mianSkillCallback(CCObject* pSender)
{//�ر�
/*#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif*/
}

//ѭ�������麯�� Ĭ���Զ�����
void Klogin::update(float delta)
{
	if  (m_Status == LL_S_ROLE_LIST_READY)
	{//����Ѿ���ȡ����ɫ������ �ͽ�����һ������
		m_Status = LL_S_IDLE;
		CCDirector::sharedDirector()->replaceScene(KSelPlayer::scene());
		return;
	}
	unsigned int nSize;
	const char* pBuffer = NULL;
	if (m_bIsClientConnecting)
	{ //�˺ŷ�����
		while (true)
		{
			if (!nAcclient)
				break;
			//����ѭ���ӷ�������ȡ����С��
			pBuffer = (const char*)nAcclient->GetPackFromServer(nSize);

			if (!(pBuffer && nSize))
				break;

			PROTOCOL_MSG_TYPE*	pMsg = (PROTOCOL_MSG_TYPE*)pBuffer;
			PROTOCOL_MSG_TYPE	Msg  = *pMsg;  //Э��ͷ			

			//CCAssert(Msg > s2c_multiserverbegin || Msg < s2c_end,"");
			if  (Msg == s2c_login || Msg == s2c_roleserver_getrolelist_result ||Msg == s2c_notifypl_ayerlogin_hfkqotivbhd || Msg == s2c_rolenewdelresponse)
			     AcceptNetMsg(pMsg);
//			if (m_MsgTargetObjs[Msg])
	//		   (m_MsgTargetObjs[Msg])->AcceptNetMsg(pMsg);  //�������緢��������Ϣ
		}
	}
	int newX = 0;
	int newY = 0;
	//�ж��Ƿ���ҡ�˼�������
    //CCPoint nCurPoint= joystick->getDirection();
	//float n = joystick->getAngleSigned();
	/*char nPiontInfo[64]={0};
	t_sprintf(nPiontInfo,"%f",n);
	pMainLabel->setString(nPiontInfo);
    */
	switch(newY)
	{
	case 1:
		break;
	case  2:
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		break;
	}
}


void Klogin::draw()
{
	/*nTestCount ++;
	char nPiontInfo[64]={0};
	t_sprintf(nPiontInfo,"%d",nTestCount);
	pMainLabel->setString(nPiontInfo);*/
	//��Ļ�ߴ��С
	//CCSize mysize      = CCDirector::sharedDirector()->getVisibleSize();
	//CCPoint origin     = CCDirector::sharedDirector()->getVisibleOrigin();
	//��
	//һ����
	/*ccPointSize(1);
	ccDrawColor4B(255, 25, 200, 250);
	ccDrawSolidRect(ccp(origin.x, origin.y), ccp(origin.x+mysize.width, origin.y+mysize.height), ccc4f(0, 0, 0, 0.5f));
	*/
	/*for (int i=0;i<=mysize.width;i++)
		for(int j=0;j<=mysize.height;j++)
		{
			CCPoint nPos;
			nPos.x = i;
			nPos.y = j;
            ccDrawPoint(nPos);
		}*/
/*
	//�����
	ccPointSize(30);
	ccDrawColor4B(255, 0, 0, 100);
	CCPoint Pointarray[] = {ccp(200, 150), ccp(200, 200), ccp(280, 150), ccp(280, 200)};
	ccDrawPoints(Pointarray, 4);


	//ֱ��
	glLineWidth(3);
	ccDrawColor4B(255, 255, 255, 130);
	ccDrawLine(ccp(10, 300), ccp(200, 300) );


	//Բ
	glLineWidth(3);
	ccDrawColor4B(255, 255, 100, 190);
	ccDrawCircle(ccp(50, 250), 40, 3.14/2, 360, true, 1, 0.5);
	*/

	//����
	//����
/*	glLineWidth(5);
	ccDrawColor4B(24, 25, 200, 140);
	//ccDrawRect(ccp(10, 150), ccp(110, 200));
	//ʵ��
	ccDrawSolidRect(ccp(origin.x, origin.y), ccp(origin.x+mysize.width, origin.y+mysize.height), ccc4f(0, 0, 0, 0.5f));
*/
/*
	//�����
	//����
	glLineWidth(10);
	ccDrawColor4B(240, 225, 100, 130);
	CCPoint Polyarray[] = {ccp(20, 20), ccp(50, 0), ccp(250, 100), ccp(300, 100), ccp(250, 50)};
	ccDrawPoly(Polyarray, 5, 1); 

	//ʵ��
	CCPoint Polyarray2[] = {ccp(250,30), ccp(280,300), ccp(450,0), ccp(410,130), ccp(420,50)};
	ccDrawSolidPoly(Polyarray2, 5, ccc4f(142, 245, 70, 0.3f)); 

	
	//����������
	//ƽ��
	glLineWidth(5);
	ccDrawColor4B(100, 100, 100, 255);
	ccDrawQuadBezier(ccp(0,320), ccp(160,100), ccp(480,320), 100); 

	//����
	glLineWidth(5);
	ccDrawColor4B(200, 200, 200, 255);
	ccDrawCubicBezier(ccp(0,0), ccp(160,300), ccp(320,20), ccp(480,320), 100);*/
}
//
bool Klogin::isFileExist(const char* pFileName)
{
	if( !pFileName ) return false;
	//strFilePathName is :/data/data/ + package name
	std::string filePath = CCFileUtils::sharedFileUtils()->getWritablePath();//getWriteablePath();
	
	filePath += pFileName;

	FILE *fp = fopen(filePath.c_str(),"r");
	if(fp)
	{
		fclose(fp);
		return true;
	}
	return false;
}

void Klogin::copyData(const char* pFileName)
{
	std::string strPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pFileName);
	unsigned long len = 0;
	unsigned char * data = NULL;
	data = CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(),"r",&len);
	std::string destPath = CCFileUtils::sharedFileUtils()->getWritablePath();//getWriteablePath();
	destPath += pFileName;
	FILE *fp = fopen(destPath.c_str(),"w+");
	fwrite(data,sizeof(unsigned char),len,fp);
	fclose(fp);
	delete [] data;
	data = NULL;
}

void Klogin::finish()
{
   if (!nisgetinfo)
	  mExit(NULL);
}
/*
void Klogin::onEnter()
{

}

void Klogin::onExit()
{

}
//����� �� �������
void Klogin::onEnterTransitionDidFinish()
{

}*/

bool Klogin::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}
void Klogin::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}

void Klogin::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{

}
// default implements are used to call script callback if exist
void Klogin::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{

}
void Klogin::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{

}

void Klogin::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	CCPoint location = touch->getLocation();//Ŀ�ĵ�����?��Ļ�����


	/*public void GetMpsByLocalPosition(Vector3 v3LocalPosition, out int nMpsX, out int nMpsY)
	{
		nMpsX = ((int) (location.x * 100f)) + ((this.m_nRegionBeginX * this.m_nRegionWidth) * this.m_nCellWidth);
		nMpsY = ((this.m_nRegionBeginY * this.m_nRegionHeight) * this.m_nCellHeight) - ((int) (location.y * 200f));
	}*/

	/*CCNode* s = getChildByTag(1);           //ͨ����ǺŻ�ȡ����Ľڵ�
	//s->stopAllActions();                    //ֹͣ���ж���
	s->runAction(CCMoveTo::create(10, ccp(location.x, location.y)));//�ƶ���

	float o = location.x - s->getPosition().x;
	float a = location.y - s->getPosition().y;

	float at = (float) CC_RADIANS_TO_DEGREES( atanf( o/a) );

	if( a < 0 ) 
	{
		if(  o < 0 )
			at = 180 + fabs(at);
		else
			at = 180 - fabs(at);    
	}

	s->runAction( CCRotateTo::create(10, at) ); //��ת��......
	*/
}
void Klogin::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{

}
void Klogin::didAccelerate(CCAcceleration* pAccelerationValue)
{
	
}
void Klogin::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{//convertTouchToNodeSpace
   /*CCPoint touchLocation = convertTouchToNodeSpace(pTouch);
   if(CCRect::CCRectContainsPoint(getRect(pSprite), touchLocation))
	{
	   printf("�ұ�������!\n");
	}*/
}

void Klogin::AcceptNetMsg(void* pMsgData) //ѭ�����ܴ�����Ϣ
{
	if (pMsgData == NULL)  //���ݰ�Ϊ�� �򷵻�
		return;
	//CCMessageBox("�ص��ɹ�","GetPackFromServer");
	switch(m_Status)       //�������ݰ���ʶ����
	{
	case LL_S_ACCOUNT_CONFIRMING:  //�˺���֤
		ProcessAccountLoginResponse((KLoginStructHead*) (((char*)pMsgData) + PROTOCOL_MSG_SIZE));
		break;
	case LL_S_WAIT_ROLE_LIST:      //�ȴ���ɫ�嵥
		ProcessRoleListResponse((TProcessData*)pMsgData);   //����Э���ɫ��Ϣ
		break;
	case LL_S_CREATING_ROLE:       //������ɫ
		//ProcessCreateRoleResponse((tagNewDelRoleResponse*)pMsgData); //������ɫ
		break;
	case LL_S_DELETING_ROLE:       //ɾ����ɫ
		//ProcessDeleteRoleResponse((tagNewDelRoleResponse*)pMsgData);
		break;
	case LL_S_WAIT_TO_DEL:         //�Ѿ�ȡ��
		//ProcessGetRoleResponse((tagNotifyPlayerLogin*)pMsgData);
		break;
	case LL_S_WAIT_TO_LOGIN_GAMESERVER:  //�ȴ����ӵ�������
		//ProcessToLoginGameServResponse((tagNotifyPlayerLogin*)pMsgData); //�������ӵ��������ĺ���
		break;
	}
}

void Klogin::SetAccountPassword(const char* pszAccount, const KSG_PASSWORD* pcPassword)
{
	int i;
	if (pszAccount)
	{
		strncpy(m_ClientChoices.Account, pszAccount, sizeof(m_ClientChoices.Account));
		for (i = 0; i < 32; ++i)
			m_ClientChoices.Account[i] = ~m_ClientChoices.Account[i];
	}

	if (pcPassword)
	{
		m_ClientChoices.Password = *pcPassword;
		for (i = 0; i < KSG_PASSWORD_MAX_SIZE; ++i)
			m_ClientChoices.Password.szPassword[i] = ~m_ClientChoices.Password.szPassword[i];
	}
}

void Klogin::GetAccountPassword(char* pszAccount, KSG_PASSWORD* pPassword)
{
	int i;
	if (pszAccount)
	{
		memcpy(pszAccount, m_ClientChoices.Account, sizeof(m_ClientChoices.Account));
		for (i = 0; i < 32; ++i)
			pszAccount[i] = ~pszAccount[i];
	}
	if (pPassword)
	{
		*pPassword = m_ClientChoices.Password;
		for (i = 0; i < KSG_PASSWORD_MAX_SIZE; ++i)
			pPassword->szPassword[i] = ~pPassword->szPassword[i];
	}
}

void Klogin::ClearAccountPassword(bool bAccount, bool bPassword)
{
	if (bAccount)
		memset(m_ClientChoices.Account, 0xff, sizeof(m_ClientChoices.Account));
	if (bPassword)
		memset(&m_ClientChoices.Password, 0xff, sizeof(m_ClientChoices.Password));
}


void Klogin::ProcessAccountLoginResponse(KLoginStructHead* pResponse)
{
	//_ASSERT(m_Status == LL_S_ACCOUNT_CONFIRMING && pResponse != NULL);

	if (((pResponse->Param & LOGIN_ACTION_FILTER) == LOGIN_A_LOGIN) &&	//������ΪҪƥ��
		pResponse->Size >= sizeof(KLoginAccountInfo))				//�������ݵĴ�СҲҪƥ��
	{
		KLoginAccountInfo* pInfo = (KLoginAccountInfo*)pResponse;
		char	szAccount[32]={0};
		KSG_PASSWORD Password;
		GetAccountPassword(szAccount, &Password);  //��ȡ���ݿ���˺ź�����
		if (strcmp(pInfo->Account,  szAccount)  == 0 &&
			strcmp(pInfo->Password.szPassword, Password.szPassword) == 0)
		{//���صĽ��
			KIniFile nBVer;
			double mBver,mBishopver;
			if (!nBVer.Load("\\Settings\\Ver\\Xfsverjx.ini"))//����������İ汾�� GetDouble
			{ 
				isthisVer = false;
				m_Result = LL_R_INVALID_PROTOCOLVERSION;  //�汾����
				//ReturnToIdle();//�Ͽ�����
				nBVer.Clear();
#ifdef WIN32
				CCMessageBox("�汾����","��ʾ:");
#else
                CCMessageBox(UTEXT("�汾����",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
				return;
			}
			nBVer.GetDouble("CilentVer","ver",0.1,&mBver);  //�������汾�� GetDouble
			CcharToDouble(pInfo->nBishopver,mBishopver)	;  //bishiop�������İ汾
			
			if  (pInfo->nSelSererIdx<=0)
			{//���з������ͻ��˶����
			  if (mBishopver!=mBver)
				isthisVer = false;
			  else
				isthisVer = true;
			}
			else
			{
			  if (m_ClientChoices.nServerRegionIndex==pInfo->nSelSererIdx)
			  {//��������������ż��
				  if (mBishopver!=mBver)
					  isthisVer = false;
				  else
					  isthisVer = true;
			  }
			  else //����Ͳ���� ֱ��ͨ��
				  isthisVer = true;
			}
			//------------�࿪���--------------
			if (pInfo->nForbitopennum)
			{
			}
			nBVer.Clear();
			//Ԥ������ɫ���
			if (pInfo->ProtocolVersion!=KPROTOCOL_VERSION)
			{
#ifdef WIN32
				CCMessageBox("�汾������!","��ʾ:");
#else
				CCMessageBox(UTEXT("�汾������!",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
				m_Result = LL_R_INVALID_PROTOCOLVERSION;  //�汾����
				return;
			}
			//----------------------------------
			int nResult = ((pResponse->Param) & ~LOGIN_ACTION_FILTER);
			if (nResult == LOGIN_R_SUCCESS)
			{
				//g_NetConnectAgent.UpdateClientRequestTime(false);
				m_Status = LL_S_WAIT_ROLE_LIST;			  //���ڻ�ȡ��ɫ��Ϣ
				//m_Result = LL_R_ACCOUNT_CONFIRM_SUCCESS;  //���
				//m_LeftTime = pInfo->nLeftTime;
				//m_VipType  = pInfo->nVipType;
				//m_ExtPiont = pInfo->nExtPiont;
				//CCMessageBox("��½�ɹ�","ProcessAccountLoginResponse");
			}
			else
			{
				LOGIN_LOGIC_RESULT_INFO eResult = LL_R_NOTHING;
				switch(nResult)
				{
				case LOGIN_R_ACCOUNT_OR_PASSWORD_ERROR:  //�������
					//eResult = LL_R_ACCOUNT_PWD_ERROR;
					m_Status = LL_S_WAIT_INPUT_ACCOUNT;
#ifdef WIN32
					CCMessageBox("�������","��ʾ:");
#else
					CCMessageBox(UTEXT("�������",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
					break;
				case LOGIN_R_ACCOUNT_EXIST:   //�˺Ÿ�����  --�˺�����ʹ��
					//eResult = LL_R_ACCOUNT_LOCKED;
#ifdef WIN32
					CCMessageBox("�ʺ�����ʹ��","��ʾ:");
#else
                   CCMessageBox(UTEXT("�ʺ�����ʹ��",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
					m_Status = LL_S_WAIT_INPUT_ACCOUNT;
					break;
				case LOGIN_R_FREEZE:  // �˺ű�����
					//eResult = LL_R_ACCOUNT_FREEZE;
#ifdef WIN32
					CCMessageBox("�ʺű�����","��ʾ:");
#else
                    CCMessageBox(UTEXT("�ʺű�����",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
					m_Status = LL_S_WAIT_INPUT_ACCOUNT;
					break;
				case LOGIN_R_INVALID_PROTOCOLVERSION: // �汾������
					//eResult = LL_R_INVALID_PROTOCOLVERSION
#ifdef WIN32
					CCMessageBox("�汾������","��ʾ:");
#else
                   CCMessageBox(UTEXT("�汾������",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
					break;
				case LOGIN_R_FAILED:  //����ʧ��
					//eResult = LL_R_CONNECT_SERV_BUSY; // ��ʾ������æ
#ifdef WIN32
					CCMessageBox("����ʧ��","��ʾ:");
#else
                    CCMessageBox(UTEXT("����ʧ��",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
					break;
				case LOGIN_R_TIMEOUT: // �˺�ʹ��ʱ�䵽
					//eResult = LL_R_ACCOUNT_NOT_ENOUGH_POINT;
#ifdef WIN32
					CCMessageBox("�ʺ��Ѿ�����","��ʾ:");
#else
                    CCMessageBox(UTEXT("�ʺ��Ѿ�����",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
					m_Status = LL_S_WAIT_INPUT_ACCOUNT;
					break;
				default:
#ifdef WIN32
					CCMessageBox("����ʧ��","��ʾ:");
#else
					CCMessageBox(UTEXT("����ʧ��",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
					break;
				}

				if (m_Status != LL_S_WAIT_INPUT_ACCOUNT)
				{//�Ͽ�����
					//ReturnToIdle();
				}
				m_Result = eResult;
			}
		}
		memset(szAccount, 0, sizeof(szAccount));
		memset(&Password, 0, sizeof(Password));
	}
}


//--------------------------------------------------------------------------
//	���ܣ���ɫ�б���
//	״̬�л����ɹ� LL_S_WAIT_ROLE_LIST -> LL_S_ROLE_LIST_READY
//			  ʧ�� ״̬����
//--------------------------------------------------------------------------
void Klogin::ProcessRoleListResponse(TProcessData* pResponse)
{
	//_ASSERT(m_Status == LL_S_WAIT_ROLE_LIST && pResponse != NULL);
	if (pResponse->nProtoId == s2c_roleserver_getrolelist_result)
	{
		m_nNumRole = pResponse->pDataBuffer[0];
		if (m_nNumRole > MAX_PLAYER_PER_ACCOUNT)
			m_nNumRole = MAX_PLAYER_PER_ACCOUNT;
		else if (m_nNumRole < 0)
			m_nNumRole = 0;

		RoleBaseInfo* pList = (RoleBaseInfo*)&pResponse->pDataBuffer[1];
		for (int i = 0; i < m_nNumRole; ++i)
		{
			if (pList->szName[0])
			{
				strcpy(m_RoleList[i].Name, pList->szName);
				m_RoleList[i].Attribute = pList->Series;
				m_RoleList[i].Gender = pList->Sex;
				m_RoleList[i].nLevel = pList->Level;
				pList ++;
			}
			else
			{
				m_nNumRole = i;
				break;
			}
		}
		//g_NetConnectAgent.UpdateClientRequestTime(true);
		m_Status = LL_S_ROLE_LIST_READY;

		///CCMessageBox("��ɫ����׼���õ�½","ProcessRoleListResponse");
		//m_Result = LL_R_NOTHING;
		/*if (m_bInAutoProgress)
		{//������Զ���½
			int nAdviceChoice;
			GetRoleCount(nAdviceChoice);
			SelectRole(nAdviceChoice);
		}*/
	}
}

void Klogin::mExit(CCObject* pSender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif

}
/*
//���̵�����������ý���ʱ����
void Klogin::editBoxEditingDidBegin(extension::CCEditBox *editBox)
{
	CCLog("begin");
}

//�������غ������ʧȥ����ʱ����
void Klogin::editBoxEditingDidEnd(extension::CCEditBox *editBox)
{
	CCLog("end");
}

//��������ı��仯ʱ����
void Klogin::editBoxTextChanged(extension::CCEditBox *editBox, const std::string &text)
{
	CCLog("change");
}

//���·��ؼ������������λ��ʱ�����������ǰ�����ʱ��������
void Klogin::editBoxReturn(extension::CCEditBox *editBox)
{
	CCLog("return");
}*/