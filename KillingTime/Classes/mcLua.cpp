//
//  mcLua.mm
//
//  Created by Robert Grzesek on 11/10/09.
//  Copyright 2009 GRZ Software LLC. All rights reserved.
//

#include "mcLua.h"

// Below are the functions used for the yield funtions.

static int LuaYieldSeconds(lua_State * l)
{
	mcLuaScript * s;
	lua_pushlightuserdata(l,l);
	lua_gettable(l, LUA_GLOBALSINDEX);
	s = (mcLuaScript *)lua_touserdata(l,-1);
	
	float n = lua_tonumber(l,1);
	s->YieldSeconds(n);
	printf("waitSeconds %f\n",n);
	
	return (lua_yield(l, 0));
}

static int LuaYieldFrames(lua_State * l)
{
	mcLuaScript * s;
	lua_pushlightuserdata(l,l);
	lua_gettable(l, LUA_GLOBALSINDEX);
	s = (mcLuaScript *)lua_touserdata(l,-1);
	
	int f = lua_tonumber(l,1);
	s->YieldFrames(f);
	printf("waitFrames %d\n",f);
	
	return (lua_yield(l, 0));
}

static int LuaYieldPause(lua_State * l)
{
	mcLuaScript * s;
	lua_pushlightuserdata(l,l);
	lua_gettable(l, LUA_GLOBALSINDEX);
	s = (mcLuaScript *)lua_touserdata(l,-1);
	
	s->YieldPause();
	printf("pause \n");
	
	return (lua_yield(l, 0));
}

//---------------------------------------------------------------

mcLuaManager::mcLuaManager (void):
head_(NULL)
{
	mainState_ = lua_open();
	
	//Add functions for waiting
	static const luaL_reg scriptLib[] = 
	{
		{"waitSeconds", LuaYieldSeconds },
		{"waitFrames", LuaYieldFrames },
		{"pause", LuaYieldPause },
		{NULL, NULL}
	};
	LuaOpenLibrary( "script", scriptLib);
	
	//add the base library
	luaopen_base(mainState_);
	luaopen_math(mainState_);
	luaopen_string(mainState_);
	lua_settop(mainState_, 0);
	
}

mcLuaManager::~mcLuaManager(void)
{
	mcLuaScript * n = head_;
	while (n) {
		mcLuaScript * i = n->next_;
		delete (n);
		n=i;
	}
	lua_close(mainState_);
}

bool mcLuaManager::LuaOpenLibrary(const char * name, const luaL_reg * libs)
{
	if(head_ != NULL)
		return 0;
	
	luaL_openlib(mainState_, name, libs, 0);
	return 1;	
}

void mcLuaManager::SetGlobalNumber(const char * name, double val)
{
	lua_pushnumber(mainState_, val);
	lua_setglobal(mainState_,name);
}

void mcLuaManager::SetGlobalInteger(const char * name, int val)
{
	lua_pushinteger(mainState_, val);
	lua_setglobal(mainState_,name);
}

void mcLuaManager::SetGlobalString(const char * name, const char * val)
{
	lua_pushstring(mainState_, val);
	lua_setglobal(mainState_,name);
}
	
mcLuaScript * mcLuaManager::CreateScript(void)
{
	lua_State * s = lua_newthread(mainState_);
	
	//Add reference to the new thread in the Lua Registry so
	//will not be garbage collected
	int r = luaL_ref(mainState_, LUA_REGISTRYINDEX);
	
	//Create a new mcLuaScript object to hold the new thread
	mcLuaScript * ns = new mcLuaScript(s,this,r);
	
	//Add an entry to map the new lua_State to the 
	//new mcLuaScript object 
	lua_pushlightuserdata(mainState_, s);
	lua_pushlightuserdata(mainState_, ns);
	lua_settable(mainState_, LUA_GLOBALSINDEX);
	
	//insert the new script into the list
	ns->next_ = head_;
	head_ = ns;
	return ns;
}

void mcLuaManager::DestroyScript(mcLuaScript * s)
{
	if(s == head_){
		head_ = s->next_;
		delete (s);
		return;
	}
	
	mcLuaScript * last = head_;
	for (mcLuaScript * l=last->next_; l != NULL; l=l->next_) {
		if (l == s) {
			last->next_ = l->next_;
			delete(l);
			return;
		}
		last = l;
	}
}

void mcLuaManager::Update(float elapsedSeconds)
{
	mcLuaScript * n = head_;
	while(n)
		n = n->Update(elapsedSeconds);
}

//---------------------------------------------------------------


mcLuaScript::mcLuaScript(lua_State * l, mcLuaManager * man, int registryRef):
next_(NULL)
,registryRef_(registryRef)
,state_(l)
,manager_(man)
,yieldMode_(YM_NONE)
,waitFrames_(0)
,waitTime_(0)
{
	
}

mcLuaScript::~mcLuaScript(void)
{
	luaL_unref(state_, LUA_REGISTRYINDEX, registryRef_);
}
	
void mcLuaScript::LoadFile(const char * name)
{
	printf("mcLuaScript::LoadFile() - %s\n",name);
	int err = luaL_loadfile(state_, name);
	if(err){
		printf("luaL_loadfile Error- %s\n",lua_tostring(state_,-1));
		lua_pop(state_,1);
	}
	
	lua_resume(state_, 0);
}

void mcLuaScript::LoadString(const char * buffer)
{
	int err = luaL_loadstring(state_, buffer);
	if(err){
		printf("%s",lua_tostring(state_,-1));
		lua_pop(state_,1);
	}

	lua_resume(state_, 0);
}

mcLuaScript * mcLuaScript::Update(float elapsedSeconds)
{
	if (yieldMode_ == YM_TIME) {
		waitTime_ -= elapsedSeconds;
		if(waitTime_ > 0)
			return next_;
	} 
	
	if( yieldMode_ == YM_FRAME){
		--waitFrames_;
		if (waitFrames_ > 0) {
			return next_;
		}
	}
	
	if( yieldMode_ == YM_PAUSE){
		return next_;
	}
	
	yieldMode_ = YM_NONE;
	lua_resume(state_, 0);
	return next_;
}
 
void mcLuaScript::YieldFrames(int num)
{
	yieldMode_ = YM_FRAME;
	waitFrames_ = num;
}

void mcLuaScript::YieldSeconds(float secs)
{
	printf("YieldSeconds \n");
	yieldMode_ = YM_TIME;
	waitTime_ = secs;
}

void mcLuaScript::YieldPause()
{
	yieldMode_ = YM_PAUSE;
}

void mcLuaScript::YieldResume()
{
	printf("resume \n");
	yieldMode_ = YM_NONE;
}