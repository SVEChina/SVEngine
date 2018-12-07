//
//  SWShareUI.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWShareUI.h"
#import "../swlogic/SWLogicSys.h"
#import "../swbasic/SWBasicSys.h"
#import "SWUISys.h"
#import "Utility.h"

@interface SWShareUI()<SWRecordDelegate>{
    
    NSString* m_vioLocal;
    NSString* m_imgLocal;
    int       m_gameScore;
    UIView*  pMediaPlayer;
    UIImageView*  pImageView;
    UIButton* pBtnShareWX;
    UIButton* pBtnShareWXTimeLine;
    UIButton* pBtnShareWB;
    UIButton* pBtnShareQQ;
    UIButton* pBtnUpLoad;
    UIButton* pBtnSaveLocal;
    UIButton* pBtnClose;
    UIButton* pBtnPrint;
    UIButton* pBtnAgain;//再来一次
    UIButton* pBtnAgainMore;//加个魔贴再来一次
    UIImageView *pImageVieweShare;
    UIImageView *pImageViewBack;
    UIImageView *pImageViewFrame;
    enum SHARETYPE curShareType;
}
@end

@implementation SWShareUI

+ (SWShareUI *)createWithFrame:(CGRect)frame{
    return [[SWShareUI alloc] initWithFrame:frame];
}

