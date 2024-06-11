#include "KCheckbox.h"
#include "cocos2d.h"
#include "gamecore/KCore.h"
using namespace cocos2d;

//����,������δѡ��ͼƬ����ѡ��ͼƬ�������֣�״̬��0-δѡ�У�1-ѡ�У�
CCSprite * KCheckbox::create(const char * picNoSelect, const char * picSelect, char * content, int status,int nKind)
{
	CCSprite * sp = CCSprite::create();

	//checkbox
	CCSprite * sp_checkbox_select = CCSprite::create(picSelect);
	CCSprite * sp_checkbox_noSelect = CCSprite::create(picNoSelect);
	CCSize size_checkbox = sp_checkbox_noSelect->getContentSize();

	//����
	CCLabelTTF * label_content = CCLabelTTF::create(UTEXT(content,1).c_str(),UI_GAME_FONT_DEFAULT,20);
	label_content->setColor(ccYELLOW);
	label_content->setHorizontalAlignment(kCCTextAlignmentLeft); //�����
	CCSize size_content = label_content->getContentSize();

	//���ű���
	float scale_h = size_checkbox.height / size_content.height;

	//��������
	label_content->setScale(scale_h);

	//���մ�С
	CCSize size = CCSize(size_checkbox.width * 2 + size_content.width * scale_h , size_checkbox.height * 1.5);
	sp->setContentSize(size);

	//����checkbox״̬
	sp->addChild(sp_checkbox_noSelect);
	sp->addChild(sp_checkbox_select);
	sp_checkbox_noSelect->setPosition(CCPointMake(size_checkbox.width,size.height/2));
	sp_checkbox_select->setPosition(CCPointMake(size_checkbox.width,size.height/2));
	sp_checkbox_noSelect->setTag(0); //��ѡ��tag��0
	sp_checkbox_select->setTag(1);   //ѡ��tag��1

	if (status == 0)
	{
		//����δѡ��״̬
		sp_checkbox_noSelect->setVisible(true);
		sp_checkbox_select->setVisible(false);
	}
	else
	{
		//����ѡ��״̬
		sp_checkbox_noSelect->setVisible(false);
		sp_checkbox_select->setVisible(true);
	}

	//��������λ��
	sp->addChild(label_content);
	label_content->setAnchorPoint(CCPoint(0,0.5));
	label_content->setPosition(CCPointMake(size_checkbox.width * 2, size.height/2));

	return sp;
}

//�������
int KCheckbox::click()
{
	int status = getStatus();
	CCSprite * sp_checkbox_noSelect = (CCSprite *)(this->getChildByTag(0));
	CCSprite * sp_checkbox_select   = (CCSprite *)(this->getChildByTag(1));

	if (status == 0)
	{
		status = 1;//״̬����Ϊѡ��
		sp_checkbox_noSelect->setVisible(false);
		sp_checkbox_select->setVisible(true);
	}
	else
	{
		status = 0;//״̬����Ϊδѡ��
		sp_checkbox_noSelect->setVisible(true);
		sp_checkbox_select->setVisible(false);
	}

	return status;
}

//��ȡ��ǰ״̬,0-δѡ�У�1-ѡ��
int KCheckbox::getStatus()
{
	CCSprite * sp_checkbox_noSelect = (CCSprite *)(this->getChildByTag(0));
	int status = 0; //δѡ��
	if (!sp_checkbox_noSelect->isVisible())
	{
		status = 1;//ѡ��
	}

	return status;
}
