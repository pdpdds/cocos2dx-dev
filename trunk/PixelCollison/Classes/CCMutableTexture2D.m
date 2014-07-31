//
//  CCMutableTexture.m
//	Created by Lam Hoang Pham.
//

#import "CCMutableTexture2D.h"
#import "ccMacros.h"


@interface CCRenderTexture (MutableExtensionPrivate)

-(GLvoid*)getPixels;

@end


@implementation CCRenderTexture (MutableExtension)

- (CCMutableTexture2D *) mutableTexture
{
	CGSize s = [texture_ contentSizeInPixels];
	int tx = s.width;
	int ty = s.height;
	
	GLvoid *pixels = [self getPixels];
	
	id tex = [[  [CCMutableTexture2D alloc] initWithData: pixels 
											 pixelFormat: texture_.pixelFormat
											  pixelsWide: tx 
											  pixelsHigh: ty 
											 contentSize: s    ] autorelease];
	free(pixels);
	return tex;
	
}

-(GLvoid*)getPixels
{
	NSAssert(pixelFormat_ == kCCTexture2DPixelFormat_RGBA8888,@"only RGBA8888 can be saved as image");
	
	CGSize s = [texture_ contentSizeInPixels];
	int tx = s.width;
	int ty = s.height;
	
	//int bitsPerComponent=8;			
//	int bitsPerPixel=32;				
//	
//	int bytesPerRow					= (bitsPerPixel/8) * tx;
//	NSInteger myDataLength			= bytesPerRow * ty;
	
//	GLubyte *buffer	= malloc(sizeof(GLubyte)*myDataLength);
	
	NSUInteger bytesPerPixel = 0;
	
	switch (texture_.pixelFormat ) {
		case kTexture2DPixelFormat_RGBA8888:	bytesPerPixel = 4; break;
		case kTexture2DPixelFormat_A8:			bytesPerPixel = 1; break;
		case kTexture2DPixelFormat_RGBA4444:
		case kTexture2DPixelFormat_RGB565:
		case kTexture2DPixelFormat_RGB5A1:
			bytesPerPixel = 2;
			break;
		default:break;
	}
	
	
	GLvoid *pixels;
	NSUInteger max = tx*ty*bytesPerPixel;
	pixels = malloc(max);
	
	
	if( ! pixels ) 
	{
		CCLOG(@"cocos2d: CCRenderTexture#getPixels: not enough memory");
		free(pixels);
		return NULL;
	}
	
	[self begin];
	//glReadPixels(0,0,tx,ty,GL_RGBA,GL_UNSIGNED_BYTE, buffer);
	switch(texture_.pixelFormat )
	{
			//GL_TEXTURE_2D -> name_
		case kCCTexture2DPixelFormat_RGBA8888:
			glReadPixels(0,0,tx,ty, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
			break;
		case kCCTexture2DPixelFormat_RGBA4444:
			glReadPixels(0,0,tx,ty, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, pixels);
			break;
		case kCCTexture2DPixelFormat_RGB5A1:
			glReadPixels(0,0,tx,ty, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, pixels);
			break;
		case kCCTexture2DPixelFormat_RGB565:
			glReadPixels(0,0,tx,ty, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pixels);
			break;
		case kCCTexture2DPixelFormat_A8:
			glReadPixels(0,0,tx,ty, GL_ALPHA, GL_UNSIGNED_BYTE, pixels);
			break;
		default:
			[NSException raise:NSInternalInconsistencyException format:@""];
	}
	[self end];
	
	return pixels;
}



@end


@implementation CCTexture2D (MutableExtension) 

- (id) mutableCopyWithZone:(NSZone *)zone
{
	
	NSUInteger bytesPerPixel = 0;
	
	switch (format_) {
		case kTexture2DPixelFormat_RGBA8888:	bytesPerPixel = 4; break;
		case kTexture2DPixelFormat_A8:			bytesPerPixel = 1; break;
		case kTexture2DPixelFormat_RGBA4444:
		case kTexture2DPixelFormat_RGB565:
		case kTexture2DPixelFormat_RGB5A1:
			bytesPerPixel = 2;
			break;
		default:break;
	}
	
	
	GLvoid *pixels;
	NSUInteger max = width_*height_*bytesPerPixel;
	pixels = malloc(max);
	
	glBindTexture(GL_TEXTURE_2D, name_);
	switch(format_)
	{
			//GL_TEXTURE_2D -> name_
		case kCCTexture2DPixelFormat_RGBA8888:
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
			break;
		case kCCTexture2DPixelFormat_RGBA4444:
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, pixels);
			break;
		case kCCTexture2DPixelFormat_RGB5A1:
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, pixels);
			break;
		case kCCTexture2DPixelFormat_RGB565:
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pixels);
			break;
		case kCCTexture2DPixelFormat_A8:
			glGetTexImage(GL_TEXTURE_2D, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pixels);
			break;
		default:
			[NSException raise:NSInternalInconsistencyException format:@""];
	}
	
	id tex = [  [CCMutableTexture2D allocWithZone: zone] initWithData: pixels 
														  pixelFormat: format_ 
														   pixelsWide: width_ 
														   pixelsHigh: height_ 
														  contentSize: size_    ];
	
	free(pixels);
	return tex;
}

