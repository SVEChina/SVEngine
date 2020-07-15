//
//  SVHead.h
//
//  Created by xiaofan Li on 2017/6/7.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SVIDef.h"

#if TARGET_OS_IPHONE

#import <GLKit/GLKit.h>
typedef void (*cb_func_percent)(float percent);
typedef void (*cb_func_record)(int _frame);
typedef void (*cb_func_op)(const char* _info, void *_obj);
typedef void (*cb_func_game)(int _type,const char* _info);
//输出回调
typedef void (*cb_out_stream)(int _w,int _h,int _formate,void* _pdata,long _timtap);

#endif
