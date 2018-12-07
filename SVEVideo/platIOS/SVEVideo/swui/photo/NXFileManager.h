//
//  NXFFileManager.h
//  NXlib
//
//  Created by AK on 15/8/30.
//  Copyright (c) 2015年 AK. All rights reserved.
//

/*

 本类功能, 文件操作.(ios, mac)

 一,iOS目录结构说明
 1,沙盒目录结构
 ├── Documents - 存储用户数据或其它应该定期备份的
 ├── Library
 │   ├── Caches -
 用于存放应用程序专用的支持文件，保存应用程序再次启动过程中需要的信息
 │   │   └── Snapshots
 │   │       └── com.youyouxingyuan.re
 │   │           ├── A85B73F0-26A8-44E4-A761-446CAB8DAB38@2x.png
 │   │           └── BFAD5885-B767-4320-9A4B-555EC881C50D@2x.png
 │   └── Preferences - 偏好设置文件 NSUserDefaults 保存的数据
 └── tmp - 这个目录用于存放临时文件，保存应用程序再次启动过程中不需要的信息

 2,在iOS8之后，应用每一次重启，沙盒路径都动态的发生了变化但不用担心数据问题,苹果会把你上一个路径中的数据转移到你新的路径中。你上一个路径也会被苹果毫无保留的删除，只保留最新的路径。

 @see  <Foundation/NSPathUtilities.h>

 */
//文件操作
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface NXFileManager : NSObject<NSFileManagerDelegate>

#pragma mark - 常用路径

/**
 *  app bundle path 每次运行会改变
 *  //mobile/Containers/Bundle/Application/84C95A20-21A7-4E39-AFFC-E63FE5586BDA/YOSticker.app>
 *  //mobile/Containers/Bundle/Application/3BC8A0E0-0177-400A-AECE-6B940F8F2DF6/YOSticker.app>
 *
 *  @return mainBundle
 */
+ (NSBundle *)getMainBundle;

+ (NSString *)getMainBundleRes;

/**
 *  获取Document path
 *
 *  @return Document path
 */
+ (NSString *)getDocumentDir;

/**
 *  获取Cache path
 *
 *  @return Cache path
 */
+ (NSString *)getCacheDir;

/**
 *  获取temp path
 *
 *  @return temp 路径
 */
+ (NSString *)getTmpDir;

/**
 *  得到文件所在目录
 *
 *  @param filePath 文件路径
 *
 *  @return 文件所在目录
 */
+ (NSString *)deletingLastPathComponent:(NSString *)filePath;

/**
 *  校验目录路径是否有效  如果目录不存在 会自动创建
 *
 *  @param dir 目录路径
 *
 *  @return 目录路径
 */
+ (NSString *)validateDir:(NSString *)dir;

/**
 *  文件路径校验是否存在
 *
 *  @param filePath 文件路径
 *
 *  @return YES 存在
 */
+ (BOOL)validateFile:(NSString *)filePath;

/**
 *  通过文件名返回Documents 目录下的全路径
 *
 *  @param filename 文件名
 *
 *  @return 返回Doc下的全路径
 */
+ (NSString *)getPathForDocuments:(NSString *)filename;

/**
 *  通过文件名返回Documents 目录下的全路径
 *
 *  @param filename 文件名
 *  @param dir      目录名可以是多层目录如"aaa/bbb"
 *
 *  @return 全路径
 */
+ (NSString *)getPathForDocuments:(NSString *)filename inDir:(NSString *)dir;

#pragma mark -  get文件的属性
/*
     NSFileAppendOnly: 文件是否只读
     NSFileBusy: 文件是否繁忙
     NSFileCreationDate: 文件创建日期
     NSFileOwnerAccountName: 文件所有者的名字
     NSFileGroupOwnerAccountName: 文件所有组的名字
     NSFileDeviceIdentifier: 文件所在驱动器的标示符
     NSFileExtensionHidden: 文件后缀是否隐藏
     NSFileGroupOwnerAccountID: 文件所有组的group ID
     NSFileHFSCreatorCode: 文件的HFS创建者的代码
     NSFileHFSTypeCode: 文件的HFS类型代码
     NSFileImmutable: 文件是否可以改变
     NSFileModificationDate: 文件修改日期
     NSFileOwnerAccountID: 文件所有者的ID
     NSFilePosixPermissions: 文件的Posix权限
     NSFileReferenceCount: 文件的链接数量
     NSFileSize: 文件的字节
     NSFileSystemFileNumber: 文件在文件系统的文件数量
     NSFileType: 文件类型
     NSDirectoryEnumerationSkipsSubdirectoryDescendants:
   浅层的枚举，不会枚举子目录
     NSDirectoryEnumerationSkipsPackageDescendants: 不会扫描pakages的内容
     NSDirectoryEnumerationSkipsHiddenFile: 不会扫描隐藏文件
 */

