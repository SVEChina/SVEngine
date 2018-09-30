//
//  SDSVView.m
//  SVEDemo
//
//  Created by 李晓帆 on 2018/9/3.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#import "SDSVView.h"
#import "SDLogicSys.h"

@interface SDSVView(){
    CAEAGLLayer* m_layer;
    unsigned int m_fboID;
    unsigned int m_colorID;
    unsigned int m_layer_w;
    unsigned int m_layer_h;
}
@end

@implementation SDSVView

- (id)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
        self.multipleTouchEnabled = false;
        self.userInteractionEnabled = YES;
    }
    return self;
}

-(void)createGLView:(EAGLContext*)_GLContext {
    if( _GLContext ){
        [EAGLContext setCurrentContext:_GLContext];
        m_layer = [CAEAGLLayer layer];
        //这里写死了
        CGFloat t_width = 720;
        CGFloat t_height = 1280;
        m_layer.frame = CGRectMake(0, 0, t_width, t_height);
        m_layer.opaque = NO;
        m_layer.drawableProperties = @{kEAGLDrawablePropertyRetainedBacking:@NO,
                                       kEAGLDrawablePropertyColorFormat:kEAGLColorFormatRGBA8};
        //
        unsigned int t_fboID;
        glGenFramebuffers(1, &t_fboID);
        m_fboID = t_fboID;
        glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
        //
        unsigned int t_colorID;
        glGenRenderbuffers(1, &t_colorID);
        m_colorID = t_colorID;
        glBindRenderbuffer(GL_RENDERBUFFER, m_colorID);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorID);
        [_GLContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:m_layer];
        int t_layer_w,t_layer_h;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &t_layer_w);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &t_layer_h);
        //
        m_layer_w = t_layer_w;
        m_layer_h = t_layer_h;
        //check success
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            NSLog(@"Failed to make complete framebuffer object: %i", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        }
        [self.layer insertSublayer:m_layer atIndex:0];
        m_layer.anchorPoint = CGPointMake(0.5, 0.5);
        float t_layer_scale_x = self.bounds.size.width/t_width;
        float t_layer_scale_y = self.bounds.size.height/t_height;
        float t_scale = t_layer_scale_x > t_layer_scale_y ? t_layer_scale_x : t_layer_scale_y;
        CATransform3D translate = CATransform3DMakeTranslation((self.bounds.size.width - t_width)*0.5, (self.bounds.size.height - t_height)*0.5, 0);
        CATransform3D transform = CATransform3DScale(translate, t_scale, t_scale ,1.0);
        m_layer.transform = transform;
        
        m_layer.backgroundColor = [UIColor greenColor].CGColor;
    }
    [self active];
}

-(void)destroyGLView:(EAGLContext*)_GLContext{
    //
    [self unactive];
    //
    if( _GLContext ){
        [EAGLContext setCurrentContext:_GLContext];
        glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, 0);
        glDeleteRenderbuffers(1, &m_colorID);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &m_fboID);
        [m_layer removeFromSuperlayer];
    }
}

-(void)active {
    //创建渲染目标
    if( [[SDLogicSys getInst] getSVE] ) {
//        [[SDLogicSys getInst].pSVI createRenderTarget:m_fboID Color:m_colorID Width:m_layer_w Height:m_layer_h Mirror:true];
    }
}

-(void)unactive {
    if( [[SDLogicSys getInst] getSVE]) {
        //[[SDLogicSys getInst].pSVI destroyRenderTarget];
    }
}


@end
