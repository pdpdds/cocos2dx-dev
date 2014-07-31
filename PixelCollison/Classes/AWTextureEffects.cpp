//
//  AWTextureEffects.m
//
//  Created by Manuel Martinez-Almeida Castañeda on 09/05/10.
//  Copyright 2010 Abstraction Works. All rights reserved.
//  http://www.abstractionworks.com
//

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "AWTextureEffects.h"
//#include "ccMacros.h"

using namespace cocos2d;

void
AWTextureEffect::blurInput(void* input, void* output, CCTexture2DPixelFormat format, int width, int height, CCPoint position, CCPoint size, CCSize contentSize, int radius)
{
	int cr, cg, cb, ca;
    int read, i, xl, yl, yi, ym, ri, riw;
	int wh = width*height;
	
	size.x = (size.x==0) ? contentSize.width : size.x;
	size.y = (size.y==0) ? contentSize.height : size.y;
	
	//Check data
	position = CCPointMake(MAX(0, position.x), MAX(0, position.y));
    size.x = position.x+size.x-MAX(0, (position.x+size.x)-width);
	size.y = position.y+size.y-MAX(0, (position.y+size.y)-height);
    yi = position.y*width;
	
	//Generate Gaussian kernel
    radius = MIN(MAX(1,radius), 248);
    int kernelSize = 1+radius*2;
    int *kernel;	//[kernelSize];	
    int g = 0, sum = 0;
	
	kernel = (int *)malloc(kernelSize);
	//Gaussian filter	
    for (i = 0; i<radius;i++){
		g = i*i+1;
		kernel[i] = kernel[kernelSize-i-1] = g;
		sum+=g*2;
    }
	g = radius*radius;
    kernel[radius] = g;
    sum+=g;
	
	if(format == kTexture2DPixelFormat_RGBA8888){
		ccColor4B* originalData = (ccColor4B*) input;
		ccColor4B* data = (ccColor4B*) output;
		ccColor4B* temp = (ccColor4B*)malloc(wh*4);
		ccColor4B *pixel;
		
		//Horizontal blur
		for (yl = position.y; yl<size.y; yl++){
			for (xl = position.x; xl<size.x; xl++){
				cb = cg = cr = ca = 0;
				ri = xl-radius;
				for (i = 0; i<kernelSize; i++){
					read = ri+i;
					if (read>=position.x && read<size.x){
						read+=yi;
						pixel = &originalData[read];
						cr+= pixel->r*kernel[i];
						cg+= pixel->g*kernel[i];
						cb+= pixel->b*kernel[i];
						ca+= pixel->a*kernel[i];
					}
				}
				ri = yi+xl;
				pixel = &temp[ri];
				pixel->r = cr/sum;
				pixel->g = cg/sum;
				pixel->b = cb/sum;
				pixel->a = ca/sum;
			}
			yi+=width;
		}
		yi = position.y*width;
		
		//Vertical blur
		for (yl = position.y; yl<size.y; yl++){
			ym = yl-radius;
			riw = ym*width;
			for (xl = position.x; xl<size.x; xl++){
				cb = cg = cr = ca = 0;
				ri = ym;
				read = xl+riw;
				for (i = 0; i<kernelSize; i++){
					if (ri<size.y && ri>=position.y){
						pixel = &temp[read];
						cr+= pixel->r*kernel[i];
						cg+= pixel->g*kernel[i];
						cb+= pixel->b*kernel[i];
						ca+= pixel->a*kernel[i];
					}
					ri++;
					read+=width;
				}
				pixel = &data[xl+yi];
				pixel->r = cr/sum;
				pixel->g = cg/sum;
				pixel->b = cb/sum;
				pixel->a = ca/sum;
			}
			yi+=width;
		}
		
		//Free temp data
		free(temp);
	}else{
		//[NSException raise:@"AWTextureEffect" format:@"Pixel format don't supported"];
	}

	free(kernel);
}


CCTexture2DMutable* 
AWTextureEffect::blur(CCTexture2DMutable* texture, int radius)
{
	return blur(texture, CCPointMake(0,0), CCPointMake(0,0), radius);
}

CCTexture2DMutable* 
AWTextureEffect::blur(CCTexture2DMutable* texture, CCPoint position, CCPoint size, int radius)
{
	if(!texture)
		return NULL;
	
	//Apply the effect to the texture
	blurInput(texture->getOriginalTexData(), texture->getTexData(), texture->getPixelFormat(), \
		texture->getPixelsWide(), texture->getPixelsHigh(), position, size, texture->getContentSize(), radius);
	//Update the GPU data
	texture->apply();
	return texture;
}
