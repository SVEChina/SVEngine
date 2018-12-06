//
//  PLAlbumContensCell.m
//  Philm
//
//  Created by liuming on 16/12/8.
//  Copyright © 2016年 yoyo. All rights reserved.
//

#import "PLAlbumContensCell.h"
#import "SDAutoLayout/SDAutoLayout.h"
#import "PLCreateUITool.h"
#import "QYPhotoService.h"

#define NX_UIColorFromRGB(rgbValue)                                      \
    [UIColor colorWithRed:((float)((rgbValue & 0xFF0000) >> 16)) / 255.0 \
                    green:((float)((rgbValue & 0xFF00) >> 8)) / 255.0    \
                     blue:((float)(rgbValue & 0xFF)) / 255.0             \
                    alpha:1.0]

@interface PLAlbumContensCell ()

@property(nonatomic, strong) UIImageView *covertImageView;
@property(nonatomic, strong) UILabel *titleLabel;
@property(nonatomic, strong) UILabel *countLabel;
@property(nonatomic, strong) UIButton *accessoryBtn;

@end
@implementation PLAlbumContensCell

- (instancetype)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];

    if (self)
    {
        [self initSubView];
    }

    return self;
}

- (void)initSubView
{
    //封面图片
    self.covertImageView = [UIImageView new];
    self.covertImageView.contentMode = UIViewContentModeScaleAspectFill;
    self.covertImageView.clipsToBounds = YES;
    [self.contentView addSubview:self.covertImageView];
    self.covertImageView.sd_layout.leftSpaceToView(self.contentView, 12)
        .topSpaceToView(self.contentView, 8)
        .widthIs(60)
        .heightIs(60);

    //标题
    self.titleLabel = [PLCreateUITool createLabelInitWithFrame:CGRectZero
                                               backgroundColor:[UIColor whiteColor]
                                                 textAlignment:NSTextAlignmentLeft
                                                          text:@""
                                                     textColor:NX_UIColorFromRGB(0x030303)
                                                          font:[UIFont systemFontOfSize:14]
                                                     superView:self.contentView];
    self.titleLabel.sd_layout.leftSpaceToView(self.covertImageView, 14)
        .topSpaceToView(self.contentView, 23)
        .widthIs(300)
        .heightIs(20);

    self.countLabel = [PLCreateUITool createLabelInitWithFrame:CGRectZero
                                               backgroundColor:[UIColor whiteColor]
                                                 textAlignment:NSTextAlignmentLeft
                                                          text:@""
                                                     textColor:NX_UIColorFromRGB(0xd8d8d8)
                                                          font:[UIFont systemFontOfSize:12]
                                                     superView:self.contentView];
    self.countLabel.sd_layout.leftEqualToView(self.titleLabel)
        .topSpaceToView(self.titleLabel, 6)
        .heightIs(12)
        .widthIs(100);

    NSString *imagePath = [[NSBundle mainBundle] pathForResource:@"accessory_arrow@2x" ofType:@"png"];
    NSString *hImagePath = [[NSBundle mainBundle] pathForResource:@"accessory_arrow_pressed@2x" ofType:@"png"];

    UIImage *image = [UIImage imageWithContentsOfFile:imagePath];
    UIImage *hImage = [UIImage imageWithContentsOfFile:hImagePath];

    self.accessoryBtn = [UIButton new];
    [self.accessoryBtn setImage:image forState:UIControlStateNormal];
    [self.accessoryBtn setImage:hImage forState:UIControlStateHighlighted];
    [self.accessoryBtn setImage:hImage forState:UIControlStateSelected];
    [self.accessoryBtn addTarget:self
                          action:@selector(accessoryBtnClicked:)
                forControlEvents:UIControlEventTouchUpInside];
    [self.contentView addSubview:self.accessoryBtn];
    self.accessoryBtn.sd_layout.centerYEqualToView(self.contentView)
        .rightSpaceToView(self.contentView, 0)
        .widthIs(44)
        .heightIs(44);
}
- (void)setContentModel:(QYGroupModel *)model
{
    QYAssetModel *coverAsset = [model.asstArray lastObject];
    [[QYPhotoService shareInstanced] requestImageForAsset:coverAsset
                                                     size:CGSizeMake(240, 240)
                                                  success:^(UIImage * _Nullable image) {
         self.covertImageView.image = image;
        
    }
                                                  failure:nil
                                            progressBlock:nil];
    self.titleLabel.text = model.collection.localizedTitle;
    self.countLabel.text = [NSString stringWithFormat:@"%lu", (unsigned long)model.count];
}

- (void)accessoryBtnClicked:(UIButton *)sender
{
    if (self.block)
    {
        self.block(self.indexPath);
    }
}

- (void)setHighlighted:(BOOL)highlighted animated:(BOOL)animated
{
    [super setHighlighted:highlighted animated:animated];
    // self.accessoryBtn.selected = highlighted ? YES : NO;
}
- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];
    // Configure the view for the selected state
}

@end
