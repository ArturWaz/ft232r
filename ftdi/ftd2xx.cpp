/*
 * UART.cpp
 *
 *  Created on: Jul 3, 2015
 *      Author: szymek
 */


#include "ftd2xx.h"

#include <stdexcept>
#include <iostream>






ftd2xx::ftd2xx(char const *deviceName, ftd2xx_t::OpenMode openMode, uint32_t baudrate, ftd2xx_t::Parity parity, ftd2xx_t::StopBits stopBits, ftd2xx_t::WordLength wordLength) noexcept :
				ftHandle_(nullptr),
				isOpen_(false),
				deviceName_(deviceName),
				openMode_(openMode) {

	char const * const funcName = "ftd2xx(char const *, OpenMode, uint32_t, ...)";

	if (wrapper_ftd2xx::open(ftHandle_,(void*)(deviceName_.c_str()),uint32_t(openMode)) == FT_OK) {
		isOpen_ = true;
		handleError(funcName, wrapper_ftd2xx::setBaudRate(ftHandle_,baudrate));
		handleError(funcName, wrapper_ftd2xx::setDataCharacteristics(ftHandle_,wordLength,stopBits,parity));
	}
}


ftd2xx::~ftd2xx() noexcept {
	if (isOpen_) {
		wrapper_ftd2xx::close(ftHandle_);
		isOpen_ = false;
	}
}



void ftd2xx::open() noexcept {
	if (isOpen_) return;
	handleError("open()", wrapper_ftd2xx::open(ftHandle_,(void*)(deviceName_.c_str()),uint32_t(openMode_)));
	isOpen_ = true;
}

void ftd2xx::close() noexcept {
	if (!isOpen_) return;
	handleError("close()", wrapper_ftd2xx::close(ftHandle_));
	isOpen_ = false;
}

void ftd2xx::resetDevice() noexcept {
	handleError("resetDevice()", wrapper_ftd2xx::resetDevice(ftHandle_));
}


void ftd2xx::setBaudrate(uint32_t baudrate) noexcept {
	handleError("setBaudrate(uint32_t)", wrapper_ftd2xx::setBaudRate(ftHandle_,baudrate));
}
void ftd2xx::setBaudrate(ftd2xx_t::Baudrate baudrate) noexcept {
	handleError("setBaudrate(uint32_t)", wrapper_ftd2xx::setBaudRate(ftHandle_,baudrate));
}
void ftd2xx::setTimeouts(uint32_t readTimeout, uint32_t writeTimeout) noexcept {
	handleError("setTimeouts(uint32_t)",FT_SetTimeouts(ftHandle_,readTimeout,writeTimeout));
}
void ftd2xx::setTransmissionSettings(ftd2xx_t::Parity parity, ftd2xx_t::StopBits stopBits, ftd2xx_t::WordLength wordLength) noexcept {
	handleError("setTransmissionSettings(Parity, StopBits, WordLength)", wrapper_ftd2xx::setDataCharacteristics(ftHandle_,wordLength,stopBits,parity));
}


void ftd2xx::purgeBuffers() noexcept {
	handleError("purgeBuffers()", wrapper_ftd2xx::purge(ftHandle_,FT_PURGE_RX | FT_PURGE_TX));
}
void ftd2xx::purgeRXbuffer() noexcept {
	handleError("purgeBuffers()", wrapper_ftd2xx::purge(ftHandle_,FT_PURGE_RX));
}
void ftd2xx::purgeTXbuffer() noexcept {
	handleError("purgeBuffers()", wrapper_ftd2xx::purge(ftHandle_,FT_PURGE_TX));
}


uint32_t ftd2xx::getNumberOfBytesInReadBuffer() noexcept {
	uint32_t bytesRead;
	FT_STATUS ftStatus = wrapper_ftd2xx::getQueueStatus(ftHandle_,&bytesRead);
	if (ftStatus != FT_OK) handleError("getNumberOfBytesInReadBuffer()", ftStatus);
	return bytesRead;
}


void ftd2xx::getNumberOfBytesInBuffers(uint32_t &readBuffer, uint32_t &writeBuffer) noexcept {
	uint32_t nothing;
	FT_STATUS ftStatus = wrapper_ftd2xx::getStatus(ftHandle_,&readBuffer,&writeBuffer,&nothing);
	if (ftStatus != FT_OK) handleError("getNumberOfBytesInBuffers(uint32_t &, uint32_t &)", ftStatus);
}


uint32_t ftd2xx::sendByte(uint8_t byte) noexcept {
	uint32_t bytesWritten;
	FT_STATUS ftStatus = wrapper_ftd2xx::write(ftHandle_,&byte,1,&bytesWritten);
	if (ftStatus != FT_OK) handleError("sendByte(uint8_t)", ftStatus);
	return bytesWritten;
}


uint32_t ftd2xx::sendBytes(uint8_t const *buffer, uint32_t bufferLength) noexcept {
	uint32_t bytesWritten;
	FT_STATUS ftStatus = wrapper_ftd2xx::write(ftHandle_,const_cast<uint8_t*>(buffer),bufferLength,&bytesWritten);
	if (ftStatus != FT_OK) handleError("sendBytes(uint8_t *, uint32_t)", ftStatus);
	return bytesWritten;
}


