#include "AGInventoryLayer.h"
#include "pugixml.hpp"

AGInventoryLayer::AGInventoryLayer(void)
{
}

AGInventoryLayer::~AGInventoryLayer(void)
{
}

bool AGInventoryLayer::init()
{
	bool bRet = false;
	do 
	{
		//////////////////////////////////////////////////////////////////////////
		// super init first
		//////////////////////////////////////////////////////////////////////////

		CC_BREAK_IF(! CCLayer::init());

		// Get window size
		CCSize windowSize = CCDirector::sharedDirector()->getWinSize();

		// 3. Add add a splash screen, show the cocos2d splash image.
		CCSprite* pSprite = CCSprite::create("HelloWorld.png");
		CC_BREAK_IF(! pSprite);

		// Place the sprite on the center of the screen
		pSprite->setPosition(ccp(windowSize.width/2, windowSize.height/2));

		// Add the sprite to HelloWorld layer as a child layer.
		this->addChild(pSprite, 0);


		// 3. Add add a splash screen, show the cocos2d splash image.
		pSprite = CCSprite::create("Inventory/inventory.png");
		CC_BREAK_IF(! pSprite);

		//pSprite->setContentSize(windowSize);
		pSprite->setScaleX(windowSize.width / pSprite->getContentSize().width);
		pSprite->setScaleY(windowSize.height / pSprite->getContentSize().height);

		// Place the sprite on the center of the screen
		pSprite->setPosition(ccp(windowSize.width/2, windowSize.height/2));
		pSprite->setOpacity(100);

		// Add the sprite to HelloWorld layer as a child layer.
		this->addChild(pSprite, 0);

		LoadXML("xml\\Itemlist.xml");
		LoadItemImage();

		bRet = true;
	} while (0);

	return bRet;
}

bool AGInventoryLayer::LoadXML(char* szXMLPath)
{
	pugi::xml_document doc;

	//pugi::xml_parse_result result = doc.load_file("tree.xml");

	//std::cout << "Load result: " << result.description() << ", mesh name: " << doc.child("mesh").attribute("name").value() << std::endl;


	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(szXMLPath);
	unsigned char* pBuffer = NULL;
	unsigned long bufferSize = 0;
	pBuffer = CCFileUtils::sharedFileUtils()->getFileData(fullPath.c_str(), "r", &bufferSize);
	if(!doc.load_buffer(pBuffer,bufferSize)) return false;

	pugi::xml_node ItemListNode = doc.child("ItemList");
	ItemListNode.begin();

	//for (ItemListNode.begin(); ItemListNode.begin() != ItemListNode.end(); ItemListNode.next_sibling())
	{   
		/*std::cout << tool.child_value("Index");
		std::cout << tool.child_value("Name");
		std::cout << tool.child_value("Image");
		std::cout << tool.child_value("GetMessage") << "'\n";
		std::cout << tool.child_value("Look") << "'\n";*/
	}

	// Create a label and initialize with string "Hello World".
	CCLabelTTF* pLabel = CCLabelTTF::create("문자열 테스트", "Arial", 16);
	

	// Get window size and place the label upper. 
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	pLabel->setPosition(ccp(pLabel->getContentSize().width/2, pLabel->getContentSize().height/2));

	// Add the label to HelloWorld layer as a child layer.
	this->addChild(pLabel, 1);

	for (pugi::xml_node_iterator it = ItemListNode.begin(); it != ItemListNode.end(); ++it)
	{
		//for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
		//{        std::cout << " " << ait->name() << "=" << ait->value();    }
		
		//std::wcout << it->attribute("Index").as_string() << std::endl;
		//std::wcout << it->attribute("Name").as_string() << std::endl;
		//std::wcout << it->attribute("Image").as_string() << std::endl;
		//std::cout << it->attribute("GetMessage").as_string() << std::endl;
		//std::string str = it->attribute("GetMessage").as_string();
		//printf("%s\n", it->attribute("GetMessage").as_string());
		//std::wcout << it->attribute("Look").as_string() << std::endl;
		//pLabel->setString(it->attribute("GetMessage").value()) ;
		
		ITEMINFO_ Info;
		Info.Index = it->attribute("Index").as_int();
		Info.szName = it->attribute("Name").as_string();
		Info.szImage = it->attribute("Image").as_string();
		Info.szGetMessage = it->attribute("GetMessage").as_string();
		Info.szLook = it->attribute("Look").as_string();

		m_ItemInfoMap.insert(make_pair(Info.Index, Info));
	}

	for(ItemInfoMap::iterator iter = m_ItemInfoMap.begin(); iter != m_ItemInfoMap.end(); iter++)
	{
		std::wcout << iter->second.Index << std::endl;
		std::wcout << iter->second.szName.c_str() << std::endl;
		//std::wcout << it->attribute("Image").as_string() << std::endl;
		//std::cout << it->attribute("GetMessage").as_string() << std::endl;
		//std::string str = it->attribute("GetMessage").as_string();
		//printf("%s\n", it->attribute("GetMessage").as_string());
		//std::wcout << it->attribute("Look").as_string() << std::endl;
		//pLabel->setString(it->attribute("GetMessage").value()) ;
	}

	return true;
}

bool AGInventoryLayer::LoadItemImage()
{
	CCSize windowSize = CCDirector::sharedDirector()->getWinSize();

	int Index = 0;
	for(ItemInfoMap::iterator iter = m_ItemInfoMap.begin(); iter != m_ItemInfoMap.end(); iter++)
	{
		// 3. Add add a splash screen, show the cocos2d splash image.
		CCSprite* pSprite = CCSprite::create(iter->second.szImage.c_str());
		CC_BREAK_IF(! pSprite);
		m_ItemImageMap.insert(make_pair(iter->second.Index, pSprite));
		pSprite->setPosition(ccp((windowSize.width / 640.0f) * (330 + (120 * (Index%3))), (windowSize.height / 480.0f) * (362 - 86 * (Index/3))));
		//pSprite->setOpacity(100);

		pSprite->setScaleX(windowSize.width / 640.0f);
		pSprite->setScaleY(windowSize.height / 480.0f);

		this->addChild(pSprite, 2);
		// Place the sprite on the center of the screen
		
		Index++;
	}

	CCSprite* pSprite = CCSprite::create("item/item_edge.png");
	pSprite->setScaleX(windowSize.width / 640.0f);
	pSprite->setScaleY(windowSize.height / 480.0f);

	pSprite->setPosition(ccp((windowSize.width / 640.0f) * 330, (windowSize.height / 480.0f) * 362));
	
	this->addChild(pSprite, 2);

	return true;
}














