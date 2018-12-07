//
//  NXFFileManager.m
//  NXlib
//
//  Created by AK on 15/8/30.
//  Copyright (c) 2015年 AK. All rights reserved.
//

#import "NXFileManager.h"

#import <ImageIO/ImageIO.h>
#import <sys/xattr.h>

@implementation NXFileManager

+ (NSBundle *)getMainBundle;
{
    return [NSBundle mainBundle];
}

+ (NSString *)getMainBundleRes { return [[NSBundle mainBundle] resourcePath]; }
+ (NSString *)getDocumentDir
{
    NSString *documentDirectory =
        [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    return documentDirectory;
}

+ (NSString *)getCacheDir
{
    NSString *cacheDirectory =
        [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    return cacheDirectory;
}

+ (NSString *)getTmpDir { return NSTemporaryDirectory(); }
+ (NSString *)getPathForDocuments:(NSString *)filename
{
    return [[self getDocumentDir] stringByAppendingPathComponent:filename];
}
+ (NSString *)getPathForDocuments:(NSString *)filename inDir:(NSString *)dir
{
    return [[self getDirectoryForDocuments:dir] stringByAppendingPathComponent:filename];
}

+ (NSString *)getDirectoryForDocuments:(NSString *)dir
{
    NSString *dirPath = [[self getDocumentDir] stringByAppendingPathComponent:dir];
    BOOL isDir = NO;
    BOOL isCreated = [[NSFileManager defaultManager] fileExistsAtPath:dirPath isDirectory:&isDir];
    if (isCreated == NO || isDir == NO)
    {
        NSError *error = nil;
        BOOL success = [[NSFileManager defaultManager] createDirectoryAtPath:dirPath
                                                 withIntermediateDirectories:YES
                                                                  attributes:nil
                                                                       error:&error];
        if (success == NO) NSLog(@"create dir error: %@", error.debugDescription);
    }
    return dirPath;
}

+ (NSString *)deletingLastPathComponent:(NSString *)filePath { return [filePath stringByDeletingLastPathComponent]; }
+ (NSArray *)scanFilesInDirectory:(NSString *)directoryPath
{
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSError *error = nil;
    // fileList便是包含有该文件夹下所有文件的文件名及文件夹名的数组
    NSArray *fileList = [fileManager contentsOfDirectoryAtPath:directoryPath error:&error];
    NSLog(@"路径==%@,fileList%@", directoryPath, fileList);
    return fileList;
}

+ (NSString *)validateDir:(NSString *)dir
{
    BOOL isDir = NO;
    NSFileManager *fileManager = [NSFileManager defaultManager];
    BOOL existed = [fileManager fileExistsAtPath:dir isDirectory:&isDir];
    if (!(isDir == YES && existed == YES))
    {
        [self createDirectoriesForPath:dir];
    }
    return dir;
}

+ (BOOL)validateFile:(NSString *)filePath { return [[NSFileManager defaultManager] fileExistsAtPath:filePath]; }
+ (NSArray *)pathComponent:(NSString *)url { return [url componentsSeparatedByString:@"/"]; }
+ (NSMutableArray *)absoluteDirectories
{
    static NSMutableArray *directories = nil;
    static dispatch_once_t token;

    dispatch_once(&token, ^{

        directories =
            [NSMutableArray arrayWithObjects:[self pathForApplicationSupportDirectory], [self pathForCachesDirectory],
                                             [self pathForDocumentsDirectory], [self pathForLibraryDirectory],
                                             [self pathForMainBundleDirectory], [self pathForTemporaryDirectory], nil];

        [directories sortUsingComparator:^NSComparisonResult(id obj1, id obj2) {

            return (((NSString *)obj1).length > ((NSString *)obj2).length) ? 0 : 1;

        }];
    });

    return directories;
}

+ (NSString *)absoluteDirectoryForPath:(NSString *)path
{
    [self assertPath:path];

    if ([path isEqualToString:@"/"])
    {
        return nil;
    }

    NSMutableArray *directories = [self absoluteDirectories];

    for (NSString *directory in directories)
    {
        NSRange indexOfDirectoryInPath = [path rangeOfString:directory];

        if (indexOfDirectoryInPath.location == 0)
        {
            return directory;
        }
    }

    return nil;
}

+ (NSString *)absolutePath:(NSString *)path
{
    [self assertPath:path];

    NSString *defaultDirectory = [self absoluteDirectoryForPath:path];

    if (defaultDirectory != nil)
    {
        return path;
    }
    else
    {
        return [self pathForDocumentsDirectoryWithPath:path];
    }
}

+ (void)assertPath:(NSString *)path
{
    NSAssert(path != nil, @"Invalid path. Path cannot be nil.");
    NSAssert(![path isEqualToString:@""], @"Invalid path. Path cannot be empty string.");
}

+ (id)attributeOfItemAtPath:(NSString *)path forKey:(NSString *)key
{
    return [[self attributesOfItemAtPath:path] objectForKey:key];
}

+ (id)attributeOfItemAtPath:(NSString *)path forKey:(NSString *)key error:(NSError **)error
{
    return [[self attributesOfItemAtPath:path error:error] objectForKey:key];
}

+ (NSDictionary *)attributesOfItemAtPath:(NSString *)path { return [self attributesOfItemAtPath:path error:nil]; }
+ (NSDictionary *)attributesOfItemAtPath:(NSString *)path error:(NSError **)error
{
    return [[NSFileManager defaultManager] attributesOfItemAtPath:[self absolutePath:path] error:error];
}

+ (BOOL)copyItemAtPath:(NSString *)path toPath:(NSString *)toPath
{
    return [self copyItemAtPath:path toPath:toPath error:nil];
}

+ (BOOL)copyItemAtPath:(NSString *)path toPath:(NSString *)toPath error:(NSError **)error
{
    return ([self createDirectoriesForFileAtPath:toPath error:error] &&
            [[NSFileManager defaultManager] copyItemAtPath:[self absolutePath:path]
                                                    toPath:[self absolutePath:toPath]
                                                     error:error]);
}

+ (BOOL)createDirectoriesForFileAtPath:(NSString *)path { return [self createDirectoriesForFileAtPath:path error:nil]; }
+ (BOOL)createDirectoriesForFileAtPath:(NSString *)path error:(NSError **)error
{
    NSString *pathLastChar = [path substringFromIndex:(path.length - 1)];

    if ([pathLastChar isEqualToString:@"/"])
    {
        [NSException raise:@"Invalid path" format:@"file path can't have a trailing '/'."];

        return NO;
    }

    return [self createDirectoriesForPath:[[self absolutePath:path] stringByDeletingLastPathComponent] error:error];
}

+ (BOOL)createDirectoriesForPath:(NSString *)path { return [self createDirectoriesForPath:path error:nil]; }
+ (BOOL)createDirectoriesForPath:(NSString *)path error:(NSError **)error
{
    return [[NSFileManager defaultManager] createDirectoryAtPath:[self absolutePath:path]
                                     withIntermediateDirectories:YES
                                                      attributes:nil
                                                           error:error];
}

+ (BOOL)createFileAtPath:(NSString *)path { return [self createFileAtPath:path withContent:nil error:nil]; }
+ (BOOL)createFileAtPath:(NSString *)path error:(NSError **)error
{
    return [self createFileAtPath:path withContent:nil error:error];
}

+ (BOOL)createFileAtPath:(NSString *)path withContent:(NSObject *)content
{
    return [self createFileAtPath:path withContent:content error:nil];
}

+ (BOOL)createFileAtPath:(NSString *)path withContent:(NSObject *)content error:(NSError **)error
{
    if (![self existsItemAtPath:path] && [self createDirectoriesForFileAtPath:path error:error])
    {
        [[NSFileManager defaultManager] createFileAtPath:[self absolutePath:path] contents:nil attributes:nil];

        if (content != nil)
        {
            [self writeFileAtPath:path content:content error:error];
        }

        return (error == nil);
    }

    return NO;
}

+ (NSDate *)creationDateOfItemAtPath:(NSString *)path { return [self creationDateOfItemAtPath:path error:nil]; }
+ (NSDate *)creationDateOfItemAtPath:(NSString *)path error:(NSError **)error
{
    return (NSDate *)[self attributeOfItemAtPath:path forKey:NSFileCreationDate error:error];
}

+ (BOOL)emptyCachesDirectory { return [self removeFilesInDirectoryAtPath:[self pathForCachesDirectory]]; }
+ (BOOL)emptyTemporaryDirectory { return [self removeFilesInDirectoryAtPath:[self pathForTemporaryDirectory]]; }
+ (BOOL)existsItemAtPath:(NSString *)path
{
    return [[NSFileManager defaultManager] fileExistsAtPath:[self absolutePath:path]];
}

+ (BOOL)isDirectoryItemAtPath:(NSString *)path { return [self isDirectoryItemAtPath:path error:nil]; }
+ (BOOL)isDirectoryItemAtPath:(NSString *)path error:(NSError **)error
{
    return ([self attributeOfItemAtPath:path forKey:NSFileType error:error] == NSFileTypeDirectory);
}

+ (BOOL)isEmptyItemAtPath:(NSString *)path { return [self isEmptyItemAtPath:path error:nil]; }
+ (BOOL)isEmptyItemAtPath:(NSString *)path error:(NSError **)error
{
    return ([self isFileItemAtPath:path error:error] && ([[self sizeOfItemAtPath:path error:error] intValue] == 0)) ||
           ([self isDirectoryItemAtPath:path error:error] &&
            ([[self listItemsInDirectoryAtPath:path deep:NO] count] == 0));
}

+ (BOOL)isFileItemAtPath:(NSString *)path { return [self isFileItemAtPath:path error:nil]; }
+ (BOOL)isFileItemAtPath:(NSString *)path error:(NSError **)error
{
    return ([self attributeOfItemAtPath:path forKey:NSFileType error:error] == NSFileTypeRegular);
}

+ (BOOL)isExecutableItemAtPath:(NSString *)path
{
    return [[NSFileManager defaultManager] isExecutableFileAtPath:[self absolutePath:path]];
}

+ (BOOL)isReadableItemAtPath:(NSString *)path
{
    return [[NSFileManager defaultManager] isReadableFileAtPath:[self absolutePath:path]];
}

+ (BOOL)isWritableItemAtPath:(NSString *)path
{
    return [[NSFileManager defaultManager] isWritableFileAtPath:[self absolutePath:path]];
}

+ (NSArray *)listDirectoriesInDirectoryAtPath:(NSString *)path
{
    return [self listDirectoriesInDirectoryAtPath:path deep:NO];
}

+ (NSArray *)listDirectoriesInDirectoryAtPath:(NSString *)path deep:(BOOL)deep
{
    NSArray *subpaths = [self listItemsInDirectoryAtPath:path deep:deep];

    return [subpaths
        filteredArrayUsingPredicate:[NSPredicate predicateWithBlock:^BOOL(id evaluatedObject, NSDictionary *bindings) {

            NSString *subpath = (NSString *)evaluatedObject;

            return [self isDirectoryItemAtPath:subpath];
        }]];
}

+ (NSArray *)listFilesInDirectoryAtPath:(NSString *)path { return [self listFilesInDirectoryAtPath:path deep:NO]; }
+ (NSArray *)listFilesInDirectoryAtPath:(NSString *)path deep:(BOOL)deep
{
    NSArray *subpaths = [self listItemsInDirectoryAtPath:path deep:deep];

    return [subpaths
        filteredArrayUsingPredicate:[NSPredicate predicateWithBlock:^BOOL(id evaluatedObject, NSDictionary *bindings) {

            NSString *subpath = (NSString *)evaluatedObject;

            return [self isFileItemAtPath:subpath];
        }]];
}

+ (NSArray *)listFilesInDirectoryAtPath:(NSString *)path withExtension:(NSString *)extension
{
    return [self listFilesInDirectoryAtPath:path withExtension:extension deep:NO];
}

+ (NSArray *)listFilesInDirectoryAtPath:(NSString *)path withExtension:(NSString *)extension deep:(BOOL)deep
{
    NSArray *subpaths = [self listFilesInDirectoryAtPath:path deep:deep];

    return [subpaths
        filteredArrayUsingPredicate:[NSPredicate predicateWithBlock:^BOOL(id evaluatedObject, NSDictionary *bindings) {

            NSString *subpath = (NSString *)evaluatedObject;
            NSString *subpathExtension = [[subpath pathExtension] lowercaseString];
            NSString *filterExtension =
                [[extension lowercaseString] stringByReplacingOccurrencesOfString:@"." withString:@""];

            return [subpathExtension isEqualToString:filterExtension];
        }]];
}

+ (NSArray *)listFilesInDirectoryAtPath:(NSString *)path withPrefix:(NSString *)prefix
{
    return [self listFilesInDirectoryAtPath:path withPrefix:prefix deep:NO];
}

+ (NSArray *)listFilesInDirectoryAtPath:(NSString *)path withPrefix:(NSString *)prefix deep:(BOOL)deep
{
    NSArray *subpaths = [self listFilesInDirectoryAtPath:path deep:deep];

    return [subpaths
        filteredArrayUsingPredicate:[NSPredicate predicateWithBlock:^BOOL(id evaluatedObject, NSDictionary *bindings) {

            NSString *subpath = (NSString *)evaluatedObject;

            return ([subpath hasPrefix:prefix] || [subpath isEqualToString:prefix]);
        }]];
}

+ (NSArray *)listFilesInDirectoryAtPath:(NSString *)path withSuffix:(NSString *)suffix
{
    return [self listFilesInDirectoryAtPath:path withSuffix:suffix deep:NO];
}

+ (NSArray *)listFilesInDirectoryAtPath:(NSString *)path withSuffix:(NSString *)suffix deep:(BOOL)deep
{
    NSArray *subpaths = [self listFilesInDirectoryAtPath:path deep:deep];

    return [subpaths
        filteredArrayUsingPredicate:[NSPredicate predicateWithBlock:^BOOL(id evaluatedObject, NSDictionary *bindings) {

            NSString *subpath = (NSString *)evaluatedObject;
            NSString *subpathName = [subpath stringByDeletingPathExtension];

            return ([subpath hasSuffix:suffix] || [subpath isEqualToString:suffix] || [subpathName hasSuffix:suffix] ||
                    [subpathName isEqualToString:suffix]);
        }]];
}

+ (NSArray *)listItemsInDirectoryAtPath:(NSString *)path deep:(BOOL)deep
{
    NSString *absolutePath = [self absolutePath:path];
    NSArray *relativeSubpaths =
        (deep ? [[NSFileManager defaultManager] subpathsOfDirectoryAtPath:absolutePath error:nil]
              : [[NSFileManager defaultManager] contentsOfDirectoryAtPath:absolutePath error:nil]);

    NSMutableArray *absoluteSubpaths = [[NSMutableArray alloc] init];

    for (NSString *relativeSubpath in relativeSubpaths)
    {
        NSString *absoluteSubpath = [absolutePath stringByAppendingPathComponent:relativeSubpath];
        [absoluteSubpaths addObject:absoluteSubpath];
    }

    return [NSArray arrayWithArray:absoluteSubpaths];
}

+ (BOOL)moveItemAtPath:(NSString *)path toPath:(NSString *)toPath
{
    return [self moveItemAtPath:path toPath:toPath error:nil];
}

+ (BOOL)moveItemAtPath:(NSString *)path toPath:(NSString *)toPath error:(NSError **)error
{
    return ([self createDirectoriesForFileAtPath:toPath error:error] &&
            [[NSFileManager defaultManager] moveItemAtPath:[self absolutePath:path]
                                                    toPath:[self absolutePath:toPath]
                                                     error:error]);
}

+ (NSString *)pathForApplicationSupportDirectory
{
    static NSString *path = nil;
    static dispatch_once_t token;

    dispatch_once(&token, ^{

        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);

        path = [paths lastObject];
    });

    return path;
}

+ (NSString *)pathForApplicationSupportDirectoryWithPath:(NSString *)path
{
    return [[NXFileManager pathForApplicationSupportDirectory] stringByAppendingPathComponent:path];
}

+ (NSString *)pathForCachesDirectory
{
    static NSString *path = nil;
    static dispatch_once_t token;

    dispatch_once(&token, ^{

        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);

        path = [paths lastObject];
    });

    return path;
}