- (void)dealloc{
    NSLog(@"destory SWShareUI");
}

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        //本地分享的视频
        m_vioLocal = @"";
        m_imgLocal = @"";
        m_gameScore = 0;
        //
        self.backgroundColor = [UIColor clearColor];
        curShareType = SHAREVIDEO;
        int t_w = frame.size.width;
        int t_h = frame.size.height;
        //back
        pImageViewBack = [[UIImageView alloc] initWithFrame:self.bounds];
        pImageViewBack.image = [UIImage imageNamed:@"share_shareback"];
        [self addSubview:pImageViewBack];
        int t_view_px,t_view_py,t_view_w,t_view_h;
        //视频框
        t_view_px = TRANS_BY_WIDTH(132,t_w);
        t_view_py = TRANS_BY_WIDTH(44,t_w);
        t_view_w = TRANS_BY_WIDTH(483,t_w);
        t_view_h = TRANS_BY_WIDTH(840,t_w);
        pImageViewFrame = [[UIImageView alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pImageViewFrame setImage:[UIImage imageNamed:@"share_frame"]];
        [self addSubview:pImageViewFrame];
        //视频
        t_view_px = TRANS_BY_WIDTH(142,t_w);
        t_view_py = TRANS_BY_WIDTH(54,t_w);
        t_view_w = TRANS_BY_WIDTH(462,t_w);
        t_view_h = TRANS_BY_WIDTH(823,t_w);
        pMediaPlayer = [[UIView alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pMediaPlayer setBackgroundColor:[UIColor clearColor]];
        [self addSubview:pMediaPlayer];
        //
        pImageView = [[UIImageView alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        pImageView.backgroundColor = [UIColor clearColor];
        pImageView.hidden = YES;
        [self addSubview:pImageView];
        //
        t_view_px = TRANS_BY_WIDTH(63,t_w);
        t_view_py = TRANS_BY_WIDTH(1092,t_w);
        t_view_w = TRANS_BY_WIDTH(625,t_w);
        t_view_h = TRANS_BY_WIDTH(27,t_w);
        pImageVieweShare = [[UIImageView alloc] initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        pImageVieweShare.image = [UIImage imageNamed:@"share_fenxiangxuanze"];
        [self addSubview:pImageVieweShare];
        //微信分享
        t_view_px = TRANS_BY_WIDTH(206,t_w);
        t_view_py = TRANS_BY_WIDTH(1189,t_w);
        t_view_w = TRANS_BY_WIDTH(71,t_w);
        t_view_h = TRANS_BY_WIDTH(71,t_w);
        pBtnShareWX = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnShareWX setImage:[UIImage imageNamed:@"share_wechat"] forState:UIControlStateNormal];
        [pBtnShareWX setImage:[UIImage imageNamed:@"share_wechat_press"] forState:UIControlStateHighlighted];
        [pBtnShareWX addTarget:self action:@selector(eventShareWX:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:pBtnShareWX];
        //微信朋友圈
        t_view_px = TRANS_BY_WIDTH(335,t_w);
        t_view_py = TRANS_BY_WIDTH(1189,t_w);
        t_view_w = TRANS_BY_WIDTH(71,t_w);
        t_view_h = TRANS_BY_WIDTH(71,t_w);
        pBtnShareWXTimeLine = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnShareWXTimeLine setImage:[UIImage imageNamed:@"share_wechattimeline"] forState:UIControlStateNormal];
        [pBtnShareWXTimeLine setImage:[UIImage imageNamed:@"share_wechattimeline_press"] forState:UIControlStateHighlighted];
        [pBtnShareWXTimeLine addTarget:self action:@selector(eventShareWXTimeLine:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:pBtnShareWXTimeLine];
        //QQ分享
        t_view_px = TRANS_BY_WIDTH(76,t_w);
        t_view_py = TRANS_BY_WIDTH(1189,t_w);
        t_view_w = TRANS_BY_WIDTH(71,t_w);
        t_view_h = TRANS_BY_WIDTH(71,t_w);
        pBtnShareQQ = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnShareQQ setImage:[UIImage imageNamed:@"share_qq"] forState:UIControlStateNormal];
        [pBtnShareQQ setImage:[UIImage imageNamed:@"share_qq_press"] forState:UIControlStateHighlighted];
        [pBtnShareQQ addTarget:self action:@selector(eventShareQQ:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:pBtnShareQQ];
        //微博分享
        t_view_px = TRANS_BY_WIDTH(463,t_w);
        t_view_py = TRANS_BY_WIDTH(1189,t_w);
        t_view_w = TRANS_BY_WIDTH(71,t_w);
        t_view_h = TRANS_BY_WIDTH(71,t_w);
        pBtnShareWB = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnShareWB setImage:[UIImage imageNamed:@"share_sinaweibo"] forState:UIControlStateNormal];
        [pBtnShareWB setImage:[UIImage imageNamed:@"share_sinaweibo_press"] forState:UIControlStateHighlighted];
        [pBtnShareWB addTarget:self action:@selector(eventShareWB:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:pBtnShareWB];
        //上传
        t_view_px = TRANS_BY_WIDTH(592,t_w);
        t_view_py = TRANS_BY_WIDTH(1189,t_w);
        t_view_w = TRANS_BY_WIDTH(71,t_w);
        t_view_h = TRANS_BY_WIDTH(71,t_w);
        pBtnUpLoad = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnUpLoad setImage:[UIImage imageNamed:@"share_upload"] forState:UIControlStateNormal];
        [pBtnUpLoad setImage:[UIImage imageNamed:@"share_upload_press"] forState:UIControlStateHighlighted];
        [pBtnUpLoad addTarget:self action:@selector(eventShareJM:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:pBtnUpLoad];
        //加个魔贴再来一次
        t_view_px = TRANS_BY_WIDTH(225,t_w);
        t_view_py = TRANS_BY_WIDTH(932,t_w);
        t_view_w = TRANS_BY_WIDTH(300,t_w);
        t_view_h = TRANS_BY_WIDTH(84,t_w);
        pBtnAgainMore = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnAgainMore setImage:[UIImage imageNamed:@"share_change_replay"] forState:UIControlStateNormal];
        [pBtnAgainMore setImage:[UIImage imageNamed:@"share_change_replay_press"] forState:UIControlStateHighlighted];
        [pBtnAgainMore addTarget:self action:@selector(eventAgainMore:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:pBtnAgainMore];
        //再来一次
        t_view_px = TRANS_BY_WIDTH(595,t_w);
        t_view_py = TRANS_BY_WIDTH(932,t_w);
        t_view_w = TRANS_BY_WIDTH(85,t_w);
        t_view_h = TRANS_BY_WIDTH(85,t_w);
        pBtnAgain = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnAgain setImage:[UIImage imageNamed:@"share_replay"] forState:UIControlStateNormal];
        [pBtnAgain setImage:[UIImage imageNamed:@"share_replay_press"] forState:UIControlStateHighlighted];
        [pBtnAgain addTarget:self action:@selector(eventAgain:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:pBtnAgain];
        //保存本地
        t_view_px = TRANS_BY_WIDTH(70,t_w);
        t_view_py = TRANS_BY_WIDTH(931,t_w);
        t_view_w = TRANS_BY_WIDTH(85,t_w);
        t_view_h = TRANS_BY_WIDTH(84,t_w);
        pBtnSaveLocal = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnSaveLocal setImage:[UIImage imageNamed:@"share_save1"] forState:UIControlStateNormal];
        [pBtnSaveLocal setImage:[UIImage imageNamed:@"share_save1_press"] forState:UIControlStateHighlighted];
        [pBtnSaveLocal setImage:[UIImage imageNamed:@"share_save1_no"] forState:UIControlStateDisabled];
        [pBtnSaveLocal addTarget:self action:@selector(eventSaveVideo:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:pBtnSaveLocal];
        //关闭
        t_view_px = TRANS_BY_WIDTH(25,t_w);
        t_view_py = TRANS_BY_WIDTH(25,t_w);
        t_view_w = TRANS_BY_WIDTH(85,t_w);
        t_view_h = TRANS_BY_WIDTH(85,t_w);
        pBtnClose = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [pBtnClose setImage:[UIImage imageNamed:@"common_close"] forState:UIControlStateNormal];
        [pBtnClose setImage:[UIImage imageNamed:@"common_close_press"] forState:UIControlStateHighlighted];
        [pBtnClose addTarget:self action:@selector(eventCloseVideo:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:pBtnClose];
    }
    return self;
}

- (void)setShareVideo:(NSString*)_vio Img:(NSString*)_img Score:(int)_score{
    curShareType = SHAREVIDEO;
    m_vioLocal = _vio;
    m_imgLocal = _img;
    m_gameScore = _score;
}

- (void)setShareImage:(NSString*)_local{
    curShareType = SHAREIMAGE;
    m_imgLocal = _local;
}

//刷新数据
- (void)refreshData{
    if (curShareType == SHAREVIDEO) {
        pMediaPlayer.hidden = NO;
        pImageView.hidden = YES;
        pBtnUpLoad.enabled = YES;
//        [[SWBasicSys getInst].pSWMediaPlayer preparePlay:[NSURL URLWithString:m_vioLocal] Frame:CGRectMake(0, 0, pMediaPlayer.bounds.size.width, pMediaPlayer.bounds.size.height) View:pMediaPlayer];
    }else if (curShareType == SHAREIMAGE){
        pMediaPlayer.hidden = YES;
        pImageView.hidden = NO;
        pBtnUpLoad.enabled = NO;
        UIImage *image = [[UIImage alloc] initWithContentsOfFile:m_imgLocal];
        pImageView.image = image;
    }
    
    if ([self.m_whereFrom isEqualToString:@"SWMainUI"]) {
        
    }else if ([self.m_whereFrom isEqualToString:@"SWMagicsUI"]){
        pBtnAgain.hidden = YES;
        pBtnAgainMore.hidden = YES;
    }else{
        pBtnAgain.hidden = YES;
        pBtnAgainMore.hidden = YES;
        pBtnSaveLocal.hidden = YES;
    }
    
}

//切换UI
- (void)changeToShow{
}

-(void)shareLogon{
}

- (void)eventShareWX:(UIButton *)btn{
    NSLog(@"eventShareWX\n");
    if (curShareType == SHAREIMAGE) {
        if( [SWLogicSys getInst].pLogicUser.m_hasLogon ){
            //
            //走分享
            [[SWLogicSys getInst].pLogicPlatform shareImageToWX:m_imgLocal];
        }else{
            //走登录
            [self shareLogon];
        }
    }else if (curShareType == SHAREVIDEO){
        if( [SWLogicSys getInst].pLogicUser.m_hasLogon ){
            //
            //走分享
            [[SWLogicSys getInst].pLogicPlatform shareVideoToWXVideo:m_vioLocal Image:m_imgLocal Score:m_gameScore];
        }else{
            //走登录
            [self shareLogon];
        }
    }
}

- (void)eventShareWXTimeLine:(UIButton *)btn{
    if (curShareType == SHAREIMAGE) {
        if( [SWLogicSys getInst].pLogicUser.m_hasLogon ){
            //
            //走分享
            [[SWLogicSys getInst].pLogicPlatform shareImageToPYQ:m_imgLocal];
        }else{
            //走登录
            [self shareLogon];
        }
    }else if (curShareType == SHAREVIDEO){
        //微信视频分享(一个是用户录制视频 一个是app宣传视频)
        if( [SWLogicSys getInst].pLogicUser.m_hasLogon ){
            //走分享
            [[SWLogicSys getInst].pLogicPlatform shareVideoToPYQVideo:m_vioLocal Image:m_imgLocal Score:m_gameScore];
        }else{
            //走登录
            [self shareLogon];
        }
    }
}

- (void)eventShareWB:(UIButton *)btn{
    return;

    NSLog(@"eventShareWB\n");
    if (curShareType == SHAREIMAGE) {
        if( [SWLogicSys getInst].pLogicUser.m_hasLogon ){
            //
            //走分享
            [[SWLogicSys getInst].pLogicPlatform shareImageToWB:m_imgLocal];
        }else{
            //走登录
            [self shareLogon];
        }
    }else if (curShareType == SHAREVIDEO){
        if( [SWLogicSys getInst].pLogicUser.m_hasLogon ){
            //走分享
            [[SWLogicSys getInst].pLogicPlatform shareVideoToWBVideo:m_vioLocal Image:m_imgLocal Score:m_gameScore];
        }else{
            //走登录
            [self shareLogon];
        }
    }
}

- (void)eventShareQQ:(UIButton *)btn{
    NSLog(@"eventShareQQ\n");
    if (curShareType == SHAREIMAGE) {
        if( [SWLogicSys getInst].pLogicUser.m_hasLogon ){
            //
            //走分享
            [[SWLogicSys getInst].pLogicPlatform shareImageToQQ:m_imgLocal];
        }else{
            //走登录
            [self shareLogon];
        }
    }else if (curShareType == SHAREVIDEO){
        if( [SWLogicSys getInst].pLogicUser.m_hasLogon ){
            //走分享
            [[SWLogicSys getInst].pLogicPlatform shareVideoToQQVideo:m_vioLocal Image:m_imgLocal Score:m_gameScore];
        }else{
            //走登录
            [self shareLogon];
        }
    }
    
}

- (void)eventShareJM:(UIButton *)btn{
    if (curShareType == SHAREVIDEO){
        if( [SWLogicSys getInst].pLogicUser.m_hasLogon ){
            //走分享
            [[SWLogicSys getInst].pLogicPlatform shareVideoToJMVideo:m_vioLocal Image:m_imgLocal Score:m_gameScore];
        }else{
            //走登录
            [self shareLogon];
        }
    }
}

- (void)eventSaveVideo:(UIButton *)btn{
    NSLog(@"eventSaveVideo\n");
    if (curShareType == SHAREIMAGE) {
//        [[SWBasicSys getInst].pSWRecord saveImageToAssetsLibrary];
    }else if (curShareType == SHAREVIDEO){
//        [[SWBasicSys getInst].pSWRecord saveVideoToAssetsLibrary];
    }
}

- (void)eventCloseVideo:(UIButton *)btn{
    NSLog(@"share ui eventCloseVideo\n");
}

- (void)eventAgainMore:(UIButton *)btn{
//    [[SWBasicSys getInst].pSWMediaPlayer close];
}

- (void)eventAgain:(UIButton *)btn{
}

- (void)eventPrint:(UIButton *)btn{
//    NSURL *imageURL = [[NSURL alloc] initFileURLWithPath:[NSString pathWithComponents:@[NSTemporaryDirectory(), @"fm2Image.jpg"]]];
//    UIImage *image = [[UIImage alloc] initWithContentsOfFile:[imageURL path]];
//    UIPrintInteractionController *printC = [UIPrintInteractionController sharedPrintController];
//    UIPrintInfo *printInfo = [NSClassFromString(@"UIprintInfo") printInfo ];
//    printInfo.duplex= UIPrintInfoDuplexLongEdge;
//    NSData * data =UIImageJPEGRepresentation(image, 1);
//    if (printC && [UIPrintInteractionController canPrintData:data]) {
//
//        printC.printFormatter.contentInsets = UIEdgeInsetsMake(10, 10, 10, 10);
//        UIPrintInfo *printInfo = [UIPrintInfo printInfo];//准备打印信息以预设值初始化的对象。
//        printInfo.outputType = UIPrintInfoOutputPhoto;//设置输出类型。
//        printC.showsPageRange = YES;//显示的页面范围
//
//        //printInfo.jobName = @"my.job";
//        printC.printInfo = printInfo;
//        printC.printingItem = data;//single NSData, NSURL, UIImage, ALAsset
//
//        // 等待完成
//        void (^completionHandler)(UIPrintInteractionController *, BOOL, NSError *) =
//        ^(UIPrintInteractionController *printController, BOOL completed, NSError *error) {
//            if (!completed && error) {
//                NSLog(@"可能无法完成，因为印刷错误: %@", error);
//            }
//        };
//        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//            //            UIBarButtonItem *item = [[UIBarButtonItem alloc] initWithCustomView:sender];//调用方法的时候，要注意参数的类型－下面presentFromBarButtonItem:的参数类型是 UIBarButtonItem..如果你是在系统的UIToolbar or UINavigationItem上放的一个打印button，就不需要转换了。
//            //            [printC presentFromBarButtonItem:item animated:YES completionHandler:completionHandler];//在ipad上弹出打印那个页面
//        } else {
//            [printC presentAnimated:YES completionHandler:completionHandler];//在iPhone上弹出打印那个页面
//        }
//    }
}

#pragma mark --- fmrecorddelegate
- (void)saveVideoToAssetsResult:(NSError *)error{
//    if (!error) {
//        pBtnSaveLocal.enabled = false;
//        [self makeToast:@"存储成功" duration:1.0 position:CSToastPositionCenter];
//    }else{
//        [self makeToast:@"存储失败" duration:1.0 position:CSToastPositionCenter];
//    }
}

- (void)saveImageToAssetsResult:(NSError *)error{
//    if (!error) {
//        pBtnSaveLocal.enabled = false;
//        [self makeToast:@"存储成功" duration:1.0 position:CSToastPositionCenter];
//    }else{
//        [self makeToast:@"存储失败" duration:1.0 position:CSToastPositionCenter];
//    }
}

@end
