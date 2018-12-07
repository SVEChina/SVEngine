//
//  SWUIBezier.m
//  UIBezierPathDemo
//
//  Created by EL on 2017/6/22.
//  Copyright © 2017年 etouch. All rights reserved.
//

#import "SWUIBezier.h"
#import "../swlogic/SWLogicSys.h"
#import "SWFilterRGBW.h"
#include <vector>
#include <list>

using namespace std;

#define PI 3.14159265358979323846

struct BesierPoint{
    float x;                ///< 坐标点x轴的值
    float y;                ///< 坐标点y轴的值s
} ;

//截取
float bezier_clip(float value,float min,float max) {
    if(value<min)
        return min;
    if(value>max)
        return max;
    return value;
}

//spine算法
void createSecondDerivative(vector<BesierPoint> ctrlpoints ,vector<double> *outarray){
    int n = int(ctrlpoints.size());
    if (n <= 1) {
        return ;
    }
    double matrix[n][3];
    double result[n];
    matrix[0][1] = 1;
    // What about matrix[0][1] and matrix[0][0]? Assuming 0 for now (Brad L.)
    matrix[0][0] = 0;
    matrix[0][2] = 0;
    for (int i = 1; i < n - 1; i++) {
        BesierPoint P1 = ctrlpoints[i - 1];
        BesierPoint P2 = ctrlpoints[i];
        BesierPoint P3 = ctrlpoints[i + 1];
        matrix[i][0] = (double) (P2.x - P1.x) / 6;
        matrix[i][1] = (double) (P3.x - P1.x) / 3;
        matrix[i][2] = (double) (P3.x - P2.x) / 6;
        result[i] = (double) (P3.y - P2.y) / (P3.x - P2.x) - (double) (P2.y - P1.y) / (P2.x - P1.x);
    }
    // What about result[0] and result[n-1]? Assuming 0 for now (Brad L.)
    result[0] = 0;
    result[n - 1] = 0;
    matrix[n - 1][1] = 1;
    // What about matrix[n-1][0] and matrix[n-1][2]? For now, assuming they are 0 (Brad L.)
    matrix[n - 1][0] = 0;
    matrix[n - 1][2] = 0;
    // solving pass1 (up->down)
    for (int i = 1; i < n; i++) {
        double k = matrix[i][0] / matrix[i - 1][1];
        matrix[i][1] -= k * matrix[i - 1][2];
        matrix[i][0] = 0;
        result[i] -= k * result[i - 1];
    }
    // solving pass2 (down->up)
    for (int i = n - 2; i >= 0; i--) {
        double k = matrix[i][2] / matrix[i + 1][1];
        matrix[i][1] -= k * matrix[i + 1][0];
        matrix[i][2] = 0;
        result[i] -= k * result[i + 1];
    }
    for (int i = 0; i < n; i++)
        outarray->push_back(result[i] / matrix[i][1]);
}

void createSplineCurve2(vector<BesierPoint> ctrlpoints,vector<BesierPoint> *output ) {
    vector<double> sdA;
    createSecondDerivative(ctrlpoints,&sdA);
    // Is [ctrlpoints count] equal to [sdA count]?
    //    int n = [ctrlpoints count];
    int n = int(sdA.size());
    if (n < 1) {
        return ;
    }
    double sd[n] ;
    // From NSMutableArray to sd[n];
    for (int i = 0; i < n; i++) {
        sd[i] = sdA[i];
    }
    for (int i = 0; i < n - 1; i++) {
        BesierPoint cur = ctrlpoints[i];
        BesierPoint next = ctrlpoints[i + 1];
        for (int x = cur.x; x < next.x; x++) {
            double t = (double) (x - cur.x) / (next.x - cur.x);
            double a = 1 - t;
            double b = t;
            double h = next.x - cur.x;
            double y = a * cur.y + b * next.y + (h * h / 6) * ((a * a * a - a) * sd[i] + (b * b * b - b) * sd[i + 1]);
            if (y > 255.0) {
                y = 255.0;
            } else if (y < 0.0) {
                y = 0.0;
            }
            BesierPoint t_point;
            t_point.x=x;
            t_point.y=(int)round(y);
            output->push_back(t_point);
        }
    }
    // If the last point is (255, 255) it doesn't get added.
    if (output->size() == 255) {
        output->push_back(ctrlpoints[ctrlpoints.size() - 1]);
    }
}

