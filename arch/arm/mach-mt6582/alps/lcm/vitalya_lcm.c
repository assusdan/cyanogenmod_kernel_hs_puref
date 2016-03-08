#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
	#include <platform/mt_pmic.h>
	
	#define Lcd_Log printf
#else
    #include <linux/string.h>
	#include <linux/kernel.h>
	#include <mach/mt_gpio.h>
	#include <mach/mt_pm_ldo.h>
	
	#define Lcd_Log printk
#endif

#include "lcm_drv.h"

////////////////////////////////////////////////////
#define FRAME_WIDTH					(480)
#define FRAME_HEIGHT					(854)

#define REGFLAG_DELAY					0XFFE
#define REGFLAG_END_OF_TABLE				0xFFF

#define LCM_DSI_CMD_MODE				0

#define LCM_ID						0x980604
/////////////////////////////////////////////////////////

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)				(lcm_util.set_reset_pin((v)))
#define UDELAY(n)					(lcm_util.udelay(n))
#define MDELAY(n)					(lcm_util.mdelay(n))

static unsigned int lcm_compare_id(void);
// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)						lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)			lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)						lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)			lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)       

static struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[120];
};


static struct LCM_setting_table lcm_initialization_setting[] = {

{0xFF,5,{0xFF,0x98,0x06,0x04,0x01}},	// Change to Page 1
{0x08,1,{0x10}},			// output SDA
{0x21,1,{0x01}},			// DE = 1 Active
{0x30,1,{0x01}},			// 480 X 854
{0x31,1,{0x02}},			// 2-dot Inversion
{0x60,1,{0x07}},			// SDTI
{0x61,1,{0x06}},			// CRTI
{0x62,1,{0x06}},			// EQTI
{0x63,1,{0x04}},			// PCTI
{0x40,1,{0x14}},			// BT  +2.5/-2.5 pump for DDVDH-L
{0x41,1,{0x22}},			// DVDDH DVDDL clamp  
{0x42,1,{0x02}},			// VGH/VGL  01
{0x43,1,{0x84}},			// VGH/VGL 
{0x44,1,{0x8A}},			// VGH/VGL 
{0x50,1,{0x78}},			// VGMP
{0x51,1,{0x78}},			// VGMN
{0x52,1,{0x00}},			//Flicker
{0x53,1,{0x55}},			//Flicker8C


{0xA0,1,{0x00}},  // Gamma 0 
{0xA1,1,{0x19}},  // Gamma 4 
{0xA2,1,{0x20}},  // Gamma 8
{0xA3,1,{0x0E}},  // Gamma 16
{0xA4,1,{0x06}},  // Gamma 24
{0xA5,1,{0x09}},  // Gamma 52
{0xA6,1,{0x06}},  // Gamma 80
{0xA7,1,{0x05}},  // Gamma 108
{0xA8,1,{0x07}},  // Gamma 147
{0xA9,1,{0x0B}},  // Gamma 175
{0xAA,1,{0x12}},  // Gamma 203
{0xAB,1,{0x07}},  // Gamma 231
{0xAC,1,{0x0E}},  // Gamma 239
{0xAD,1,{0x15}},  // Gamma 247
{0xAE,1,{0x0C}},  // Gamma 251
{0xAF,1,{0x00}},  // Gamma 255


{0xC0,1,{0x00}},  // Gamma 0 
{0xC1,1,{0x19}},  // Gamma 4
{0xC2,1,{0x20}},  // Gamma 8
{0xC3,1,{0x0E}},  // Gamma 16
{0xC4,1,{0x06}},  // Gamma 24
{0xC5,1,{0x09}},  // Gamma 52
{0xC6,1,{0x06}},  // Gamma 80
{0xC7,1,{0x04}},  // Gamma 108
{0xC8,1,{0x07}},  // Gamma 147
{0xC9,1,{0x0B}},  // Gamma 175
{0xCA,1,{0x12}},  // Gamma 203
{0xCB,1,{0x07}},  // Gamma 231
{0xCC,1,{0x0E}},  // Gamma 239
{0xCD,1,{0x15}},  // Gamma 247
{0xCE,1,{0x0C}},  // Gamma 251
{0xCF,1,{0x00}},  // Gamma 255

{0xFF,5,{0xFF,0x98,0x06,0x04,0x06}},     // Change to Page 6
{0x00,1,{0x20}},
{0x01,1,{0x04}},
{0x02,1,{0x00}},    
{0x03,1,{0x00}},
{0x04,1,{0x01}},
{0x05,1,{0x01}},
{0x06,1,{0x88}},    
{0x07,1,{0x04}},
{0x08,1,{0x01}},
{0x09,1,{0x90}},    
{0x0A,1,{0x03}},    
{0x0B,1,{0x01}},    
{0x0C,1,{0x01}},
{0x0D,1,{0x01}},
{0x0E,1,{0x00}},
{0x0F,1,{0x00}},
{0x10,1,{0x55}},
{0x11,1,{0x53}},
{0x12,1,{0x01}},
{0x13,1,{0x0D}},
{0x14,1,{0x0D}},
{0x15,1,{0x43}},
{0x16,1,{0x0B}},
{0x17,1,{0x00}},
{0x18,1,{0x00}},
{0x19,1,{0x00}},
{0x1A,1,{0x00}},
{0x1B,1,{0x00}},
{0x1C,1,{0x00}},
{0x1D,1,{0x00}},


{0x20,1,{0x01}},
{0x21,1,{0x23}},
{0x22,1,{0x45}},
{0x23,1,{0x67}},
{0x24,1,{0x01}},
{0x25,1,{0x23}},
{0x26,1,{0x45}},
{0x27,1,{0x67}},

{0x30,1,{0x02}},
{0x31,1,{0x22}},
{0x32,1,{0x11}},
{0x33,1,{0xAA}},
{0x34,1,{0xBB}},
{0x35,1,{0x66}},
{0x36,1,{0x00}},
{0x37,1,{0x22}},
{0x38,1,{0x22}},
{0x39,1,{0x22}},
{0x3A,1,{0x22}},
{0x3B,1,{0x22}},
{0x3C,1,{0x22}},
{0x3D,1,{0x22}},
{0x3E,1,{0x22}},
{0x3F,1,{0x22}},
{0x40,1,{0x22}},
{0x53,1,{0x10}},	// 0x10:VGLO tie VGL; 0x12:VGLO tie VGL_REG           

{0xFF,5,{0xFF,0x98,0x06,0x04,0x07}},	// Change to Page 7
{0x17,1,{0x22}},	// 0x22:VGLO tie VGL; 0x32:VGLO tie VGL_REG
{0x18,1,{0x1D}},	// VREG1 VREG2 output
{0x06,1,{0x13}},
{0x02,1,{0x77}},                 
{0xE1,1,{0x79}},                 

//////////////////////////////////////////////

{0xFF,5,{0xFF,0x98,0x06,0x04,0x00}},	// Change to Page 0

{0x11, 1, {0x00}},			// Sleep-Out
{REGFLAG_DELAY, 120, {}},
{0x29, 1, {0x00}},			// Display on
{REGFLAG_DELAY, 5, {}},
{0x2c,1,{0x00}},
{REGFLAG_DELAY, 5, {}},

{REGFLAG_END_OF_TABLE, 0x00, {}}

};

