#ifndef __HX711_H
#define __HX711_H

#define HX711_SCK  PAout(6)
#define HX711_DOUT   PAin(7)
#define HX711_SCK2  PAout(4)
#define HX711_DOUT2   PAin(5)
#define HX711_SCK3  PBout(5)
#define HX711_DOUT3   PBin(6)
typedef uint8_t u8;
typedef uint32_t u32;
typedef int32_t s32;
extern void Init_HX711pin(void);
extern void HX711_Init3(void);
extern void Get_Maopi1(void);
extern s32 Get_Weight1(void);
extern void Get_Maopi2(void);
extern s32 Get_Weight2(void);
extern void Get_Maopi3(void);
extern s32 Get_Weight3(void);
#endif
