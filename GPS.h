/**
  ******************************************************************************
  * @file    NMEAlib/GPS.h
  * @author  Mohammad Saleh vahedi , m.salehvahedi@gmail.com
  * @brief   this header file include structs for storing  GPRMC  message  data ,
  *          and  GPS data parsing , converting and calculation functions definition .
  *
  ******************************************************************************
  */


  /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _GPS_H_
#define _GPS_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>


/* Exported types ------------------------------------------------------------*/
typedef struct
{

    uint8_t UTC_Hour;
    uint8_t UTC_Min;
    uint8_t UTC_Sec;
    uint8_t UTC_MicroSec;
    float   UTC_Time;
    char    status ;
    float   Latitude;
    char    NS_Indicator;
     float   Longitude;
    double  LatitudeDecimal;
    double  LongitudeDecimal;
    char    EW_Indicator;
    float   Speed_Knot;
    float   COG;                    //course over ground
    uint8_t Day;
    uint8_t Mont;
    uint8_t Year;
    long int Date;
    uint8_t magnetic_variation ;
    char    EW_Indicator_geodetic;
    char    Mode;
    int  CheckSum;
    float Speed_KPH;

}GPRMC_t;

typedef struct{
    uint8_t rx_Buffer[_GPS_BUFFER_SIZE];
    uint8_t rx_temp;
    uint16_t rx_index;
    uint32_t LastTime;
    GPRMC_t   GPRMC;

}GPS_t;

extern GPS_t GPS;

/* Exported functions ------------------------------------------------------- */
int CheckSum_calculation(char *CheckSum_str);
float KNOT_TO_KPH(float speed);
double DM_TO_DD(float degmin)
void	GPS_Init(void);
void	GPS_CallBack(void);
void	GPS_Parser(void);

#endif
