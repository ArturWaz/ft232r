/*
 * UART.cpp
 *
 *  Created on: Jul 3, 2015
 *      Author: szymek
 */


#include "FT232R.h"

#include <stdexcept>
#include <iostream>

#include "ftdi/ftd2xx.h"





FT232R::FT232R(char const *deviceName, _ftd2xx::OpenMode openMode, uint32_t baudrate, _ftd2xx::Parity parity, _ftd2xx::StopBits stopBits, _ftd2xx::WordLength wordLength) noexcept :
				ftHandle_(nullptr),
				isOpen_(false),
				deviceName_(deviceName),
				openMode_(openMode) {

	char const * const funcName = "FT232R(char const *, OpenMode, uint32_t, ...)";

	if (_ftd2xx::open(ftHandle_,(void*)(deviceName_.c_str()),uint32_t(openMode)) == FT_OK) {
		isOpen_ = true;
		handleError(funcName,_ftd2xx::setBaudRate(ftHandle_,baudrate));
		handleError(funcName,_ftd2xx::setDataCharacteristics(ftHandle_,wordLength,stopBits,parity));
	}
}


FT232R::~FT232R() noexcept {
	if (isOpen_) {
		_ftd2xx::close(ftHandle_);
		isOpen_ = false;
	}
}



void FT232R::open() noexcept {
	if (isOpen_) return;
	handleError("open(char const*,OpenMode)",_ftd2xx::open(ftHandle_,(void*)(deviceName_.c_str()),uint32_t(openMode_)));
	isOpen_ = true;
}

void FT232R::close() noexcept {
	if (!isOpen_) return;
	handleError("close()",_ftd2xx::close(ftHandle_));
	isOpen_ = false;
}

void FT232R::resetDevice() noexcept {
	handleError("resetDevice()",_ftd2xx::resetDevice(ftHandle_));
}


void FT232R::setBaudrate(uint32_t baudrate) noexcept {
	handleError("setBaudrate(uint32_t)",_ftd2xx::setBaudRate(ftHandle_,baudrate));
}
void FT232R::setBaudrate(_ftd2xx::Baudrate baudrate) noexcept {
	handleError("setBaudrate(uint32_t)",_ftd2xx::setBaudRate(ftHandle_,baudrate));
}
void FT232R::setTimeouts(uint32_t readTimeout, uint32_t writeTimeout) noexcept {
	handleError("setTimeouts(uint32_t)",FT_SetTimeouts(ftHandle_,readTimeout,writeTimeout));
}
void FT232R::setTransmissionSettings(_ftd2xx::Parity parity, _ftd2xx::StopBits stopBits, _ftd2xx::WordLength wordLength) noexcept {
	handleError("setTransmissionSettings(Parity, StopBits, WordLength)",_ftd2xx::setDataCharacteristics(ftHandle_,wordLength,stopBits,parity));
}


void FT232R::purgeBuffers() noexcept {
	handleError("purgeBuffers()",_ftd2xx::purge(ftHandle_,FT_PURGE_RX | FT_PURGE_TX));
}
void FT232R::purgeRXbuffer() noexcept {
	handleError("purgeBuffers()",_ftd2xx::purge(ftHandle_,FT_PURGE_RX));
}
void FT232R::purgeTXbuffer() noexcept {
	handleError("purgeBuffers()",_ftd2xx::purge(ftHandle_,FT_PURGE_TX));
}


uint32_t FT232R::getNumberOfBytesInReadBuffer() noexcept {
	uint32_t bytesRead;
	FT_STATUS ftStatus = _ftd2xx::getQueueStatus(ftHandle_,&bytesRead);
	if (ftStatus != FT_OK) handleError("getNumberOfBytesInReadBuffer()", ftStatus);
	return bytesRead;
}


void FT232R::getNumberOfBytesInBuffers(uint32_t &readBuffer, uint32_t &writeBuffer) noexcept {
	uint32_t nothing;
	FT_STATUS ftStatus = _ftd2xx::getStatus(ftHandle_,&readBuffer,&writeBuffer,&nothing);
	if (ftStatus != FT_OK) handleError("getNumberOfBytesInBuffers(uint32_t &, uint32_t &)", ftStatus);
}


