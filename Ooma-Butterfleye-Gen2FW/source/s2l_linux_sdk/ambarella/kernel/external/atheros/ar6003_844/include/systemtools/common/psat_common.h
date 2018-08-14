#if !defined(_PSAT_COMMON_CODE)
#define  _PSAT_COMMON_CODE

#define DEVIDE_COEFF 10000

// CMAC to power lookup table, for platform such as embedded without sophisticated math function 
typedef struct {
    A_UINT32 cmac;
    A_INT32  pwr_t10;
} _CMAP_PWR_MAPPING;

_CMAP_PWR_MAPPING CmacPwrLkupTbl[] = { 
{100000, -433},
{110000, -429},
{120000, -425},
{130000, -421},
{140000, -418},
{150000, -415},
{160000, -412},
{170000, -410},
{180000, -407},
{190000, -405},
{200000, -403},
{210000, -400},
{220000, -398},
{230000, -397},
{240000, -395},
{250000, -393},
{260000, -391},
{280000, -388},
{290000, -386},
{310000, -384},
{330000, -381},
{350000, -378},
{380000, -375},
{400000, -372},
{430000, -369},
{460000, -366},
{500000, -363},
{550000, -359},
{600000, -355},
{650000, -351},
{700000, -348},
{750000, -345},
{800000, -342},
{850000, -340},
{900000, -337},
{950000, -335},
{1000000, -333},
{1100000, -329},
{1200000, -325},
{1300000, -321},
{1400000, -318},
{1500000, -315},
{1600000, -312},
{1700000, -310},
{1800000, -307},
{1900000, -305},
{2000000, -303},
{2100000, -300},
{2200000, -298},
{2400000, -295},
{2500000, -293},
{2700000, -290},
{2800000, -288},
{3000000, -285},
{3200000, -282},
{3500000, -278},
{3800000, -275},
{4000000, -272},
{4200000, -270},
{4400000, -268},
{4800000, -265},
{5200000, -261},
{5400000, -259},
{5800000, -256},
{6200000, -253},
{6800000, -249},
{7400000, -246},
{7800000, -243},
{8400000, -240},
{8600000, -239},
{9300000, -236},
{10200000, -232},
{10800000, -229},
{11700000, -226},
{12400000, -223},
{13600000, -219},
{14800000, -216},
{15600000, -213},
{16800000, -210},
{17600000, -208},
{18800000, -205},
{20500000, -202},
{21500000, -199},
{23500000, -196},
{25000000, -193},
{26500000, -190},
{29000000, -186},
{33500000, -180},
{42000000, -170},
{53000000, -160},
{67000000, -150},
{84000000, -140},
{106000000, -130},
{135000000, -120},
{155000000, -114},
{175000000, -109},
{200000000, -103},
{230000000, -97},
{265000000, -91},
{300000000, -85},
{330000000, -81},
{360000000, -78},
{390000000, -74},
{430000000, -70},
{470000000, -66},
{520000000, -62},
{570000000, -58},
{620000000, -54},
{680000000, -50},
{750000000, -46},
};
#define CMAC_PWR_LOOKUP_MAX (sizeof(CmacPwrLkupTbl) / sizeof(_CMAP_PWR_MAPPING))

A_INT32
interpolate_round(A_INT32 target, A_INT32 srcLeft, A_INT32 srcRight,
            A_INT32 targetLeft, A_INT32 targetRight, A_INT32 roundUp)
{
    A_INT32 rv, tmp;

    if (srcRight == srcLeft) {
        rv = targetLeft;
    } else {
#if 0
        rv = (((((target - srcLeft) * targetRight +
              (srcRight - target) * targetLeft)) * roundUp)) / (srcRight - srcLeft);
        if (rv < 0) {
            rv -= (roundUp/2); 
        } else {
            rv += (roundUp/2); 
        }

        rv /= roundUp;
#endif
#if 0
        rv = (((((target - srcLeft) * targetRight +
              (srcRight - target) * targetLeft)) )) / (srcRight - srcLeft);
#endif
        tmp = srcRight - srcLeft;
        rv = (((target - srcLeft) * targetRight) / tmp) +
             (((srcRight - target) * targetLeft) / tmp);
    }

    return(rv);
}


A_INT16 cmac2Pwr_t10(A_UINT32 cmac)
{
   A_INT32 i;
   A_INT16 cmacResult;
   for (i=0;i<CMAC_PWR_LOOKUP_MAX;i++) {
       if (cmac < CmacPwrLkupTbl[i].cmac) {
           break;
       }
   } 
   if (0 == i) {
       return(CmacPwrLkupTbl[0].pwr_t10);
   }
   else if (CMAC_PWR_LOOKUP_MAX == i) {
       return(CmacPwrLkupTbl[CMAC_PWR_LOOKUP_MAX -1].pwr_t10);
   }
   else {
        /* Added a "DEVIDE_COEFF" to avoid the overflow */	
        cmacResult = interpolate_round((cmac / DEVIDE_COEFF), (CmacPwrLkupTbl[i-1].cmac / DEVIDE_COEFF), (CmacPwrLkupTbl[i].cmac / DEVIDE_COEFF), 
            CmacPwrLkupTbl[i-1].pwr_t10, CmacPwrLkupTbl[i].pwr_t10, 10);
   }

   return(cmacResult);
}

#endif //#if !defined(_PSAT_COMMON_CODE)

