//
//  QYImagePreVC.m
//  QYPhotoAlbum
//
//  Created by liuming on 2017/11/1.
//  Copyright © 2017年 burning. All rights reserved.
//

#import "QYImagePreVC.h"
#import "QYPhotoService.h"
@interface QYImagePreVC ()
@property(nonatomic, strong) UIImageView *imageView;
@end

@implementation QYImagePreVC

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.view.backgroundColor = [UIColor whiteColor];
    self.imageView = [[UIImageView alloc] initWithFrame:self.view.bounds];
    self.imageView.contentMode = UIViewContentModeScaleAspectFit;
    [self.view addSubview:self.imageView];

    [[QYPhotoService shareInstanced] requestOriginalImageForAsset:self.assetModel
                                                          success:^(UIImage * _Nullable image)
    {
        
        self.imageView.image = image;
    }
                                                          failure:nil
                                                    progressBlock:nil];
}

-(NSArray<id<UIPreviewActionItem>> *)previewActionItems
{
    UIPreviewAction * action1 = [UIPreviewAction actionWithTitle:@"收藏" style:1 handler:^(UIPreviewAction * _Nonnull action, UIViewController * _Nonnull previewViewController) {
        NSLog(@"收藏");
    }];
    
    UIPreviewAction * action2 = [UIPreviewAction actionWithTitle:@"喜欢" style:0 handler:^(UIPreviewAction * _Nonnull action, UIViewController * _Nonnull previewViewController) {
        NSLog(@"喜欢");
        
    }];
    NSArray *items = @[action1,action2];
    return items;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
