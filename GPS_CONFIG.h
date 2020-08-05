
/**
  ******************************************************************************
  * @file     NMEAlib/GPS_CONFIG.h
  * @author   Mohammad Saleh vahedi ,Email: m.salehvahedi@gmail.com
  * @brief   this header file Includes Configuration defines
  *
  *
  ******************************************************************************
  */


#ifndef _GPSCONFIG_H_
#define _GPSCONFIG_H_

/* Exported constants --------------------------------------------------------*/
#define	_GPS_USART					huart3
//size of UART receive buffer
#define _GPS_BUFFER_SIZE    512

// NMEA message ID
#define MESSAGE_ID           "$GPRMC"
//each of nmea message end with carriage return and line feed
#define ENDOF_MESSAGE        "\r\n"
//checksum of each message string are located after * sign in hexadecimal
#define CHECKSUM_SIGN        "*"

#endif
