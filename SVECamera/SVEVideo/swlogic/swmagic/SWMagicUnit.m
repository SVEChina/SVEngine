//
//  SWMagicUnit.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWMagicUnit.h"
#import "SWLogicSys.h"
@implementation SWMagicUnit
- (instancetype)init{
    self = [super init];
    if (self) {
        _categoryID = @"";
        _ctime = @"";
        _cutime = @"";
        _magicEffectID = @"";
        _mark = @"";
        _downloadNum = @"";
        _ownerUid = @"";
        _score = @"";
        _status = @"";
        _sutime = @"";
        _categoryName = @"";
        _cover = @"";
        _name = @"";
        _source = @"";
        _tags = @"";
        _isDownload = @"0";
        _price = @"";
    }
    return self;
}

- (void)dealloc{
    NSLog(@"destory SWMagicUnit");
}

- (void)refreshData:(NSDictionary *)dic{
    if (dic) {
        _categoryID = dic[@"categoryID"];
        _ctime = dic[@"ctime"];
        _cutime = dic[@"cutime"];
        _magicEffectID = dic[@"id"];
        _mark = dic[@"mark"];
        _downloadNum = dic[@"downloadNum"];
        _ownerUid = dic[@"ownerUid"];
        _score = dic[@"score"];
        _status = dic[@"status"];
        _sutime = dic[@"sutime"];
        _categoryName = dic[@"categoryName"];
        _cover = dic[@"cover"];
        _name = dic[@"name"];
        _source = dic[@"source"];
        _tags = dic[@"tags"];
        _isDownload = dic[@"isDownLoad"];
        _price = @"10";
    }
}

- (NSDictionary *)convertUnitToDictionary{
    NSDictionary *spineDic = @{@"categoryID":_categoryID,
                               @"ctime":_ctime,
                               @"cutime":_cutime,
                               @"id":_magicEffectID,
                               @"mark":_mark,
                               @"downloadNum":_downloadNum,
                               @"ownerUid":_ownerUid,
                               @"score":_score,
                               @"status":_status,
                               @"sutime":_sutime,
                               @"categoryName":_categoryName,
                               @"cover":_cover,
                               @"name":_name,
                               @"source":_source,
                               @"tags":_tags,
                               @"isDownLoad":_isDownload,
                               @"price":_price
                               };
    return spineDic;
}

@end
