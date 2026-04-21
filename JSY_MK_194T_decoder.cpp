#include "esphome/core/log.h"
#include "JSY_MK_194T_decoder.h"

#include <deque>
#include <vector>

namespace esphome {
namespace JSY_MK_194T_decoder {

static const char *TAG = "JSY_MK_194T_decoder.sensor";

void JSYMK194TDecoder::setup() {
  
	this->set_interval("sendSerial", 5000, [this]() {	//every 5s
		
		std::deque<uint8_t> queueByteForCrc;
		queueByteForCrc.push_back(0x01);	// Address
		queueByteForCrc.push_back(0x03);	// Command
		queueByteForCrc.push_back(0x00);	// Starting address (high bit before)
		queueByteForCrc.push_back(0x4A);
		queueByteForCrc.push_back(0x00);	// Number of registers (high order before)
		queueByteForCrc.push_back(0x0C);
		uint16_t crc = calculCRC(queueByteForCrc, 8);
		queueByteForCrc.push_back((crc & 0x00FF));	//CRC 1
		queueByteForCrc.push_back(((crc & 0xFF00)>>8));	//CRC 1
		
		std::vector<uint8_t> queueByteForCrcVector(queueByteForCrc.begin(), queueByteForCrc.end());
		
		this->_parent->write_array(queueByteForCrcVector);
	});
}

void JSYMK194TDecoder::loop() {
	
	uint8_t byteRead = 0;
	while(this->_parent->available()) {
		this->_parent->read_byte(&byteRead);
		
		queueByte.push_back((int)byteRead);
		
		if(queueByte.size() > 53) queueByte.pop_front();
		
		
		if(queueByte[0] == 0x01 && queueByte[1] == 0x03 && queueByte[2] == 0x30 && queueByte.size() >= 53) {
			if(checksumValidation(53)) {
				
				//First channel active power
				valueActivePowerA = ((uint32_t)((queueByte[3] << 24) | (queueByte[4] << 16) | (queueByte[5] << 8) | queueByte[6])) / 10000.0;
				activePowerA->publish_state(valueActivePowerA);
				
				//Positive active energy of first channel
				valuePositiveActiveEnergyA = ((uint32_t)((queueByte[7] << 24) | (queueByte[8] << 16) | (queueByte[9] << 8) | queueByte[10])) / 10000.0;
				positiveActiveEnergyA->publish_state(valuePositiveActiveEnergyA);
				
				//First channel power factor
				
				//Negative active energy of first channel
				valueNegativeActiveEnergyA = ((uint32_t)((queueByte[15] << 24) | (queueByte[16] << 16) | (queueByte[17] << 8) | queueByte[18])) / 10000.0;
				negativeActiveEnergyA->publish_state(valueNegativeActiveEnergyA);
				
				//Power direction
				//Frequency
				//Second channel voltage
				//Second channel current
				
				//Second channel active power
				valueActivePowerB = ((uint32_t)((queueByte[35] << 24) | (queueByte[36] << 16) | (queueByte[37] << 8) | queueByte[38])) / 10000.0;
				activePowerB->publish_state(valueActivePowerB);
				
				//Positive active energy of second channel
				valuePositiveActiveEnergyB = ((uint32_t)((queueByte[39] << 24) | (queueByte[40] << 16) | (queueByte[41] << 8) | queueByte[42])) / 10000.0;
				positiveActiveEnergyB->publish_state(valuePositiveActiveEnergyB);
				
				//Second channel power factor
				
				//Negative active energy of second channel
				valueNegativeActiveEnergyB = ((uint32_t)((queueByte[47] << 24) | (queueByte[48] << 16) | (queueByte[49] << 8) | queueByte[50])) / 10000.0;
				negativeActiveEnergyB->publish_state(valueNegativeActiveEnergyB);
				
				queueByte.erase(queueByte.begin(),queueByte.begin()+53); //1byte d'adresse + 1byte de fonction + 1byte de taille de donnée + 48byte de donnée + 2byte CRC
			}
		}
	}  
}

void JSYMK194TDecoder::update() {
}

void JSYMK194TDecoder::dump_config() {
    ESP_LOGCONFIG(TAG, "JSYMK194TDecoder");
}

} //namespace JSY_MK_194T_decoder
} //namespace esphome