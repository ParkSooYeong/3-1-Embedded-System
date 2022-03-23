#include	"stm32f4xx_hal.h"
#include	"core_cm4.h" 

#ifndef  _LIB_SENSOR_
	#define  _LIB_SENSOR_

	void	sht11_external_isr(void);
	void	start_SHT11_sensor(void);
	void	s_transstart(void);
	void	s_connectionreset(void);
	void	calc_SHT11(uint16_t p_humidity ,uint16_t p_temperature);
	void	initialize_SHT11(void);
	void	sht11_delay(unsigned short time_us);

	uint8_t		s_write_byte(uint8_t value);
	uint8_t		s_read_byte(uint8_t ack);
	uint8_t		s_softreset(void);
	uint8_t		s_read_statusreg(uint8_t *p_value, uint8_t *p_checksum);
	uint8_t		s_write_statusreg(uint8_t *p_value);
	uint8_t		s_measure(uint16_t *p_value, uint16_t *p_checksum, uint8_t mode);
	uint16_t	get_SHT11_data(uint8_t type);
	float			calc_dewpoint(float h,float t);

	enum	{DEW,TEMP,HUMI};
#endif 
