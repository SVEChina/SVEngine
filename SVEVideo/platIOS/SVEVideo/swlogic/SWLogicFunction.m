//
//  SWLogicFunction.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWLogicFunction.h"
#import "SWFunction.h"
#import "../swui/SWUISys.h"
#import "../swbasic/SWBasicSys.h"
#import "../swlogic/SWLogicSys.h"
#import "../utility/SWFileUtil.h"
#import "../utility/Utility.h"

//功能模块
@interface SWLogicFunction (){
    //功能列表
    NSMutableArray* m_FunctionLib;
    NSMutableArray* m_FunctionLibSel;
}
@end

@implementation SWLogicFunction

- (void)initModule{
    NSArray* function_name_array = [NSArray arrayWithObjects:@"磨皮",
                                  @"美白",
                                  @"亮度",
                                  @"对比度",
                                  @"饱和度",
                                  @"颜色",
                                  @"高光",
                                  @"阴影",
                                  @"色温",
                                  @"曲线",
                                  @"锐度",
                                  @"GAMMA",
                                  @"大眼",
                                  @"瘦脸",
                                  @"曝光",
                                  @"色彩平衡",
                                  @"重置",
                                  @"保存",
                                    nil];
    NSArray* function_img_array = [NSArray arrayWithObjects:@"m_mopi",
                                   @"m_meibai",
                                   @"m_liangdu",
                                   @"m_duibidu",
                                   @"m_baohedu",
                                   @"m_yanse",
                                   @"m_gaoguang",
                                   @"m_yinying",
                                   @"m_sewen",
                                   @"m_quxian",
                                   @"m_ruidu",
                                   @"m_gamma",
                                   @"m_dayan",
                                   @"m_shoulian",
                                   @"m_baoguang",
                                   @"m_colorbalance",
                                   @"m_chongzhi",
                                   @"m_baocun",
                                   nil];
    //初始化滤镜列表
    m_FunctionLib = [[NSMutableArray alloc] init];
    if( function_name_array.count == function_img_array.count ) {
        for( int i=0; i<function_name_array.count; i++ ) {
            SWFunction* t_function = [[SWFunction alloc] init];
            t_function.m_image = function_img_array[i];
            t_function.m_name = function_name_array[i];
            t_function.m_enable = true;
            t_function.m_owner = @"sve";
            [m_FunctionLib addObject:t_function];
        }
    }
    //选择库(这个其实要来自于配置文件的)
    m_FunctionLibSel = [[NSMutableArray alloc] init];
    for( int i=0; i<m_FunctionLib.count; i++ ) {
        SWFunction* t_function = m_FunctionLib[i];
        if(t_function.m_enable ) {
            [m_FunctionLibSel addObject:t_function];
        }
    }
}

- (void)destroyModule{
    m_FunctionLibSel = nil;
    m_FunctionLib = nil;
}

//获取功能库
- (NSMutableArray *)getFunctionLib {
    return m_FunctionLib;
}

//获取选中的功能库
- (NSMutableArray *)getFunctionLibSel {
    return m_FunctionLibSel;
}

//开启功能
-(void)enableFunction:(NSString*)_name {
   
   
    
}

//关闭功能
-(void)disableFunction:(NSString*)_name {
    
}

@end
