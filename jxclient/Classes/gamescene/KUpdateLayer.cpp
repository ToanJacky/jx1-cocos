#include "KUpdateLayer.h"
#include "gamecore/KCore.h"
#include "gamecore/CoreShell.h"
#include "gamescene/KgameWorld.h"
#include "engine/KGbktoUtf8.h"
#include "gameui/Klogin.h"
#include "gameui/Klogin_f.h"
#include "gameui/KuiInPutIp.h"
#include "engine/dataChecksum.h"
#include <stdio.h>
#include <string.h>
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#include <jni.h>
#include <memory.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#endif
#ifdef WIN32
#include <process.h>
#endif
iCoreShell * g_pCoreShell = NULL;
extern KImageStore2 m_ImageStore; //ȫ�ֵ���
//extern KPakList * g_pPakList;
#define ERROR_TIME_OUT 300
KUpdateLayer::KUpdateLayer()
{
	nDownState = STATE_NONE_UP;
	downPtr = NULL;
	isOpenDown = true;
	errorLoop = 0;
	nPakCount =0;
	pakDownCount = 0;
	//pakreload    = 0;
	m_WritablePath ="";
	pListData = NULL;
	isHaveClient = false;
	m_IsInPutIpPadIsOpen=false;
	ZeroMemory(downUrlmask,sizeof(downUrlmask));
	isOver=false;
}

KUpdateLayer::~KUpdateLayer()
{
	if (downPtr)
		downPtr->release();
	if (pListData)
	{
		free(pListData);
		pListData=NULL;
	}
	isOver=false;
	m_IsInPutIpPadIsOpen=false;
}

