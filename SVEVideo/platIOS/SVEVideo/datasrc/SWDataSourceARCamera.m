//
//  SWDataSourceARCamera.m
//  SVEVideo
//
//  Created by 李晓帆 on 2018/7/5.
//  Copyright © 2018年 付一洲. All rights reserved.
//

#import "SWDataSourceARCamera.h"
#import "../media/SWARCamera.h"
#import "../swbasic/SWBasicSys.h"
#import "../swlogic/SWLogicSys.h"
@interface SWDataSourceARCamera()<ARCameraDelegate>{
    SWARCamera *m_arCamera;
}
@end

@implementation SWDataSourceARCamera
- (instancetype)init{
    self = [super init];
    if (self) {
        m_arCamera = [[SWARCamera alloc] init];
        m_arCamera.delegate = self;
        [m_arCamera setupCamera];
    }
    return self;
}


- (void)initModule{
    [super initModule];
}

- (void)destroyModule{
    [super destroyModule];
}

- (void)start{
    if(m_arCamera){
        [m_arCamera runCamera];
    }
}

- (void)stop{
    if(m_arCamera){
        [m_arCamera pauseCamera];
    }
}

- (void)hitPoint:(CGPoint)_point{
    ARSession *session = [m_arCamera getSession];
    NSArray<ARHitTestResult *> *result = [session.currentFrame hitTest:_point types:ARHitTestResultTypeFeaturePoint];
    ARHitTestResult * hitResult = [result firstObject];
    float x = hitResult.worldTransform.columns[3].x;
    float y = hitResult.worldTransform.columns[3].y;
    float z = hitResult.worldTransform.columns[3].z;
//    [[SWLogicSys getInst].pSVI pushAnchorPointX:x Y:y Z:z];
    int a = 0;
    
}

- (void)session:(ARSession *)session updateFrame:(ARFrame *)frame{
    // 同步YUV信息到 yTexture 和 uvTexture
    CVPixelBufferRef pixelBuffer = frame.capturedImage;
    OSType pixelFormat = CVPixelBufferGetPixelFormatType(pixelBuffer);
    GLsizei imageWidth = (GLsizei)CVPixelBufferGetWidthOfPlane(pixelBuffer, 0);
    GLsizei imageHeight = (GLsizei)CVPixelBufferGetHeightOfPlane(pixelBuffer, 0);
//    [[SWLogicSys getInst].pSVI.pCamera pushInStream:SCENENAME PixelBuffer:pixelBuffer];
    
    // 同步摄像机
    matrix_float4x4 oldCameraMatrix = [frame.camera transform];
    matrix_float4x4 cameraMatrix = matrix_invert(oldCameraMatrix);
    matrix_float4x4 newCameraMatrix = cameraMatrix;
    GLKMatrix4 newCameraMat;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            newCameraMat.m[row * 4 + col] = newCameraMatrix.columns[row][col];
        }
//        printf("camera matrix %f %f %f %f\n",newCameraMat.m[row*4 + 0], newCameraMat.m[row*4 + 1], newCameraMat.m[row*4 + 2], newCameraMat.m[row*4 + 3]);
    }
    
    GLKMatrix4 cameraRotMatrix = newCameraMat;
    GLKVector3 forward = GLKVector3Make(-cameraRotMatrix.m13, -cameraRotMatrix.m23, -cameraRotMatrix.m33);
    
    GLKMatrix4 rotationMatrix = GLKMatrix4MakeRotation(M_PI / 2, forward.x, forward.y, forward.z);
    cameraRotMatrix = GLKMatrix4Multiply(rotationMatrix, newCameraMat);
    
    cameraRotMatrix.m[12] = cameraRotMatrix.m[12]*150;
    cameraRotMatrix.m[13] = cameraRotMatrix.m[13]*150;
    cameraRotMatrix.m[14] = cameraRotMatrix.m[14]*150;
    
//    [[SWLogicSys getInst].pSVI pushCameraMatrix:(&cameraRotMatrix.m[0]) len:16*sizeof(float)];
    
    
    
    matrix_float4x4 projectionMatrix = [frame.camera projectionMatrixForOrientation:UIInterfaceOrientationPortrait viewportSize:CGSizeMake(720, 1280) zNear:0.1 zFar:1000];
    GLKMatrix4 newWorldProjectionMatrix = GLKMatrix4Identity;
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            newWorldProjectionMatrix.m[col * 4 + row] = projectionMatrix.columns[col][row];
        }
    }
//    [[SWLogicSys getInst].pSVI pushCameraMatrix:&(newWorldProjectionMatrix.m[0]) len:16*sizeof(float)];
}

void YUV420spRotate90(char *des, char *src,int width,int height)
{
    int wh = width * height;
    //旋转Y
    int k = 0;
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++)
        {
            des[k] = src[width * j + i];
            k++;
        }
    }
    
    for(int i = 0; i < width; i += 2) {
        for(int j = 0; j < height / 2; j++)
        {
            des[k] = src[wh+ width * j + i];
            des[k+1] = src[wh + width * j + i + 1];
            k+=2;
        }
    }
}

- (void)dealloc{

}


@end
