/*
 * UART.h
 *
 *  Created on: Jul 3, 2015
 *      Author: szymek
 */


#ifndef FT232_H_
#define FT232_H_




#include <string>
#include <cstdint>


class FT232R {
public:

	enum class OpenMode : std::uint32_t {
		SERIAL_NUMBER = 1,
		DESCRIPTION = 2
	};

	enum class StdBaud : std::uint32_t {
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

	enum class DataLength : std::uint8_t {
		SEVEN = 7,
		EIGHT = 8
	};

	enum class StopBits : std::uint8_t {
		ONE = 0,
		TWO = 2
	};

	enum class Parity : std::uint8_t {
		NONE = 0,
		ODD = 1,
		EVEN = 2,
		MARK = 3,
		SPACE = 4
	};



	FT232R(char const *deviceName, OpenMode openMode, std::uint32_t baudrate, Parity =Parity::NONE, StopBits =StopBits::ONE, DataLength =DataLength::EIGHT);
	~FT232R();

	void open();
	void close();
	void resetDevice();
	inline bool isOpen() const { return isOpen_; }

	void setBaudrate(std::uint32_t baudrate);
	void setDataLength(DataLength length);
	void setStopBits(StopBits stopBits);
	void setParity(Parity parity);

	void purgeBuffers();
	void purgeRXbuffer();
	void purgeTXbuffer();


	std::uint32_t sendByte(std::uint8_t byte);
	std::uint32_t sendBytes(std::uint8_t *buffer, std::uint32_t bufferLength);

	std::uint32_t readByte(std::uint8_t &byte);
	std::uint32_t readBytes(std::uint8_t *buffer, std::uint32_t bufferLength);

	std::uint32_t getNumberOfBytesInReadBuffer();
	void getNumberOfBytesInBuffers(std::uint32_t &readBuffer, std::uint32_t &writeBuffer);


	static void connectedDevicesList();

private:

	std::string deviceName_;
	std::uint32_t baudrate_;
	DataLength dataLength_;
	StopBits stopBits_;
	Parity parity_;
	OpenMode openMode_;

	bool isOpen_;
	void *ftHandle_;

	FT232R(FT232R&) {}
	void operator=(FT232R) {}

	void handleError(std::string function, std::uint32_t status); // generate exceptions
	std::uint32_t configureTransmissionSettings();

};


#endif
