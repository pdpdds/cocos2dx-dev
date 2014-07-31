//
//  CCMutableTexture.h
//	Created by Lam Hoang Pham.
//
#ifndef	__CCMUTABLETEXTURE2D_H__
#define	__CCMUTABLETEXTURE2D_H__

#include "cocos2d.h"
//#include "CCTexture2D.h"
//#include "ccTypes.h"
//#include "CCRenderTexture.h"

class CCRenderTexture (MutableExtension)

- (id) mutableTexture;

@end

@interface CCTexture2D (MutableExtension) <NSMutableCopying>

- (id) mutableCopyWithZone:(NSZone *)zone;

@end

@class CCRenderTexture;
@interface CCMutableTexture2D : CCTexture2D {
	void *originalData_;
	void *data_;
	NSUInteger bytesPerPixel_;
	bool dirty_;
}
@property(nonatomic, readonly) void *originalTexData;
@property(nonatomic, readwrite) void *texData;

- (ccColor4B) pixelAt:(CGPoint) pt;

///
//	@param pt is a point to get a pixel (0,0) is top-left to (width,height) bottom-right
//	@param c is a ccColor4B which is a colour.
//	@returns if a pixel was set
//	Remember to call apply to actually update the texture canvas.
///
- (BOOL) setPixelAt:(CGPoint) pt rgba:(ccColor4B) c;

///
//	Fill with specified colour
///
- (void) fill:(ccColor4B) c;

///
//	@param textureToCopy is the texture image to copy over
//	@param offset also offset's the texture image
///
- (id) copy;
- (void) copy:(CCMutableTexture2D*)textureToCopy offset:(CGPoint) offset;

///
//	apply actually updates the texture with any new data we added.
///
- (void) apply;

@end

#endif	// __CCMUTABLETEXTURE2D_H__