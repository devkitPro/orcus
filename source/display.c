#include <gp2xregs.h>
#include <orcus.h>

// NOTE! Using lots of magic numbers here which were just lifted straight from an F100 via JTAG while running official GPH firmware, after trying to figure out the values based on the data sheet took too long. These may be different for an F200, to confirm
void orcus_configure_display() {
  REG16(DISPCSETREG) = 0x6000; // DISPCLKSRC(FPLL_CLK) | DISPCLKDIV(32) | DISPCLKPOL(0);
  REG16(DPC_CNTL) = PAL(0) | CISCYNC(0) | HDTV(0) | DOT(0) | INTERLACE(0) | SYNCCBR(0) | ESAVEN(0) | DOF(2); //0x5
  REG16(DPC_CLKCNTL) = 0x10; //CLKSRC(2) | CLK2SEL(0) | CLK1SEL(0) | CLK1POL(0);

  REG16(DPC_X_MAX) = 320 - 1;
  REG16(DPC_Y_MAX) = 240 - 1;

  REG16(DPC_HS_WIDTH) = 0x041D; //HSPADCFG(0) | HSWID(30 - 1);
  REG16(DPC_HS_END) = 0x0009; //(20/2)-1;
  REG16(DPC_HS_STR) = 0x0009; //(20/2) + (20%1) - 1;
  REG16(DPC_DE) = 0x250; //DESTR(38-1);
  REG16(DPC_V_SYNC) = 0x0403;
  REG16(DPC_V_END) 0x0816;
  REG16(DPC_FPIPOL1) = 0x0010;
  REG16(DPC_FPIPOL2) = 0xFFFF;
  REG16(DPC_FPIPOL3) = 0x00FF;

  REG16(DPC_FPIATV1) = 0xFFFF;
  REG16(DPC_FPIATV2) = 0xFFFF;
  REG16(DPC_FPIATV3) = 0xFFFF;

  REG16(DPC_CNTL) |= ENB(1);
}

  /*     
    MLC_RGB_SetBPP(MLC_RGB_16BPP);
        MLC_RGB_SetColorKey(0xff, 0xff, 0xff);
        MLC_RGB_MixMux(MLC_RGB_RGN_1, MLC_RGB_MIXMUX_PRI, 0x8 & 0xf);
        MLC_RGB_SetScale(LCD_WIDTH, LCD_HEIGHT, LCD_WIDTH*2, LCD_WIDTH, LCD_HEIGHT);
        MLC_RGB_SetActivate(MLC_RGB_RGN_1, MLC_RGB_RGN_ACT);
        MLC_RGB_SetCoord(MLC_RGB_RGN_1, PLANE_X_OFFSET, PLANE_X_OFFSET+PLANE_X_WIDTH, PLANE_Y_OFFSET, PLANE_Y_OFFSET+PLANE_Y_HEIGHT);
        MLC_RGB_SetAddress((unsigned long)PA_FB0_BASE, (unsigned long)PA_FB0_BASE);
        MLC_RGBOn(MLC_RGB_RGN_1);
        
        DPC->DPC_FPIATV1 = 0xffff;
        DPC->DPC_FPIATV2 = 0xffff;
        DPC->DPC_FPIATV3 = 0xffff;
        

  */
