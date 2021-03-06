//
//  NuoTextureBase.m
//  ModelViewer
//
//  Created by middleware on 9/23/16.
//  Copyright © 2016 middleware. All rights reserved.
//

#import "NuoTextureBase.h"

#import <CoreImage/CoreImage.h>



@implementation NuoTexture

@end






@interface NuoTextureBase()



@property (strong) CIContext* CIContext;
@property (weak) id<MTLDevice> device;

@property (strong) NSMutableDictionary<NSString*, NuoTexture*>* texturePool;


@end



static NuoTextureBase* sInstance;




@implementation NuoTextureBase



+ (NuoTextureBase*)getInstance:(id<MTLDevice>)device
{
    if (!sInstance)
    {
        sInstance = [NuoTextureBase new];
        sInstance.device = device;
        sInstance.texturePool = [NSMutableDictionary new];
    }
    
    return sInstance;
}



- (NuoTexture*)texture2DWithImageNamed:(NSString *)imagePath
                             mipmapped:(BOOL)mipmapped
                     checkTransparency:(BOOL)checkTransparency
{
    NuoTexture* result = [_texturePool objectForKey:imagePath];
    if (result)
    {
        goto handleTransparency;
    }
    
    {
        CIImage *image = [[CIImage alloc] initWithContentsOfURL:[NSURL fileURLWithPath:imagePath]];
        
        if (image == nil)
        {
            return nil;
        }
        
        BOOL hasTransparency = NO;
        
        NSSize imageSize = image.extent.size;
        const NSUInteger bytesPerPixel = 4;
        const NSUInteger bytesPerRow = bytesPerPixel * imageSize.width;
        uint8_t *imageData = [self dataForImage:image hasTransparent:&hasTransparency];
        
        MTLTextureDescriptor *textureDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
                                                                                                     width:imageSize.width
                                                                                                    height:imageSize.height
                                                                                                 mipmapped:mipmapped];
        id<MTLTexture> texture = [self.device newTextureWithDescriptor:textureDescriptor];
        
        MTLRegion region = MTLRegionMake2D(0, 0, imageSize.width, imageSize.height);
        [texture replaceRegion:region mipmapLevel:0 withBytes:imageData bytesPerRow:bytesPerRow];
        
        free(imageData);
        
        result = [NuoTexture new];
        result.texture = texture;
        result.hasTransparency = hasTransparency;
        
        [_texturePool setObject:result forKey:imagePath];
    }
    
handleTransparency:
    if (!checkTransparency)
    {
        NuoTexture* opaque = [NuoTexture new];
        opaque.texture = result.texture;
        opaque.hasTransparency = NO;
        return opaque;
    }
    else
    {
        return result;
    }
}



- (uint8_t *)dataForImage:(CIImage *)image hasTransparent:(BOOL*)hasTransparency
{
    if (!_CIContext)
        _CIContext = [CIContext contextWithOptions:nil];
    
    CGImageRef imageRef = [_CIContext createCGImage:image fromRect:image.extent];
    
    // Create a suitable bitmap context for extracting the bits of the image
    const NSUInteger width = CGImageGetWidth(imageRef);
    const NSUInteger height = CGImageGetHeight(imageRef);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    uint8_t *rawData = (uint8_t *)calloc(height * width * 4, sizeof(uint8_t));
    const NSUInteger bytesPerPixel = 4;
    const NSUInteger bytesPerRow = bytesPerPixel * width;
    const NSUInteger bitsPerComponent = 8;
    CGContextRef context = CGBitmapContextCreate(rawData, width, height,
                                                 bitsPerComponent, bytesPerRow, colorSpace,
                                                 kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(colorSpace);
    
    CGContextTranslateCTM(context, 0, height);
    CGContextScaleCTM(context, 1, -1);
    
    CGRect imageRect = CGRectMake(0, 0, width, height);
    CGContextDrawImage(context, imageRect, imageRef);
    
    CGContextRelease(context);
    CGImageRelease(imageRef);
    
    *hasTransparency = [self checkTransparency:rawData withWidth:width withHeight:height];
    
    return rawData;
}



- (BOOL)checkTransparency:(uint8_t *)dataForImage withWidth:(size_t)width withHeight:(size_t)height
{
    const NSUInteger bytesPerPixel = 4;
    const NSUInteger bytesPerRow = bytesPerPixel * width;
    
    for (size_t row = 0; row < height; ++row)
    {
        uint8_t* rowPtr = dataForImage + row * bytesPerRow;
        
        for (size_t col = 0; col < width; ++col)
        {
            uint8_t* pixel = rowPtr + col * bytesPerPixel;
            if (pixel[3] < 250)
                return YES;
        }
    }
    
    return NO;
}




@end
