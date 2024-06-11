//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __PopupDemo__PopupLayer__
#define __PopupDemo__PopupLayer__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;

class PopupLayer:public CCLayer
{
  
public:
    PopupLayer();
    ~PopupLayer();
    
    virtual bool init();
    CREATE_FUNC(PopupLayer);
    
     // ��Ҫ��д����ע�ắ�������¸�����������
    virtual void registerWithTouchDispatcher(void);
     // ��д������������Զ���� true �����������㣬�ﵽ ��ģ̬�� Ч��
    bool ccTouchBegan(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent);
    // ���ܣ������öԻ��򱳾�ͼƬ
    static PopupLayer* create(const char* backgroundImage,int nKind=0);
	//static PopupLayer* createWith();
    
     // ��������ʾ���⣬�����趨��ʾ���ִ�С
    void setTitle(const char*title,int fontsize = 20);
    // �ı����ݣ�padding Ϊ���ֵ��Ի�������Ԥ���ľ��룬���ǿɿصģ����Ϸ��ľ����������
    void setContentText(const char *text, int fontsize = 20, int padding = 50, int paddintTop = 100);
    // �ص��������������ť�����ǹرյ������ͬ�£���Ҫһ���ص���������֪ͨ���ǵ�����ĸ���ť������ж����
    void setCallbackFunc(CCObject* target, SEL_CallFuncN callfun);
     // Ϊ����Ӱ�ť���棬��װ��һ������������Щ��Ҫ�Ĳ���
    bool addButton(const char* normalImage, const char* selectedImage, const char* title, int tag = 0,int isSpr=0);
    
    // Ϊ������ʾ��ʱ֮ǰ��������Ч��ѡ���� onEnter �ﶯ̬չʾ
    virtual void onEnter();
    virtual void onExit();
	virtual void update(float delta); 
	virtual void draw();
    
private:
    
    void buttonCallback(CCObject* pSender);
    
    // �����������ߵĿհ���
    int m_contentPadding;
    int m_contentPaddingTop;
    
    CCObject* m_callbackListener;
    SEL_CallFuncN m_callback;
    
    CC_SYNTHESIZE_RETAIN(CCMenu*, m__pMenu, MenuButton);
    CC_SYNTHESIZE_RETAIN(CCSprite*, m__sfBackGround, SpriteBackGround);
    CC_SYNTHESIZE_RETAIN(CCScale9Sprite*, m__s9BackGround, Sprite9BackGround);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m__ltTitle, LabelTitle);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m__ltContentText, LabelContentText);

};
#endif /* defined(__PopupDemo__PopupLayer__) */