@end




@implementation CCMutableTexture2D
@synthesize texData = data_;
@synthesize originalTexData = originalData_;

- (void) releaseData:(void*)data {
	//Don't free the data
}

- (id) initWithData:(void*)data pixelFormat:(CCTexture2DPixelFormat)pixelFormat pixelsWide:(NSUInteger)width pixelsHigh:(NSUInteger)height contentSize:(CGSize)size{
	if((self = [super initWithData:data pixelFormat:pixelFormat pixelsWide:width pixelsHigh:height contentSize:size])){
		switch (pixelFormat) {
			case kTexture2DPixelFormat_RGBA8888:	bytesPerPixel_ = 4; break;
			case kTexture2DPixelFormat_A8:			bytesPerPixel_ = 1; break;
			case kTexture2DPixelFormat_RGBA4444:
			case kTexture2DPixelFormat_RGB565:
			case kTexture2DPixelFormat_RGB5A1:
				bytesPerPixel_ = 2;
				break;
			default:break;
		}
		NSUInteger max = width*height*bytesPerPixel_;
		data_ = (void*) data;
		originalData_ = malloc(max);
		memcpy(originalData_, data_, max);
	}
	return self;
}

- (ccColor4B) pixelAt:(CGPoint) pt {
	ccColor4B c = {0, 0, 0, 0};
	if(!data_) return c;
	if(pt.x < 0 || pt.y < 0) return c;
	if(pt.x >= size_.width || pt.y >= size_.height) return c;
	
	uint x = pt.x, y = pt.y;
	
	if(format_ == kTexture2DPixelFormat_RGBA8888){
		uint *pixel = data_;
		pixel = pixel + (y * width_) + x;
		c.r = *pixel & 0xff;
		c.g = (*pixel >> 8) & 0xff;
		c.b = (*pixel >> 16) & 0xff;
		c.a = (*pixel >> 24) & 0xff;
	} else if(format_ == kTexture2DPixelFormat_RGBA4444){
		GLushort *pixel = data_;
		pixel = pixel + (y * width_) + x;
		c.a = ((*pixel & 0xf) << 4) | (*pixel & 0xf);
		c.b = (((*pixel >> 4) & 0xf) << 4) | ((*pixel >> 4) & 0xf);
		c.g = (((*pixel >> 8) & 0xf) << 4) | ((*pixel >> 8) & 0xf);
		c.r = (((*pixel >> 12) & 0xf) << 4) | ((*pixel >> 12) & 0xf);
	} else if(format_ == kTexture2DPixelFormat_RGB5A1){
		GLushort *pixel = data_;
		pixel = pixel + (y * width_) + x;
		c.r = ((*pixel >> 11) & 0x1f)<<3;
		c.g = ((*pixel >> 6) & 0x1f)<<3;
		c.b = ((*pixel >> 1) & 0x1f)<<3;
		c.a = (*pixel & 0x1)*255;
	} else if(format_ == kTexture2DPixelFormat_RGB565){
		GLushort *pixel = data_;
		pixel = pixel + (y * width_) + x;
		c.b = (*pixel & 0x1f)<<3;
		c.g = ((*pixel >> 5) & 0x3f)<<2;
		c.r = ((*pixel >> 11) & 0x1f)<<3;
		c.a = 255;
	} else if(format_ == kTexture2DPixelFormat_A8){
		GLubyte *pixel = data_;
		c.a = pixel[(y * width_) + x];
		// Default white
		c.r = 255;
		c.g = 255;
		c.b = 255;
	}
	
	return c;
}