/**
 *   取文件的所有属性
 *
 *  @param path 文件path
 *
 *  @return 文件的所有属性(MAC)
 */
+ (NSDictionary *)attributesOfItemAtPath:(NSString *)path;

/**
 *  取文件的所有属性 &error
 *
 *  @param path  文件path
 *  @param error &error
 *
 *  @return 文件的所有属性
 */
+ (NSDictionary *)attributesOfItemAtPath:(NSString *)path error:(NSError **)error;
/**
 *  取文件属性
 *
 *  @param path 文件目录
 *  @param key  属性KEY
 *
 *  @return 属性 value
 */
+ (id)attributeOfItemAtPath:(NSString *)path forKey:(NSString *)key;

/**
 *  取文件属性
 *
 *  @param path  文件目录
 *  @param key   属性KEY
 *  @param error &error
 *
 *  @return 属性 value
 */
+ (id)attributeOfItemAtPath:(NSString *)path forKey:(NSString *)key error:(NSError **)error;

#pragma mark - 文件C-M-D-L操作
/**
 *  复制一个文件到指定目录
 *
 *  @param path 原目录
 *  @param toPath 目标目录
 *
 *  @return 操作结果
 */
+ (BOOL)copyItemAtPath:(NSString *)path toPath:(NSString *)toPath;

+ (BOOL)copyItemAtPath:(NSString *)path toPath:(NSString *)path error:(NSError **)error;

/**
 *  创建文件夹
 *
 *  @param path
 *
 *  @return 创建文件夹结果
 */

/**
 创建文件所在目录 如~/aaaa/xxxx.txt 生成的为 文件夹 ~/aaaa/

 @param path 文件所在目录
 @return 是否创建成功
 */
+ (BOOL)createDirectoriesForFileAtPath:(NSString *)path;
+ (BOOL)createDirectoriesForFileAtPath:(NSString *)path error:(NSError **)error;

/**
 创建目录 如~/aaaa/xxxx 生成的为 文件夹 ~/aaaa/xxxx/   (注意 即使指定xxxx的类型 生成的也是文件夹)

 @param path 目录
 @return 是否创建成功
 */
+ (BOOL)createDirectoriesForPath:(NSString *)path;
+ (BOOL)createDirectoriesForPath:(NSString *)path error:(NSError **)error;

/**

 创建指定文件 如~/aaaa/xxxx.txt 生成的为 文件 ~/aaaa/xxxx.txt   (注意 即使不指定xxxx的类型 生成的也是文件)
 @param path 文件路径
 @return 是否创建成功
 */
+ (BOOL)createFileAtPath:(NSString *)path;
+ (BOOL)createFileAtPath:(NSString *)path error:(NSError **)error;

+ (BOOL)createFileAtPath:(NSString *)path withContent:(NSObject *)content;
+ (BOOL)createFileAtPath:(NSString *)path withContent:(NSObject *)content error:(NSError **)error;

#pragma mark -
/**
 *  取文件的创建时间
 *
 *  @param path 文件路径
 *
 *  @return 时间
 */
+ (NSDate *)creationDateOfItemAtPath:(NSString *)path;
+ (NSDate *)creationDateOfItemAtPath:(NSString *)path error:(NSError **)error;

/**
 *  清空Caches目录
 *
 *  @return 清空结果
 */
+ (BOOL)emptyCachesDirectory;
/**
 *  清空Temporary目录
 *
 *  @return 清空结果
 */
+ (BOOL)emptyTemporaryDirectory;

/**
 *  判断文件是否存在
 *
 *  @param path 文件路径
 *
 *  @return 存在 返回YES
 */
+ (BOOL)existsItemAtPath:(NSString *)path;

/**
 *  判断路径是不是目录 类型
 *
 *  @param path 路径
 *
 *  @return YES 是目录
 */
+ (BOOL)isDirectoryItemAtPath:(NSString *)path;
+ (BOOL)isDirectoryItemAtPath:(NSString *)path error:(NSError **)error;

+ (BOOL)isEmptyItemAtPath:(NSString *)path;
+ (BOOL)isEmptyItemAtPath:(NSString *)path error:(NSError **)error;

/**
 *  判断路径是不是一个文件类型
 *
 *  @param path 路径
 *
 *  @return YES 是文件
 */
+ (BOOL)isFileItemAtPath:(NSString *)path;
+ (BOOL)isFileItemAtPath:(NSString *)path error:(NSError **)error;