bool KUpdateLayer::init()
{
	   //CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/Am0.wav",true);//��������
	   //������
	   //CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/Am0.wav");//ר���������ż�̵���Ч
	   if (!CCLayer::init())
	   {  
		  return false;
	   }
		// ������Դ������Ŀ¼
	   __pSound       = CocosDenshion::SimpleAudioEngine::sharedEngine();
	   ccFileUtils    = CCFileUtils::sharedFileUtils();
	    m_downloadDir = ccFileUtils->getWritablePath();
	    m_SDcardDirPath = m_WritablePath= m_downloadDir;
#ifndef WIN32
		m_MobileKey   = getappstrInfo();
#endif
		//CCMessageBox(m_downloadDir.c_str(),"m_downloadDir");
		m_downloadDir += "data"; //download
		initDownloadDir(); //�����ļ���
		creatDownloadDir("music");
		creatDownloadDir("sound");
		m_bIsClientConnecting = false;
		nCurpFileName = "";
		writelen      = 0;
		//----------------------------------����һ��������
		m_nAllLoadResources = 50;     //����Դ����
		m_nLoadedResources  = 0;     //�Ѿ������˵�����
		m_fProgressIndex    = 0.0;
		m_nLoadedFinish     = false;
		// �����Դ������·��������·���������������µ���Դ	Init
		//std::vector 
		/*std::vector<std::string> searchPaths = CCFileUtils::sharedFileUtils()->getSearchPaths();
		searchPaths.insert(searchPaths.begin(), m_downloadDir);
		CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
		*/
		visibleSize        = CCDirector::sharedDirector()->getVisibleSize();//�ɼ���
		origin             = CCDirector::sharedDirector()->getVisibleOrigin();
		size               = CCDirector::sharedDirector()->getWinSize();

		//if (!isFileExist("package.ini"))
		//�����ھͿ�ʼ����
	if (_clientlanguage!=1)
		copyData("package_vn.ini");
	else
		copyData("package.ini");

	/*if  (_clientNoNetWork)
	{
		copyData("package_temp.ini");
	}*/
		//else
		++m_nLoadedResources;
		//��ʼ����pak
		std::string fielDir;
if (_clientlanguage!=1)
{
	fielDir = "package_vn.ini";
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	fielDir = "data\\package_vn.ini";
#endif
}
else
{
	fielDir = "package.ini";
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	fielDir = "data\\package.ini";
#endif
}
//���ر���
    CCSprite* sprite = CCSprite::create("longin_bg.png");
    //ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y)
    sprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    float winw = visibleSize.width; //��ȡ��Ļ���
    float winh = visibleSize.height;//��ȡ��Ļ�߶�
    float spx = sprite->getTextureRect().getMaxX();
    float spy = sprite->getTextureRect().getMaxY();
    sprite->setScaleX(winw/spx); //���þ��������ű���
    sprite->setScaleY(winh/spy);
    this->addChild(sprite,0);

if (!_clientNoNetWork)
{//���ǵ���
		KIniFile dwmask;
		char nTempstr[128];
		t_sprintf(nTempstr,fielDir.c_str());
		if (dwmask.Load(nTempstr))
		{
			dwmask.GetString("Package","down","",downUrlmask,sizeof(downUrlmask));
			dwmask.Clear();
			//dataChecksum nurlMask;
			char donwPath[64]={0},nTempPaht[128];
			//ZeroMemory(donwPath,sizeof(donwPath));
			//ZeroMemory(nTempPaht,sizeof(nTempPaht));
			//nurlMask.SimplyDecrypt(donwPath,downUrlmask);
			t_sprintf(nTempPaht,"http://%s/", downUrlmask);
			//std::string nTempPaht ="http://"+donwPath;
			//����һ������ָ��
			downPtr = downloadFile::GetInst(nTempPaht,m_WritablePath);
			//if (downPtr)
			//CCMessageBox(m_downloadDir.c_str(),"22222");
		}
		if (sprite)//�����߳�
			startload();
}
else
{//����һ������Ի��� д��ip
	/*KIniFile dwmask;
	char nTempstr[128];
	t_sprintf(nTempstr,"data\\package_temp.ini");
	if (dwmask.Load(nTempstr))
	{
		dwmask.GetString("Package","down","",downUrlmask,sizeof(downUrlmask));
		dwmask.Clear();

		if  (downUrlmask[0])
		{
			char nTempPaht[128];
			ZeroMemory(nTempPaht,sizeof(nTempPaht));
			t_sprintf(nTempPaht,"http://%s:%d/",downUrlmask,8080);
			//std::string nTempPaht ="http://"+donwPath;
			//����һ������ָ��
			downPtr = downloadFile::GetInst(nTempPaht,m_WritablePath);
			isOver=true;
			if (sprite)//�����߳�
				startload();
		}
		else
		{
			CCLayer * sAccLayer = KuiInPutIp::create(this,callfuncN_selector(KUpdateLayer::startInPutCallback));
			m_IsInPutIpPadIsOpen = true;
			this->addChild(sAccLayer,ORDER_UI_DIALOD);
		}
	}
	else
	{//�����Ի���
		CCLayer * sAccLayer = KuiInPutIp::create(this,callfuncN_selector(KUpdateLayer::startInPutCallback));
		m_IsInPutIpPadIsOpen = true;
		this->addChild(sAccLayer,ORDER_UI_DIALOD);
	}*/

}
		// ״̬��ʾ
		m_label = CCLabelTTF::create("",UI_GAME_FONT_DEFAULT,18);
		m_label->setAnchorPoint(ccp(1,0.5));
		m_label->setPosition(ccp(465,20));
		this->addChild(m_label,2);

		m_ImageStore.Init(); //��ʼ���������
		    ++m_nLoadedResources;
		//m_fWaitTime = 0;
		m_pLabelLoading = CCLabelTTF::create("loading...", "Arial", 15);
		m_pLabelPercent = CCLabelTTF::create("%0", "Arial", 15);
		m_pLabelLoading->setPosition(ccp(size.width/2, size.height/4-70));
		m_pLabelPercent->setPosition(ccp(size.width/2, size.height/4-90));
		this->addChild(m_pLabelLoading,2);
		this->addChild(m_pLabelPercent,2);
		//loading�Ķ���Ч��
		m_pLoadBarStart = CCSprite::create("loadingStart.png");
		m_pLoadBarStart->setPosition(ccp(size.width/2, size.height/4-50));
		float sx  = m_pLoadBarStart->getTextureRect().getMaxX();
		//float sy  = m_pLoadBarStart->getTextureRect().getMaxY();
		m_pLoadBarStart->setScaleY(2.0f);
		m_pLoadBarStart->setScaleX(winw/sx);
		this->addChild(m_pLoadBarStart,2);

		CCSprite *m_pLoadEndSpr = CCSprite::create("loadingEnd.png");
		sx = m_pLoadEndSpr->getTextureRect().getMaxX();
		//m_pLoadEndSpr->setScaleX(winw/sx);

		m_pLoadBarEnd = CCProgressTimer::create(m_pLoadEndSpr);
		m_pLoadBarEnd->setPercentage(1.0f);
		m_pLoadBarEnd->setPosition(ccp(size.width/2, size.height/4-50));
		m_pLoadBarEnd->setType(kCCProgressTimerTypeBar);
		m_pLoadBarEnd->setBarChangeRate(ccp(1, 0));
		m_pLoadBarEnd->setMidpoint(ccp(0, 0));
		m_pLoadBarEnd->setScaleX(winw/sx);
		m_pLoadBarEnd->setScaleY(2.0f);
		this->addChild(m_pLoadBarEnd,2);
		this->scheduleUpdate();
		return true;
}