+ (NSString *)pathForCachesDirectoryWithPath:(NSString *)path
{
    return [[NXFileManager pathForCachesDirectory] stringByAppendingPathComponent:path];
}

+ (NSString *)pathForDocumentsDirectory
{
    static NSString *path = nil;
    static dispatch_once_t token;

    dispatch_once(&token, ^{

        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);

        path = [paths lastObject];
    });

    return path;
}

+ (NSString *)pathForDocumentsDirectoryWithPath:(NSString *)path
{
    return [[NXFileManager pathForDocumentsDirectory] stringByAppendingPathComponent:path];
}

+ (NSString *)pathForLibraryDirectory
{
    static NSString *path = nil;
    static dispatch_once_t token;

    dispatch_once(&token, ^{

        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);

        path = [paths lastObject];
    });

    return path;
}

+ (NSString *)pathForLibraryDirectoryWithPath:(NSString *)path
{
    return [[NXFileManager pathForLibraryDirectory] stringByAppendingPathComponent:path];
}

+ (NSString *)pathForMainBundleDirectory { return [NSBundle mainBundle].resourcePath; }
+ (NSString *)pathForMainBundleDirectoryWithPath:(NSString *)path
{
    return [[NXFileManager pathForMainBundleDirectory] stringByAppendingPathComponent:path];
}

