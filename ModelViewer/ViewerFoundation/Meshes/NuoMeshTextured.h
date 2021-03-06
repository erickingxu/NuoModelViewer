//
//  NuoMeshTextured.h
//  ModelViewer
//
//  Created by middleware on 9/7/16.
//  Copyright © 2016 middleware. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import "NuoMesh.h"


@interface NuoMeshTextured : NuoMesh


@property (nonatomic, strong) id<MTLTexture> diffuseTex;
@property (nonatomic, readonly) id<MTLSamplerState> samplerState;

@property (nonatomic, readonly) bool hasTransparency;


- (instancetype)initWithDevice:(id<MTLDevice>)device
            withVerticesBuffer:(void*)buffer withLength:(size_t)length
                   withIndices:(void*)indices withLength:(size_t)indicesLength;

- (void)makeTexture:(NSString*)texPath checkTransparency:(BOOL)check;


@end
