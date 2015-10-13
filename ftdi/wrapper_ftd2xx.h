/*
 * _base_FT232.h
 *
 *  Created on: Jul 7, 2015
 *      Author: szymek
 */

#ifndef BASE_FT232R_H_
#define BASE_FT232R_H_


#include <stdint.h>
#include <iostream>

#include "ftd2xx_from_FTDI/ftd2xx.h"



#define CAST_TO_LPDWORD(value) static_cast<DWORD*>(static_cast<void*>(value))


/*
 *  Wrapper (from c language)
 */
namespace ftd2xx_t {

	enum class OpenMode : uint32_t {
		SERIAL_NUMBER = 1,
		DESCRIPTION = 2
	};

	enum class Baudrate : uint32_t {
		B300 = 300,
		B600 = 600,
		B1200 = 1200,
		B2400 = 2400,
		B4800 = 4800,
		B9600 = 9600,
		B14400 = 14400,
		B19200 = 19200,
		B38400 = 38400,
		B57600 = 57600,
		B115200 = 115200,
		B230400 = 230400,
		B460800 = 460800,
		B921600 = 921600
	};

	enum class WordLength : uint8_t {
		SEVEN = 7,
		EIGHT = 8
	};

	enum class StopBits : uint8_t {
		ONE = 0,
		TWO = 2
	};

	enum class Parity : uint8_t {
		NONE = 0,
		ODD = 1,
		EVEN = 2,
		MARK = 3,
		SPACE = 4
	};

}

namespace wrapper_ftd2xx {

	inline FT_STATUS open(FT_HANDLE &handler, void *pArg1, uint32_t Flags) noexcept {
		return FT_OpenEx(pArg1,Flags,&handler); 
	}
	inline FT_STATUS close(FT_HANDLE handler) noexcept {
		return FT_Close(handler); 
	}
	inline FT_STATUS resetDevice(FT_HANDLE handler) noexcept {
		return FT_ResetDevice(handler); 
	}

	
	inline FT_STATUS setBaudRate(FT_HANDLE handler, uint32_t BaudRate) noexcept {
		return FT_SetBaudRate(handler,BaudRate); 
	}
	inline FT_STATUS setBaudRate(FT_HANDLE handler, ftd2xx_t::Baudrate BaudRate) noexcept {
		return FT_SetBaudRate(handler,uint32_t(BaudRate));
	}
	inline FT_STATUS setDataCharacteristics(FT_HANDLE handler, uint8_t WordLength, uint8_t StopBits, uint8_t Parity) noexcept {
		return FT_SetDataCharacteristics(handler,WordLength,StopBits,Parity); 
	}
	inline FT_STATUS setDataCharacteristics(FT_HANDLE handler, ftd2xx_t::WordLength wordLength, ftd2xx_t::StopBits stopBits, ftd2xx_t::Parity parity) noexcept {
		return FT_SetDataCharacteristics(handler,UCHAR(wordLength),UCHAR(stopBits),UCHAR(parity));
	}
	inline FT_STATUS setTimeouts(FT_HANDLE handler, uint32_t ReadTimeout, uint32_t WriteTimeout) noexcept {
		return FT_SetTimeouts(handler,ReadTimeout,WriteTimeout);
	}

	
	inline FT_STATUS getQueueStatus(FT_HANDLE handler, uint32_t *RxBytes) noexcept {
		return FT_GetQueueStatus(handler,CAST_TO_LPDWORD(RxBytes)); 
	}
	inline FT_STATUS getStatus(FT_HANDLE handler, uint32_t *RxBytes, uint32_t *TxBytes, uint32_t *EventDWord) noexcept {
		return FT_GetStatus(handler,CAST_TO_LPDWORD(RxBytes),CAST_TO_LPDWORD(TxBytes),CAST_TO_LPDWORD(EventDWord)); 
	}

	
	inline FT_STATUS purge(FT_HANDLE handler, uint32_t Mask) noexcept {
		return FT_Purge(handler,Mask); 
	}
	inline FT_STATUS read(FT_HANDLE handler, uint8_t *Buffer, uint32_t BytesToRead, uint32_t *BytesReturned) noexcept {
		return FT_Read(handler,Buffer,BytesToRead,CAST_TO_LPDWORD(BytesReturned)); 
	}
	inline FT_STATUS write(FT_HANDLE handler, uint8_t *Buffer, uint32_t BytesToWrite, uint32_t *BytesWritten) noexcept {
		return FT_Write(handler,Buffer,BytesToWrite,CAST_TO_LPDWORD(BytesWritten)); 
	}

}


#endif /* BASE_FT232_H_ */
