//
//  SVCamera.m
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import "SVIGameRedPacket.h"
#import "SVInst.h"
#import "SVOpLoadBase.h"
#import "SVGlobalMgr.h"
#import "SVThreadMain.h"
#import "SVThreadPool.h"
#import "SVOpFilter.h"
#import "SVDataSwap.h"
#import "SVOpCreate.h"
#import "SVOpDestroy.h"
#import "SVIAudio.h"

#if TARGET_OS_IPHONE
void redpacket_op_callback(const char* _info) {
    printf("redpacket_op_callback %s\n", _info);
    NSDictionary *t_dic = @{@"operate":[NSString stringWithUTF8String:_info]};
    [[NSNotificationCenter defaultCenter] postNotificationName:@"sv_game_callback" object:nil userInfo:t_dic];
}

@interface SVIGameRedPacket(){
    SVIAudio *m_audio;
    int m_mode;
}
@end

@implementation SVIGameRedPacket
- (instancetype)init{
    self = [super init];
    if (self) {
        m_mode = 0;
        m_audio = [[SVIAudio alloc] init];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(sv_game_callback:) name:@"sv_game_callback" object:nil];
    }
    return self;
}

- (void)loadGameResource{
    NSString *t_path = [[NSBundle mainBundle] pathForResource:@"libao" ofType:@"bundle"];
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpLoadRedPacketPtr t_op = MakeSharedPtr<SVOpLoadRedPacket>(t_app,[t_path UTF8String]);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    //
    NSString *t_path0 = [t_path stringByAppendingPathComponent:@"321.wav"];
    [m_audio loadSound:t_path0];
    NSString *t_path1 = [t_path stringByAppendingPathComponent:@"ready2.wav"];
    [m_audio loadSound:t_path1];
    NSString *t_path2 = [t_path stringByAppendingPathComponent:@"ready.wav"];
    [m_audio loadSound:t_path2];
    NSString *t_path3 = [t_path stringByAppendingPathComponent:@"ready1.wav"];
    [m_audio loadSound:t_path3];
    NSString *t_path4 = [t_path stringByAppendingPathComponent:@"explosion.wav"];
    [m_audio loadSound:t_path4];
    NSString *t_path5 = [t_path stringByAppendingPathComponent:@"showgold.wav"];
    [m_audio loadSound:t_path5];
    NSString *t_path6 = [t_path stringByAppendingPathComponent:@"goldend.wav"];
    [m_audio loadSound:t_path6];
    NSString *t_path7 = [t_path stringByAppendingPathComponent:@"background.wav"];
    [m_audio loadSound:t_path7];
    NSString *t_path8 = [t_path stringByAppendingPathComponent:@"54321.wav"];
    [m_audio loadSound:t_path8];
}

- (void)startGame:(int)_packetNum Mode:(int)_mode msg:(NSString *)_msg{
    int t_packetNum = 100;
    if (_packetNum>0 && _packetNum<=100) {
        t_packetNum = _packetNum;
    }
    m_mode = _mode;
    NSString *t_path = [[NSBundle mainBundle] pathForResource:@"libao" ofType:@"bundle"];
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpCreateRedPacketPtr t_op = MakeSharedPtr<SVOpCreateRedPacket>(t_app,[t_path UTF8String], t_packetNum, _mode);
    t_op->setCallBack(redpacket_op_callback, [_msg UTF8String]);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    
}

- (void)stopGame{
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpDestroyRedPacketPtr t_op = MakeSharedPtr<SVOpDestroyRedPacket>(t_app);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
    [m_audio removeAllSounds];
}

- (void)openPacketMoney:(int)_value{
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpOpenRedPacketMoneyPtr t_op = MakeSharedPtr<SVOpOpenRedPacketMoney>(t_app, _value);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

- (void)setWalletPosX:(int)_x PosY:(int)_y{
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpWalletPosPtr t_op = MakeSharedPtr<SVOpWalletPos>(t_app, _x, _y);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

- (void)setTotalMoney:(NSString *)_text{
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpTotalMoneyPtr t_op = MakeSharedPtr<SVOpTotalMoney>(t_app,[_text UTF8String]);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

- (void)removeOnePacket:(int)_value{
    SVInst* t_app = (SVInst*)m_pApp;
    SVOpRemoveOnePacketPtr t_op = MakeSharedPtr<SVOpRemoveOnePacket>(t_app, -1);
    t_app->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

- (void)dealloc{
    [m_audio removeAllSounds];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"sv_game_callback" object:nil];
}
//测试代码
- (void) sv_game_callback:(id)sender{
    dispatch_async(dispatch_get_main_queue(), ^{
        NSString *name = [[sender userInfo] objectForKey:@"operate"];
        if ([name isKindOfClass:[NSString class]]) {
            if ([name isEqualToString:@"gamebegin"]){
                if (m_mode == 0) {
                    [m_audio playSound:@"54321.wav"];
                }else if (m_mode == 1){
                    [m_audio playSound:@"321.wav"];
                }
            }else if ([name isEqualToString:@"gameend"]){
                
            }else if([name isEqualToString:@"gameready"]){
                [m_audio playSound:@"ready.wav"];
            }else if([name isEqualToString:@"gameready1"]){
                [m_audio playSound:@"ready1.wav"];
            }else if([name isEqualToString:@"gameready2"]){
                [m_audio playSound:@"ready2.wav"];
            }else if ([name isEqualToString:@"candydown"]){
                [m_audio playSound:@"background.wav"];
            }else if([name isEqualToString:@"packageexplosion"]){
                [m_audio playSound:@"explosion.wav"];
            }else if ([name isEqualToString:@"packageshowgold"]){
                [m_audio playSound:@"showgold.wav"];
            }else if ([name isEqualToString:@"packagegoldend"]){
                [m_audio playSound:@"goldend.wav"];
            }
        }
        
    });
    
}
@end


#endif
