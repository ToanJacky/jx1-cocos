
#include "engine/KbugInfo.h"
#include "KgameWorld.h"
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
extern iCoreShell*	g_pCoreShell; //ȫ�ֵ���
extern KImageStore2 m_ImageStore; //ȫ�ֵ���
//extern KPakList     g_pPakList;
//extern KPakList * g_pPakList;
#include <list>
USING_NS_CC;

CCScene* KgameWorld::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	KgameWorld *layer = KgameWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool KgameWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	
	visibleSize        = CCDirector::sharedDirector()->getVisibleSize();//�ɼ���
	origin             = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSize size        = CCDirector::sharedDirector()->getWinSize();
	m_sprCount         =0;
	nTimeCount = 0;
	m_nWidth   = 0;
	m_nHeight  = 0;
	pWW        =NULL;
	animate    =NULL;
	setTouchEnabled(true);

	if (!m_ImageStore.Init())  //��ʼ��ͼƬ������
		CCMessageBox(UTEXT("�ռ䲻��").c_str(),"gameclient");
	if (g_pCoreShell)
	{
		if (g_pCoreShell->test())
		  CCMessageBox("test is suss","g_pCoreShell");
	}
	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
		"mainskillmix.png",
		"mianskillbig.png",
		this,
		menu_selector(KgameWorld::mianSkillCallback)); //�ص�����
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2-20 ,
		origin.y + pCloseItem->getContentSize().height/2+20));
	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition(CCPointZero);
	m_sprCount ++;
	this->addChild(pMenu,1,m_sprCount);
	/////////////////////////////
	// 3. add your codes below...
	// add a label shows "Hello World"
	// create and initialize a label
	const char * strTiele = CXmlStream::GetStringByKeyFromFile("string.xml","title"); //Resources ��Դ����Ŀ¼��,���û������ļ���������
	KIniFile test;
	//KPakList g_pPakList;
	char strtest[32]={0};
	//CCFileUtils::getPathForFilename()
	std::string nPath = CCFileUtils::sharedFileUtils()->getWritablePath();

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
	std::string fielDir;
	fielDir = "package.ini";
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	fielDir = "download\\package.ini";
#endif
	//nCount = g_pPakList.Open(fielDir.c_str());  //����PAK
	//CCFileUtils::getFullPathForDirectoryAndFilename("Resources","test.ini");
	//fullPathForFilename(filename.c_str());
	if (test.Load(fielDir.c_str()))  //"D:\\cocos2dx226\\projects\\jxclient\\Resources\\test.ini"
	{
		test.GetString("Package","Path","world",strtest,sizeof(strtest));
	}
	test.Clear();
	start.Start();
	char nCurStr[128]={0};
	sprintf(nCurStr,"(%s)%d \ntime:%u",strtest,nCount,start.GetElapse());
	//CCLabelTTF* 
	pMainLabel = CCLabelTTF::create(strTiele,"Arial", 24);
	pMainLabel->setString(nCurStr);//������ʾ������
	//CCMessageBox(nPath.c_str(),"title");
	//CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
	// position the label on the center of the screen
	pMainLabel->setPosition(ccp(origin.x + visibleSize.width/2,
		origin.y + visibleSize.height - pMainLabel->getContentSize().height));

	// add the label as a child to this layer
	//m_sprCount++;
	this->addChild(pMainLabel, 1);

	CCSprite *bgSprite=CCSprite::create("bg.jpg");
	bgSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	float winw = visibleSize.width;   //��ȡ��Ļ���
	float winh = visibleSize.height;  //��ȡ��Ļ�߶�
	float spx  = bgSprite->getTextureRect().getMaxX();
	float spy  = bgSprite->getTextureRect().getMaxY();
	bgSprite->setScaleX(winw/spx); //���þ��������ű���
	bgSprite->setScaleY(winh/spy);
	//m_sprCount++;
	//this->addChild(bgSprite,0,m_sprCount);

	CCMenuItemFont* itemReset = CCMenuItemFont::create(UTEXT("�˳�").c_str(),this,menu_selector(KgameWorld::beiyongb));
	//itemReset->setFontName("fonts/����������.TTF");
	itemReset->setPosition(ccp(visibleSize.width/2, 50));
	itemReset->setFontSize(16);

	CCMenuItemFont* itemGetClientVersion = CCMenuItemFont::create(UTEXT("����").c_str(),this,menu_selector(KgameWorld::beiyonga));
	//itemGetClientVersion->setFontName("fonts/����������.TTF");
	itemGetClientVersion->setPosition(ccp(visibleSize.width/2, 100));
	itemGetClientVersion->setFontSize(16);
	CCMenuItemFont* itemGetServerVersion = CCMenuItemFont::create(UTEXT("����NPC").c_str(),this,menu_selector(KgameWorld::nextsenc));
	itemGetServerVersion->setPosition(ccp(visibleSize.width/2, 150));
	itemGetServerVersion->setFontSize(16);
	CCMenuItemFont* itemUpdateVersion = CCMenuItemFont::create(UTEXT("ɾ������").c_str(),this,menu_selector(KgameWorld::delspr));
	itemUpdateVersion->setPosition(ccp(visibleSize.width/2, 200));
	itemUpdateVersion->setFontSize(16);
	CCMenuItemFont* itemEnterScene = CCMenuItemFont::create(UTEXT("���Ӿ���").c_str(),this,menu_selector(KgameWorld::addspr));
	itemEnterScene->setPosition(ccp(visibleSize.width/2, 250));
	itemEnterScene->setFontSize(16);

	CCMenuItemFont* itemaddmaop = CCMenuItemFont::create(UTEXT("���ص�ͼ").c_str(),this,menu_selector(KgameWorld::addmap));
	itemaddmaop->setPosition(ccp(visibleSize.width/2, 300));
	itemaddmaop->setFontSize(16);
	itemaddmaop->setTag(1);   //���ñ�־
	//CCAssert(,"");//����
	CCMenu *newmenu=CCMenu::create(itemaddmaop,itemEnterScene,itemUpdateVersion,itemGetServerVersion,itemGetClientVersion,itemReset,NULL);  // �˲��ܹؼ�������ֻ�ǲ˵�������ܵ��
	newmenu->setPosition(ccp(origin.x+70, visibleSize.height/2 + origin.y+150));//�����˵���λ��
	newmenu->alignItemsVertically();       //����Ϊ��������
	//m_sprCount++;
	this->addChild(newmenu,2);             //
	CCNode * d = newmenu->getChildByTag(1);//��ȡ���ڵ��µ��ӽڵ�
	//newmenu->removeChild(d,true);          //ɾ�����ڵ��µ��ӽڵ�
	//newmenu->removeChildByTag(1);
	//itemaddmaop->removeFromParentAndCleanup(true);  //
	d->removeFromParentAndCleanup(true);
	//d->release();//�ͷ����ڴ档������

	if (itemaddmaop)
	   newmenu->addChild(itemaddmaop,true);  //���ڵ�������һ���ӽڵ�
	//����һ�������
	//CCComponent::create();
	//newmenu->setUserData();//Ϊĳ���ڵ���Զ�������
	//��Ⱦ���� ͼƬ����
	/*
	List<CCTexture2D> loadTexture = new list_t<CCTexture2D>();
	List<CCSprite> loadsprite     = new list_t<CCSprite>();
	*/
	m_pTexture = NULL;
	
	m_pTexture = new CCTexture2D();

	if (m_pTexture)
	{//�����￪ʼ��ȡͼ�����ݵ�ָ��data
		//unsigned char *data = NULL;
		//unsigned long m_size = 0;
		//memset()
		/*CCImage* image = new CCImage();

		if (!image)
		{
			CCMessageBox("image a","title");
			return false;
		}
		//bool bRet = image->initWithImageFile("bg.jpg",CCImage::kFmtJpg);
		*/
		//std::string bFilePath = CCFileUtils::sharedFileUtils()->fullPathForFilename("laohuji.spr");//("MA_BD_075_ST01.spr");//
		/*FILE *fp=NULL;
		    fp = CCFileUtils::sharedFileUtils()->getFilePtr("laohuji.spr", "rb", &m_size);
		if (!fp)
		{
			CCMessageBox("fp is NULL","title");
			return true;
		}
		*/

		//char nSprInfo[128]={0};
		//sprintf(nSprInfo,"m_nWidth:%d,m_nHeight:%d,m_nFrames:%d %d/%d\n��ɫ:%d ƫ��:%d/%d",m_nWidth,m_nHeight,m_nFrames,pFrame->Width,pFrame->Height,m_nColors,pFrame->OffsetX,pFrame->OffsetY);
		//CCMessageBox(nSprInfo,"title");
	     //RenderToA4R4G4B4Alpha2(pW,(BYTE *)pNode->m_pBitmap,pNode->m_nWidth,pNode->m_nHeight,(BYTE *)pNode->m_pPalette,m_ptDes,m_rtSrc,nPitch,pCanvas->m_nMask32);
		//SprDecode.RenderToSprDecode(pW,(BYTE *)pFrame->Sprite,m_nWidth,m_nHeight,m_Palette);
		
		 CCTexture2D *Cur = SprDecode.RenderToSprDecode_New("\\spr\\skill\\1502\\kl\\kl_150_jiankun_fu.spr",10,&m_nWidth,&m_nHeight);
		//SprDecode.RenderToSprDecode("laohuji.spr",0,pW);\\spr\\skill\\1502\\kl\\kl_150_jiankun_fu.spr
		/*stImageInfo m_info;
		m_info.bytes_per_pixel=2;
		m_info.width          =m_nWidth;
		m_info.height         =m_nHeight;
		m_info.buffer         = (BYTE *)pW;
		SprDecode.SaveBMPFile("D://test.bmp",m_info);*/
		//---------------
		/*��Ҫ���� png ���ݸ�ʽ
		bool bRet = image->initWithImageData(pFrame->Sprite,m_size, CCImage::kFmtJpg);
		if (!bRet)
		{
			CCMessageBox("load bg.jpg er","title");
			return true;
		}
		bool ret = false;
		     ret = m_pTexture->initWithImage(image);
	    */
		//CCSize imageSize =CCSizeMake((float)(m_nWidth), (float)(m_nHeight));

		/*CCRenderTexture* renderTexture=CCRenderTexture::create(m_nWidth, m_nHeight);//ָ�������Ĵ�С
		renderTexture->beginWithClear(0, 23, 5, 200);                    //����begin()��ʼ�ڻ����ϻ�������
	    renderTexture->end();                                            //��Ⱦ���
		renderTexture->getSprite()->getTexture()->initWithData(pWW,(CCTexture2DPixelFormat)kTexture2DPixelFormat_RGBA8888,m_nWidth,m_nHeight,imageSize);
		CCSprite * dynamicTexture=CCSprite::createWithTexture(renderTexture->getSprite()->getTexture());//ʹ�ö�̬����������
		dynamicTexture->setPosition(ccp(m_nWidth + origin.x+400, visibleSize.height/2 + origin.y));
	    this->addChild(dynamicTexture);*/

		//std::string bFilePath = CCFileUtils::sharedFileUtils()->fullPathForFilename("bg.jpg");
		//data = CCFileUtils::sharedFileUtils()->getFileData(bFilePath.c_str(), "rb", &m_size);
		
		//imageSize.width = visibleSize.width;
		//imageSize.height= visibleSize.height;
		//m_pTexture->initWithData(pWW,(CCTexture2DPixelFormat)kTexture2DPixelFormat_RGBA8888,m_nWidth,m_nHeight,imageSize); //CCSizeMake((float)w, (float)h)
		/*CCRect rect;
			 rect.size = CCSizeMake((float)(100), (float)(200));
			 rect.origin.x =0;
			 rect.origin.y =0; �ϻ������
		*/
		//CCMessageBox(m_pTexture->description(),"title");
		testSprite = NULL;
		if (Cur)
		{ 
			/*char nSprFilePath[256]={0};
			sprintf(nSprFilePath,"%s_%d","kl_150_jiankun_fu.spr",10);
			CCTexture2D* OldText=NULL;
			if (OldText = CCTextureCache::sharedTextureCache()->isFileExist(nSprFilePath))
			   CCMessageBox(Cur->description(),"title");*/
			testSprite=CCSprite::createWithTexture(Cur);
			testSprite->setPosition(ccp(m_nWidth + origin.x+100, visibleSize.height/2 + origin.y));
			m_sprCount++;
			this->addChild(testSprite,2,m_sprCount);

			char nSprFilePath[64]={0};
			CCAnimation*animation = CCAnimation::create();//����һ������

            //std:string nFileName ="\\spr\\skill\\1502\\kl\\kl_150_jiankun_fu.spr";
			//g_StrLower(nFileName);
			DWORD nFielpahtdwid = g_FileName2Id("\\spr\\skill\\1502\\kl\\kl_150_jiankun_fu.spr");
			
			for (int i=1;i<21;i++)
			{   
				//CCTexture2D* nCurCount=NULL;
				sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,i);  
				CCMessageBox(nSprFilePath,"animation");
				//nCurCount = CCTextureCache::sharedTextureCache()->isFileExist(nSprFilePath);//������ڵ� ��
				//if (nCurCount)
				//testSprite->setTexture(nCurCount);//���û�� ���Զ�����һ��
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
			   testSprite->runAction(CCSequence::create(animate,CCCallFunc::create(this, callfunc_selector(KgameWorld::finish)),NULL));

			//if(testSprite->isActionDone(animate))
			 // removeChild(testSprite,true);
			//CCMoveTo* move = CCMoveTo::create(30, ccp(-20, 200));
			//testSprite->runAction(move); 
			//removeChild(testSprite,true);
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

		/*if (pWW)
		{
		   free (pWW);
		}*/

		//delete pW;pW=NULL;
		//delete data;data=NULL;
		CCSprite *lpSprite=CCSprite::create("jin.png");
		lpSprite->setPosition(ccp(visibleSize.width/2 +origin.x, visibleSize.height/2 + origin.y));
		m_sprCount++;
		this->addChild(lpSprite,1,m_sprCount);
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
	}
	else
	{

	}

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
	//ҡ��
	CCSprite *controlSprite=CCSprite::create("yaoganx.png");
	//ҡ�˱���
	CCSprite *ygSprite=CCSprite::create("yaoganz.png");
	joystick=HRocker::HRockerWithCenter(ccp(100.0f,100.0f),60.0f ,controlSprite ,ygSprite,false);
	m_sprCount++;
	this->addChild(joystick,2,m_sprCount);
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

	return true;
}