void createSplineCurve(vector<BesierPoint>* ctrlpoints,vector<BesierPoint>* t_new_splinePoints) {
    if (ctrlpoints->size() <= 0) {
        return ;
    }
    //拉伸到0-255
    vector<BesierPoint> convertedPoints;
    for (int i = 0; i < ctrlpoints->size(); i++) {
        BesierPoint point = (*ctrlpoints)[i];
        BesierPoint point01;
        point01.x=(int) (point.x * 255);
        point01.y=(int) (point.y * 255);
        convertedPoints.push_back(point01);
    }
    //插值生成曲线
    vector<BesierPoint> splinePoints ;
    createSplineCurve2(convertedPoints,&splinePoints);
    //补点
    //推头部点
    if(splinePoints.size()>0) {
        BesierPoint firstSplinePoint = splinePoints[0];
        if (firstSplinePoint.x > 0) {
            for (int i = 0; i < firstSplinePoint.x; i++) {
                BesierPoint t_point;
                t_point.x = i;
                t_point.y = firstSplinePoint.y;
                t_new_splinePoints->push_back(t_point);
            }
        }
    }
    //推中间点
    for(int i=0;i<splinePoints.size();i++){
        t_new_splinePoints->push_back(splinePoints[i]);
    }
    //推尾部点
    if(splinePoints.size()>0) {
        BesierPoint lastSplinePoint = splinePoints[splinePoints.size() - 1];
        if (lastSplinePoint.x < 255) {
            for (int i = lastSplinePoint.x + 1; i <= 255; i++) {
                BesierPoint t_point;
                t_point.x = i;
                t_point.y = lastSplinePoint.y;
                t_new_splinePoints->push_back(t_point);
            }
        }
    }
}

//蒙版图层

@interface SWUIBezierMaskLayer : CALayer

//@property (strong, nonatomic) UIColor* lineColor;

@end


@implementation SWUIBezierMaskLayer{
    
}

@end

//贝塞尔UI

@implementation SWUIBezier{
    SWFilterRGBW* m_pLinkRGBW;
    //
    UIButton* m_clearPt;    //清理按钮
    UILabel* m_pCoord;      //坐标
    //
    UIBezierPath *_bezierLine;
    CAShapeLayer *_bezierLineLayer;
    float _border;
    CGRect _inRect;
    int _curCtrlIndex;  //当前控制点索引
    float _ctrlRadius;  //控制点范围
    float _ctrlDert;    //控制点的距离修正
    vector<CGPoint> _ptArray;   //控制点 这里面的坐标系是inFrame内部的 所以 在添加控制点的时候 都需要转换到inframe内部
    float _bezierData[256];
}

