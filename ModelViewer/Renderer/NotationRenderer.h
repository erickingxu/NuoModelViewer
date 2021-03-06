//
//  NotationRenderer.h
//  ModelViewer
//
//  Created by dfeng on 11/8/16.
//  Copyright © 2016 middleware. All rights reserved.
//



#import "NuoIntermediateRenderPass.h"


@class LightSource;


@interface NotationRenderer : NuoIntermediateRenderPass


@property (nonatomic, readonly) NSArray<LightSource*>* lightSources;

@property (nonatomic, assign) CGRect notationArea;


// manipulator to the current selected light source

@property (nonatomic, assign) float rotateX;
@property (nonatomic, assign) float rotateY;
@property (nonatomic, assign) float density;

- (void)selectCurrentLightVector:(CGPoint)point;


@end
