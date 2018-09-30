//
//  SWStareFilterSave.m
//  SVEVideo
//
//  Created by 徐子昱 on 2018/7/20.
//  Copyright © 2018年 付一洲. All rights reserved.
//

#import "SWStateFilterFile.h"
@implementation SWStateFilterFile

+(void) saveSate:(SWState *) _state fileName:(NSString* )_nameFile{
    NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:10];
    [dict setValue:@(_state.svDesignW) forKey:@"svDesignW"];
    [dict setValue:@(_state.svDesignH) forKey:@"svDesignH"];
    [dict setValue:@(_state.svOutW) forKey:@"svOutW"];
    [dict setValue:@(_state.svOutH) forKey:@"svOutH"];
    [dict setValue:@(_state.svAdaptMode) forKey:@"svAdaptMode"];
    [dict setValue:@(_state.svAdaptRate) forKey:@"svAdaptRate"];
    [dict setValue:@(_state.svBeautyRadius) forKey:@"svBeautyRadius"];
    [dict setValue:@(_state.svShadows) forKey:@"svShadows"];
    [dict setValue:@(_state.svHighlights) forKey:@"svHighlights"];
    [dict setValue:@(_state.svContrast) forKey:@"svContrast"];
    [dict setValue:@(_state.svSaturation) forKey:@"svSaturation"];
    [dict setValue:@(_state.svAcutance) forKey:@"svAcutance"];
    [dict setValue:@(_state.svBrightness) forKey:@"svBrightness"];
    [dict setValue:@(_state.svWhitening) forKey:@"svWhitening"];
    [dict setValue:@(_state.svGamma) forKey:@"svGamma"];
    [dict setValue:@(_state.svRedShift) forKey:@"svRedShift"];
    [dict setValue:@(_state.svGreenShift) forKey:@"svGreenShift"];
    [dict setValue:@(_state.svBlueShift) forKey:@"svBlueShift"];
    [dict setValue:@(_state.svSDRedShift) forKey:@"svSDRedShift"];
    [dict setValue:@(_state.svSDGreenShift) forKey:@"svSDGreenShift"];
    [dict setValue:@(_state.svSDBlueShift) forKey:@"svSDBlueShift"];
    [dict setValue:@(_state.svHHRedShift) forKey:@"svHHRedShift"];
    [dict setValue:@(_state.svHHGreenShift) forKey:@"svHHGreenShift"];
    [dict setValue:@(_state.svHHBlueShift) forKey:@"svHHBlueShift"];
    [dict setValue:@(_state.svFaceEye) forKey:@"svFaceEye"];
    [dict setValue:@(_state.svFaceShape) forKey:@"svFaceShape"];
    [dict setValue:@(_state.svTemperature) forKey:@"svTemperature"];
    [dict setValue:@(_state.svTint) forKey:@"svTint"];
    [dict setValue:@(_state.svExposure) forKey:@"svExposure"];
    NSData *t_jsonData = [NSJSONSerialization dataWithJSONObject:dict options:0 error:NULL];
    NSFileManager *t_fileManager = [NSFileManager defaultManager];
    //获取路径
    //参数NSDocumentDirectory要获取那种路径
    NSArray *t_paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *t_documentsDirectory = [t_paths objectAtIndex:0];//去处需要的路径
    //更改到待操作的目录下
    [t_fileManager changeCurrentDirectoryPath:[t_documentsDirectory stringByExpandingTildeInPath]];
    //获取文件路径
    NSString *path = [t_documentsDirectory stringByAppendingPathComponent:_nameFile];
    [t_jsonData writeToFile:path atomically:YES];
}

