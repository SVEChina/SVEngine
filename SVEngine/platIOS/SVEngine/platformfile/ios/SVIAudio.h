//
//  SVIAudio.h
//  SVEngine
//
//  Created by 李晓帆 on 2018/9/7.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#import "SVInterfaceBase.h"
#if TARGET_OS_IPHONE
@interface SVIAudio : SVInterfaceBase
- (void)loadSound:(NSString *)_path;
- (void)playSound:(NSString *)_key;
- (void)removeAllSounds;
@end
#endif