//��ȷ���� ��ʼ�߳�
void KUpdateLayer::startInPutCallback(CCNode *pNode)
{
	if (!pNode) return;

	if  (pNode->getTag()==2)
	{//ȡ����ť
		m_IsInPutIpPadIsOpen = false;
		return;
	}

	m_IsInPutIpPadIsOpen = false;
	KIniFile dwmask;
	char nTempstr[128];
	t_sprintf(nTempstr,"data\\package_temp.ini");
	if (dwmask.Load(nTempstr))
	{
		dwmask.GetString("Package","down","",downUrlmask,sizeof(downUrlmask));
		dwmask.Clear();
		char nTempPaht[128];
		ZeroMemory(nTempPaht,sizeof(nTempPaht));
		t_sprintf(nTempPaht,"http://%s:%d/",downUrlmask,8080);
		//std::string nTempPaht ="http://"+donwPath;
		//����һ������ָ��
		downPtr = downloadFile::GetInst(nTempPaht,m_WritablePath);
		isOver=true;
		startload();
		return;
	}
	
    CCMessageBox("The Server Ip Error","Error");
}

void KUpdateLayer::creatDownloadDir(std::string nTempPath){

	//�������Ŀ¼�����ڣ��򴴽�����Ŀ¼
    std::string m_TempDir = ccFileUtils->getWritablePath();
	//CCMessageBox(m_downloadDir.c_str(),"m_downloadDir");
	m_TempDir += nTempPath; //download
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	DIR *pDir = NULL;
	pDir = opendir(m_TempDir.c_str());
	if (!pDir)
	{
		mkdir(m_TempDir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	}
	else
		closedir(pDir);
#else
	if ((GetFileAttributesA(m_TempDir.c_str())) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(m_TempDir.c_str(), 0);
	}
#endif
}

void KUpdateLayer::initDownloadDir(){

//�������Ŀ¼�����ڣ��򴴽�����Ŀ¼
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	DIR *pDir = NULL;

	pDir = opendir(m_downloadDir.c_str());
	if (!pDir)
	{
		mkdir(m_downloadDir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	}
#else
	if ((GetFileAttributesA(m_downloadDir.c_str())) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(m_downloadDir.c_str(), 0);
	}
#endif
}
//ɾ��Ŀ¼
void KUpdateLayer::deleteDir(std::string dir){
	// Remove downloaded files
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	std::string command = "rm -r ";
	// Path may include space.
	command += "\"" + dir + "\"";
	system(command.c_str());
#else
	std::string command = "rd /s /q ";
	// Path may include space.
	command += "\"" + dir + "\"";
	system(command.c_str());
#endif
}

//������Դ
void KUpdateLayer::updates(CCObject* pSender){
	// ��ʼ������Ŀ¼
	initDownloadDir();
}
//������ť
void KUpdateLayer::reset(CCObject* pSender){
	if ("" != m_downloadDir)
	{
		// ɾ������Ŀ¼
		deleteDir(m_downloadDir);
	}

}
//��ȡ�汾
void  KUpdateLayer::getClientVersion(CCObject* pSender){
	CCString* msg = CCString::createWithFormat("current client version : %s","test");
	//CCLOG("%s",msg->getCString());
	m_label->setString(msg->getCString());
}
//������
void KUpdateLayer::checkUpdate(CCObject* pSender){	

}
//���볡��
void KUpdateLayer::enterScene(CCObject* pSender){

	/*if (nAcclient)
	{
		if (!nAcclient->FsGameServerConnectTo("192.168.100.8",56722))
		{   
			m_bIsClientConnecting = true;
			nAcclient->Startup();     //������Ϸ���������߳�
			CCMessageBox("���ӷ�����ʧ�ܣ�","��ʾ:");
			return;
		}
	}
	else
		return;*/
	//CCDirector::sharedDirector()->replaceScene(Klogin_f::scene());
}
//���볡���� ������Դ �ͳ�ʼ���ͻ���


int KUpdateLayer::isFileExist(const char* pFileName)
{
	if( !pFileName ) return false;
	//strFilePathName is :/data/data/ + package name
	//std::string filePath = CCFileUtils::sharedFileUtils()->getWritablePath();//getWriteablePath();
	
	std::string filePath = m_downloadDir;
	filePath +=  "/";
	filePath +=pFileName;
	//filePath +=pFileName;
	int pSize = 0;
	FILE *fp = fopen(filePath.c_str(),"r");
	if(fp)
	{
		fseek(fp,0,SEEK_END);
		pSize = ftell(fp);
		fseek(fp,0,SEEK_SET);
		fclose(fp);
	}
	return pSize;
}

void KUpdateLayer::copyData(const char* pFileName)
{
	nCurpFileName = pFileName;
	writelen      = 0;
#ifndef WIN32
	if (!ccFileUtils->isFileExist(nCurpFileName))
	{
		CCMessageBox(nCurpFileName.c_str(),"file is Noexist!");
		return;
	}
#endif
	std::string strPath = ccFileUtils->fullPathForFilename(pFileName);
	unsigned long readlen = 0;
	unsigned char * buffer = NULL;
	buffer = ccFileUtils->getFileData(strPath.c_str(),"rb+",&readlen);
	//buffer = CCFileUtils::sharedFileUtils()->getFileDataFromZip("pak.zip",pFileName,&readlen);
	//std::string destPath = CCFileUtils::sharedFileUtils()->getWritablePath();//getWriteablePath();
	//destPath += pFileName;
	//char nMsg[128]={0};
	//t_sprintf(nMsg,"last size:%u bit",len);
	//CCMessageBox(nMsg,"getFileData");
	if (!buffer)
	{
		//char mag[128]={0};
		//t_sprintf(mag,"%s:%d",strPath.c_str(),readlen);
		CCMessageBox(strPath.c_str(),"copy Error");
	}
	//CCFileUtils::sharedFileUtils()->addSearchPath();
	//CCFileUtils::sharedFileUtils()->SearchPath();
	nCurpFileName = pFileName;
	std::string destPath = m_downloadDir;
	destPath +="/";
	destPath +=pFileName;
	FILE *fp = fopen(destPath.c_str(),"wb+"); //·��
	if (fp)
	{
		
		while(writelen < readlen)
		{//ѭ��д��
			writelen += fwrite(buffer + writelen,1,(readlen - writelen),fp);//1�ֽ�1�ֽ�д��dstFile�ļ���buffer + writelen��ʾ����һ�δ�����д
			
#ifdef WIN32
			Sleep(10);
#else
			sleep(1); 
#endif 
			//loadingCallBack_new(pFileName,0,writelen);
		}
		//fwrite(data,sizeof(unsigned char),len,fp);
		fclose(fp);
	}

	++m_nLoadedResources;

	if (buffer)
	{
	   delete [] buffer;
	   buffer = NULL;
	}
}

void KUpdateLayer::update(float delta)
{
	if  (_clientNoNetWork && !isOver) return;

	 char tmp[64]={0};
	 if  (!isHaveClient)
	 {
		 errorLoop ++;
		 if (errorLoop>=ERROR_TIME_OUT)
		 {
			 errorLoop = 0;
#ifdef WIN32
			 CCMessageBox("�ͻ���ȱʧ��\n�뵽�ٷ����ظ��Ƶ�SD���С�","��ʾ");
#else
			 CCMessageBox(UTEXT("�ͻ���ȱʧ��\n�뵽�ٷ����ظ��Ƶ�SD���С�",1).c_str(),UTEXT("��ʾ",1).c_str());
#endif
		 }
		 return;
	 }
	 
	 if  (nDownState==STATE_ON_NET_UP)
	 {//�����ϰ�
		 errorLoop ++;
		 if (errorLoop>=ERROR_TIME_OUT)
		 {
			 errorLoop = 0;
#ifdef WIN32
			 CCMessageBox("�����ϰ�","��ʾ");
#else
			 CCMessageBox(UTEXT("�����ϰ�",1).c_str(),UTEXT("��ʾ",1).c_str());
#endif
		 }
		 return;
	 }
	 if  (nDownState==STATE_ON_URLFIEL_UP)
	 {//�����ϰ���STATE_ON_FIEL_UP
		 errorLoop++;
		 if (errorLoop>=ERROR_TIME_OUT)
		 {
			 errorLoop = 0;
#ifdef WIN32
			 CCMessageBox("�����ϰ��򲹶�������","��ʾ");
#else
			 CCMessageBox(UTEXT("�����ϰ��򲹶�������",1).c_str(),UTEXT("��ʾ",1).c_str());
#endif
		 }
		 return;
	 }

	 if  (nDownState==STATE_ON_FIEL_UP)
	 {//�����ϰ���
		 errorLoop++;
		 if (errorLoop>=ERROR_TIME_OUT)
		 {
			 errorLoop = 0;
#ifdef WIN32
			 CCMessageBox("������ʧ","��ʾ");
#else
			 CCMessageBox(UTEXT("������ʧ",1).c_str(),UTEXT("��ʾ",1).c_str());
#endif
		 }
		 return;
	 }
	 errorLoop = 0;
	 if (nDownState==STATE_LOAD_LISTDATA)
	 {//��ʼ�����������
		 KIniFile pPakList;
		 char nTempStr[128];
		 ZeroMemory(nTempStr,sizeof(nTempStr));
		 
	     if (_clientlanguage!=1)
			 t_sprintf(nTempStr,"data/paklist_vn.ini");
		 else
			 t_sprintf(nTempStr,"data/paklist.ini");

		 if (pPakList.Load(nTempStr))
		 {//�����ȡ�б�ɹ�
			 if (pListData)
			 {
				 free(pListData);
				 pListData=NULL;
			 }

			 int nPakNum=0,reload=0,_isOpen=0;
			 pPakList.GetInteger("list","count",0,&nPakNum);
			 //pPakList.GetInteger("list","reload",0,&reload);//�Ƿ���������
			 pakDownCount = nPakNum;
			 pPakList.GetInteger("list","isopen",1,&_isOpen);
			 isOpenDown   = _isOpen;
			 //pakreload    = reload;
			 pListData = (Kpaklistdata*)malloc(sizeof(Kpaklistdata) * nPakNum);
			 if (pListData &&nPakNum>0)
			 {
				 Kpaklistdata* pInfo = (Kpaklistdata*)pListData;
				 for(int i=0;i<nPakNum;i++)
				 {
					 char nKey[32];
					 t_sprintf(nKey,"pak_%d",i);
					 pPakList.GetString("list",nKey,"",pInfo->pakName,sizeof(pInfo->pakName));
					 t_sprintf(nKey,"pak_%d_r",i);
					 pPakList.GetInteger("list",nKey,0,&pInfo->_pakreload);

					 if  (pInfo->_pakreload)
					     pInfo->_isbreak = false;
					 else
						 pInfo->_isbreak = true;

					 pInfo++;
				 }
				 //��ʼ����
				 nDownState=STATE_DWON_LISTDATA;
			 }
			 pPakList.Clear();
		 }
	 }

	 if  (nDownState == STATE_FLS_UP)
	 {//���������ɾͼ���pak
		 nDownState =STATE_NONE_UP;
		 std::string fielDir;
if (_clientlanguage!=1)
{
	     fielDir = "package_vn.ini";
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	     fielDir = "data\\package_vn.ini";
#endif
}
else
{
		 fielDir = "package.ini";
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
		 fielDir = "data\\package.ini";
#endif
}
         char nTempstr[128];
		 t_sprintf(nTempstr,fielDir.c_str());
		 nPakCount = g_pcPakList.Open(nTempstr);  //����PAK
		 if (nPakCount)
		 {
			 ++m_nLoadedResources;
			 if (m_nLoadedResources<m_nAllLoadResources)
				 m_nLoadedResources = m_nAllLoadResources+1;

			 //char msg[64];
			 //t_sprintf(msg,"%d",nPakCount);
		     //CCMessageBox(msg,"pak");
		 }
	 }

	 if (nCurpFileName.length()>0)
	 {
		t_sprintf(tmp,"%%%d,%s:%u",(int)(((float)m_nLoadedResources/m_nAllLoadResources)*100),nCurpFileName.c_str(),writelen);
	 }
	 else
		t_sprintf(tmp,"%%%d",(int)(((float)m_nLoadedResources/m_nAllLoadResources)*100));

	 if  (m_nLoadedFinish)
	 {
		 if (nCurpFileName.length()>0)
		 {
			 t_sprintf(tmp,"%%%d,%s:%u",100,nCurpFileName.c_str(),writelen);
		 }
		 else
			 t_sprintf(tmp,"%%%d",100);
	 }
	  m_pLabelPercent->setString(tmp);

	  if (downPtr)
	  {
		  char mstrInfo[64];
		  sprintf(mstrInfo,"loading:%02f",downPtr->getCurpercent());
		  m_pLabelLoading->setString(mstrInfo);
	  }
	 
	  m_fProgressIndex = (((float)m_nLoadedResources/m_nAllLoadResources)*100);
	 if  (m_nLoadedFinish)
	 {   
		 m_fProgressIndex = (float)100;
	     m_pLoadBarEnd->setPercentage(m_fProgressIndex);
	 }
	  else
		 m_pLoadBarEnd->setPercentage(m_fProgressIndex);
	    //m_pLoadBarEnd->setPercentage(m_fProgressIndex);

	if (m_nLoadedResources >= m_nAllLoadResources && nDownState == STATE_NONE_UP)
	{//�������
		m_nLoadedResources = 0;
		m_nLoadedFinish    = true;
        //��ʼ ��ת
		loadSuccess();
		return;
	}
}
void KUpdateLayer::loadSuccess()
{
	Terminate(1);
	CCDirector::sharedDirector()->replaceScene(Klogin_f::scene());
	//CCDirector::sharedDirector()->pushScene(loginSuccessScene);
}

void KUpdateLayer::loadingCallBack(CCObject *obj)
{//���ý������Ľ���
	++m_nLoadedResources;
	char tmp[10];
	t_sprintf(tmp,"%%%d",(int)(((float)m_nLoadedResources/m_nAllLoadResources)*100));
	m_pLabelPercent->setString(tmp);
	m_fProgressIndex = (((float)m_nLoadedResources/m_nAllLoadResources)*100);
	m_pLoadBarEnd->setPercentage(m_fProgressIndex);
}

void KUpdateLayer::loadingCallBack_new(const char * nFileNane,int nTalSize,int nCurSize)
{//���ý������Ľ���
	++m_nLoadedResources;
	char tmp[64]={0};
	if (nFileNane)
	{
      t_sprintf(tmp,"%%%d,%s:%u",(int)(((float)m_nLoadedResources/m_nAllLoadResources)*100),nFileNane,nCurSize);
	}
	else
	  t_sprintf(tmp,"%%%d",(int)(((float)m_nLoadedResources/m_nAllLoadResources)*100));

	m_pLabelPercent->setString(tmp);
	m_fProgressIndex = (((float)m_nLoadedResources/m_nAllLoadResources)*100);
	m_pLoadBarEnd->setPercentage(m_fProgressIndex);
}

void KUpdateLayer::Run()
{
	//if (nPakCount)
	{//��ʼ����Զ����Դ
		if (downPtr)
		{
		   int ndwonstate=0;
		   bool ret = false;
		   //std::string nFullPath = m_WritablePath;
		   //nFullPath +="";
		   int  isReLoad=1;
		   bool isBreak=true;
		   while (true) //�������б�
		   { // ѭ������ ÿ30��������� ����������
		      if (_clientlanguage!=1)
				  ret = downPtr->DownLoadFile(m_WritablePath,"data/paklist_vn.ini",isReLoad); //ֱ������ ���ж����߳�
		      else
			      ret = downPtr->DownLoadFile(m_WritablePath,"data/paklist.ini",isReLoad); //ֱ������ ���ж����߳�
			   //ֻ���һ�Ρ�ʣ�µ�Ϊ�ϵ�����
			   isReLoad = 0;
			   if (downPtr->getDownState()==1)
			   {
				   nDownState =STATE_ON_NET_UP;
				   //++m_nLoadedResources;
			   }
			   if (downPtr->getDownState()==2)
			   {
				  nDownState =STATE_ON_FIEL_UP;
			      //++m_nLoadedResources;
			   }
			   if (downPtr->getDownState()==3)
			   {
				   nDownState =STATE_ON_URLFIEL_UP;
				   //++m_nLoadedResources;
			   }
			   
			   if (ret)
			   { //�������
				   nDownState =STATE_LOAD_LISTDATA;
				   ++m_nLoadedResources;
				   break;
			   }
#ifndef WIN32
			   sleep(0.5); //ÿ�������м���0.5��
#else
			   Sleep(500);
#endif
		   } 

		   while (true) //�������б�
		   { // ѭ���������ȴ���ȡ�����������
			   if  (nDownState == STATE_DWON_LISTDATA)
				   break;
#ifndef WIN32
			   sleep(0.5); //ÿ�������м���0.5��
#else
			   Sleep(500);
#endif
		   }

		   /*if  (!isOpenDown && nDownState==STATE_DWON_LISTDATA)
		   {//���û�п������ص�
			   ++m_nLoadedResources;
			   nDownState = STATE_FLS_UP;
			   return;
		   }*/

		   int  nCurSelCount=0; 
		   char strVer[128];
		   Kpaklistdata* pInfo = (Kpaklistdata*)pListData;
		   t_sprintf(strVer,pInfo->pakName);
		   isReLoad = pInfo->_pakreload;
		   isBreak  = pInfo->_isbreak;
		   while (true)
		   { // ѭ������ ÿ30��������� ���������� foldername + "/" 
		      nDownState = STATE_RUN_UP;

			  if  (!isOpenDown && isBreak && !isReLoad)
			  {//���û�п������ص� �����ǲ���Ҫ���ص� ��ֱ�ӷ��� ������һ��
				  nCurSelCount++;
				  if  (nCurSelCount>=pakDownCount)
				  {
					  nDownState = STATE_FLS_UP;
					  break;
				  }
				  else
					  pInfo++;
				  //��һ��
				  t_sprintf(strVer,pInfo->pakName);
				  isReLoad = pInfo->_pakreload;
				  isBreak  = pInfo->_isbreak;
				  ++m_nLoadedResources;
				  continue;
			  }

			  ret = downPtr->DownLoadFile(m_WritablePath,strVer,(bool)isReLoad); //ֱ������ ���ж����߳�
			  if (isReLoad)
			      isReLoad = 0;
			  if (downPtr->getDownState()==1)
			  {
				  nDownState =STATE_ON_NET_UP;;
				  //++m_nLoadedResources;
			  }
			  if (downPtr->getDownState()==2)
			  {
				  nDownState =STATE_ON_FIEL_UP;
				  //++m_nLoadedResources;
			  }
			  if (downPtr->getDownState()==3)
			  {
				  nDownState =STATE_ON_URLFIEL_UP;
				  //++m_nLoadedResources;
			  }
			  
			  if (ret && downPtr->getDownState()== 5)
			  { //�������
					 nCurSelCount++;
					 if  (nCurSelCount>=pakDownCount)
					 {
						 nDownState = STATE_FLS_UP;
						 break;
					 }
					 else
                        pInfo++;

					 t_sprintf(strVer,pInfo->pakName);
					 isReLoad = pInfo->_pakreload;
				     ++m_nLoadedResources;
				     //continue;
			   }
#ifndef WIN32
			   sleep(0.5); //ÿ�������м���0.5��
#else
			   Sleep(500);
#endif
		   } 
		}
		++m_nLoadedResources;
	}//end pakcount	
}

void KUpdateLayer::startload()
{//����һ���̸߳��ƽ�ѹ����
//	_loadingThread = new std::thread(&KUpdateLayer::loadImage, this);
	//���������߳�
	/*if (!isFileExist("map.pak"))
	{//�����ھͿ�ʼ����
		isHaveClient = false;
#ifdef WIN32
		CCMessageBox("�ͻ���û�а�װ\n�뵽�ٷ����ؿ�����SD���С�","��ʾ");
#else
		CCMessageBox(UTEXT("�ͻ���û�а�װ��\n�뵽�ٷ����ؿ�����SD���С�",1).c_str(),UTEXT("��ʾ",1).c_str());
#endif
		return;
	}*/

	isHaveClient = true;
	Start();
	return;
	
	/*CCTextureCache::sharedTextureCache()->addImageAsync("transence.png", this, callfuncO_selector(KTranSence::loadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync("transence.png", this, callfuncO_selector(KTranSence::loadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync("transence.png", this, callfuncO_selector(KTranSence::loadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync("transence.png", this, callfuncO_selector(KTranSence::loadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync("transence.png", this, callfuncO_selector(KTranSence::loadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync("transence.png", this, callfuncO_selector(KTranSence::loadingCallBack));*/
}




