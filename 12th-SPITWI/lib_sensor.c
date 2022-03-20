#include "lib_sensor.h"
#include <math.h>

uint16_t mytemp, myhumi;
uint16_t sht11_humi ,sht11_temp;
uint8_t acq_type;
float dew_point;
uint16_t error,checksum;
uint8_t acq_complete;

#ifndef false
  #define false 0
#endif

#ifndef true
  #define true 1
#endif

//----------------------------------------------------------------------------------
// modul-var
//----------------------------------------------------------------------------------


#define	SETDATA()   	SET_I2CDATA_PIN()
#define	CLRDATA()   	CLR_I2CDATA_PIN()
#define	READDATA()   	READ_I2CDATA_PIN()

#define	SETSCK()   	SET_I2CSCK_PIN()
#define	CLRSCK()   	CLR_I2CSCK_PIN()

#define noACK 0
#define ACK   1

#define SHT11_PORT      GPIOB
#define SHT11_SCK       GPIO_PIN_6
#define SHT11_SDA       GPIO_PIN_7

                            //adr  command  r/w
#define STATUS_REG_W 0x06   //000   0011    0
#define STATUS_REG_R 0x07   //000   0011    1
#define MEASURE_TEMP 0x03   //000   0001    1
#define MEASURE_HUMI 0x05   //000   0010    1
#define RESET        0x1e   //000   1111    0


GPIO_InitTypeDef SHT11_GPIO_InitStructure;

static inline void SET_I2CDATA_PIN() {HAL_GPIO_WritePin(SHT11_PORT, SHT11_SDA, GPIO_PIN_SET);}
static inline void CLR_I2CDATA_PIN() {HAL_GPIO_WritePin(SHT11_PORT, SHT11_SDA, GPIO_PIN_RESET);}
static inline int READ_I2CDATA_PIN() {return HAL_GPIO_ReadPin(SHT11_PORT,SHT11_SDA);}

static inline void MAKE_I2CDATA_OUTPUT() { SHT11_GPIO_InitStructure.Pin = SHT11_SDA;\
                                            SHT11_GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;\
                                            HAL_GPIO_Init(SHT11_PORT, &SHT11_GPIO_InitStructure);}

static inline void MAKE_I2CDATA_INPUT() {SHT11_GPIO_InitStructure.Pin = SHT11_SDA;\
                                            SHT11_GPIO_InitStructure.Mode = GPIO_MODE_INPUT;\
                                            HAL_GPIO_Init(SHT11_PORT, &SHT11_GPIO_InitStructure);}

//ASSIGN_PIN(I2CSCK, D, 0);
static inline void SET_I2CSCK_PIN() {HAL_GPIO_WritePin(SHT11_PORT, SHT11_SCK, GPIO_PIN_SET);}
static inline void CLR_I2CSCK_PIN() {HAL_GPIO_WritePin(SHT11_PORT, SHT11_SCK, GPIO_PIN_RESET);}

/*static inline int READ_I2CSCK_PIN() {return HAL_GPIO_ReadPin(SHT11_PORT,SHT11_SCK);}
static inline void MAKE_I2CSCK_OUTPUT() { SHT11_GPIO_InitStructure.Pin = SHT11_SCK;\
                                            SHT11_GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;\
                                            HAL_GPIO_Init(SHT11_PORT, &SHT11_GPIO_InitStructure);}

static inline void MAKE_I2CSCK_INPUT() {SHT11_GPIO_InitStructure.Pin = SHT11_SCK;\
                                            SHT11_GPIO_InitStructure.Mode = GPIO_MODE_INPUT;\
                                            HAL_GPIO_Init(SHT11_PORT, &SHT11_GPIO_InitStructure);}
*/

void SHT11_delay(unsigned short time_us) 
{
	unsigned int i,j;   
	for(j=0;j<time_us;j++)
		for(i=0;i<3;i++);												//오실리코프 측정에 의한 us 딜레이
          
}

//----------------------------------------------------------------------------------
uint8_t s_write_byte(uint8_t value)
//----------------------------------------------------------------------------------
// writes a byte on the Sensibus and checks the acknowledge 
{ 
  uint8_t i,error=0;
  MAKE_I2CDATA_OUTPUT();
  for (i=0x80;i>0;i/=2)             //shift bit for masking
  {
    if (i & value) SETDATA();          //masking value with i , write to SENSI-BUS
    else CLRDATA();                        
    SETSCK();                          //clk for SENSI-BUS
    __asm("nop");__asm("nop");        //pulswith approx. 250 ns  	
    CLRSCK();
  }
  SETDATA();                           //release DATA-line
  MAKE_I2CDATA_INPUT();
  __asm("nop");__asm("nop");
  SETSCK();                            //clk #9 for ack 
  error=READDATA();                       //check ack (DATA will be pulled down by SHT11)
  CLRSCK();
  MAKE_I2CDATA_OUTPUT();
  return error;                     //error=1 in case of no acknowledge
}