uint32_t ftd2xx::readByte(uint8_t &byte) noexcept {
	uint32_t bytesRead;
	FT_STATUS ftStatus = wrapper_ftd2xx::getQueueStatus(ftHandle_,&bytesRead);
	if (ftStatus != FT_OK) handleError("readBytes(uint8_t *, uint32_t)",ftStatus);
	if (bytesRead == 0) return 0;
	ftStatus = wrapper_ftd2xx::read(ftHandle_,&byte,1,&bytesRead);
	if (ftStatus != FT_OK) handleError("readBytes(uint8_t *, uint32_t)",ftStatus);
	return bytesRead;
}


uint32_t ftd2xx::readBytes(uint8_t *buffer, uint32_t bufferLength) noexcept {
	uint32_t bytesRead;
	FT_STATUS ftStatus = wrapper_ftd2xx::getQueueStatus(ftHandle_,&bytesRead);
	if (ftStatus != FT_OK) handleError("readBytes(uint8_t *, uint32_t)",ftStatus);
	if (bytesRead == 0) return 0;
	if (bufferLength < bytesRead) bytesRead = bufferLength;
	ftStatus = wrapper_ftd2xx::read(ftHandle_,buffer,bytesRead,&bytesRead);
	if (ftStatus != FT_OK) handleError("readBytes(uint8_t *, uint32_t)",ftStatus);
	return bytesRead;
}


uint32_t ftd2xx::readBytes(uint8_t *buffer, uint32_t bufferLength, uint32_t bytesToRead) noexcept {
	if (bytesToRead == 0) return 0;
	if (bufferLength < bytesToRead) bytesToRead = bufferLength;
	uint32_t bytesRead;
	FT_STATUS ftStatus = wrapper_ftd2xx::read(ftHandle_,buffer,bytesToRead,&bytesRead);
	if (ftStatus != FT_OK) handleError("readBytes(uint8_t *, uint32_t)",ftStatus);
	return bytesRead;
}



void ftd2xx::connectedDevicesList() noexcept {
	FT_STATUS ftStatus;
	FT_DEVICE_LIST_INFO_NODE *devInfo;
	DWORD numDevs;
	// create the device information list
	ftStatus = FT_CreateDeviceInfoList(&numDevs);
	if (ftStatus == FT_OK) {
		std::cout << "Number of devices is " << numDevs << ".\n";
	}
	if (numDevs > 0) {
		// allocate storage for list based on numDevs
		devInfo = (FT_DEVICE_LIST_INFO_NODE*)malloc(sizeof(FT_DEVICE_LIST_INFO_NODE)*numDevs);
		// get the device information list
		ftStatus = FT_GetDeviceInfoList(devInfo,&numDevs);
		if (ftStatus == FT_OK) {
			for (int i = 0; i < numDevs; i++) {
				std::cout << " Dev " << i << ":\n";
				std::cout << std::hex;
				std::cout << "    Flags = 0x" << devInfo[i].Flags << std::endl;
				std::cout << "    Type = 0x" << devInfo[i].Type << std::endl;
				std::cout << "    ID = 0x" << devInfo[i].ID << std::endl;
				std::cout << "    LocId = 0x" << devInfo[i].LocId << std::endl;
				std::cout << "    Serial number = " << devInfo[i].SerialNumber << std::endl;
				std::cout << "    Description = " << devInfo[i].Description << std::endl;
				std::cout << "    ftHandle = 0x" << devInfo[i].ftHandle << std::endl;
				std::cout << std::dec;
			}
		}
	}
}



void ftd2xx::handleError(std::string function, uint32_t status) {
	switch(status) {
	case FT_OK:
		return;
	case FT_INVALID_HANDLE:
		throw std::logic_error(std::string("[ ftd2xx::") + function + std::string(" ]: Invalid handler.\n"));
	case FT_DEVICE_NOT_FOUND:
		throw std::logic_error(std::string("[ ftd2xx::") + function + std::string(" ]: Cannot find device.\n"));
	case FT_DEVICE_NOT_OPENED:
		throw std::logic_error(std::string("[ ftd2xx::") + function + std::string(" ]: Device is not opened.\n"));
	case FT_IO_ERROR:
		throw std::logic_error(std::string("[ ftd2xx::") + function + std::string(" ]: Input/output error.\n"));
	case FT_INSUFFICIENT_RESOURCES:
		throw std::logic_error(std::string("[ ftd2xx::") + function + std::string(" ]: Insufficient resources.\n"));
	case FT_INVALID_PARAMETER:
		throw std::logic_error(std::string("[ ftd2xx::") + function + std::string(" ]: Invalid parameters.\n"));
	case FT_INVALID_BAUD_RATE:
		throw std::logic_error(std::string("[ ftd2xx::") + function + std::string(" ]: Invalid baudrate.\n"));
	default:
		throw std::logic_error(std::string("[ ftd2xx::") + function + std::string(" ]: Unknown error.\n"));
	}
}




