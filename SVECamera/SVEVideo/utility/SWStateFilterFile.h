
//
//  SWStateFilterSave.h
//  SVEVideo
//
//  Created by 徐子昱 on 2018/7/20.
//  Copyright © 2018年 付一洲. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "SWState.h"

@interface SWStateFilterFile : NSObject

+(void) saveSate:(SWState *) _state fileName:(NSString* )_nameFile;

+(void) loadSate:(SWState *) _state fileName:(NSString* )_nameFile;

@end
