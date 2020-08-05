
/**
  ******************************************************************************
  * @file     NMEAlib/GPS.c
  * @author   Mohammad Saleh vahedi ,Email: m.salehvahedi@gmail.com
  * @brief   this file Include functions for getting nmea message through uart and parsing
  *          gps data ,converting and calculation function .
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "uart.h"
#include "GPS.h"
#include "GPS_CONFIG.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


GPS_t GPS ;

/* Private functions --------------------------------------------------------- */

/**
  * @brief  this function get speed in knot and return it in  kilometer per hour
  * @param  speed , KPH
  * @retval KPH-kilometer per hour
  *
  */

double KNOT_TO_KPH(double speed)
{
  float KPH = 0.0 ;
  KPH = speed * 1.8 ;
  return KPH ;
  }


  /**
    * @brief  this function convert Latitude and Longitude from degree minute to decimal degree
    * @param  Latitude or Longitude in degree minute
    * @retval Latitude or Longitude in decimal degree
    *
    */

  double DM_TO_DD (float degmin)
{
    double min = 0.0 ;
    double decDeg = 0.0;

    /* get the minutes, fmod() requires double */
    min = fmod((double)degMin, 100.0);

    /* rebuild coordinates in decimal degrees */
    degMin = (int) ( degMin / 100 );
    decDeg = degMin + ( min / 60 );
    return decDeg;
  }


    /**
      * @brief  this function calculate checksum of the passed NMEA message string and return its result
      * @param  pointer to checksum string of desired message (characters between $ and * of message)
      * @retval checksum of characters
      *
      */

  int CheckSum_calculation(char *CheckSum_str)
{
      int crc = 0;

      while (*CheckSum_str)
          crc ^= *CheckSum_str++;

      return crc;
  }

  /**
    * @brief  this function  call uart receive function and initiate the uart temp buffer
    * @param  void
    * @retval void
    *
    */
  void	GPS_Init(void)
{
  	GPS.rx_index=0;
  	HAL_UART_Receive_IT(&_GPS_USART,&GPS.rx_temp,1);
  }


  /**
    * @brief  gps uart callback function
    * @param  void
    * @retval void
    *
    */
  void	GPS_CallBack(void)
{
  	GPS.LastTime=HAL_GetTick();
  	if(GPS.rx_index < sizeof(GPS.rx_Buffer)-2)
  	{
  		GPS.rx_Buffer[GPS.rx_index] = GPS.rx_temp;
  		GPS.rx_indexx++;
  	}
  	HAL_UART_Receive_IT(&_GPS_USART,&GPS.rx_temp,1);
  }



    /**
      * @brief  this function include gps data parser algorithms
      * @param  void
      * @retval void
      *
      */


  void GPS_Parser(void)
{
        char  *message_find;  //pointer to desired message in buffer
        char  *CheckSum_str;  //pointer to string between $ and *
        char  *CheckSum;      // pointer to checksum of message in buffer
        int   crc=0;         //result of checksum calculation function

        if((HAL_GetTick()-GPS.LastTime>50)&& (GPS.rx_index>0))
        {
          message_find=strstr((char*)GPS.rx_Buffer,MESSAGE_ID);
          CheckSum=strstr(message_find,CHECKSUM_SIGN);
          if((message_find != NULL) && (CheckSum != NULL))
          {
            CheckSum = strtok(CheckSum,ENDOF_MESSAGE);
            /* skip the *  */
            CheckSum++;
            sscanf(CheckSum,"%x",GPS.GPRMC.CheckSum)
            CheckSum_str=strtok(message_find,CHECKSUM_SIGN);
            /* skip the $   */
            CheckSum_str++;
            crc=CheckSum_calculation(CheckSum_str);
            if(crc==GPS.GPRMC.CheckSum)
            {
              memset(GPS.GPRMC,0,sizeof(GPS.GPRMC));

              sscanf(message_find,"$GPRMC,%f,%c,%f,%c,%f,%C,%f,%f,%ld",&GPS.GPRMC.UTC_Time,
              &GPS.GPRMC.status,&GPS.GPRMC.Latitude,&GPS.GPRMC.NS_Indicator,&GPS.GPRMC.Longitude,
              &GPS.GPRMC.EW_Indicator,&GPS.GPRMC.Speed_Knot,&GPS.GPRMC.COG,&GPS.GPRMC.Date);

              GPS.GPRMC.Speed_KPH=KNOT_TO_KPH(GPS.GPRMC.Speed_Knot);
              GPS.GPRMC.LatitudeDecimal=DM_TO_DD(GPS.GPRMC.Latitude);
              GPS.GPRMC.LongitudeDecimal=DM_TO_DD(GPS.GPRMC.Longitude);

              memset(GPS.rx_Buffer,0,sizeof(GPS.rx_Buffer));
              GPS.rx_index=0;
            }
          }
        }

        HAL_UART_Receive_IT(&_GPS_USART,&GPS.rx_temp,1);
}
