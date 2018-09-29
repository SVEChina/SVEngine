#import "SWNavigationController.h"

@interface SWNavigationController ()

@end

@implementation SWNavigationController

- (instancetype)initWithRootViewController:(UIViewController *)rootViewController {
    self = [super initWithRootViewController:rootViewController];
    if(self) {
        [self hide];
    }
    return self;
}

-(UIStatusBarStyle)preferredStatusBarStyle{
    return UIStatusBarStyleLightContent;
}

-(void)hide {
    [self.navigationBar setHidden:true];
    [self.toolbar setHidden:true];
}

-(void)show {
    [self.navigationBar setHidden:false];
    [self.toolbar setHidden:false];
}

@end