// 判断某个路径下文件是否可执行
+ (BOOL)isExecutableItemAtPath:(NSString *)path;
// 判断某个路径下文件是否可读
+ (BOOL)isReadableItemAtPath:(NSString *)path;
// 判断某个路径下文件是否可写
+ (BOOL)isWritableItemAtPath:(NSString *)path;

/**
 *  获取某个路径下的所有子路径
 *
 *  @param path 路径
 *
 *  @return 子路径数组
 */
+ (NSArray *)listDirectoriesInDirectoryAtPath:(NSString *)path;
+ (NSArray *)listDirectoriesInDirectoryAtPath:(NSString *)path deep:(BOOL)deep;

+ (NSArray *)listFilesInDirectoryAtPath:(NSString *)path;
+ (NSArray *)listFilesInDirectoryAtPath:(NSString *)path deep:(BOOL)deep;

+ (NSArray *)listFilesInDirectoryAtPath:(NSString *)path withExtension:(NSString *)extension;
+ (NSArray *)listFilesInDirectoryAtPath:(NSString *)path withExtension:(NSString *)extension deep:(BOOL)deep;

+ (NSArray *)listFilesInDirectoryAtPath:(NSString *)path withPrefix:(NSString *)prefix;
+ (NSArray *)listFilesInDirectoryAtPath:(NSString *)path withPrefix:(NSString *)prefix deep:(BOOL)deep;

+ (NSArray *)listFilesInDirectoryAtPath:(NSString *)path withSuffix:(NSString *)suffix;
+ (NSArray *)listFilesInDirectoryAtPath:(NSString *)path withSuffix:(NSString *)suffix deep:(BOOL)deep;

+ (NSArray *)listItemsInDirectoryAtPath:(NSString *)path deep:(BOOL)deep;

/**
 *  移动某个路径下文件到另一个路径
 *
 *  @param path 路径
 *  @param toPath 目标路径
 *  @return 是否移动成功
 */
+ (BOOL)moveItemAtPath:(NSString *)path toPath:(NSString *)toPath;
+ (BOOL)moveItemAtPath:(NSString *)path toPath:(NSString *)toPath error:(NSError **)error;

// location of application support files
+ (NSString *)pathForApplicationSupportDirectory;
+ (NSString *)pathForApplicationSupportDirectoryWithPath:(NSString *)path;

// location of discardable cache files (Library/Caches)
+ (NSString *)pathForCachesDirectory;
+ (NSString *)pathForCachesDirectoryWithPath:(NSString *)path;

// documents (Documents)
+ (NSString *)pathForDocumentsDirectory;
+ (NSString *)pathForDocumentsDirectoryWithPath:(NSString *)path;

// various documentation, support, and configuration files, resources (Library)
+ (NSString *)pathForLibraryDirectory;
+ (NSString *)pathForLibraryDirectoryWithPath:(NSString *)path;

// 沙盒路径
+ (NSString *)pathForMainBundleDirectory;
+ (NSString *)pathForMainBundleDirectoryWithPath:(NSString *)path;

// plist 文件路径
+ (NSString *)pathForPlistNamed:(NSString *)name;

// 临时文件路径
+ (NSString *)pathForTemporaryDirectory;
+ (NSString *)pathForTemporaryDirectoryWithPath:(NSString *)path;

#pragma mark - 以不同的形式读写文件的内容
// 读取指定路径中的文件内容
+ (NSString *)readFileAtPath:(NSString *)path;
+ (NSString *)readFileAtPath:(NSString *)path error:(NSError **)error;

+ (NSArray *)readFileAtPathAsArray:(NSString *)path;

+ (NSObject *)readFileAtPathAsCustomModel:(NSString *)path;

+ (NSData *)readFileAtPathAsData:(NSString *)path;
+ (NSData *)readFileAtPathAsData:(NSString *)path error:(NSError **)error;

+ (NSDictionary *)readFileAtPathAsDictionary:(NSString *)path;

+ (UIImage *)readFileAtPathAsImage:(NSString *)path;
+ (UIImage *)readFileAtPathAsImage:(NSString *)path error:(NSError **)error;

+ (UIImageView *)readFileAtPathAsImageView:(NSString *)path;
+ (UIImageView *)readFileAtPathAsImageView:(NSString *)path error:(NSError **)error;

+ (NSJSONSerialization *)readFileAtPathAsJSON:(NSString *)path;
+ (NSJSONSerialization *)readFileAtPathAsJSON:(NSString *)path error:(NSError **)error;

+ (NSMutableArray *)readFileAtPathAsMutableArray:(NSString *)path;

+ (NSMutableData *)readFileAtPathAsMutableData:(NSString *)path;
+ (NSMutableData *)readFileAtPathAsMutableData:(NSString *)path error:(NSError **)error;

+ (NSMutableDictionary *)readFileAtPathAsMutableDictionary:(NSString *)path;