+ (NSString *)pathForPlistNamed:(NSString *)name
{
    NSString *nameExtension = [name pathExtension];
    NSString *plistExtension = @"plist";

    if ([nameExtension isEqualToString:@""])
    {
        name = [name stringByAppendingPathExtension:plistExtension];
    }

    return [self pathForMainBundleDirectoryWithPath:name];
}

+ (NSString *)pathForTemporaryDirectory
{
    static NSString *path = nil;
    static dispatch_once_t token;

    dispatch_once(&token, ^{

        path = NSTemporaryDirectory();
    });

    return path;
}

+ (NSString *)pathForTemporaryDirectoryWithPath:(NSString *)path
{
    return [[NXFileManager pathForTemporaryDirectory] stringByAppendingPathComponent:path];
}

+ (NSString *)readFileAtPath:(NSString *)path { return [self readFileAtPathAsString:path error:nil]; }
+ (NSString *)readFileAtPath:(NSString *)path error:(NSError **)error
{
    return [self readFileAtPathAsString:path error:error];
}

+ (NSArray *)readFileAtPathAsArray:(NSString *)path
{
    return [NSArray arrayWithContentsOfFile:[self absolutePath:path]];
}

+ (NSObject *)readFileAtPathAsCustomModel:(NSString *)path
{
    return [NSKeyedUnarchiver unarchiveObjectWithFile:[self absolutePath:path]];
}

