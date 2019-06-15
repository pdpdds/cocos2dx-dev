//
//  mcLua.h
//
//  Created by Robert Grzesek on 11/10/09.
//  Copyright 2009 GRZ Software LLC. All rights reserved.
//
#ifndef MCLUA_H
#define MCLUA_H

extern "C"{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

class mcLuaScript;

class mcLuaManager{
public:
	mcLuaManager (void);
	~mcLuaManager(void);
	
	bool LuaOpenLibrary(const char * name, const luaL_reg * libs);

	void SetGlobalNumber(const char * name, double val);
	void SetGlobalInteger(const char * name, int val);
	void SetGlobalString(const char * name, const char * val);
	
	mcLuaScript * CreateScript(void);
	void DestroyScript(mcLuaScript *);
	
	//call this function at every frame with the number of seconds since the last call
	void Update(float elapsedSeconds);

private:
	lua_State * mainState_;
	mcLuaScript * head_;
};

class mcLuaScript{
public:
	void LoadFile(const char * name);
	void LoadString(const char * buffer);
	
	//used internally by library- not for end user use
	void YieldFrames(int num);
	void YieldSeconds(float secs);
	void YieldPause();
	void YieldResume();
	
protected:
	friend class mcLuaManager;
	
	mcLuaScript(lua_State *, mcLuaManager * man, int registryRef);
	~mcLuaScript(void);
	mcLuaScript * Update(float elapsedSeconds);
	mcLuaScript * next_;
	int registryRef_;
	
private:
	typedef enum {
		YM_NONE,
		YM_FRAME,
		YM_TIME,
		YM_PAUSE
	} YIELDMODE;
	
	lua_State * state_;
	mcLuaManager * manager_;
	YIELDMODE yieldMode_;
	int waitFrames_;
	float waitTime_;
};

#endif //MCLUA_H