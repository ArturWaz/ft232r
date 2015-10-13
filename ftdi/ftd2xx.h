/*
 * UART.h
 *
 *  Created on: Jul 3, 2015
 *      Author: szymek
 */


#ifndef FT232_H_
#define FT232_H_




#include <string>
#include <stdint.h>

#include "wrapper_ftd2xx.h"


class ftd2xx {
public:

	ftd2xx(char const *deviceName, ftd2xx_t::OpenMode openMode, uint32_t baudrate, ftd2xx_t::Parity = ftd2xx_t::Parity::NONE, ftd2xx_t::StopBits = ftd2xx_t::StopBits::ONE, ftd2xx_t::WordLength = ftd2xx_t::WordLength::EIGHT) noexcept;
	~ftd2xx() noexcept;

	void open() noexcept;
	void close() noexcept;
	void resetDevice() noexcept;
	inline bool isOpen() const noexcept { return isOpen_; }


    void setBaudrate(uint32_t baudrate) noexcept;
	void setBaudrate(ftd2xx_t::Baudrate baudrate) noexcept;
	void setTransmissionSettings(ftd2xx_t::Parity, ftd2xx_t::StopBits, ftd2xx_t::WordLength) noexcept;


    void setTimeouts(uint32_t readTimeout, uint32_t writeTimeout) noexcept;

	void purgeBuffers() noexcept;
	void purgeRXbuffer() noexcept;
	void purgeTXbuffer() noexcept;


	uint32_t sendByte(uint8_t byte) noexcept;
	uint32_t sendBytes(uint8_t const *buffer, uint32_t bufferLength) noexcept;

	uint32_t readByte(uint8_t &byte) noexcept;
	uint32_t readBytes(uint8_t *buffer, uint32_t bufferLength) noexcept;
	uint32_t readBytes(uint8_t *buffer, uint32_t bufferLength, uint32_t bytesToRead) noexcept;

	uint32_t getNumberOfBytesInReadBuffer() noexcept;
	void getNumberOfBytesInBuffers(uint32_t &readBuffer, uint32_t &writeBuffer) noexcept;


	inline static void connectedDevicesList() noexcept;


    FT_HANDLE const &operator*() const noexcept { return ftHandle_; }

private:

	FT_HANDLE ftHandle_;
    bool isOpen_;

    std::string deviceName_;
	ftd2xx_t::OpenMode openMode_;

	ftd2xx(ftd2xx &) {}
	void operator=(ftd2xx) {}

	void handleError(std::string function, uint32_t status); // generate exceptions

};


#endif