+ (NSData *)readFileAtPathAsData:(NSString *)path { return [self readFileAtPathAsData:path error:nil]; }
+ (NSData *)readFileAtPathAsData:(NSString *)path error:(NSError **)error
{
    return [NSData dataWithContentsOfFile:[self absolutePath:path] options:NSDataReadingMapped error:error];
}

+ (NSDictionary *)readFileAtPathAsDictionary:(NSString *)path
{
    return [NSDictionary dictionaryWithContentsOfFile:[self absolutePath:path]];
}

+ (UIImage *)readFileAtPathAsImage:(NSString *)path { return [self readFileAtPathAsImage:path error:nil]; }
+ (UIImage *)readFileAtPathAsImage:(NSString *)path error:(NSError **)error
{
    NSData *data = [self readFileAtPathAsData:path error:error];

    if (error == nil)
    {
        return [UIImage imageWithData:data];
    }

    return nil;
}

+ (UIImageView *)readFileAtPathAsImageView:(NSString *)path { return [self readFileAtPathAsImageView:path error:nil]; }
+ (UIImageView *)readFileAtPathAsImageView:(NSString *)path error:(NSError **)error
{
    UIImage *image = [self readFileAtPathAsImage:path error:error];

    if (error == nil)
    {
        UIImageView *imageView = [[UIImageView alloc] initWithImage:image];
        [imageView sizeToFit];
        return imageView;
    }

    return nil;
}

