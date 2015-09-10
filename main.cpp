/*
 * main.cpp
 *
 *  Created on: Jul 3, 2015
 *      Author: szymek
 */



#include "ft232r/FT232R.h"
#include "ft232r/_ftd2xx.h"
#include "test.h"

#include <exception>
#include <stdexcept>
#include <iostream>
//
// #include <windows.h>



using namespace std;

void deviceList();
void test();

int main() {

	//FT232R::connectedDevicesList();
	//test();


	//testClass::connectedDevicesList();

}



void test() {
	FT232R f("FT232R ElectroPark", _ftd2xx::OpenMode::DESCRIPTION, 3000000);
	f.open();

	cout << "Number of bytes in buffer: " << f.getNumberOfBytesInReadBuffer() << endl;

	uint32_t lol0, lol1;
	f.getNumberOfBytesInBuffers(lol0,lol1);

	uint8_t lol = 0;
	uint32_t j = 'a';
	while (f.sendByte(lol) == 1 && j < 10000) {
		//cout << "Sent byte: " << lol << endl;
		//lol += 1;
		j += 1;
		if (!(j%100))
			cout << "Number of bytes in buffer: " << f.getNumberOfBytesInReadBuffer() << endl;
		//Sleep(5);
	}

	//Sleep(1000);
	cout << "Number of bytes in buffer: " << f.getNumberOfBytesInReadBuffer() << endl;
	/*Sleep(500);


	uint8_t buffer[500] = {};
	uint32_t read = f.readBytes(buffer,500);
	cout << "Read bytes [" << read << "]: ";
	for (uint32_t i = 0; i < read; ++i) {
		cout << buffer[i];
	}
	cout << endl;

	cout << "Number of bytes in buffer: " << f.getNumberOfBytesInReadBuffer() << endl;*/
}