//�رհ�ť�ص�����
void KgameWorld::mianSkillCallback(CCObject* pSender)
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
void KgameWorld::update(float delta)
{
	if (nTimeCount*1000 <= start.GetElapse()*18) //
	{//�ÿͻ��������� ÿ������һ��
		if (g_pCoreShell)
		    g_pCoreShell->Breathe();
		nTimeCount++;
	//g_SubWorldSet.m_nLoopRate=10000;
	char nPiontInfo[64*3]={0}; //start.GetElapse()
	sprintf(nPiontInfo,"%u,%u,%u",nTimeCount,g_SubWorldSet.m_nLoopRate,start.GetElapse()); //g_pCoreShell->GetSubGameTime()
	pMainLabel->setString(nPiontInfo);
	}
	if (nTimeCount*1000 >= start.GetElapse()*18)
	{
		//UiPaint(nGameFps);    //�滭��Ϸ���� 
		Sleep(1);               //̫�����ͣһ��
	}
	else if ((nTimeCount >> 3))   //ÿ��֡��ͣ
	{//����ֹͣ�滭��Ϸ���磬�Ϳ�ס
		Sleep(1);
	}

	DWORD nFielpahtdwid = g_FileName2Id("\\spr\\skill\\1502\\kl\\kl_150_jiankun_fu.spr");
	for (int i=1;i<21;i++)
	{   		
		char nSprFilePath[64]={0};
		sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,i);  
		CCTexture2D* nCurTexture = CCTextureCache::sharedTextureCache()->isFileExist(nSprFilePath);//������ڵ� ��
		if (!nCurTexture)
			continue;
		CCAnimation*animation = CCAnimation::create();//����һ������
		animation->addSpriteFrameWithSprName(nSprFilePath);//�����������������֡
		animation->setDelayPerUnit(0.02f);
		//���ò���ѭ�� һֱ���� Ϊ-1
		animation->setLoops(1);
		//���ö���������ָ�����һ֡ setRestoreOriginalFrame
		animation->setRestoreOriginalFrame(TRUE);
		// ����һ����Զѭ���Ķ���
		animate = CCAnimate::create(animation);
		CCSprite * tSprite=CCSprite::createWithTexture(nCurTexture);
		tSprite->setPosition(ccp(m_nWidth + origin.x+300, visibleSize.height/2 + origin.y));
		this->addChild(tSprite,2,100);
		tSprite->runAction(CCSequence::create(animate,CCCallFunc::create(this, callfunc_selector(KgameWorld::endSprite)),NULL));
		
	}

	if  (m_pTexture && testSprite)
	{//������������ڵĻ�,ˢ�������������� �ܴﵽ��������ʾ��
			//pWW = SprDecode.RenderToSprDecode_New("kl_150_jiankun_fu.spr",18,&m_nWidth,&m_nHeight);//�ڴ�й¶
				/*m_pTexture->DeleteTexture();

				CCSize imageSize =CCSizeMake((float)(m_nWidth), (float)(m_nHeight));
			    //�ڴ�й¶
				m_pTexture->initWithData(pWW,(CCTexture2DPixelFormat)kTexture2DPixelFormat_RGBA8888,m_nWidth,m_nHeight,imageSize); //CCSizeMake((float)w, (float)h)
				*/
		/*if (animate)
		{
			if (testSprite->isActionDone(animate))
			{
				testSprite->stopAction(animate);
				removeChild(testSprite,true);
				testSprite->release();
				testSprite=NULL;
			}
		}*/
	}

	int newX = 0;
	int newY = 0;
	//�ж��Ƿ���ҡ�˼�������
    //CCPoint nCurPoint= joystick->getDirection();
	//float n = joystick->getAngleSigned();
	/*char nPiontInfo[64]={0};
	sprintf(nPiontInfo,"%f",n);
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


void KgameWorld::draw()
{
	/*nTestCount ++;
	char nPiontInfo[64]={0};
	sprintf(nPiontInfo,"%d",nTestCount);
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
bool KgameWorld::isFileExist(const char* pFileName)
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

void KgameWorld::copyData(const char* pFileName)
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


void KgameWorld::endSprite()
{
	//removeChild(tSprite,true);
	CCNode* s = getChildByTag(100);
	if (s)
		removeChild(s,true);
}

void KgameWorld::finish()
{
		if (testSprite)
		{
		   testSprite->stopAction(animate);
		   removeChild(testSprite,true);
		   //testSprite->removeFromParent();
		   testSprite->release();
		   //testSprite=NULL;
		}

		/*if (testSprite)
		{
		 addChild(testSprite,2,1);
		}*/
}
/*
void KgameWorld::onEnter()
{

}

void KgameWorld::onExit()
{

}
//����� �� �������
void KgameWorld::onEnterTransitionDidFinish()
{

}*/

