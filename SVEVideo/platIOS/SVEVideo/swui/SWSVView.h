//
//  mixTestView.h
//  FlowerDance
//
//  Created by yizhou Fu on 2017/12/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import <UIKit/UIKit.h>

#import <GLKit/GLKit.h>
#import <OpenGLES/EAGL.h>

@interface SWSVView : UIView

@property (assign, nonatomic) unsigned int m_fboID;
@property (assign, nonatomic) unsigned int m_colorID;
@property (assign, nonatomic) float m_layer_scale_x;
@property (assign, nonatomic) float m_layer_scale_y;
@property (assign, nonatomic) int m_layer_w;
@property (assign, nonatomic) int m_layer_h;

-(void)createGLView:(EAGLContext*)_GLContext;

-(void)destroyGLView:(EAGLContext*)_GLContext;

-(void)active;

-(void)unactive;

- (void)changeToShow;

@end
