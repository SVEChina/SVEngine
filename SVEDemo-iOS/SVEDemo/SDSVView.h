//
//  SDSVView.h
//  SVEDemo
//
//  Created by 李晓帆 on 2018/9/3.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
@interface SDSVView : UIView
-(void)createGLView:(EAGLContext*)_GLContext;
-(void)destroyGLView:(EAGLContext*)_GLContext;
@end