+(void) loadSate:(SWState *) _state fileName:(NSString *) _namefile{
    NSFileManager *t_fileManager = [NSFileManager defaultManager];
    //获取路径
    //参数NSDocumentDirectory要获取那种路径
    NSArray *t_paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *t_documentsDirectory = [t_paths objectAtIndex:0];//去处需要的路径
    //更改到待操作的目录下
    [t_fileManager changeCurrentDirectoryPath:[t_documentsDirectory stringByExpandingTildeInPath]];
    //获取文件路径
    NSString *t_path = [t_documentsDirectory stringByAppendingPathComponent:_namefile];
    NSData *t_reader = [NSData dataWithContentsOfFile:t_path];
    
    NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:t_reader options:kNilOptions error:nil];
    if([dict isEqual:@"svDesignW"]){
        _state.svDesignW=[[dict objectForKey:@"svDesignW"] intValue];
    }
    if([dict isEqual:@"svDesignH"]){
        _state.svDesignH=[[dict objectForKey:@"svDesignH"] intValue];
    }
    if([dict isEqual:@"svOutW"]){
        _state.svOutW=[[dict objectForKey:@"svOutW"] intValue];
    }
    if([dict isEqual:@"svOutH"]){
        _state.svOutH=[[dict objectForKey:@"svOutH"] intValue];
    }
    if([dict isEqual:@"svAdaptMode"]){
        _state.svAdaptMode=[[dict objectForKey:@"svAdaptMode"] intValue];
    }
    if([dict isEqual:@"svAdaptRate"]){
        _state.svAdaptRate=[[dict objectForKey:@"svAdaptRate"] floatValue];
    }
    if([dict isEqual:@"svBeautyRadius"]){
        _state.svBeautyRadius=[[dict objectForKey:@"svBeautyRadius"] floatValue];
    }
    if([dict isEqual:@"svShadows"]){
        _state.svShadows=[[dict objectForKey:@"svShadows"] floatValue];
    }
    if([dict isEqual:@"svHighlights"]){
        _state.svHighlights=[[dict objectForKey:@"svHighlights"] floatValue];
    }
    if([dict isEqual:@"svContrast"]){
        _state.svContrast=[[dict objectForKey:@"svContrast"] floatValue];
    }
    if([dict isEqual:@"svSaturation"]){
        _state.svSaturation=[[dict objectForKey:@"svSaturation"] floatValue];
    }
    if([dict isEqual:@"svAcutance"]){
        _state.svAcutance=[[dict objectForKey:@"svAcutance"] floatValue];
    }
    if([dict isEqual:@"svBrightness"]){
        _state.svBrightness=[[dict objectForKey:@"svBrightness"] floatValue];
    }
    if([dict isEqual:@"svWhitening"]){
        _state.svWhitening=[[dict objectForKey:@"svWhitening"] floatValue];
    }
    if([dict isEqual:@"svGamma"]){
        _state.svGamma=[[dict objectForKey:@"svGamma"] floatValue];
    }
    if([dict isEqual:@"svRedShift"]){
        _state.svRedShift=[[dict objectForKey:@"svRedShift"] floatValue];
    }
    if([dict isEqual:@"svGreenShift"]){
        _state.svGreenShift=[[dict objectForKey:@"svGreenShift"] floatValue];
    }
    if([dict isEqual:@"svBlueShift"]){
        _state.svBlueShift=[[dict objectForKey:@"svBlueShift"] floatValue];
    }
    if([dict isEqual:@"svSDRedShift"]){
        _state.svSDRedShift=[[dict objectForKey:@"svSDRedShift"] floatValue];
    }
    if([dict isEqual:@"svSDGreenShift"]){
        _state.svSDGreenShift=[[dict objectForKey:@"svSDGreenShift"] floatValue];
    }
    if([dict isEqual:@"svSDBlueShift"]){
        _state.svSDBlueShift=[[dict objectForKey:@"svSDBlueShift"] floatValue];
    }
    if([dict isEqual:@"svHHRedShift"]){
        _state.svHHRedShift=[[dict objectForKey:@"svHHRedShift"] floatValue];
    }
    if([dict isEqual:@"svHHGreenShift"]){
        _state.svHHGreenShift=[[dict objectForKey:@"svHHGreenShift"] floatValue];
    }
    if([dict isEqual:@"svHHBlueShift"]){
        _state.svHHBlueShift=[[dict objectForKey:@"svHHBlueShift"] floatValue];
    }
    if([dict isEqual:@"svFaceEye"]){
        _state.svFaceEye=[[dict objectForKey:@"svFaceEye"] floatValue];
    }
    if([dict isEqual:@"svFaceShape"]){
        _state.svFaceShape=[[dict objectForKey:@"svFaceShape"] floatValue];
    }
    if([dict isEqual:@"svTemperature"]){
        _state.svTemperature=[[dict objectForKey:@"svTemperature"] floatValue];
    }
    if([dict isEqual:@"svTint"]){
        _state.svTint=[[dict objectForKey:@"svTint"] floatValue];
    }
    if([dict isEqual:@"svExposure"]){
        _state.svExposure=[[dict objectForKey:@"svExposure"] floatValue];
    }
}

@end