- (instancetype)initWithFrame:(CGRect)frame{
    if (self = [super initWithFrame:frame]) {
        int t_w = [[UIScreen mainScreen] bounds].size.width;
        int t_h = [[UIScreen mainScreen] bounds].size.height;
        int t_view_px,t_view_py,t_view_w,t_view_h;
        //
        memset(_bezierData,0,sizeof(float)*256);
        //
        self.lineColor = [UIColor whiteColor];
        m_pLinkRGBW = nullptr;
        _curCtrlIndex = -1; //当前控制点索引
        _ctrlRadius = 10;
        _ctrlDert = 2*_ctrlRadius;
        //曲线控件边
        _border = TRANS_BY_WIDTH(43,t_w);
        //曲线控件Frame(坐标系已经更改成Y轴朝上)
        _inRect.origin.x = _border;
        _inRect.origin.y = _border;
        _inRect.size.width = frame.size.width - _border*2;
        _inRect.size.height = frame.size.height - _border*2;
        //
        t_view_px = TRANS_BY_WIDTH(552,t_w);    //607-55
        t_view_py = TRANS_BY_WIDTH(575,t_w);    //1159 - 584
        t_view_w = TRANS_BY_WIDTH(50,t_w);
        t_view_h = TRANS_BY_WIDTH(50,t_w);
        m_clearPt = [[UIButton alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [m_clearPt setBackgroundColor:[UIColor clearColor]];
        [m_clearPt setImage:[UIImage imageNamed:@"m_cha"] forState:UIControlStateNormal];
        [m_clearPt setImage:[UIImage imageNamed:@"m_cha"] forState:UIControlStateSelected];
        [m_clearPt addTarget:self action:@selector(eventClearPt:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:m_clearPt];
        //坐标
        t_view_px = TRANS_BY_WIDTH(252,t_w);    //307-55
        t_view_py = TRANS_BY_WIDTH(575,t_w);    //1159 - 584
        t_view_w = TRANS_BY_WIDTH(149,t_w);
        t_view_h = TRANS_BY_WIDTH(43,t_w);
        m_pCoord = [[UILabel alloc]initWithFrame:CGRectMake(t_view_px, t_view_py, t_view_w, t_view_h)];
        [m_pCoord setBackgroundColor:[UIColor clearColor]];
        [m_pCoord setFont:[UIFont systemFontOfSize:14.0]];
        [m_pCoord setText:@"303,678"];
        [m_pCoord setTextAlignment:NSTextAlignmentCenter];
        [m_pCoord setTextColor:[UIColor lightGrayColor]];
        [self addSubview:m_pCoord];
        //
        _bezierLine = [UIBezierPath bezierPath];
        //
        _bezierLineLayer = [CAShapeLayer layer];
        _bezierLineLayer.strokeColor = [self.lineColor CGColor];
        _bezierLineLayer.fillColor = [[UIColor clearColor] CGColor];
        // 默认设置路径宽度为0，使其在起始状态下不显示
        _bezierLineLayer.lineWidth = 3;
        _bezierLineLayer.lineCap = kCALineCapRound;
        _bezierLineLayer.lineJoin = kCALineJoinRound;
        [self.layer addSublayer:_bezierLineLayer];
        //初始化路径
        [self initPath];
    }
    return self;
}

-(void)linkRGBW:(SWFilterRGBW*)_rgbw {
    m_pLinkRGBW = _rgbw;
}

-(void)active {
    [m_pCoord setHidden:FALSE];
    [m_clearPt setHidden:FALSE];
    [self setAlpha:1.0];
}

-(void)unactive {
    [m_pCoord setHidden:TRUE];
    [m_clearPt setHidden:TRUE];
    [self setAlpha:0.2];
}

-(float*)getBezierData {
    return _bezierData;
}

//初始化路径
- (void)initPath {
    //控制的点在(0，0) - (insize) 之间
    _ptArray.push_back(CGPointMake(0,0));
    _ptArray.push_back(CGPointMake(_inRect.size.width,_inRect.size.height));
}

- (void)drawRect:(CGRect)rect{
    [self drawBezierPath];
    [self drawCtrlPt];
}

- (void)drawBezierPath{
    //更新控制点(压缩到0-1)
    vector<BesierPoint> ctrlpoints;
    vector<BesierPoint> output_points;
    for (int i = 0; i < _ptArray.size(); i++) {
        CGPoint p1 = _ptArray[i];
        BesierPoint t_pt;
        t_pt.x = p1.x/_inRect.size.width;
        t_pt.y = p1.y/_inRect.size.height;
        ctrlpoints.push_back(t_pt);
    }
    //创建曲线 插值
    createSplineCurve(&ctrlpoints,&output_points);
    //输出点在转换到屏幕上进行渲染(插值坐标转换成渲染坐标)
    [_bezierLine removeAllPoints];
    if( output_points.size()!=256 )
        return ;
    for (int i = 0; i < output_points.size(); i++) {
        CGPoint lerp_p1 = CGPointMake(output_points[i].x, output_points[i].y);
        CGPoint draw_p1;
        draw_p1.x = _inRect.size.width * lerp_p1.x / 255.0f + _inRect.origin.x;
        draw_p1.y = self.frame.size.height - (_inRect.size.height * lerp_p1.y / 255.0f + _inRect.origin.y);
        if (i == 0) {
            [_bezierLine moveToPoint:draw_p1];
        }else {
            [_bezierLine addLineToPoint:draw_p1];
        }
    }
    //[_bezierLine closePath];
    /** 将折线添加到折线图层上，并设置相关的属性 */
    _bezierLineLayer.path = _bezierLine.CGPath;
    _bezierLineLayer.strokeColor = [self.lineColor CGColor];
    _bezierLineLayer.fillColor = [[UIColor clearColor] CGColor];
    // 默认设置路径宽度为0，使其在起始状态下不显示
    _bezierLineLayer.lineWidth = 1;
    _bezierLineLayer.lineCap = kCALineCapRound;
    _bezierLineLayer.lineJoin = kCALineJoinRound;
    //生成数据
    for(int i=0;i<output_points.size();i++){
        BesierPoint origPoint;
        origPoint.x=output_points[i].x;
        origPoint.y=output_points[i].x;
        float distance = (float)sqrt(pow((origPoint.x - output_points[i].x), 2.0) + pow((origPoint.y - output_points[i].y), 2.0));
        if (origPoint.y > output_points[i].y) {
            distance = -distance;
        }
        _bezierData[i] = distance;
    }
    //走合成数据的回调用
    if(m_pLinkRGBW) {
        [m_pLinkRGBW combineData];
    }
}

-(void)drawCtrlPt {
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetFillColorWithColor(context, [self.lineColor CGColor]);
    CGContextSetStrokeColorWithColor(context, [self.lineColor CGColor]);
    for(int i=0;i<_ptArray.size();i++) {
        float t_sc_x = _ptArray[i].x + _inRect.origin.x;
        float t_sc_y = self.frame.size.height - (_ptArray[i].y + _inRect.origin.y);
        if( _curCtrlIndex == i ) {
            CGContextAddArc(context, t_sc_x, t_sc_y, _ctrlRadius, 0, 2*PI, 0);
            CGContextDrawPath(context, kCGPathFill);
        }else {
            CGContextAddArc(context, t_sc_x, t_sc_y, _ctrlRadius, 0, 2*PI, 0);
            CGContextDrawPath(context, kCGPathStroke);
        }
    }
}

-(bool)selectPt:(CGPoint)point {
    //控件截取 修正控制点到固定区域
    point.y = self.frame.size.height - point.y;
    point.x = bezier_clip(point.x,_inRect.origin.x,_inRect.origin.x + _inRect.size.width) - _inRect.origin.x;
    point.y = bezier_clip(point.y,_inRect.origin.y,_inRect.origin.y + _inRect.size.height) - _inRect.origin.y;
    //
    for(int i=0;i<_ptArray.size();i++) {
        if( fabs(_ptArray[i].x - point.x) < _ctrlRadius ) {
            _curCtrlIndex = i;
            return true;
        }
    }
    return false;
}

//增加控制点
- (void)addCtrlPt:(CGPoint)point {
    //控件截取 修正控制点到固定区域
    point.y = self.frame.size.height - point.y;
    point.x = bezier_clip(point.x,_inRect.origin.x,_inRect.origin.x + _inRect.size.width) - _inRect.origin.x;
    point.y = bezier_clip(point.y,_inRect.origin.y,_inRect.origin.y + _inRect.size.height) - _inRect.origin.y;
    //
    int i = 0;
    vector<CGPoint>::iterator it = _ptArray.begin();
    while( it!=_ptArray.end() ) {
        CGPoint t_p = (*it);
        if(t_p.x>point.x) {
            _ptArray.insert(it, point);
            _curCtrlIndex = i;
            break;
        }
        i++;
        it++;
    }
}

//删除当前控制节点
-(void)delCtrlPt {
    //如果是第一个控制点 则不删除
    if(_curCtrlIndex > 0 && _ptArray.size() > 2 ) {
        int i = 0;
        vector<CGPoint>::iterator it = _ptArray.begin();
        while( it!=_ptArray.end() ) {
            if(i == _curCtrlIndex) {
                _ptArray.erase(it);
                //重新选取控制点
                if(_ptArray.size() <= 2) {
                    _curCtrlIndex = -1; //无控制点了(就剩首尾两个点了)
                    [self setNeedsDisplay];
                    return ;
                }
                //重新定位控制点
                if(_curCtrlIndex-1>0) {
                    _curCtrlIndex--;    //向前查找
                }
//                //
//                if(_curCtrlIndex == ) {
//                    
//                }
                [self setNeedsDisplay];
                return;
            }
            i++;
            it++;
        }
    }
}

- (void)moveCtrlPt:(CGPoint)point {
    if( _curCtrlIndex>=0 && _curCtrlIndex<_ptArray.size() && _ptArray.size()>0 ) {
        //控件截取 修正控制点到固定区域
        point.y = self.frame.size.height - point.y;
        point.x = bezier_clip(point.x,_inRect.origin.x,_inRect.origin.x + _inRect.size.width) - _inRect.origin.x;
        point.y = bezier_clip(point.y,_inRect.origin.y,_inRect.origin.y + _inRect.size.height) - _inRect.origin.y;
        //
        if( _curCtrlIndex == 0) {
            //第一个点(右侧截取)
            float t_block_x = _ptArray[_curCtrlIndex+1].x;
            if(t_block_x - point.x < _ctrlDert) {
                _ptArray[_curCtrlIndex].x = t_block_x - _ctrlDert;
            } else {
                _ptArray[_curCtrlIndex].x = point.x;
            }
            _ptArray[_curCtrlIndex].y = point.y;
        }else if(_curCtrlIndex == int(_ptArray.size() - 1) ){
            //最后一个点(左侧截取)
            float t_block_x = _ptArray[_curCtrlIndex-1].x;
            if( point.x - t_block_x < _ctrlDert) {
                _ptArray[_curCtrlIndex].x = t_block_x + _ctrlDert;
            } else {
                _ptArray[_curCtrlIndex].x = point.x;
            }
            _ptArray[_curCtrlIndex].y = point.y;
        }else {
            //双侧截取
            int t_preIndex = _curCtrlIndex - 1;
            int t_aftIndex = _curCtrlIndex + 1;
            //防止数据越界
            point.x = bezier_clip(point.x,_ptArray[t_preIndex].x,_ptArray[t_aftIndex].x);
            //数值修正
            float t_block_pre_x = _ptArray[t_preIndex].x;
            float t_block_after_x = _ptArray[t_aftIndex].x;
            if( point.x - t_block_pre_x < _ctrlDert ) {
                point.x = t_block_pre_x + _ctrlDert;
            } else if( t_block_after_x - point.x < _ctrlDert ) {
                point.x = t_block_after_x - _ctrlDert;
            }
            //
            _ptArray[_curCtrlIndex].x = point.x;
            _ptArray[_curCtrlIndex].y = point.y;
        }
        //
        int t_x = int(_ptArray[_curCtrlIndex].x/_inRect.size.width * 255);
        int t_y = int(_ptArray[_curCtrlIndex].y/_inRect.size.height * 255);
        NSString* t_tap =  [NSString stringWithFormat:@"%d：%d",t_x,t_y];
        [m_pCoord setText:t_tap];
    }
}


- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
    [super touchesBegan:touches withEvent:event];
    UITouch * pTouch = [touches anyObject];
    CGPoint point = [pTouch locationInView:self];
    NSLog(@"Bezier begin:%@",NSStringFromCGPoint(point));
    //
    if( [self selectPt:point] == false ) {
        [self addCtrlPt:point];
    }
    //
    [self setNeedsDisplay];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{
    [super touchesEnded:touches withEvent:event];
    UITouch * pTouch = [touches anyObject];
    CGPoint point = [pTouch locationInView:self];
    NSLog(@"Bezier End:%@",NSStringFromCGPoint(point));
    [self setNeedsDisplay];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event{
    [super touchesMoved:touches withEvent:event];
    UITouch * pTouch = [touches anyObject];
    CGPoint point = [pTouch locationInView:self];
    [self moveCtrlPt:point];
    NSLog(@"Bezier Move:%@",NSStringFromCGPoint(point));
    [self setNeedsDisplay];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event{
    [super touchesCancelled:touches withEvent:event];
}

//
- (void)eventClearPt:(UIButton *)btn{
    NSLog(@"eventClearPt\n");
    [self delCtrlPt];
}

@end
