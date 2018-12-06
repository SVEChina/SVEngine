//
//  SWOutStream.h
//  SVEVideo
//
//  Created by 李晓帆 on 2018/7/5.
//  Copyright © 2018年 付一洲. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "../media/RecordImage.h"
#import "../media/RecordVideo.h"
@interface SWOutStream : NSObject
@property(assign, atomic)bool m_outStreaming;
@property(strong, nonatomic)RecordImage *m_pRecordImg;
@property(strong, nonatomic)RecordVideo *m_pRecordVideo;
@property(strong, nonatomic)UIImage *m_image;
+ (SWOutStream *)createOutStream:(SWSTREAMTYPE)type;
- (void)initModule;
- (void)destroyModule;
- (void)setOutPutBuffer:(void *)buffer;
- (void)openStream;
- (void)closeStream;
@end