//----------------------------------------------------------------------------------
uint8_t s_read_byte(uint8_t ack)
//----------------------------------------------------------------------------------
// reads a byte form the Sensibus and gives an acknowledge in case of "ack=1" 
{ 
  uint8_t i,val=0;
  MAKE_I2CDATA_INPUT();
  SETDATA();                           //release DATA-line
  __asm("nop");__asm("nop");
  for (i=0x80;i>0;i/=2)             //shift bit for masking
  { SETSCK();                          //clk for SENSI-BUS
    __asm("nop");__asm("nop");
    if (READDATA()) val=(val | i);        //read bit  
    CLRSCK();
    __asm("nop");__asm("nop");					 
  }
  MAKE_I2CDATA_OUTPUT();
  if(ack) CLRDATA();
  else SETDATA();                      //in case of "ack==1" pull down DATA-Line
  SETSCK();                            //clk #9 for ack
  __asm("nop");__asm("nop");          //pulswith approx. 250 ns 
  CLRSCK();
  __asm("nop");__asm("nop");					    
  SETDATA();                           //release DATA-line
  return val;
}

//----------------------------------------------------------------------------------
void s_transstart(void)
//----------------------------------------------------------------------------------
// generates a transmission start 
//       _____         ________
// DATA:      |_______|
//           ___     ___
// SCK : ___|   |___|   |______
{  
   SETDATA(); CLRSCK();                   //Initial state
   __asm("nop");__asm("nop");
   SETSCK();
   __asm("nop");__asm("nop");
   CLRDATA();
   __asm("nop");__asm("nop");
   CLRSCK();  
   __asm("nop");__asm("nop");
   SETSCK();
   __asm("nop");__asm("nop");
   SETDATA();		   
   __asm("nop");__asm("nop");
   CLRSCK();
}

//----------------------------------------------------------------------------------
void s_connectionreset(void)
//----------------------------------------------------------------------------------
// communication reset: DATA-line=1 and at least 9 SCK cycles followed by transstart
//       _____________________________________________________         ________
// DATA:                                                      |_______|
//          _    _    _    _    _    _    _    _    _        ___     ___
// SCK : __| |__| |__| |__| |__| |__| |__| |__| |__| |______|   |___|   |______
{  
  uint8_t i; 
  SETDATA(); CLRSCK();                    //Initial state
  for(i=0;i<9;i++)                  //9 SCK cycles
  { SETSCK();
    CLRSCK();
  }
  s_transstart();                   //transmission start
}

//----------------------------------------------------------------------------------
uint8_t s_softreset(void)
//----------------------------------------------------------------------------------
// resets the sensor by a softreset 
{ 
  uint8_t error=0;  
  s_connectionreset();              //reset communication
  error+=s_write_byte(RESET);       //send RESET-command to sensor
  return error;                     //error=1 in case of no response form the sensor
}

//----------------------------------------------------------------------------------
uint8_t s_read_statusreg(uint8_t *p_value, uint8_t *p_checksum)
//----------------------------------------------------------------------------------
// reads the status register with checksum (8-bit)
{ 
  uint8_t error=0;
  s_transstart();                   //transmission start
  error=s_write_byte(STATUS_REG_R); //send command to sensor
  *p_value=s_read_byte(ACK);        //read status register (8-bit)
  *p_checksum=s_read_byte(noACK);   //read checksum (8-bit)  
  return error;                     //error=1 in case of no response form the sensor
}

//----------------------------------------------------------------------------------
uint8_t s_write_statusreg(uint8_t *p_value)
//----------------------------------------------------------------------------------
// writes the status register with checksum (8-bit)
{ 
  uint8_t error=0;
  s_transstart();                   //transmission start
  error+=s_write_byte(STATUS_REG_W);//send command to sensor
  error+=s_write_byte(*p_value);    //send value of status register
  return error;                     //error>=1 in case of no response form the sensor
}

//void dummyfunction(unsigned int *dummy){
//	*dummy++;
//}