+ (NSJSONSerialization *)readFileAtPathAsJSON:(NSString *)path { return [self readFileAtPathAsJSON:path error:nil]; }
+ (NSJSONSerialization *)readFileAtPathAsJSON:(NSString *)path error:(NSError **)error
{
    NSData *data = [self readFileAtPathAsData:path error:error];

    if (error == nil)
    {
        NSJSONSerialization *json = [NSJSONSerialization JSONObjectWithData:data options:0 error:error];

        if ([NSJSONSerialization isValidJSONObject:json])
        {
            return json;
        }
    }

    return nil;
}

+ (NSMutableArray *)readFileAtPathAsMutableArray:(NSString *)path
{
    return [NSMutableArray arrayWithContentsOfFile:[self absolutePath:path]];
}

+ (NSMutableData *)readFileAtPathAsMutableData:(NSString *)path
{
    return [self readFileAtPathAsMutableData:path error:nil];
}

+ (NSMutableData *)readFileAtPathAsMutableData:(NSString *)path error:(NSError **)error
{
    return [NSMutableData dataWithContentsOfFile:[self absolutePath:path] options:NSDataReadingMapped error:error];
}

+ (NSMutableDictionary *)readFileAtPathAsMutableDictionary:(NSString *)path
{
    return [NSMutableDictionary dictionaryWithContentsOfFile:[self absolutePath:path]];
}

+ (NSString *)readFileAtPathAsString:(NSString *)path { return [self readFileAtPath:path error:nil]; }
+ (NSString *)readFileAtPathAsString:(NSString *)path error:(NSError **)error
{
    return [NSString stringWithContentsOfFile:[self absolutePath:path] encoding:NSUTF8StringEncoding error:error];
}

+ (BOOL)removeFilesInDirectoryAtPath:(NSString *)path
{
    return [self removeItemsAtPaths:[self listFilesInDirectoryAtPath:path] error:nil];
}

+ (BOOL)removeFilesInDirectoryAtPath:(NSString *)path error:(NSError **)error
{
    return [self removeItemsAtPaths:[self listFilesInDirectoryAtPath:path] error:error];
}

+ (BOOL)removeFilesInDirectoryAtPath:(NSString *)path withExtension:(NSString *)extension
{
    return [self removeItemsAtPaths:[self listFilesInDirectoryAtPath:path withExtension:extension] error:nil];
}

+ (BOOL)removeFilesInDirectoryAtPath:(NSString *)path withExtension:(NSString *)extension error:(NSError **)error
{
    return [self removeItemsAtPaths:[self listFilesInDirectoryAtPath:path withExtension:extension] error:error];
}

+ (BOOL)removeFilesInDirectoryAtPath:(NSString *)path withPrefix:(NSString *)prefix
{
    return [self removeItemsAtPaths:[self listFilesInDirectoryAtPath:path withPrefix:prefix] error:nil];
}

+ (BOOL)removeFilesInDirectoryAtPath:(NSString *)path withPrefix:(NSString *)prefix error:(NSError **)error
{
    return [self removeItemsAtPaths:[self listFilesInDirectoryAtPath:path withPrefix:prefix] error:error];
}

+ (BOOL)removeFilesInDirectoryAtPath:(NSString *)path withSuffix:(NSString *)suffix
{
    return [self removeItemsAtPaths:[self listFilesInDirectoryAtPath:path withSuffix:suffix] error:nil];
}

+ (BOOL)removeFilesInDirectoryAtPath:(NSString *)path withSuffix:(NSString *)suffix error:(NSError **)error
{
    return [self removeItemsAtPaths:[self listFilesInDirectoryAtPath:path withSuffix:suffix] error:error];
}

