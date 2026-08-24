#ifndef __HX711_H
#define __HX711_H

#define HX711_SCK  PAout(6)
#define HX711_DOUT   PAin(7)
typedef uint8_t u8;
typedef uint32_t u32;
typedef int32_t s32;
extern void Init_HX711pin(void);
extern void Get_Maopi(void);
extern s32 Get_Weight(void);
#endif
