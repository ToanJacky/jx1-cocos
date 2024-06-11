//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KList.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef	KUPDATELAYER_H
#define	KUPDATELAYER_H
#pragma once
//---------------------------------------------------------------------------
#include "cocos2d.h"
#include "engine/KPakList.h"
#include "engine/KDownLoadFile.h"
USING_NS_CC;
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
USING_NS_CC_EXT;
#include "AssetsManager/AssetsManager.h"
//#include "network/ODSocket.h"
#include "network/ClientStage.h"
#include "network/Thread.h"
#include "LoginDef.h"
using namespace CocosDenshion;
// �ȸ���ʵ��ʾ��
struct Kpaklistdata
{
	char    pakName[128];
	int     _pakreload;
	bool    _isbreak;
};
class KUpdateLayer:public CCLayer, public AssetsManagerDelegateProtocol,protected CThread
{
public:
	KUpdateLayer();
	~KUpdateLayer();
	static CCScene* scene(){
		CCScene* scene = CCScene::create();
		scene->addChild(KUpdateLayer::create());
		return scene;
	};

	static KUpdateLayer* create(){
		KUpdateLayer* pLayer = new KUpdateLayer;
		if (pLayer && pLayer->init())
		{
			pLayer->autorelease();
			return pLayer;
		}
		delete pLayer;
		return NULL;
	};
	
	// ��ʼ��
	bool init();
	using CThread::Start;
	//CThread UpThread;
	//virtual void onEnter();
	// �˵��ص�����
	void reset(CCObject* pSender);					// ���ð汾
	void getClientVersion(CCObject* pSender);	    // ��ȡ��ǰ�ͻ��˰汾��
	void checkUpdate(CCObject* pSender);		    // ����Ƿ��а汾����
	void updates(CCObject* pSender);				// ���°汾
	void enterScene(CCObject* pSender);				// ���볡�������δ������Ļ�м����ʾ̾�ŵ�ͼƬ�����º����ʾ��һ��ͼƬ
	//ODSocket * sock_client;
	virtual void update(float delta);
	virtual void Run();
protected:
	// ��ʼ������Ŀ¼
	void initDownloadDir();
	void creatDownloadDir(std::string nTempPath);
	// ɾ��Ŀ¼
	void deleteDir(std::string dir);
private:
	downloadFile *downPtr;
	//��ȡ��ʼʱ��Ľ�����
	CCSprite *m_pLoadBarStart;
	//��ȡ���ʱ��Ľ�����
	CCProgressTimer *m_pLoadBarEnd;
	//�߳���صĺ���
	//�ܵļ���ͼƬ��
	int m_nAllLoadResources;
	//��ǰ����ͼƬ��
	int m_nLoadedResources;
	int m_nLoadedFinish;
	//��ȡ����
	float m_fProgressIndex;
	CCLabelTTF *m_pLabelLoading;
	CCLabelTTF *m_pLabelPercent;
	void    startload();//������ɺ� ִ��ɾ������

	CCLabelTTF* m_label;
	std::string m_downloadDir;
	std::string m_WritablePath;
	void copyData(const char* pFileName);
	int isFileExist(const char* pFileName);
	void loadSuccess();
	//��ȡ�Ļص�����
	void loadingCallBack(CCObject *obj);
	void loadingCallBack_new(const char * nFileNane,int nTalSize,int nCurSize);
	//CGameClient *nAcclient;
	//int m_bIsClientConnecting;
	//int m_bIsGameSevConnecting;
	enum UpdataState
	{//������״̬
		STATE_NONE_UP,//����
		STATE_RUN_UP,//��������
		STATE_FLS_UP,//���
		STATE_ON_FIEL_UP,
		STATE_ON_URLFIEL_UP,
		STATE_ON_NET_UP,
		STATE_LOAD_LISTDATA,
		STATE_DWON_LISTDATA,
	};
	size_t writelen;
	std::string nCurpFileName;
	int nPakCount;
    char downUrlmask[64];
	int nDownState;
	Kpaklistdata* pListData;
	int  pakDownCount;
	int  isOpenDown;
	//int  pakreload;
	unsigned int errorLoop;
	bool isHaveClient;
	bool isOver;
	bool m_IsInPutIpPadIsOpen;
	void startInPutCallback(CCNode *pNode);
	//std::vector<std::strging> searchPaths;

};

#endif
