
include $(srctree)/drivers/misc/mediatek/Makefile.custom

# Module project related part

ifeq ($(CONFIG_MTK_SENSOR_SUPPORT),y)
ifeq ($(CONFIG_MTK_EPL2182),y)
obj-y	+=  alsps/epl2182/
endif

ifeq ($(CONFIG_MTK_KXTJ2_1009),y)
obj-y   +=  accelerometer/KXTJ2_1009/
endif

ifeq ($(CONFIG_MTK_LIS3DH_AUTO),y)
obj-y   +=  accelerometer/lis3dh_auto/
endif

#ifeq ($(CONFIG_MTK_MPU3000),y)
#obj-y   +=  gyroscope/mpu3000/
#endif

#ifeq ($(CONFIG_MTK_MPU3000_NEW),y)
#obj-y   +=  gyroscope/mpu3000-new/
#endif

ifeq ($(CONFIG_MTK_ST480_AUTO),y)
obj-y	+=  magnetometer/st480_auto/
endif

ifeq ($(CONFIG_MTK_MMC3516X_AUTO),y)
obj-y	+=  magnetometer/mmc3516x_auto/
endif


endif

ifneq ($(CONFIG_ARCH_MTK_PROJECT),)
ARCH_MTK_PROJECT := $(shell echo $(CONFIG_ARCH_MTK_PROJECT))
obj-y	+= core/
obj-y	+= dct/dct/
endif

obj-y   +=  lcm/
#obj-y	+=  hdmi/
obj-y	+=  camera/camera/
#obj-y	+=  imgsensor/hi551_mipi_raw/
obj-y	+=  imgsensor/gc0311_yuv/
obj-y	+=  imgsensor/hi708_yuv/
#obj-y   +=  flashlight/constant_flashlight/
obj-y	+=  vibrator/
obj-y	+=  leds/
obj-y	+=  keypad/
obj-y	+=  sound/
obj-y	+=  accdet/
obj-y	+=  partition/

