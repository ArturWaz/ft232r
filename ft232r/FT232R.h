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

#include "_ftd2xx.h"


class FT232R {
public:

	FT232R(char const *deviceName, _ftd2xx::OpenMode openMode, uint32_t baudrate, _ftd2xx::Parity =_ftd2xx::Parity::NONE, _ftd2xx::StopBits =_ftd2xx::StopBits::ONE, _ftd2xx::WordLength =_ftd2xx::WordLength::EIGHT) noexcept;
	~FT232R() noexcept;

	void open() noexcept;
	void close() noexcept;
	void resetDevice() noexcept;
	inline bool isOpen() const noexcept { return isOpen_; }


    void setBaudrate(uint32_t baudrate) noexcept;
	void setBaudrate(_ftd2xx::Baudrate baudrate) noexcept;
	void setTransmissionSettings(_ftd2xx::Parity, _ftd2xx::StopBits, _ftd2xx::WordLength) noexcept;


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
    _ftd2xx::OpenMode openMode_;

	FT232R(FT232R&) {}
	void operator=(FT232R) {}

	void handleError(std::string function, uint32_t status); // generate exceptions

};


#endif