static struct LCM_setting_table lcm_set_window[] = {
	{0x2A,	4,	{0x00, 0x00, (FRAME_WIDTH>>8), (FRAME_WIDTH&0xFF)}},
	{0x2B,	4,	{0x00, 0x00, (FRAME_HEIGHT>>8), (FRAME_HEIGHT&0xFF)}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_sleep_out_setting[] = {
	{0x11, 0, {0x00}},
    {REGFLAG_DELAY, 120, {}},
	{0x29, 0, {0x00}},
    {REGFLAG_DELAY, 100, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	{0x28, 0, {0x00}},
    {REGFLAG_DELAY, 120, {}},
	{0x10, 0, {0x00}},
    {REGFLAG_DELAY, 20, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_backlight_level_setting[] = {
	{0x51, 1, {0xFF}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
	unsigned int i;
    for(i = 0; i < count; i++) {
        unsigned cmd;
        cmd = table[i].cmd;
        switch (cmd) {
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
            case REGFLAG_END_OF_TABLE :
                break;
            default:
				dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
       	}
    }
	
}


// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
  memset(params, 0, sizeof(LCM_PARAMS));
  params->dsi.mode = 3;
  params->dsi.horizontal_sync_active = 6;
  params->dsi.PLL_CLOCK = 210;
  params->dsi.fbk_div = 19;
  params->dsi.lcm_esd_check_table[0].para_list[0] = -100;
  params->type = 2;
  params->dsi.LANE_NUM = 2;
  params->dsi.data_format.format = 2;
  params->dsi.PS = 2;
  params->width = 480;
  params->dsi.horizontal_active_pixel = 480;
  params->height = 854;
  params->dsi.vertical_active_line = 854;
  params->dbi.te_mode = 1;
  params->dsi.esd_check_enable = 1;
  params->dsi.customization_esd_check_enable = 1;
  params->dsi.lcm_esd_check_table[0].count = 1;
  params->dbi.te_edge_polarity = 0;
  params->dsi.data_format.color_order = 0;
  params->dsi.data_format.trans_seq = 0;
  params->dsi.data_format.padding = 0;
  params->dsi.pll_div1 = 0;
  params->dsi.pll_div2 = 0;
  params->dsi.vertical_sync_active = 4;
  params->dsi.ssc_range = 4;
  params->dsi.vertical_backporch = 10;
  params->dsi.vertical_frontporch = 10;
  params->dsi.lcm_esd_check_table[0].cmd = 10;
  params->dsi.horizontal_backporch = 60;
  params->dsi.horizontal_frontporch = 60;	
}

static void lcm_init(void)
{

    SET_RESET_PIN(1);
    MDELAY(5);
    SET_RESET_PIN(0);
    MDELAY(30);
    SET_RESET_PIN(1);
    MDELAY(100);
	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}

static void lcm_suspend(void)
{
	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(20);
}

static void lcm_resume(void)
{
	lcm_init();
}

static unsigned int lcm_compare_id(void)
{
    return 1;
}

static unsigned int lcm_esd_recover()
{
	 lcm_init();
}

LCM_DRIVER ili9806e_dsi_vdo_lcm_drv = 
{
    .name			= "ili9806e_dsi_vdo",
	.set_util_funcs = lcm_set_util_funcs,
    .compare_id    = lcm_compare_id,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.esd_recover = lcm_esd_recover,
};
