//
//  Detection.m
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//
#import "Detection.h"

#import <CommonCrypto/CommonDigest.h>
#import "../swlogic/SWLogicSys.h"
#import "st_mobile_license.h"
#import "st_mobile_common.h"
#import "st_mobile_human_action.h"
#import "st_mobile_face_attribute.h"


@interface Detection(){
  st_handle_t svDetector;  //human action句柄
  st_handle_t _hAttribute;  //人脸属性句柄
  unsigned char* detectData;
}
@property (nonatomic, assign) unsigned long long detectConfig;
@end
//st


@implementation Detection

- (instancetype)init {
    self = [super init];
    if (self) {

    }
    return self;
}
- (void)createSTEngine{
    
    if (![self checkActiveCode]) {
        
        return;
    }
    //初始化检测模块句柄
    NSString *strModelPath = [[NSBundle mainBundle] pathForResource:@"M_SenseME_Face_Video_5.3.3" ofType:@"model"];
    st_result_t iRet = st_mobile_human_action_create(strModelPath.UTF8String,
                                                    ST_MOBILE_DETECT_MODE_VIDEO |
                                                    ST_MOBILE_ENABLE_FACE_DETECT,
                                                    &svDetector);
    if (ST_OK != iRet || !svDetector) {
        
    }else{
    // [self addSubModel:svDetector mode:<#(NSString *)#>]
    }
    //初始化人脸属性
    NSString *strAttributeModelPath = [[NSBundle mainBundle] pathForResource:@"M_SenseME_Attribute_1.0.1" ofType:@"model"];
    iRet = st_mobile_face_attribute_create(strAttributeModelPath.UTF8String, &_hAttribute);
    if (iRet != ST_OK) {
        NSLog(@"st mobile face attribute create failed: %d", iRet);
    }
}


- (void) addSubModel:(st_handle_t) handle mode: (NSString*) modelName {
    st_result_t iRet = st_mobile_human_action_add_sub_model(handle, [[NSBundle mainBundle] pathForResource:modelName ofType:@"model"].UTF8String);
    if (iRet != ST_OK) {
        NSLog(@"st mobile human action add %@ model failed: %d", modelName, iRet);
    }
}

- (NSString *)getSHA1StringWithData:(NSData *)data {
    uint8_t digest[CC_SHA1_DIGEST_LENGTH];
    CC_SHA1(data.bytes, (unsigned int)data.length, digest);
    NSMutableString *strSHA1 = [NSMutableString string];
    for (int i = 0 ; i < CC_SHA1_DIGEST_LENGTH ; i ++) {
        [strSHA1 appendFormat:@"%02x" , digest[i]];
    }
    return strSHA1;
}

- (BOOL)checkActiveCode {
    NSString *strLicensePath = [[NSBundle mainBundle] pathForResource:@"SENSEME" ofType:@"lic"];
    NSData *dataLicense = [NSData dataWithContentsOfFile:strLicensePath];
    NSString *strKeySHA1 = @"SENSEME";
    NSString *strKeyActiveCode = @"ACTIVE_CODE";
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    NSString *strStoredSHA1 = [userDefaults objectForKey:strKeySHA1];
    NSString *strLicenseSHA1 = [self getSHA1StringWithData:dataLicense];
    st_result_t iRet = ST_OK;
    if (strStoredSHA1.length > 0 && [strLicenseSHA1 isEqualToString:strStoredSHA1]) {
        
        // Get current active code
        // In this app active code was stored in NSUserDefaults
        // It also can be stored in other places
        NSData *activeCodeData = [userDefaults objectForKey:strKeyActiveCode];
        
        // Check if current active code is available
        // use file
        iRet = st_mobile_check_activecode(
                                          strLicensePath.UTF8String,
                                          (const char *)[activeCodeData bytes],
                                          (int)[activeCodeData length]
                                          );
        
        if (ST_OK == iRet) {
            // check success
            return YES;
        }
    }
    
    /*
     1. check fail
     2. new one
     3. update
     */
    
    char active_code[1024];
    int active_code_len = 1024;
    
    // generate one
    // use file
    iRet = st_mobile_generate_activecode(
                                         strLicensePath.UTF8String,
                                         active_code,
                                         &active_code_len
                                         );
    
    if (ST_OK != iRet) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"错误提示" message:@"使用 license 文件生成激活码时失败，可能是授权文件过期。" delegate:nil cancelButtonTitle:@"好的" otherButtonTitles:nil, nil];
        [alert show];
        return NO;
    } else {
        // Store active code
        NSData *activeCodeData = [NSData dataWithBytes:active_code length:active_code_len];
        [userDefaults setObject:activeCodeData forKey:strKeyActiveCode];
        [userDefaults setObject:strLicenseSHA1 forKey:strKeySHA1];
        [userDefaults synchronize];
    }
    return YES;
}