bool KgameWorld::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}
void KgameWorld::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}

void KgameWorld::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{

}
// default implements are used to call script callback if exist
void KgameWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{

}
void KgameWorld::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{

}
void KgameWorld::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
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
void KgameWorld::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{

}
void KgameWorld::didAccelerate(CCAcceleration* pAccelerationValue)
{
	
}
void KgameWorld::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{//convertTouchToNodeSpace
   /*CCPoint touchLocation = convertTouchToNodeSpace(pTouch);
   if(CCRect::CCRectContainsPoint(getRect(pSprite), touchLocation))
	{
	   printf("�ұ�������!\n");
	}*/
}

void KgameWorld::beiyongb(CCObject* pSender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif

}
//�����Ի����
void KgameWorld::beiyonga(CCObject* pSender)
{//ģ̬�Ի���ֻ�ܶԱ������в���
	// ����һ�������㣬����һ�ű���ͼ
	PopupLayer* pl = PopupLayer::create("laohuji.png"); //����һ���� ���ڵ�
	// ContentSize �ǿ�ѡ�����ã����Բ����ã�������ð������� 9 ͼ����
	pl->setContentSize(CCSizeMake(400, 360));
	pl->setTitle(UTEXT("����һҶ").c_str());
	pl->setContentText(UTEXT("������÷�����ͣ�ȴ���ķҰ���ء������ٻ������ޣ�������ӣһ֦�㡣").c_str(), 20, 50, 150);
	// ���ûص��������ص�����һ�� CCNode �Ի�ȡ tag �жϵ���İ�ť
	// ��ֻ����Ϊһ�ַ�װʵ�֣����ʹ�� delegate �Ǿ��ܹ������Ŀ��Ʋ�����
	pl->setCallbackFunc(this, callfuncN_selector(KgameWorld::buttonCallback));
	// ��Ӱ�ť������ͼƬ�����֣�tag ��Ϣ
	pl->addButton("ok.png","ok.png","", 0);      //�ڸ��ڵ��´���һ���ӽڵ㰴ť 0
	pl->addButton("close.png","close.png","", 1);//�ڸ��ڵ��´���һ���ӽڵ㰴ť 1
	// ��ӵ���ǰ��
	pl->setPosition(ccp(0,0));
	this->addChild(pl,2,1000); //�����������Ӹ��ڵ�
}
//�����Ի���Ļص�����
void KgameWorld::buttonCallback(CCNode *pNode)
{//��������б�Ǻţ���ȷ�����ĸ���ť
	CCLog("button call back. tag: %d", pNode->getTag());
}


void  KgameWorld::nextsenc(CCObject* pSender)
{//����һ��NPC

}

//���ص�ͼ
void KgameWorld::addmap(CCObject* pSender)
{	
	//MTNotificationQueue::sharedNotificationQueue()->postNotification("connectok", NULL
}

void KgameWorld::addspr(CCObject* pSender){	
	if (m_sprCount>0)
	{
		for (int i=1;i<=m_sprCount;i++)
		{
			CCNode* s = getChildByTag(i);
			if (s)
			   addChild(s,2,i);
		}
	}

}

void KgameWorld::delspr(CCObject* pSender){

	if (m_sprCount>0)
	{
		for (int i=1;i<=m_sprCount;i++)
		{
			CCNode* s = getChildByTag(i);
			if (s)
				removeChild(s,true);
		}
		
	}	
}