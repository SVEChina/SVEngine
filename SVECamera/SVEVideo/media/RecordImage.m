//
//  ViewController.h
//  SVEVideo
//
//  Created by yizhou Fu on 2018/5/9.
//  Copyright © 2018年 yizhou Fu. All rights reserved.
//

#import "RecordImage.h"
#import <Photos/Photos.h>
#import "SWUISys.h"
#import "SWMainUI.h"
#define  maxwidth 720
#define maxheight 1280
@interface RecordImage(){
    UIImage *savedImage;
    NSURL   *imagedURL;
    CMSampleBufferRef curSampleBuffer;
    unsigned char *frameData;
    int     mType;
    int     mWidth;
    int     mHeight;
}
@end

@implementation RecordImage

- (instancetype)init{
    self = [super init];
    if (self) {
        frameData = malloc(maxwidth*maxheight*4);
    }
    return self;
}

- (void)setImageBuffer:(CMSampleBufferRef)sampleBuffer{
    CVPixelBufferRef pixelBuffer = CMSampleBufferGetImageBuffer( sampleBuffer );
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    mWidth = (int)CVPixelBufferGetWidth(pixelBuffer);
    mHeight = (int)CVPixelBufferGetHeight(pixelBuffer);
    mType = CVPixelBufferGetPixelFormatType(pixelBuffer);
    memset(frameData, 0, maxwidth*maxheight*4);
    if (mType == kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange || mType == kCVPixelFormatType_420YpCbCr8BiPlanarFullRange) {
        unsigned char *buffer = (UInt8 *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer,0);
        memcpy(frameData, buffer, mWidth*mHeight*1.5);
    }else if (mType == kCVPixelFormatType_32BGRA || mType == kCVPixelFormatType_32RGBA){
        unsigned char *buffer = (UInt8*)CVPixelBufferGetBaseAddress(pixelBuffer);
        memcpy(frameData, buffer, mWidth*mHeight*4);
//        _m_image = [self bgraPixelBufferToImage:frameData w:mWidth h:mHeight];
        
//        dispatch_async(dispatch_get_main_queue(), ^{
//            // UI更新代码
//            [SWUISys getInst].pMainVC.pMainView.m_imageView.image = _m_image;
//        });
        
    }
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    
    
    
}

- (void)dealloc{
    free(frameData);
}

- (void)setImagedURL:(NSURL *)url{
    imagedURL = url;
}

- (NSURL*)getImageURL{
    return imagedURL;
}

- (void)saveImageToAssetsLibrary{
    UIImageWriteToSavedPhotosAlbum(savedImage, self, @selector(image:didFinishSavingWithError:contextInfo:), NULL);
}

- (void)savedImageToFileCompression:(float)compression{
    float t_compression = 1.0;
    if (compression > 0.0 && compression < 1.0) {
        t_compression = compression;
    }
    if (mType == kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange || mType == kCVPixelFormatType_420YpCbCr8BiPlanarFullRange) {
        savedImage = [self yuvToImage:frameData w:mWidth h:mHeight];
    }else if (mType == kCVPixelFormatType_32BGRA || mType == kCVPixelFormatType_32RGBA){
        savedImage = [self bgraPixelBufferToImage:frameData w:mWidth h:mHeight];
    }


    bool saved = [UIImageJPEGRepresentation(savedImage, t_compression) writeToURL:imagedURL atomically:YES];
    if (saved) {
        if (self.saveImageToFileResult) {
            self.saveImageToFileResult(@"success");
        }
    }else{
        if (self.saveImageToFileResult) {
            self.saveImageToFileResult(@"fail");
        }
    }
}

// 指定回调方法
- (void)image: (UIImage *) image didFinishSavingWithError: (NSError *) error contextInfo: (void *) contextInfo
{
    NSString *msg = nil ;
    if(error != NULL){
        msg = @"保存图片失败" ;
    }else{
        msg = @"保存图片成功" ;
    }
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.saveImageToAssetsResult) {
            self.saveImageToAssetsResult(error);
        }
    });
   
}

- (UIImage *)yuvToImage:(unsigned char *)plane0 w:(int)w h:(int)h
{
    //YUV(NV12)-->CIImage--->UIImage Conversion
    NSDictionary *pixelAttributes = @{(id)kCVPixelBufferIOSurfacePropertiesKey : @{}};
    CVPixelBufferRef pixelBuffer = NULL;
    CVReturn result = CVPixelBufferCreate(kCFAllocatorDefault,
                                          w,
                                          h,
                                          kCVPixelFormatType_420YpCbCr8BiPlanarFullRange,
                                          (__bridge CFDictionaryRef)(pixelAttributes),
                                          &pixelBuffer);
    CVPixelBufferLockBaseAddress(pixelBuffer,0);
    unsigned char *yDestPlane = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
    memcpy(yDestPlane, plane0, w * h); //Here y_ch0 is Y-Plane of YUV(NV12) data.
    unsigned char *uvDestPlane = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1);
    memcpy(uvDestPlane, plane0 + w * h, w * h / 2); //Here y_ch1 is UV-Plane of YUV(NV12) data.
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    
    if (result != kCVReturnSuccess) {
        NSLog(@"Unable to create cvpixelbuffer %d", result);
    }
    CIImage *coreImage = [CIImage imageWithCVPixelBuffer:pixelBuffer];//CIImage Conversion DONE!!!!
    CIContext *MytemporaryContext = [CIContext contextWithOptions:nil];
    CGImageRef MyvideoImage = [MytemporaryContext
                               createCGImage:coreImage
                               fromRect:CGRectMake(0, 0,
                                                   w,
                                                   h)];
    UIImage *Mynnnimage = [[UIImage alloc] initWithCGImage:MyvideoImage scale:1.0 orientation:UIImageOrientationUp];//UIImage Conversion DONE!!!
    CVPixelBufferRelease(pixelBuffer);
    CGImageRelease(MyvideoImage);
    return Mynnnimage;
}


- (UIImage *)bgraPixelBufferToImage:(unsigned char*)bgraFrame w:(int)w h:(int)h{
    if (bgraFrame == NULL) {
        return nil;
    }
    NSDictionary *pixelAttributes = @{(id)kCVPixelBufferIOSurfacePropertiesKey : @{}};
    CVPixelBufferRef pixelBuffer = NULL;
    CVReturn result = CVPixelBufferCreate(kCFAllocatorDefault,
                                          w,
                                          h,
                                          kCVPixelFormatType_32BGRA,
                                          (__bridge CFDictionaryRef)(pixelAttributes),
                                          &pixelBuffer);
    CVPixelBufferLockBaseAddress(pixelBuffer,0);
    unsigned char *destPlane = (unsigned char *)CVPixelBufferGetBaseAddress(pixelBuffer);
    memcpy(destPlane, bgraFrame, w*h*4);
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    if (result != kCVReturnSuccess) {
        NSLog(@"Unable to create cvpixelbuffer %d", result);
    }
    CIImage *coreImage = [CIImage imageWithCVPixelBuffer:pixelBuffer];
    CIContext *MytemporaryContext = [CIContext contextWithOptions:nil];
    CGImageRef MyvideoImage = [MytemporaryContext
                               createCGImage:coreImage
                               fromRect:CGRectMake(0, 0,
                                                   w,
                                                   h)];
    UIImage *Mynnnimage = [[UIImage alloc] initWithCGImage:MyvideoImage scale:1.0 orientation:UIImageOrientationUp];
    CVPixelBufferRelease(pixelBuffer);
    CGImageRelease(MyvideoImage);
    return Mynnnimage;
}

@end
