#!/bin/sh

function createFiles()
{
    local srcPath=$1
    local tarPath=$2
    local ignoreFiles=$3
    local FOLDERS=`ls ${srcPath}`
    for file in $FOLDERS
    do
        local canIgnore=0
        for t_ignoreFile in ${ignoreFiles[*]}
        do
            if [ $file = $t_ignoreFile ]
            then
                canIgnore=1
                break
            fi
        done
        if [ -d "${srcPath}/${file}" -a $canIgnore -eq 0 ]
        then
        local NEW_FOLDERS="${tarPath}/${file}"
        mkdir -p "${NEW_FOLDERS}"
        find "${srcPath}/${file}" -name "*.h" -exec cp {} "${NEW_FOLDERS}" \;
        find "${srcPath}/${file}" -name "*.hh" -exec cp {} "${NEW_FOLDERS}" \;

        local SUB_SRC_FOLDERS="${srcPath}/${file}"
        local SUB_TAR_FOLDERS="${tarPath}/${file}"
        createFiles $SUB_SRC_FOLDERS $SUB_TAR_FOLDERS
        fi
    done
}

#要build的target名
target_Name=${PROJECT_NAME}
if [[ $1 ]]
then
target_Name=$1
fi

ignoreFiles=()
if [[ $2 ]]
then
ignoreFiles=$2
fi

UNIVERSAL_OUTPUT_FOLDER="${SRCROOT}/${PROJECT_NAME}_Products"

# 创建输出目录
mkdir -p "${UNIVERSAL_OUTPUT_FOLDER}"

# 编译真机
xcodebuild -target "${target_Name}" ONLY_ACTIVE_ARCH=NO -configuration ${CONFIGURATION} -sdk iphoneos  BUILD_DIR="${BUILD_DIR}" BUILD_ROOT="${BUILD_ROOT}" clean build

# 判断build文件夹是否存在，存在则删除
if [ -d "${SRCROOT}/build" ]
then
rm -rf "${SRCROOT}/build"
fi
#生成头文件
createFiles ${SRCROOT}/../src ${UNIVERSAL_OUTPUT_FOLDER}/${target_Name}/src "${ignoreFiles[*]}"
#createFiles ${SRCROOT}/${PROJECT_NAME} ${UNIVERSAL_OUTPUT_FOLDER}/${target_Name}/${PROJECT_NAME} "${ignoreFiles[*]}"

#复制头文件到目标文件夹
HEADER_FOLDER="${BUILD_DIR}/${CONFIGURATION}-iphoneos/include"
if [ -d "${HEADER_FOLDER}" ]
then
   rm -rf "${HEADER_FOLDER}"
fi
mkdir -p "${HEADER_FOLDER}/${target_Name}"
cp -R "${UNIVERSAL_OUTPUT_FOLDER}/${target_Name}" "${HEADER_FOLDER}"

#删除输出文件夹
if [ -d "${UNIVERSAL_OUTPUT_FOLDER}" ]
then
    rm -rf "${UNIVERSAL_OUTPUT_FOLDER}"
fi

#打开目标文件夹
open "${BUILD_DIR}/${CONFIGURATION}-iphoneos"