+ (BOOL)removeItemsInDirectoryAtPath:(NSString *)path { return [self removeItemsInDirectoryAtPath:path error:nil]; }
+ (BOOL)removeItemsInDirectoryAtPath:(NSString *)path error:(NSError **)error
{
    return [self removeItemsAtPaths:[self listItemsInDirectoryAtPath:path deep:NO] error:error];
}

+ (BOOL)removeItemAtPath:(NSString *)path { return [self removeItemAtPath:path error:nil]; }
+ (BOOL)removeItemAtPath:(NSString *)path error:(NSError **)error
{
    return [[NSFileManager defaultManager] removeItemAtPath:[self absolutePath:path] error:error];
}

+ (BOOL)removeItemsAtPaths:(NSArray *)paths { return [self removeItemsAtPaths:paths error:nil]; }
+ (BOOL)removeItemsAtPaths:(NSArray *)paths error:(NSError **)error
{
    BOOL success = YES;

    for (NSString *path in paths)
    {
        success &= [self removeItemAtPath:[self absolutePath:path] error:error];
    }

    return success;
}

+ (BOOL)renameItemAtPath:(NSString *)path withName:(NSString *)name
{
    return [self renameItemAtPath:path withName:name error:nil];
}

+ (BOOL)renameItemAtPath:(NSString *)path withName:(NSString *)name error:(NSError **)error
{
    NSRange indexOfSlash = [name rangeOfString:@"/"];

    if (indexOfSlash.location < name.length)
    {
        [NSException raise:@"Invalid name" format:@"file name can't contain a '/'."];

        return NO;
    }

    return [self moveItemAtPath:path
                         toPath:[[[self absolutePath:path] stringByDeletingLastPathComponent]
                                    stringByAppendingPathComponent:name]
                          error:error];
}

+ (NSString *)sizeFormatted:(NSNumber *)size
{
    // TODO if OS X 10.8 or iOS 6
    // return [NSByteCountFormatter stringFromByteCount:[size intValue]
    // countStyle:NSByteCountFormatterCountStyleFile];

    double convertedValue = [size doubleValue];
    int multiplyFactor = 0;

    NSArray *tokens = @[ @"bytes", @"KB", @"MB", @"GB", @"TB" ];

    while (convertedValue > 1024)
    {
        convertedValue /= 1024;

        multiplyFactor++;
    }

    NSString *sizeFormat = ((multiplyFactor > 1) ? @"%4.2f %@" : @"%4.0f %@");

    return [NSString stringWithFormat:sizeFormat, convertedValue, tokens[multiplyFactor]];
}

+ (NSString *)sizeFormattedOfDirectoryAtPath:(NSString *)path
{
    return [self sizeFormattedOfDirectoryAtPath:path error:nil];
}

+ (NSString *)sizeFormattedOfDirectoryAtPath:(NSString *)path error:(NSError **)error
{
    NSNumber *size = [self sizeOfDirectoryAtPath:path error:error];

    if (size != nil && error == nil)
    {
        return [self sizeFormatted:size];
    }

    return nil;
}

+ (NSString *)sizeFormattedOfFileAtPath:(NSString *)path { return [self sizeFormattedOfFileAtPath:path error:nil]; }
+ (NSString *)sizeFormattedOfFileAtPath:(NSString *)path error:(NSError **)error
{
    NSNumber *size = [self sizeOfFileAtPath:path error:error];

    if (size != nil && error == nil)
    {
        return [self sizeFormatted:size];
    }

    return nil;
}

+ (NSString *)sizeFormattedOfItemAtPath:(NSString *)path { return [self sizeFormattedOfItemAtPath:path error:nil]; }
+ (NSString *)sizeFormattedOfItemAtPath:(NSString *)path error:(NSError **)error
{
    NSNumber *size = [self sizeOfItemAtPath:path error:error];

    if (size != nil && error == nil)
    {
        return [self sizeFormatted:size];
    }

    return nil;
}

+ (NSNumber *)sizeOfDirectoryAtPath:(NSString *)path { return [self sizeOfDirectoryAtPath:path error:nil]; }
+ (NSNumber *)sizeOfDirectoryAtPath:(NSString *)path error:(NSError **)error
{
    if ([self isDirectoryItemAtPath:path error:error])
    {
        if (error == nil)
        {
            NSNumber *size = [self sizeOfItemAtPath:path error:error];
            double sizeValue = [size doubleValue];

            if (error == nil)
            {
                NSArray *subpaths = [self listItemsInDirectoryAtPath:path deep:YES];
                NSUInteger subpathsCount = [subpaths count];

                for (NSUInteger i = 0; i < subpathsCount; i++)
                {
                    NSString *subpath = [subpaths objectAtIndex:i];
                    NSNumber *subpathSize = [self sizeOfItemAtPath:subpath error:error];

                    if (error == nil)
                    {
                        sizeValue += [subpathSize doubleValue];
                    }
                    else
                    {
                        return nil;
                    }
                }

                return [NSNumber numberWithDouble:sizeValue];
            }
        }
    }

    return nil;
}

