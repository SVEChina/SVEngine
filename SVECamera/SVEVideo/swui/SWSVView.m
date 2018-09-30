//
//  SWSVView.m
//  FlowerDance
//
//  Created by yizhou Fu on 2017/12/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SWSVView.h"
#import "SWUISys.h"
#import "SWMainUI.h"
#import "SWMainVC.h"
#import "../swlogic/SWLogicSys.h"
#import "../swbasic/SWBasicSys.h"
#import "SWDataSourceCamera.h"
@interface SWSVView() {
    CAEAGLLayer* m_layer;
}

@property (nonatomic,strong)UIImageView *focusView;
@end

@implementation SWSVView

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
        CGFloat t_width = [SWLogicSys getInst].pSWState.svOutW;
        CGFloat t_height = [SWLogicSys getInst].pSWState.svOutH;
        m_layer.frame = CGRectMake(0, 0, t_width, t_height);
        m_layer.opaque = NO;
        m_layer.drawableProperties = @{kEAGLDrawablePropertyRetainedBacking:@NO,
                                       kEAGLDrawablePropertyColorFormat:kEAGLColorFormatRGBA8};
        //
        unsigned int t_fboID;
        glGenFramebuffers(1, &t_fboID);
        _m_fboID = t_fboID;
        glBindFramebuffer(GL_FRAMEBUFFER, _m_fboID);
        //
        unsigned int t_colorID;
        glGenRenderbuffers(1, &t_colorID);
        _m_colorID = t_colorID;
        glBindRenderbuffer(GL_RENDERBUFFER, _m_colorID);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _m_colorID);
        [_GLContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:m_layer];
        int t_layer_w,t_layer_h;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &t_layer_w);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &t_layer_h);
        self.m_layer_w = t_layer_w;
        self.m_layer_h = t_layer_h;
        //
        //check success
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            NSLog(@"Failed to make complete framebuffer object: %i", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        }
        [self.layer insertSublayer:m_layer atIndex:0];
        m_layer.anchorPoint = CGPointMake(0.5, 0.5);
        _m_layer_scale_x = self.bounds.size.width/t_width;
        _m_layer_scale_y = self.bounds.size.height/t_height;
        float t_scale = _m_layer_scale_x > _m_layer_scale_y ? _m_layer_scale_x : _m_layer_scale_y;
        CATransform3D translate = CATransform3DMakeTranslation((self.bounds.size.width - t_width)*0.5, (self.bounds.size.height - t_height)*0.5, 0);
        CATransform3D transform = CATransform3DScale(translate, t_scale, t_scale ,1.0);
        m_layer.transform = transform;
    }
    [self insertSubview:[self focusView] atIndex:1];
    [self active];
}

-(void)destroyGLView:(EAGLContext*)_GLContext{
    //
    [self unactive];
    //
    if( _GLContext ){
        [EAGLContext setCurrentContext:_GLContext];
        glBindFramebuffer(GL_FRAMEBUFFER, _m_fboID);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, 0);
        unsigned int t_colorID = _m_colorID;
        glDeleteRenderbuffers(1, &t_colorID);
        _m_colorID = 0;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        unsigned int t_fboID = _m_fboID;
        glDeleteFramebuffers(1, &t_fboID);
        _m_fboID = 0;
        [m_layer removeFromSuperlayer];
    }
}

-(void)active {
    //创建渲染目标
    if( [SWLogicSys getInst].pSVI ) {
        [[SWLogicSys getInst].pSVI createRenderTarget:_m_fboID Color:_m_colorID Width:_m_layer_w Height:_m_layer_h Mirror:true];
    }
}

-(void)unactive {
    if( [SWLogicSys getInst].pSVI ) {
        [[SWLogicSys getInst].pSVI destroyRenderTarget];
    }
}


- (void)layoutSubviews {
    // 一定要调用super的方法
    [super layoutSubviews];
}

//对焦层
-(UIImageView*)focusView{
    if (!_focusView) {
        int t_width=100/ [SWLogicSys getInst].pSWState.svAdaptRate;
        _focusView = [[UIImageView alloc]initWithFrame:CGRectMake(0, 0, t_width, t_width)];
        _focusView.image =[UIImage imageNamed:@"duijiao"];
        _focusView.alpha=.5;
        //  _focusView.hidden = false;
    }
    return _focusView;
}

#pragma mark - touches

- (void)focusGesture:(CGPoint) point{
    _focusView.center = point;
    _focusView.hidden = NO;
    [UIView animateWithDuration:0.15 animations:^{
        //        CGRect frame = _settingView.frame;
        //        frame.origin.y = self.view.frame.size.height;
        //        _settingView.frame = frame;
        self.focusView.alpha=1;
        self.focusView.transform = CGAffineTransformMakeScale(1.2, 1.2);
    }completion:^(BOOL finished) {
        [UIView animateWithDuration:1 animations:^{
            self.focusView.transform = CGAffineTransformIdentity;
        } completion:^(BOOL finished) {
            self.focusView.hidden = YES;
        }];
    }];
}

