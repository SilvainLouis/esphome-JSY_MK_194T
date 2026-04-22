#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include <deque>

namespace esphome {
namespace JSY_MK_194T_decoder {

class JSYMK194TDecoder : public sensor::Sensor, public PollingComponent, public uart::UARTDevice {
 public:
 
  void set_active_power_1(sensor::Sensor *sensor) { activePowerA = sensor; }
  void set_active_power_2(sensor::Sensor *sensor) { activePowerB = sensor; }
  void set_positive_energy_1(sensor::Sensor *sensor) { positiveActiveEnergyA = sensor; }
  void set_positive_energy_2(sensor::Sensor *sensor) { positiveActiveEnergyB = sensor; }
  void set_negative_energy_1(sensor::Sensor *sensor) { negativeActiveEnergyA = sensor; }
  void set_negative_energy_2(sensor::Sensor *sensor) { negativeActiveEnergyB = sensor; }
  void set_uart_id(esphome::uart::UARTComponent *parent) { _parent = parent; }
	
  void setup() override;
  void update() override;
  void loop() override;
  void dump_config() override;
  
protected:
	sensor::Sensor *activePowerA = new Sensor();
    sensor::Sensor *activePowerB = new Sensor();
    sensor::Sensor *positiveActiveEnergyA = new Sensor();
    sensor::Sensor *positiveActiveEnergyB = new Sensor();
    sensor::Sensor *negativeActiveEnergyA = new Sensor();
    sensor::Sensor *negativeActiveEnergyB = new Sensor();
	
private:
	esphome::uart::UARTComponent *_parent;
	std::deque<int> queueByte;
	float valueActivePowerA = 0;
	float valueActivePowerB = 0;
	float valuePositiveActiveEnergyA = 0;
	float valuePositiveActiveEnergyB = 0;
	float valueNegativeActiveEnergyA = 0;
	float valueNegativeActiveEnergyB = 0;
	
	bool checksumValidation(int messageLength) {

		uint16_t crc = 0xFFFF;
		for (int pos = 0; pos < (messageLength-2); pos++)
		{
			crc ^= (uint16_t)queueByte[pos];    // XOR byte into least sig. byte of crc

			for (int i = 8; i != 0; i--) {    // Loop over each bit
				if ((crc & 0x0001) != 0) {      // If the LSB is set
					crc >>= 1;                    // Shift right and XOR 0xA001
					crc ^= 0xA001;
				}
				else                            // Else LSB is not set
					crc >>= 1;                    // Just shift right
			}
		}

		if( ((crc & 0x00FF) == queueByte[messageLength-2]) &&
			(((crc & 0xFF00)>>8) == queueByte[messageLength-1])
			) return true;
		else return false;
	}
	
	uint16_t calculCRC(std::deque<uint8_t> queueByteForCrc, int messageLength) {

		uint16_t crc = 0xFFFF;
		for (int pos = 0; pos < (messageLength-2); pos++)
		{
			crc ^= (uint16_t)queueByteForCrc[pos];    // XOR byte into least sig. byte of crc

			for (int i = 8; i != 0; i--) {    // Loop over each bit
				if ((crc & 0x0001) != 0) {      // If the LSB is set
					crc >>= 1;                    // Shift right and XOR 0xA001
					crc ^= 0xA001;
				}
				else                            // Else LSB is not set
					crc >>= 1;                    // Just shift right
			}
		}
		
		return crc;
	}
};

}  // namespace JSY_MK_194T_decoder
}  // namespace esphome