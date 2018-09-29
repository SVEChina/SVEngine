//
//  SWClauseUI.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "SWClauseUI.h"
#import "../utility/Utility.h"
@interface SWClauseUI(){
    UITextView *m_pTextView;
}
@end

@implementation SWClauseUI
+ (SWClauseUI *)createWithFrame:(CGRect)frame{
    return [[SWClauseUI alloc] initWithFrame:frame];
}

- (instancetype)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor = [UIColor whiteColor];
        m_pTextView = [[UITextView alloc] initWithFrame:CGRectMake(10, 10, frame.size.width-20, frame.size.height)];
        m_pTextView.font = [UIFont systemFontOfSize:15];
        m_pTextView.text = @"1、 用户不得在啾咪制作、复制、发布、传播含有下列内容的信息：\n（一）反对宪法所确定的基本原则的；\n（二）危害国家安全，泄露国家秘密，颠覆国家政权，破坏国家统一的；\n（三）损害国家荣誉和利益的；\n（四）煽动民族仇恨、民族歧视，破坏民族团结的；\n（五）破坏国家宗教政策，宣扬邪教和封建迷信的；\n（六）散布谣言，扰乱社会秩序，破坏社会稳定的；\n（七）散布淫秽、色情、赌博、暴力、凶杀、恐怖或者教唆犯罪的；\n（八）侮辱或者诽谤他人，侵害他人合法权益的；\n（九）不遵守法律法规底线、社会主义制度底线、国家利益底线、公民合法权益底线、社会公共秩序底线、道德风尚底线和信息真实性底线的“七条底线”要求的；\n（十）含有法律、行政法规禁止的其他内容的信息。\n2、用户不得利用啾咪帐号或本服务制作、上载、复制、发布、传播如下干扰啾咪正常运营，以及侵犯其他用户或第三方合法权益的内容：\n（一）含有任何性或性暗示的；\n（二）含有辱骂、恐吓、威胁内容的；\n（三）含有骚扰、垃圾广告、恶意信息、诱骗信息的；\n（四）涉及他人隐私、个人信息或资料的；\n（五）侵害他人名誉权、肖像权、知识产权、商业秘密等合法权利的；\n（六）含有其他干扰本服务正常运营和侵犯其他用户或第三方合法权益内容的信息。\n";
        [m_pTextView setEditable:NO];
        [self addSubview:m_pTextView];
    }
    return self;
}

- (void)changeToShow{
    UIViewController *targetVC = [Utility findViewController:self];
    targetVC.title = @"服务条款";

}
/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

@end