uint32_t FT232R::sendByte(uint8_t byte) noexcept {
	uint32_t bytesWritten;
	FT_STATUS ftStatus = _ftd2xx::write(ftHandle_,&byte,1,&bytesWritten);
	if (ftStatus != FT_OK) handleError("sendByte(uint8_t)", ftStatus);
	return bytesWritten;
}


uint32_t FT232R::sendBytes(uint8_t const *buffer, uint32_t bufferLength) noexcept {
	uint32_t bytesWritten;
	FT_STATUS ftStatus = _ftd2xx::write(ftHandle_,const_cast<uint8_t*>(buffer),bufferLength,&bytesWritten);
	if (ftStatus != FT_OK) handleError("sendBytes(uint8_t *, uint32_t)", ftStatus);
	return bytesWritten;
}


uint32_t FT232R::readByte(uint8_t &byte) noexcept {
	uint32_t bytesRead;
	FT_STATUS ftStatus = _ftd2xx::getQueueStatus(ftHandle_,&bytesRead);
	if (ftStatus != FT_OK) handleError("readBytes(uint8_t *, uint32_t)",ftStatus);
	if (bytesRead == 0) return 0;
	ftStatus = _ftd2xx::read(ftHandle_,&byte,1,&bytesRead);
	if (ftStatus != FT_OK) handleError("readBytes(uint8_t *, uint32_t)",ftStatus);
	return bytesRead;
}


uint32_t FT232R::readBytes(uint8_t *buffer, uint32_t bufferLength) noexcept {
	uint32_t bytesRead;
	FT_STATUS ftStatus = _ftd2xx::getQueueStatus(ftHandle_,&bytesRead);
	if (ftStatus != FT_OK) handleError("readBytes(uint8_t *, uint32_t)",ftStatus);
	if (bytesRead == 0) return 0;
	if (bufferLength < bytesRead) bytesRead = bufferLength;
	ftStatus = _ftd2xx::read(ftHandle_,buffer,bytesRead,&bytesRead);
	if (ftStatus != FT_OK) handleError("readBytes(uint8_t *, uint32_t)",ftStatus);
	return bytesRead;
}


uint32_t FT232R::readBytes(uint8_t *buffer, uint32_t bufferLength, uint32_t bytesToRead) noexcept {
	if (bytesToRead == 0) return 0;
	if (bufferLength < bytesToRead) bytesToRead = bufferLength;
	uint32_t bytesRead;
	FT_STATUS ftStatus = _ftd2xx::read(ftHandle_,buffer,bytesToRead,&bytesRead);
	if (ftStatus != FT_OK) handleError("readBytes(uint8_t *, uint32_t)",ftStatus);
	return bytesRead;
}



void FT232R::connectedDevicesList() noexcept {
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



void FT232R::handleError(std::string function, uint32_t status) {
	switch(status) {
	case FT_OK:
		return;
	case FT_INVALID_HANDLE:
		throw std::logic_error(std::string("[ FT232R::") + function + std::string(" ]: Invalid handler.\n"));
	case FT_DEVICE_NOT_FOUND:
		throw std::logic_error(std::string("[ FT232R::") + function + std::string(" ]: Cannot find device.\n"));
	case FT_DEVICE_NOT_OPENED:
		throw std::logic_error(std::string("[ FT232R::") + function + std::string(" ]: Device is not opened.\n"));
	case FT_IO_ERROR:
		throw std::logic_error(std::string("[ FT232R::") + function + std::string(" ]: Input/output error.\n"));
	case FT_INSUFFICIENT_RESOURCES:
		throw std::logic_error(std::string("[ FT232R::") + function + std::string(" ]: Insufficient resources.\n"));
	case FT_INVALID_PARAMETER:
		throw std::logic_error(std::string("[ FT232R::") + function + std::string(" ]: Invalid parameters.\n"));
	case FT_INVALID_BAUD_RATE:
		throw std::logic_error(std::string("[ FT232R::") + function + std::string(" ]: Invalid baudrate.\n"));
	default:
		throw std::logic_error(std::string("[ FT232R::") + function + std::string(" ]: Unknown error.\n"));
	}
}