+ (NSNumber *)sizeOfFileAtPath:(NSString *)path { return [self sizeOfFileAtPath:path error:nil]; }
+ (NSNumber *)sizeOfFileAtPath:(NSString *)path error:(NSError **)error
{
    if ([self isFileItemAtPath:path error:error])
    {
        if (error == nil)
        {
            return [self sizeOfItemAtPath:path error:error];
        }
    }

    return nil;
}

+ (NSNumber *)sizeOfItemAtPath:(NSString *)path { return [self sizeOfItemAtPath:path error:nil]; }
+ (NSNumber *)sizeOfItemAtPath:(NSString *)path error:(NSError **)error
{
    return (NSNumber *)[self attributeOfItemAtPath:path forKey:NSFileSize error:error];
}

+ (NSURL *)urlForItemAtPath:(NSString *)path { return [NSURL fileURLWithPath:[self absolutePath:path]]; }
+ (BOOL)writeFileAtPath:(NSString *)path content:(NSObject *)content
{
    return [self writeFileAtPath:path content:content error:nil];
}

+ (BOOL)writeFileAtPath:(NSString *)path content:(NSObject *)content error:(NSError **)error
{
    if (content == nil)
    {
        [NSException raise:@"Invalid content" format:@"content can't be nil."];
    }

    [self createFileAtPath:path withContent:nil error:error];

    NSString *absolutePath = [self absolutePath:path];

    if ([content isKindOfClass:[NSMutableArray class]])
    {
        [((NSMutableArray *)content) writeToFile:absolutePath atomically:YES];
    }
    else if ([content isKindOfClass:[NSArray class]])
    {
        [((NSArray *)content) writeToFile:absolutePath atomically:YES];
    }
    else if ([content isKindOfClass:[NSMutableData class]])
    {
        [((NSMutableData *)content) writeToFile:absolutePath atomically:YES];
    }
    else if ([content isKindOfClass:[NSData class]])
    {
        [((NSData *)content) writeToFile:absolutePath atomically:YES];
    }
    else if ([content isKindOfClass:[NSMutableDictionary class]])
    {
        [((NSMutableDictionary *)content) writeToFile:absolutePath atomically:YES];
    }
    else if ([content isKindOfClass:[NSDictionary class]])
    {
        [((NSDictionary *)content) writeToFile:absolutePath atomically:YES];
    }
    else if ([content isKindOfClass:[NSJSONSerialization class]])
    {
        [((NSDictionary *)content) writeToFile:absolutePath atomically:YES];
    }
    else if ([content isKindOfClass:[NSMutableString class]])
    {
        [[((NSString *)content) dataUsingEncoding:NSUTF8StringEncoding] writeToFile:absolutePath atomically:YES];
    }
    else if ([content isKindOfClass:[NSString class]])
    {
        [[((NSString *)content) dataUsingEncoding:NSUTF8StringEncoding] writeToFile:absolutePath atomically:YES];
    }
    else if ([content isKindOfClass:[UIImage class]])
    {
        [UIImagePNGRepresentation((UIImage *)content) writeToFile:absolutePath atomically:YES];
    }
    else if ([content isKindOfClass:[UIImageView class]])
    {
        return [self writeFileAtPath:absolutePath content:((UIImageView *)content).image error:error];
    }
    else if ([content conformsToProtocol:@protocol(NSCoding)])
    {
        [NSKeyedArchiver archiveRootObject:content toFile:absolutePath];
    }
    else
    {
        [NSException raise:@"Invalid content type"
                    format:@"content of type %@ is not handled.", NSStringFromClass([content class])];

        return NO;
    }

    return YES;
}

+ (NSDictionary *)metadataOfImageAtPath:(NSString *)path
{
    if ([self isFileItemAtPath:path])
    {
        // http://blog.depicus.com/getting-exif-data-from-images-on-ios/

        NSURL *url = [self urlForItemAtPath:path];
        CGImageSourceRef sourceRef = CGImageSourceCreateWithURL((CFURLRef)url, NULL);
        NSDictionary *metadata =
            (NSDictionary *)CFBridgingRelease(CGImageSourceCopyPropertiesAtIndex(sourceRef, 0, NULL));

        return metadata;
    }

    return nil;
}

+ (NSDictionary *)exifDataOfImageAtPath:(NSString *)path
{
    NSDictionary *metadata = [self metadataOfImageAtPath:path];

    if (metadata)
    {
        return [metadata objectForKey:(NSString *)kCGImagePropertyExifDictionary];
    }

    return nil;
}

