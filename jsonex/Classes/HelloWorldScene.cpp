#include "HelloWorldScene.h"
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/reader.h"
//#include "rapidjson/writer.h"
//#include "rapidjson/prettywriter.h" // for stringify JSON
//#include "rapidjson/filestream.h"   // wrapper of C stream for prettywriter as output

using namespace cocos2d;
using namespace rapidjson;


CCScene* HelloWorld::scene()
{
    CCScene *scene = CCScene::create();
    
    HelloWorld *layer = HelloWorld::create();

    scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{
    if ( !CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)) )
    {
        return false;
    }

    /////////////////////////////

    // Default template parameter uses UTF8 and MemoryPoolAllocator.
    Document document;

    
//    const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";
    
    const char* fileName = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile("test_data.json", "data/test_data.json");
    
    
    unsigned long bufferSize = 0;
    unsigned char* json = CCFileUtils::sharedFileUtils()->getFileData(fileName, "rb", &bufferSize );
    
    // garbage 데이타 제거
    std::string clearData((const char*)json);
    size_t pos = clearData.rfind("}");   // } or ]
    clearData = clearData.substr(0, pos+1);
    
    printf("Original JSON:\n %s\n", json);
    printf("Original JSON:\n %s\n", clearData.c_str());
    
    
    
    if (document.Parse<0>(clearData.c_str()).HasParseError()) {
        CCLog("Err..%s", document.Parse<0>(clearData.c_str()).GetParseError());
        return 1;
    }
    
    printf("\nParsing to document succeeded.\n");
    
    
    
    // 2. Access values in document.
    
    printf("\nAccess values in document:\n");
    assert(document.IsObject());    // Document is a JSON value represents the root of DOM. Root can be either an object or array.
    
    assert(document.HasMember("hello"));
    assert(document["hello"].IsString());
    
    printf("hello = %s\n", document["hello"].GetString());
    
    assert(document["t"].IsBool());        // JSON true/false are bool. Can also uses more specific function IsTrue().
    printf("t = %s\n", document["t"].GetBool() ? "true" : "false");
    
    assert(document["f"].IsBool());
    printf("f = %s\n", document["f"].GetBool() ? "true" : "false");
    
    printf("n = %s\n", document["n"].IsNull() ? "null" : "?");
    
    assert(document["i"].IsNumber());           // Number is a JSON type, but C++ needs more specific type.
    assert(document["i"].IsInt());              // In this case, IsUint()/IsInt64()/IsUInt64() also return true.
    printf("i = %d\n", document["i"].GetInt()); // Alternative (int)document["i"]
    
    assert(document["pi"].IsNumber());
    assert(document["pi"].IsDouble());
    printf("pi = %g\n", document["pi"].GetDouble());
    
    {
        const Value& a = document["a"];     // Using a reference for consecutive access is handy and faster.
        assert(a.IsArray());
        for (SizeType i = 0; i < a.Size(); i++)    // rapidjson uses SizeType instead of size_t.
            printf("a[%d] = %d\n", i, a[i].GetInt());
        
        // Note:
        //int x = a[0].GetInt();            // Error: operator[ is ambiguous, as 0 also mean a null pointer of const char* type.
        int y = a[SizeType(0)].GetInt();    // Cast to SizeType will work.
        int z = a[0u].GetInt();             // This works too.
        (void)y;
        (void)z;
    }
    

    
    return true;
}