-(unsigned char *) detectFace:(CMSampleBufferRef)sampleBuffer{
    CVPixelBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    uint8_t *baseAddress = (uint8_t*)CVPixelBufferGetBaseAddress(pixelBuffer);
    int iBytesPerRow = (int)CVPixelBufferGetBytesPerRow(pixelBuffer);
    int iHeight = (int)CVPixelBufferGetHeight(pixelBuffer);
    int iWidth = (int)CVPixelBufferGetWidth(pixelBuffer);
    st_mobile_human_action_t detectResult;
    memset(&detectResult, 0, sizeof(st_mobile_human_action_t));
    st_result_t  iRet = st_mobile_human_action_detect(svDetector,
                                         baseAddress,
                                         ST_PIX_FMT_BGRA8888,
                                         iWidth,
                                         iHeight,
                                         iBytesPerRow,
                                         ST_CLOCKWISE_ROTATE_0,
                                         ST_MOBILE_FACE_DETECT,
                                         &detectResult);
    [self transDetectData:detectResult];
    
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    
    return detectData;
}

-(void)transDetectData:(st_mobile_human_action_t) detectResult{
    
    int t_pointnum = 106;
    ////人脸(int)  3个转角(float)  脸的轮廓（top left bottom right 4个值 int 点(x,y,x,y.... int)

    int maxDataSize = sizeof(signed int)+(3*sizeof(float)+4*sizeof(int)+t_pointnum*2*sizeof(float))*4;
    if (detectData == NULL) {
        detectData = (unsigned char *)malloc(maxDataSize);
    }
    char *t_result_data = (char *)detectData;
    //写入人脸数目
     int data_len = sizeof( int);
    int off_len = 0;
    char *t_datapoint_data = t_result_data+off_len;
    int count =detectResult.face_count;
    memcpy(t_datapoint_data, &count, data_len); //人脸数目
    off_len += data_len;
    //写入具体数据
    for (int i = 0; i < detectResult.face_count; i++) {
        //        NSLog(@"Magic === 🙁🙁🙁🙁 人脸个数============== %d",nFaceCountInOut);
        //写入人脸角度
        data_len = 3 * sizeof(float);
        t_datapoint_data = t_result_data + off_len;
        float faceorientout[3];
        faceorientout[0]=detectResult.p_faces[i].face106.yaw;
        faceorientout[1]=detectResult.p_faces[i].face106.pitch;
        faceorientout[2]=detectResult.p_faces[i].face106.roll;
        memcpy(t_datapoint_data, &faceorientout[i*3], data_len); //YPR角度 3个float
        off_len += data_len;
        //写入人脸矩形
        data_len = 4 * sizeof(int32_t);
        int rcFaceRectOut[4];
        rcFaceRectOut[0]=detectResult.p_faces[i].face106.rect.left;
        rcFaceRectOut[1]=detectResult.p_faces[i].face106.rect.top;
        rcFaceRectOut[2]=detectResult.p_faces[i].face106.rect.right;
        rcFaceRectOut[3]=detectResult.p_faces[i].face106.rect.bottom;
        t_datapoint_data = t_result_data + off_len;
        memcpy(t_datapoint_data, &rcFaceRectOut[i*4], data_len); //面部矩形 4个Int
        off_len += data_len;
        //写入人脸数据
        data_len = sizeof(float) * 2 * t_pointnum;
        t_datapoint_data = t_result_data + off_len;
        memcpy(t_datapoint_data,detectResult.p_faces[i].face106.points_array, data_len);
        off_len += data_len;
    }
    
    
}


@end