- (BOOL) setPixelAt:(CGPoint) pt rgba:(ccColor4B) c {
	if(!data_)return NO;
	if(pt.x < 0 || pt.y < 0) return NO;
	if(pt.x >= size_.width || pt.y >= size_.height) return NO;
	uint x = pt.x, y = pt.y;
	
	dirty_ = true;
	
	//	Shifted bit placement based on little-endian, let's make this more
	//	portable =/
	
	if(format_ == kTexture2DPixelFormat_RGBA8888){
		uint *pixel = data_;
		pixel[(y * width_) + x] = (c.a << 24) | (c.b << 16) | (c.g << 8) | c.r;
	} else if(format_ == kTexture2DPixelFormat_RGBA4444){
		GLushort *pixel = data_;
		pixel = pixel + (y * width_) + x;
		*pixel = ((c.r >> 4) << 12) | ((c.g >> 4) << 8) | ((c.b >> 4) << 4) | (c.a >> 4);
	} else if(format_ == kTexture2DPixelFormat_RGB5A1){
		GLushort *pixel = data_;
		pixel = pixel + (y * width_) + x;
		*pixel = ((c.r >> 3) << 11) | ((c.g >> 3) << 6) | ((c.b >> 3) << 1) | (c.a > 0);
	} else if(format_ == kTexture2DPixelFormat_RGB565){
		GLushort *pixel = data_;
		pixel = pixel + (y * width_) + x;
		*pixel = ((c.r >> 3) << 11) | ((c.g >> 2) << 5) | (c.b >> 3);
	} else if(format_ == kTexture2DPixelFormat_A8){
		GLubyte *pixel = data_;
		pixel[(y * width_) + x] = c.a;
	} else {
		dirty_ = false;
		return NO;
	}
	return YES;
}

- (void) fill:(ccColor4B) p {
	for(int r = 0; r < size_.height;++r){
		for(int c = 0; c < size_.width; ++c){
			[self setPixelAt:CGPointMake(c, r) rgba:p];
		}
	}
}

- (id) copy{
	return [[[CCMutableTexture2D alloc] initWithData:data_ pixelFormat:format_ pixelsWide:width_ pixelsHigh:height_ contentSize:size_] autorelease];
}

- (void) copy:(CCMutableTexture2D*)textureToCopy offset:(CGPoint) offset {
	for(int r = 0; r < size_.height;++r){
		for(int c = 0; c < size_.width; ++c){
			[self setPixelAt:CGPointMake(c + offset.x, r + offset.y) rgba:[textureToCopy pixelAt:CGPointMake(c, r)]];
		}
	}
}

- (void) restore {
	memcpy(data_, originalData_, bytesPerPixel_*width_*height_);
	[self apply];
}

- (void) apply {
	//if(!dirty_) return;
	if(!data_) return;
	
	glBindTexture(GL_TEXTURE_2D, name_);
	
	switch(format_)
	{
		case kTexture2DPixelFormat_RGBA8888:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data_);
			break;
		case kTexture2DPixelFormat_RGBA4444:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, data_);
			break;
		case kTexture2DPixelFormat_RGB5A1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, data_);
			break;
		case kTexture2DPixelFormat_RGB565:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data_);
			break;
		case kTexture2DPixelFormat_A8:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width_, height_, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data_);
			break;
		default:
			[NSException raise:NSInternalInconsistencyException format:@""];
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	dirty_ = NO;
}

- (void) dealloc
{
	free(data_);
	free(originalData_);
	[super dealloc];
}


@end