+ (NSString *)readFileAtPathAsString:(NSString *)path;
+ (NSString *)readFileAtPathAsString:(NSString *)path error:(NSError **)error;

// 删除指定路径的文件/目录
+ (BOOL)removeFilesInDirectoryAtPath:(NSString *)path;
+ (BOOL)removeFilesInDirectoryAtPath:(NSString *)path error:(NSError **)error;

+ (BOOL)removeFilesInDirectoryAtPath:(NSString *)path withExtension:(NSString *)extension;
+ (BOOL)removeFilesInDirectoryAtPath:(NSString *)path withExtension:(NSString *)extension error:(NSError **)error;

+ (BOOL)removeFilesInDirectoryAtPath:(NSString *)path withPrefix:(NSString *)prefix;
+ (BOOL)removeFilesInDirectoryAtPath:(NSString *)path withPrefix:(NSString *)prefix error:(NSError **)error;

+ (BOOL)removeFilesInDirectoryAtPath:(NSString *)path withSuffix:(NSString *)suffix;
+ (BOOL)removeFilesInDirectoryAtPath:(NSString *)path withSuffix:(NSString *)suffix error:(NSError **)error;

+ (BOOL)removeItemsInDirectoryAtPath:(NSString *)path;
+ (BOOL)removeItemsInDirectoryAtPath:(NSString *)path error:(NSError **)error;

+ (BOOL)removeItemAtPath:(NSString *)path;
+ (BOOL)removeItemAtPath:(NSString *)path error:(NSError **)error;

// 对指定路径的文件/目录重新命名
+ (BOOL)renameItemAtPath:(NSString *)path withName:(NSString *)name;
+ (BOOL)renameItemAtPath:(NSString *)path withName:(NSString *)name error:(NSError **)error;

#pragma mark - 文件 size
+ (NSString *)sizeFormatted:(NSNumber *)size;

+ (NSString *)sizeFormattedOfDirectoryAtPath:(NSString *)path;
+ (NSString *)sizeFormattedOfDirectoryAtPath:(NSString *)path error:(NSError **)error;

+ (NSString *)sizeFormattedOfFileAtPath:(NSString *)path;
+ (NSString *)sizeFormattedOfFileAtPath:(NSString *)path error:(NSError **)error;

+ (NSString *)sizeFormattedOfItemAtPath:(NSString *)path;
+ (NSString *)sizeFormattedOfItemAtPath:(NSString *)path error:(NSError **)error;

+ (NSNumber *)sizeOfDirectoryAtPath:(NSString *)path;
+ (NSNumber *)sizeOfDirectoryAtPath:(NSString *)path error:(NSError **)error;

+ (NSNumber *)sizeOfFileAtPath:(NSString *)path;
+ (NSNumber *)sizeOfFileAtPath:(NSString *)path error:(NSError **)error;

+ (NSNumber *)sizeOfItemAtPath:(NSString *)path;
+ (NSNumber *)sizeOfItemAtPath:(NSString *)path error:(NSError **)error;

#pragma mark -
//将文件路径转化为url
+ (NSURL *)urlForItemAtPath:(NSString *)path;

#pragma mark -
//将文件写入指定路径
+ (BOOL)writeFileAtPath:(NSString *)path content:(NSObject *)content;
+ (BOOL)writeFileAtPath:(NSString *)path content:(NSObject *)content error:(NSError **)error;
#pragma mark -
// 获取指定路径的 图像 元数据，EXIF数据，TIFF数据
+ (NSDictionary *)metadataOfImageAtPath:(NSString *)path;
+ (NSDictionary *)exifDataOfImageAtPath:(NSString *)path;
+ (NSDictionary *)tiffDataOfImageAtPath:(NSString *)path;

#pragma mark -
// 读写xattr
+ (NSDictionary *)xattrOfItemAtPath:(NSString *)path;
+ (NSString *)xattrOfItemAtPath:(NSString *)path getValueForKey:(NSString *)key;
+ (BOOL)xattrOfItemAtPath:(NSString *)path hasValueForKey:(NSString *)key;
+ (BOOL)xattrOfItemAtPath:(NSString *)path removeValueForKey:(NSString *)key;
+ (BOOL)xattrOfItemAtPath:(NSString *)path setValue:(NSString *)value forKey:(NSString *)key;

#pragma mark -

// 当前文件所在文件夹
+ (NSString *)currentDirectory:(NSString *)filename;

//获取完整文件名
+ (NSString *)fullpath:(NSString *)filename;

//文件是否存在
+ (BOOL)isExistFile:(NSString *)filename;

+ (BOOL)isExistFile:(NSString *)filename outDirpath:(NSMutableString *)outString;

@end
