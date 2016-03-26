#!/bin/bash

#get changes from  GitHub 
#echo 'Get changes from GitHub'
#git pull
#echo 'Start building...'

# Set options
export ARCH=arm CROSS_COMPILE=../android_toolchain/linaro_4.9bin/arm-cortex_a7-linux-gnueabihf-
export KBUILD_BUILD_USER=assusdan
export KBUILD_BUILD_HOST=SRT

    #For checking errors
echo 'Remove kernel...'
rm -rf arch/arm/boot/zImage


echo 'Configure CM Pure F '
make alps_defconfig >/dev/null

echo 'Building CM Pure F...'
make -j4 zImage >/dev/null 2>buildlog.log

    #check errors
if [ ! -f arch/arm/boot/zImage ]
then
    echo "BUID ERRORS!"
else
 #if OK
echo 'Moving CM Pure F'
mv arch/arm/boot/zImage /var/www/compiled/CM-zImage-puref
fi


#write worktime
echo $[$SECONDS / 60]' minutes '$[$SECONDS % 60]' seconds' 


#echo 'Configure A'
#make zerasra_defconfig >/dev/null
#echo 'Build A'
#make -j4 zImage >/dev/null 2>buildlog.log
#echo 'Moving A'
#mv arch/arm/boot/zImage /var/www/html/zImage_A 
