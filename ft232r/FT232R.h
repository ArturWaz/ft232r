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


class FT232R {
public:

	enum class OpenMode : uint32_t {
		SERIAL_NUMBER = 1,
		DESCRIPTION = 2
	};

	enum class StdBaud : uint32_t {
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

	enum class DataLength : uint8_t {
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



	FT232R(char const *deviceName, OpenMode openMode, uint32_t baudrate, Parity =Parity::NONE, StopBits =StopBits::ONE, DataLength =DataLength::EIGHT);
	~FT232R();

	void open();
	void close();
	void resetDevice();
	inline bool isOpen() const { return isOpen_; }

	void setBaudrate(uint32_t baudrate);
	void setTimeouts(uint32_t readTimeout, uint32_t writeTimeout);
	void setDataLength(DataLength length);
	void setStopBits(StopBits stopBits);
	void setParity(Parity parity);

	void purgeBuffers();
	void purgeRXbuffer();
	void purgeTXbuffer();


	uint32_t sendByte(uint8_t byte);
	uint32_t sendBytes(uint8_t *buffer, uint32_t bufferLength);

	uint32_t readByte(uint8_t &byte);
	uint32_t readBytes(uint8_t *buffer, uint32_t bufferLength);

	uint32_t getNumberOfBytesInReadBuffer();
	void getNumberOfBytesInBuffers(uint32_t &readBuffer, uint32_t &writeBuffer);


	static void connectedDevicesList();


private:

	std::string deviceName_;
	uint32_t baudrate_;
	DataLength dataLength_;
	StopBits stopBits_;
	Parity parity_;
	OpenMode openMode_;
	uint32_t readTimeout_;
	uint32_t writeTimeout_;

	bool isOpen_;
	void *ftHandle_;

	FT232R(FT232R&) {}
	void operator=(FT232R) {}

	void handleError(std::string function, uint32_t status); // generate exceptions
	uint32_t configureTransmissionSettings();

};


#endif