+ (NSDictionary *)tiffDataOfImageAtPath:(NSString *)path
{
    NSDictionary *metadata = [self metadataOfImageAtPath:path];

    if (metadata)
    {
        return [metadata objectForKey:(NSString *)kCGImagePropertyTIFFDictionary];
    }

    return nil;
}

+ (NSDictionary *)xattrOfItemAtPath:(NSString *)path
{
    NSMutableDictionary *values = [[NSMutableDictionary alloc] init];

    const char *upath = [path UTF8String];

    ssize_t ukeysSize = listxattr(upath, NULL, 0, 0);

    if (ukeysSize > 0)
    {
        char *ukeys = malloc(ukeysSize);

        ukeysSize = listxattr(upath, ukeys, ukeysSize, 0);

        NSUInteger keyOffset = 0;
        NSString *key;
        NSString *value;

        while (keyOffset < ukeysSize)
        {
            key = [NSString stringWithUTF8String:(keyOffset + ukeys)];
            keyOffset += ([key length] + 1);

            value = [self xattrOfItemAtPath:path getValueForKey:key];
            [values setObject:value forKey:key];
        }

        free(ukeys);
    }

    return [NSDictionary dictionaryWithObjects:[values allKeys] forKeys:[values allValues]];
}

+ (NSString *)xattrOfItemAtPath:(NSString *)path getValueForKey:(NSString *)key
{
    NSString *value = nil;

    const char *ukey = [key UTF8String];
    const char *upath = [path UTF8String];

    ssize_t uvalueSize = getxattr(upath, ukey, NULL, 0, 0, 0);

    if (uvalueSize > -1)
    {
        if (uvalueSize == 0)
        {
            value = @"";
        }
        else
        {
            char *uvalue = malloc(uvalueSize);

            if (uvalue)
            {
                getxattr(upath, ukey, uvalue, uvalueSize, 0, 0);
                uvalue[uvalueSize] = '\0';
                value = [NSString stringWithUTF8String:uvalue];
                free(uvalue);
            }
        }
    }

    return value;
}

+ (BOOL)xattrOfItemAtPath:(NSString *)path hasValueForKey:(NSString *)key
{
    return ([self xattrOfItemAtPath:path getValueForKey:key] != nil);
}

+ (BOOL)xattrOfItemAtPath:(NSString *)path removeValueForKey:(NSString *)key
{
    int result = removexattr([path UTF8String], [key UTF8String], 0);

    return (result == 0);
}

+ (BOOL)xattrOfItemAtPath:(NSString *)path setValue:(NSString *)value forKey:(NSString *)key
{
    if (value == nil)
    {
        return NO;
    }

    int result = setxattr([path UTF8String], [key UTF8String], [value UTF8String], [value length], 0, 0);

    return (result == 0);
}

+ (NSString *)currentDirectory:(NSString *)filename
{
    NSString *result = nil;
    //创建文件管理器
    NSFileManager *fileManager = [NSFileManager defaultManager];
    //获取路径
    //参数NSDocumentDirectory要获取那种路径
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths lastObject];  //去处需要的路径
    NSString *userNeedDirectory = [NSString stringWithFormat:@"%@", documentsDirectory];
    //更改到待操作的目录下
    [fileManager changeCurrentDirectoryPath:[userNeedDirectory stringByExpandingTildeInPath]];
    //创建文件fileName文件名称，contents文件的内容，如果开始没有内容可以设置为nil，attributes文件的属性，初始为nil
    //获取文件路径
    result = [userNeedDirectory stringByAppendingPathComponent:filename];

    return result;
}

//文件是否存在
+ (BOOL)isExistFile:(NSString *)filename
{
    BOOL bret = NO;
    bret = [NXFileManager isExistFile:filename outDirpath:nil];
    return bret;
}

+ (BOOL)isExistFile:(NSString *)filename outDirpath:(NSMutableString *)outString
{
    //创建文件管理器
    NSFileManager *fileManager = [NSFileManager defaultManager];
    //获取路径
    //参数NSDocumentDirectory要获取那种路径
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];  //去处需要的路径
    NSString *userNeedDirectory = [NSString stringWithFormat:@"%@", documentsDirectory];
    //更改到待操作的目录下
    [fileManager changeCurrentDirectoryPath:[userNeedDirectory stringByExpandingTildeInPath]];
    //创建文件fileName文件名称，contents文件的内容，如果开始没有内容可以设置为nil，attributes文件的属性，初始为nil
    //获取文件路径
    NSString *path = [userNeedDirectory stringByAppendingPathComponent:filename];
    if (outString != nil)
    {
        [outString setString:path];
    }

    return [fileManager fileExistsAtPath:path];
}

+ (NSString *)fullpath:(NSString *)filename
{
    NSString *path = [NXFileManager currentDirectory:filename];
    if (path == nil)
    {
#ifdef _DEBUG
        NSLog(@"获取路径错误");
#endif
        return nil;
    }
    return path;
}

@end
