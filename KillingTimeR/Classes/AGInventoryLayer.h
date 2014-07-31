#pragma once

#include "cocos2d.h"
#include "AGStructure.h"

using namespace cocos2d;
using namespace std;

class AGInventoryLayer : public CCLayer
{
	typedef std::map<int, ITEMINFO_> ItemInfoMap;
	typedef std::map<int, CCSprite*> ItemImageMap;
public:
	AGInventoryLayer(void);
	virtual ~AGInventoryLayer(void);

	virtual bool init(); 
	bool RegisterItem();
	bool UnRegisterItem();
	bool LoadXML(char* szXMLPath);
	bool LoadItemImage();

	// implement the "static node()" method manually
	CREATE_FUNC(AGInventoryLayer);

protected:

private:
	ItemInfoMap m_ItemInfoMap;
	ItemImageMap m_ItemImageMap;
};