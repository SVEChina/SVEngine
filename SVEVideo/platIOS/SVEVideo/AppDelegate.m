//
//  ViewController.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "AppDelegate.h"
#import "SWBasicSys.h"
#import "SWLogicSys.h"
#import "SWUISys.h"
#import "swui/SWNavigationController.h"
#import "swui/SWMainVC.h"
#import "SWIntroduceViewController.h"
#import "swui/SWStartupUI.h"

@interface AppDelegate ()<SWIntroduceViewControllerDelegate>

@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    [self initAllSys];
    application.statusBarStyle = UIStatusBarStyleLightContent;
    
    return YES;
}

- (void)initAllSys{
    //初始化基础系统
    [[SWBasicSys getInst] initSys];
    //初始化逻辑系统
    [[SWLogicSys getInst] initSys];
    //初始化界面系统
    [[SWUISys getInst] initSys:[[UIScreen mainScreen] bounds]];
    /********************************************************/
    //
    [self.window makeKeyAndVisible];
    UIColor * tintColor = [UIColor colorWithRed:29.0/255.0
                                          green:173.0/255.0
                                           blue:234.0/255.0
                                          alpha:1.0];
    [self.window setTintColor:tintColor];
    //启动页
    SWStartupUI* t_startup = [[SWStartupUI alloc] init];
    [SWUISys getInst].pNavi = [[SWNavigationController alloc] initWithRootViewController:t_startup];
//    - (BOOL)prefersStatusBarHidden {
//        return YES;
//    }
//    //导航透明
//    float alpha = 0.5f;
//    UIImage *t_image = [self createImageWithColor:[UIColor colorWithRed:73/255.0f green:174/255.0f blue:175/255.0f alpha:alpha]];
//    [t_navigate.navigationBar setBackgroundImage:t_image forBarMetrics:UIBarMetricsDefault];
//    [t_navigate.navigationBar setShadowImage:t_image];
//    t_navigate.navigationBar.barStyle = UIBarStyleBlackOpaque;
//    //toolbar
//    [t_navigate.toolbar setTranslucent: YES];
//    [t_navigate.toolbar setTintColor: [UIColor redColor]];
//    [t_navigate.toolbar setBarStyle:UIBarStyleDefault];
    self.window.rootViewController = [SWUISys getInst].pNavi;
    //
    [t_startup gcdTimer];
}

- (UIImage *) createImageWithColor: (UIColor *) color{
    CGRect rect = CGRectMake(0.0f,0.0f,1.0f,1.0f);
    UIGraphicsBeginImageContext(rect.size);
    CGContextRef context =UIGraphicsGetCurrentContext();
    CGContextSetFillColorWithColor(context, [color CGColor]);
    CGContextFillRect(context, rect);
    UIImage *myImage =UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return myImage;
}


#pragma mark --- SWIntroduceViewControllerDelegate
- (void)introduceSkip{
    [self initAllSys];
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and invalidate graphics rendering callbacks. Games should use this method to pause the game.
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
    [[NSNotificationCenter defaultCenter] postNotificationName:@"didEnterBackground" object:nil];
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the active state; here you can undo many of the changes made on entering the background.
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    [[NSNotificationCenter defaultCenter] postNotificationName:@"didEnterForeground" object:nil];
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}


- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url options:(nonnull NSDictionary<NSString *,id> *)options
{
    NSString* path = [url absoluteString];
    if ([path containsString:@"file:///private"]) {
        path =  [path substringFromIndex:15];
    }
    NSString *copy_path = [[SWLogicSys getInst].m_airdropFilePath stringByAppendingPathComponent:[path lastPathComponent]];
    NSError *error;
    [[NSFileManager defaultManager] copyItemAtPath:path toPath:copy_path error:&error];
    [[NSNotificationCenter defaultCenter]postNotificationName:@"effectPath" object:path];
    return YES;
}

@end
