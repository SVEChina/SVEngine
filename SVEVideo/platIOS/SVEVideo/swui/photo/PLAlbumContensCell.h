//
//  PLAlbumContensCell.h
//  Philm
//
//  Created by liuming on 16/12/8.
//  Copyright © 2016年 yoyo. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "QYGroupModel.h"

typedef void (^accessoryClickedBlock)(NSIndexPath *indexPath);
@interface PLAlbumContensCell : UITableViewCell

@property(nonatomic, copy) accessoryClickedBlock block;

@property(nonatomic, strong) NSIndexPath *indexPath;
- (void)setContentModel:(QYGroupModel *)mdoel;

@end