////获取所有的触摸对象
//NSArray * array = [touches allObjects];
////分别取出两个touch对象
//UITouch * pTouch1 = [array objectAtIndex:0];
//UITouch * pTouch2 = [array objectAtIndex:1];
////获取两个touch对象的坐标点
//CGPoint point1 = [pTouch1 locationInView:self];
//CGPoint point2 = [pTouch2 locationInView:self];

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
    [super touchesBegan:touches withEvent:event];
    //以字符的形式输出触摸点
    UITouch * pTouch = [touches anyObject];
    CGPoint point = [pTouch locationInView:self];
    NSUInteger tapCount = [pTouch tapCount];
      [self focusGesture:point];
    //对触摸次数判断
    if (tapCount == 1){
//        //在0.2秒内只触摸一次视为单击
//        [self performSelector:@selector(singleTouch:) withObject:nil afterDelay:0.2];
    }
    else if(tapCount == 2){
//        //取消单击响应，若无此方法则双击看做是：单击事件和双击事件
//        [NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(singleTouch:) object:nil];
//        //确定为双击事件
//        [self doubleTouch:nil];
    }
    
    //修正坐标
    //扩大后的尺寸

    //映射到背景分辨率上，左下角要是（0， 0）
    CGPoint n_p = CGPointMake(point.x/_m_layer_scale_x, [SWLogicSys getInst].pSWState.svOutH - point.y/_m_layer_scale_y);
    [[SWLogicSys getInst].pSVI pushTouchX:n_p.x Y:n_p.y State:1];
    
//    //
    float t_view_w = self.bounds.size.width;
    float t_view_h = self.bounds.size.height;
    float t_real_w = self.frame.size.width / [SWLogicSys getInst].pSWState.svAdaptRate;
    float t_real_h = self.frame.size.height / [SWLogicSys getInst].pSWState.svAdaptRate;
    CGPoint t_off_center = CGPointMake(point.x - self.frame.size.width*0.5  ,  point.y - self.frame.size.height*0.5 );
    float t_n_x = t_real_w*0.5f + t_off_center.x;
    float t_n_y = t_real_h*0.5f + t_off_center.y;
    //
    CGPoint pointCamera =  CGPointMake( (t_n_y + t_real_h*0.5f)/(t_real_h*[SWLogicSys getInst].pSWState.svAdaptRate),(t_n_x+t_real_w*0.5f)/(t_real_w*[SWLogicSys getInst].pSWState.svAdaptRate) );
    if ([[SWBasicSys getInst].m_pDataSrc getDataSrcType] == DATASRCCAMERA) {
        SWDataSourceCamera *m_camera = (SWDataSourceCamera *)[SWBasicSys getInst].m_pDataSrc;
        [m_camera chageCameraExposure:pointCamera isContinuousAuto:true];
        [m_camera autoContinuousWhiteMode:true];
        [m_camera changeCameraFocusing:pointCamera];
    }
//
   
}


- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{
    [super touchesEnded:touches withEvent:event];
//    if( [SWUISys getInst].pMainVC.pMainView.m_uihide ) {
//        [[SWUISys getInst].pMainVC.pMainView showFrame];
//    } else {
//        UITouch * pTouch = [touches anyObject];
//        CGPoint point = [pTouch locationInView:self];
//        [[SWLogicSys getInst].pSVI pushTouchX:point.x Y:point.y State:2];
//    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event{
//    [super touchesMoved:touches withEvent:event];
//    UITouch * pTouch = [touches anyObject];
//    CGPoint point = [pTouch locationInView:self];
//    //修正坐标
////    //扩大后的尺寸
////    float t_view_w = self.frame.size.width;
////    float t_view_h = self.frame.size.height;
////    float t_real_w = self.frame.size.width / [SWLogicSys getInst].pSWState.svAdaptRate;
////    float t_real_h = self.frame.size.height / [SWLogicSys getInst].pSWState.svAdaptRate;
////    CGPoint t_off_center = CGPointMake(point.x - self.frame.size.width*0.5  ,  point.y - self.frame.size.height*0.5 );
////    point.x = t_real_w*0.5f + t_off_center.x;
////    point.y = t_real_h*0.5f + t_off_center.y;
//    [[SWLogicSys getInst].pSVI pushTouchX:point.x Y:point.y State:3];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event{
    [super touchesCancelled:touches withEvent:event];
}

@end