//----------------------------------------------------------------------------------
uint8_t s_measure(uint16_t *p_value, uint16_t *p_checksum, uint8_t mode)
//----------------------------------------------------------------------------------
// makes a measurement (humidity/temperature) with checksum
{ 
  unsigned short error=0;
//  unsigned int i;
  uint16_t sht11_msb, sht11_lsb;

  s_transstart();                   //transmission start
  switch(mode){                     //send command to sensor
    case TEMP	: error+=s_write_byte(MEASURE_TEMP); break;
    case HUMI	: error+=s_write_byte(MEASURE_HUMI); break;
    default     : break;	 
  }
  if(error != 0){return error;}
  
  /* Enable External_Interrupt1 of SHT11. */
  MAKE_I2CDATA_INPUT();
  //SHT11_INTERRUPT_ENABLE();
  while(1){ SHT11_delay(500); if(READ_I2CDATA_PIN() == 0) break; }
  
  MAKE_I2CDATA_INPUT();
  
  sht11_msb = s_read_byte(ACK);    //read the first byte (MSB)
  sht11_lsb = s_read_byte(ACK);    //read the second byte (LSB)
  *p_value = (sht11_msb * 256) + sht11_lsb;
  *p_checksum =s_read_byte(noACK);  //read checksum
  acq_complete = false;
  return error;
}

void calc_SHT11(uint16_t p_humidity ,uint16_t p_temperature)
  //----------------------------------------------------------------------------------------
  // calculates temperature [C] and humidity [%RH]
  // input : humi [Ticks] (12 bit)
  // temp [Ticks] (14 bit)
  // output: humi [%RH]
  // temp [C]
  { 
    const float C1=-4.0; // for 12 Bit
    const float C2= 0.0405; // for 12 Bit
    const float C3=-0.0000028; // for 12 Bit
    const float T1=0.01; // for 14 Bit @ 5V
    const float T2=0.00008; // for 14 Bit @ 5V
 
    float rh_lin; // rh_lin: Humidity linear
    float rh_true; // rh_true: Temperature compensated humidity
    float t_C; // t_C : Temperature [C]
    float rh=(float)p_humidity; // rh: Humidity [Ticks] 12 Bit
    float t=(float)p_temperature; // t: Temperature [Ticks] 14 Bit
    
    t_C=t*(float)0.01 - 40; //calc. Temperature from ticks to [C]
    rh_lin=C3*rh*rh + C2*rh + C1; //calc. Humidity from ticks to [%RH]
    rh_true=(t_C-25)*(T1+T2*rh)+rh_lin; //calc. Temperature compensated humidity [%RH]
    if(rh_true>100)rh_true=100; //cut if the value is outside of
    if(rh_true<(float)0.1)rh_true=(float)0.1; //the physical possible range
    mytemp=(uint16_t)(t_C*10); //return temperature [C]
    myhumi=(uint16_t)(rh_true*10); //return humidity[%RH]
  }

//--------------------------------------------------------------------
float calc_dewpoint(float h,float t)
//--------------------------------------------------------------------
// calculates dew point
// input:   humidity [%RH], temperature [캜]
// output:  dew point [캜]
{ 
	float logEx,dew_point;
	logEx=(float)0.66077+(float)7.5*t/((float)237.3+t)+log10(h)-2;
	dew_point = (logEx - (float)0.66077)*(float)237.3/(0.66077+7.5-logEx);
	return dew_point;
}

void start_SHT11_sensor(void)
{
}

uint16_t get_SHT11_data(uint8_t type)
{
		acq_type=type;

		error=0;
		error+=s_measure(&sht11_humi,&checksum,HUMI);  //measure humidity
		error+=s_measure(&sht11_temp,&checksum,TEMP);  //measure temperature
		if(error!=0) s_connectionreset();                 //in case of an error: connection reset
		else
		{ 
			calc_SHT11(sht11_humi,sht11_temp);            //calculate humidity, temperature
			if(acq_type==DEW)dew_point=calc_dewpoint(sht11_humi,sht11_temp); //calculate dew point
			//char buf[10];
			//puts("sht11_humi="); itoa(sht11_humi,buf,10); puts(buf); puts("\r\n");
			//puts("sht11_temp="); itoa(sht11_temp,buf,10); puts(buf); puts("\r\n");
		}
		if(acq_type == DEW)return (uint16_t)(dew_point*10);
		else if(acq_type == HUMI)return myhumi;
		else if(acq_type == TEMP)return mytemp;
		else return 0;
}

void initialize_SHT11(void)
{
			__HAL_RCC_GPIOB_CLK_ENABLE();
      SHT11_GPIO_InitStructure.Pin = SHT11_SCK | SHT11_SDA;
      SHT11_GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
      SHT11_GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
      HAL_GPIO_Init(SHT11_PORT, &SHT11_GPIO_InitStructure);
      
      s_connectionreset();     
}
