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


#ifndef CAST_TO_LPDWORD
#define CAST_TO_LPDWORD(value) static_cast<DWORD*>(static_cast<void*>(value))
#endif


FT232R::FT232R(char const *deviceName, OpenMode openMode, uint32_t baudrate, Parity parity, StopBits stopBits, DataLength length)
				: deviceName_(deviceName),
				  baudrate_(baudrate),
				  dataLength_(length),
				  stopBits_(stopBits),
				  parity_(parity),
				  openMode_(openMode),
				  isOpen_(false),
				  ftHandle_(nullptr) {}


FT232R::~FT232R() {	if (isOpen_) close(); }



void FT232R::open() {
	if (isOpen_) return;
	FT_STATUS ftStatus = FT_OpenEx((void*)(deviceName_.c_str()),static_cast<DWORD>(openMode_),&ftHandle_);
	if (ftStatus != FT_OK) handleError("open()", ftStatus);
	isOpen_ = true;
	setBaudrate(baudrate_);
	setDataLength(dataLength_);
	setParity(parity_);
	purgeBuffers();
}


void FT232R::close() {
	FT_STATUS ftStatus = FT_Close(ftHandle_);
	if (ftStatus != FT_OK) handleError("close()", ftStatus);
	isOpen_ = false;
}


void FT232R::resetDevice() {
	FT_STATUS ftStatus = FT_ResetDevice(ftHandle_);
	if (ftStatus != FT_OK) handleError("resetDevice()", ftStatus);
}


void FT232R::setBaudrate(uint32_t baudrate) {
	baudrate_ = baudrate;
	FT_STATUS ftStatus = FT_SetBaudRate(ftHandle_,baudrate_);
	if (ftStatus != FT_OK) handleError("setBaudrate(uint32_t)", ftStatus);
}


void FT232R::setDataLength(DataLength length) {
	dataLength_ = length;
	FT_STATUS ftStatus = configureTransmissionSettings();
	if (ftStatus != FT_OK) handleError("setDataLenght(DataLength)", ftStatus);
}


void FT232R::setStopBits(StopBits stopBits) {
	stopBits_ = stopBits;
	FT_STATUS ftStatus = configureTransmissionSettings();
	if (ftStatus != FT_OK) handleError("setStopBits(StopBits)", ftStatus);
}


void FT232R::setParity(Parity parity) {
	parity_ = parity;
	FT_STATUS ftStatus = configureTransmissionSettings();
	if (ftStatus != FT_OK) handleError("setParity(Parity)", ftStatus);
}


void FT232R::purgeBuffers() {
	FT_STATUS ftStatus = FT_Purge(ftHandle_, FT_PURGE_RX | FT_PURGE_TX);
	if (ftStatus != FT_OK) handleError("purgeBuffers()", ftStatus);
}


void FT232R::purgeRXbuffer() {
	FT_STATUS ftStatus = FT_Purge(ftHandle_, FT_PURGE_RX);
	if (ftStatus != FT_OK) handleError("purgeRXbuffer()", ftStatus);
}


void FT232R::purgeTXbuffer() {
	FT_STATUS ftStatus = FT_Purge(ftHandle_, FT_PURGE_TX);
	if (ftStatus != FT_OK) handleError("purgeTXbuffer()", ftStatus);
}


uint32_t FT232R::getNumberOfBytesInReadBuffer() {
	uint32_t bytesRead;
	FT_STATUS ftStatus = FT_GetQueueStatus(ftHandle_,CAST_TO_LPDWORD(&bytesRead));
	if (ftStatus != FT_OK) handleError("getNumberOfBytesInReadBuffer()", ftStatus);
	return bytesRead;
}


void FT232R::getNumberOfBytesInBuffers(uint32_t &readBuffer, uint32_t &writeBuffer) {
	uint32_t nothing;
	FT_STATUS ftStatus = FT_GetStatus(ftHandle_,CAST_TO_LPDWORD(&readBuffer),CAST_TO_LPDWORD(&writeBuffer),CAST_TO_LPDWORD(&nothing));
	if (ftStatus != FT_OK) handleError("getNumberOfBytesInBuffers(uint32_t &, uint32_t &)", ftStatus);
}


uint32_t FT232R::sendByte(uint8_t byte) {
	uint32_t bytesWritten;
	FT_STATUS ftStatus = FT_Write(ftHandle_, &byte, 1, CAST_TO_LPDWORD(&bytesWritten));
	if (ftStatus != FT_OK) handleError("sendByte(uint8_t)", ftStatus);
	return bytesWritten;
}


uint32_t FT232R::sendBytes(uint8_t *buffer, uint32_t bufferLength) {
	uint32_t bytesWritten;
	FT_STATUS ftStatus = FT_Write(ftHandle_, buffer, bufferLength, CAST_TO_LPDWORD(&bytesWritten));
	if (ftStatus != FT_OK) handleError("sendBytes(uint8_t *, uint32_t)", ftStatus);
	return bytesWritten;
}


uint32_t FT232R::readByte(uint8_t &byte) {
	uint32_t bytesRead;
	FT_STATUS ftStatus = FT_GetQueueStatus(ftHandle_,CAST_TO_LPDWORD(&bytesRead));
	if (ftStatus != FT_OK) handleError("readBytes(uint8_t *, uint32_t)", ftStatus);
	if (bytesRead == 0) return 0;
	ftStatus = FT_Read(ftHandle_, &byte, 1, CAST_TO_LPDWORD(&bytesRead));
	if (ftStatus != FT_OK) handleError("readBytes(uint8_t *, uint32_t)", ftStatus);
	return bytesRead;
}


uint32_t FT232R::readBytes(uint8_t *buffer, uint32_t bufferLength) {
	uint32_t bytesRead;
	FT_STATUS ftStatus = FT_GetQueueStatus(ftHandle_,CAST_TO_LPDWORD(&bytesRead));
	if (ftStatus != FT_OK) handleError("readBytes(uint8_t *, uint32_t)", ftStatus);
	if (bytesRead == 0) return 0;
	if (bufferLength < bytesRead) bytesRead = bufferLength;
	ftStatus = FT_Read(ftHandle_, buffer, bytesRead, CAST_TO_LPDWORD(&bytesRead));
	if (ftStatus != FT_OK) handleError("readBytes(uint8_t *, uint32_t)", ftStatus);
	return bytesRead;
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


uint32_t FT232R::configureTransmissionSettings() {
	return FT_SetDataCharacteristics(ftHandle_,uint8_t(dataLength_),uint8_t(stopBits_),uint8_t(parity_));
}


void FT232R::connectedDevicesList() {
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
