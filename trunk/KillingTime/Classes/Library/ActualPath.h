#ifndef ACTUALPATH_H
#define ACTUALPATH_H
#include <Foundation/Foundation.h>

/* This returns the full absolute path to a specified file in the bundle */
CCString* getActualPath( CCString* file )
{
	CCArray* path = file->componentsSeparatedByString( ".");
	CCString* actualPath = NSBundle::mainBundle()->pathForResource( [path ,  0) ,  path->objectAtIndex( 1)];
		
	return actualPath;



#endif //ACTUALPATH_H