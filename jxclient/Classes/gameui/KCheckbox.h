#ifndef __MyGame__CCCheckbox__
#define __MyGame__CCCheckbox__

#include "cocos2d.h"
#include "engine/KGbktoUtf8.h"
using namespace cocos2d;

class KCheckbox:public CCSprite
{
public:
	//����checkbox
	static CCSprite * create(const char * picNoSelect,const char * picSelect,char * content  ,int status = 0,int nKind=0);

	//�������
	int click();

private:
	//��ȡ��ǰ״̬
	int getStatus();
};

#endif /* defined(__MyGame__CCCheckbox__) */
