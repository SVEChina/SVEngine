
//
//  SVIAudio.m
//  SVEngine
//
//  Created by 李晓帆 on 2018/9/7.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#import "SVIAudio.h"
#if TARGET_OS_IPHONE
//#import <AudioToolbox/AudioToolbox.h>
#import <AVFoundation/AVFoundation.h>
@interface SVIAudio (){
    NSMutableDictionary *m_soundsDic;
}
@end

@implementation SVIAudio
- (instancetype)init{
    self = [super init];
    if (self) {
        m_soundsDic = [[NSMutableDictionary alloc] init];
    }
    return self;
}

- (void)loadSound:(NSString *)_path{
    /*
    NSURL *_url = [NSURL URLWithString:_path];
    SystemSoundID soundID = 0;
    AudioServicesCreateSystemSoundID((__bridge CFURLRef)(_url), &soundID);
    [m_soundsDic setObject:[NSNumber numberWithInteger:soundID] forKey:[_path lastPathComponent]];
     */
    NSString *key = [_path lastPathComponent];
    if ([m_soundsDic objectForKey:key]) {
        return;
    }
    NSURL *url=[NSURL fileURLWithPath:_path];
    NSError *error=nil;
    //初始化播放器，注意这里的Url参数只能时文件路径，不支持HTTP Url
    AVAudioPlayer *audioPlayer=[[AVAudioPlayer alloc]initWithContentsOfURL:url error:&error];
    if (audioPlayer) {
        //设置播放器属性
        audioPlayer.numberOfLoops=0;//设置为0不循环
        //audioPlayer.delegate=self;
        [audioPlayer prepareToPlay];//加载音频文件到缓存
        
        [m_soundsDic setObject:audioPlayer forKey:key];
    }

}

- (void)playSound:(NSString *)_key{
    /*
    NSUInteger soundID = [[m_soundsDic objectForKey:_key] integerValue];
    AudioServicesPlaySystemSound((SystemSoundID)soundID);
     */
    AVAudioPlayer *audioPlayer = [m_soundsDic objectForKey:_key];
    if (audioPlayer) {
        if ([audioPlayer isPlaying]) {
            audioPlayer.currentTime = 0;
        }else{
            [audioPlayer play];
        }
    }
}

- (void)removeAllSounds{
    [m_soundsDic removeAllObjects];
}

- (void)dealloc{
    [self removeAllSounds];
    /*
    for (int i = 0; i<m_soundsDic.allValues.count; i++) {
        NSInteger soundID = m_soundsDic.allValues[i];
        AudioServicesDisposeSystemSoundID(soundID);
    }
    */
}
@end
#endif
