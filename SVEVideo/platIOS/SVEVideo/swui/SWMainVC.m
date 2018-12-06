// Copyright (c) 2013 Mutual Mobile (http://mutualmobile.com/)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#import "SWMainVC.h"
#import "SWSVView.h"
#import "SWMainUI.h"
#import "../swlogic/SWLogicSys.h"

@implementation SWMainVC

- (void)viewDidLoad{
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor colorWithRed:215.0f/255.0f green:215.0f/255.0f blue:215.0f/255.0f alpha:1.0];
    //创建渲染器
    float t_sc = [UIScreen mainScreen].nativeScale;
    CGRect  _rc = [[UIScreen mainScreen] bounds];
    //计算该手机dpi下 目标分辨率的大小
    float t_dpi_w = _rc.size.width;
    float t_dpi_h = _rc.size.height;
    float t_svout_w = [SWLogicSys getInst].pSWState.svOutW/t_sc;
    float t_svout_h = [SWLogicSys getInst].pSWState.svOutH/t_sc;
    //适配(iphonex算的)
    float t_rate_w = t_dpi_w/t_svout_w;
    float t_rate_h = t_dpi_h/t_svout_h;
    //1 全屏(等比) 2.像素不裁剪(等比)
    if( [SWLogicSys getInst].pSWState.svAdaptMode == 1 ) {
        [SWLogicSys getInst].pSWState.svAdaptRate = t_rate_w>t_rate_h?t_rate_w:t_rate_h;   //取大
    }else if( [SWLogicSys getInst].pSWState.svAdaptMode == 2 ) {
        [SWLogicSys getInst].pSWState.svAdaptRate = t_rate_w<t_rate_h?t_rate_w:t_rate_h;   //取小
    }
    //计算真实的大小
    float t_real_w = t_svout_w*[SWLogicSys getInst].pSWState.svAdaptRate;
    float t_real_h = t_svout_h*[SWLogicSys getInst].pSWState.svAdaptRate;
    //创建主UI并加入主视图
    self.pMainView = [[SWMainUI alloc] initWithFrame: _rc];
    [self.view addSubview:self.pMainView];
    
    //创建相机窗口
    self.pSVView = [[SWSVView alloc] initWithFrame: CGRectMake(0, 0, t_dpi_w ,t_dpi_h )];
    [self.pSVView createGLView: [[SWLogicSys getInst] getGLContext]];
    self.view.backgroundColor = [UIColor redColor];
}

- (void)dealloc{
    if( self.pSVView ){
        [self.pSVView destroyGLView:[[SWLogicSys getInst] getGLContext]];
        [self.pSVView removeFromSuperview];
        self.pSVView = nil;
    }
    [self.pMainView destroy];
}



@end
