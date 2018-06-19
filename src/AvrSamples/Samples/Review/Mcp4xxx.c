#include "Mcp4xxx.h" 

/*

PB5 =	SCK, CLK, SCLK (Slave clock)
PB3 =	MOSI, SDI, SI, DI (Master Out Slave In)
PB4	=	MISO, SDO, SO, DO (Master In Slave Out)
PB2 =	CS, SS  0 (Chip Select / Slave select)

*/

const uint8_t MCP4XXX_WIPER_0 = 0;
const uint8_t MCP4XXX_WIPER_1 = 1;
const unsigned int MCP4XXX_POT_MIN_STEPS = 0;
const unsigned int MCP4XXX_POT_MAX_STEPS = 256;

const uint8_t CMDERR_ADDRESS = 0b10;

#define MCP4XXX_SET_SS_CS_LOW	(*_ssRegister &= ~(1 << _ssPort))  // Activate slave// TODO: port must be configurable
#define MCP4XXX_SET_SS_CS_HIGH	(*_ssRegister |= (1 << _ssPort))	// Deactivate slave // TODO: review wheter correct port is used

uint8_t Mcp4xxxInternalIncrementDecrementStep(uint8_t wiperAddress);
void Mcp4xxxSpiSendInternal(unsigned char byte);

uint8_t * _ssRegister;
uint8_t _ssPort;

void Mcp4xxxInitialize(uint8_t * ssRegister, uint8_t  ssPort)
{
	_ssRegister = ssRegister;
	_ssPort = ssPort;

	MCP4XXX_SET_SS_CS_HIGH;

	// Enable SPI, set cloct speed F_CPU/4
	SPCR = (1 << SPE) | (1 << MSTR);
}

// TODO: TCON not completed
// Reads TCON register
uint8_t Mcp4xxxReadTcon(void)
{
	uint8_t response;

	MCP4XXX_SET_SS_CS_LOW; // activate slave

						   //SPDR = 0b01001100; // read address from
						   //while (!(SPSR & (1 << SPIF)));

	Mcp4xxxSpiSendInternal(0b01001100);

	/*SPDR = 0;
	while (!(SPSR & (1 << SPIF)));*/

	Mcp4xxxSpiSendInternal(0);

	response = SPDR;

	MCP4XXX_SET_SS_CS_HIGH; // deactivate slave

	return response;
}

// TODO: TCON not completed
// Write to TCON register
// Value to write 0-256
// Returns 0 if success
uint8_t Mcp4xxxWriteTcon(uint8_t value)
{
	if (value < MCP4XXX_POT_MIN_STEPS || value > MCP4XXX_POT_MAX_STEPS)
	{
		return 1;
	}

	uint8_t response;

	MCP4XXX_SET_SS_CS_LOW; // activate slave

						   //SPDR = 0x40; // set address to write to TCON
						   //while (!(SPSR & (1 << SPIF))); // wait for 1th bit write

	Mcp4xxxSpiSendInternal(0x40);

	response = SPDR;

	if (!(response & CMDERR_ADDRESS))
	{
		MCP4XXX_SET_SS_CS_HIGH;
		return 1; // error CMDERR (if low then error)
	}

	//SPDR = value; // send data to TCON
	//while (!(SPSR & (1 << SPIF))); // wait for completion

	Mcp4xxxSpiSendInternal(value);

	MCP4XXX_SET_SS_CS_LOW; // deactivate slave

	return 0;
}

uint8_t Mcp4xxxRead(uint16_t *wiper0, uint16_t *wiper1)
{
	char high, low; // TODO: what is this for???

	MCP4XXX_SET_SS_CS_LOW;

	Mcp4xxxSpiSendInternal(0b1100);

	high = SPDR; // rider uppper position

	if (!(high & CMDERR_ADDRESS))  // error CMDERR (if low then error)(if low then error)
	{
		MCP4XXX_SET_SS_CS_HIGH;
		return 1;
	}

	Mcp4xxxSpiSendInternal(0);

	low = SPDR; // rider lower position

	*wiper0 = (((uint16_t)(high & 0b1) << 8) | low); // TODO: what is this for?

													 //SPDR = 0b11100; // address wiper 1
													 //while (!(SPSR & (1 << SPIF)));

	Mcp4xxxSpiSendInternal(0b11100);

	high = SPDR;

	if (!(high & 0b10)) // error CMDERR (if low then error)
	{
		MCP4XXX_SET_SS_CS_HIGH;
		return 1;
	}

	//SPDR = 0;
	//while (!(SPSR & (1 << SPIF)));

	Mcp4xxxSpiSendInternal(0);

	low = SPDR;

	*wiper1 = (((uint16_t)(high & 0b1) << 8) | low);

	MCP4XXX_SET_SS_CS_HIGH;

	return 0;
}

// Set wiper 0 or 1
// Steps 0-256
// Returns 0 if success
uint8_t Mcp4xxxSetStep(uint8_t wiper, uint16_t value)
{
	if (wiper < 0 || wiper > 1)
	{
		return 1;
	}
	if (value < MCP4XXX_POT_MIN_STEPS || value > MCP4XXX_POT_MAX_STEPS)
	{
		return 1;
	}

	uint8_t wiperAddress;

	MCP4XXX_SET_SS_CS_LOW; // activate slave 

	if (wiper == 0)
	{
		//SPDR = 0x00 | (uint8_t)((value >> 8) & 0b1); // send 9th bit
		wiperAddress = 0x00 | (uint8_t)((value >> 8) & 0b1); // send 9th bit
	}
	else
	{
		//SPDR = 0x10 | (uint8_t)((value >> 8) & 0b1); // send 9th bit
		wiperAddress = 0x10 | (uint8_t)((value >> 8) & 0b1); // send 9th bit
	}
	
	// TODO: this is not right, why two times

	//while (!(SPSR & (1 << SPIF))); // wait for completion 1 bit

	Mcp4xxxSpiSendInternal(wiperAddress);

	uint8_t response = SPDR;

	if (!(response & CMDERR_ADDRESS))// error CMDERR (if low then error)
	{
		MCP4XXX_SET_SS_CS_HIGH;
		return 1;
	}

	//SPDR = (uint8_t)(value); // send bits 7-0
	//while (!(SPSR & (1 << SPIF))); // wait for completion

	Mcp4xxxSpiSendInternal(value);

	MCP4XXX_SET_SS_CS_HIGH; // deactivate slave

	return 0;
}

// Set wiper 0 and 1
// Steps 0-256
// Returns 0 if success
uint8_t Mcp4xxxSetBothWiper(uint16_t value)
{
	if (value < MCP4XXX_POT_MIN_STEPS || value > MCP4XXX_POT_MAX_STEPS)
	{
		return 1;
	}

	uint8_t response;

	MCP4XXX_SET_SS_CS_LOW; // activate slave 

						   // WIPER 0

						   //SPDR = 0x00 | (uint8_t)((value >> 8) & 0b1); // send 9th bit
						   //while (!(SPSR & (1 << SPIF))); // wait for completion

	Mcp4xxxSpiSendInternal(0x00 | (uint8_t)((value >> 8) & 0b1));

	response = SPDR;
	if (!(response & CMDERR_ADDRESS))// error CMDERR (if low then error)
	{
		MCP4XXX_SET_SS_CS_HIGH;
		return 1;
	}

	//SPDR = (uint8_t)(value); // send other bits
	//while (!(SPSR & (1 << SPIF))); // wait for completion	  	 

	Mcp4xxxSpiSendInternal(value);

	// WIPER 1

	//SPDR = 0x10 | (uint8_t)((value >> 8) & 0b1); // send 9th bit
	//while (!(SPSR & (1 << SPIF))); // wait for completion

	Mcp4xxxSpiSendInternal(0x10 | (uint8_t)((value >> 8) & 0b1));

	response = SPDR;
	if (!(response & CMDERR_ADDRESS))// error CMDERR (if low then error)
	{
		MCP4XXX_SET_SS_CS_HIGH;
		return 1;
	}

	//SPDR = (uint8_t)(value); // send other bits
	//while (!(SPSR & (1 << SPIF)));// wait for completion

	Mcp4xxxSpiSendInternal(value);

	MCP4XXX_SET_SS_CS_HIGH; // deactivate slave

	return 0;
}

// Set wiper 0 and 1
// Percent 0-100
// Returns 0 if success
uint8_t Mcp4xxxSetBothWiperByPercent(uint8_t percent)
{
	if (percent < 0)
	{
		percent = 0;
	}
	if (percent > 100)
	{
		percent = 100;
	}

	uint16_t steps = (MCP4XXX_POT_MAX_STEPS / 100) * percent;

	return Mcp4xxxSetBothWiper(steps);
}

// Set wiper 0 max
uint8_t Mcp4xxxSetMaxStepWiper0()
{
	return Mcp4xxxSetStep(MCP4XXX_WIPER_0, MCP4XXX_POT_MAX_STEPS);
}

// Set wiper 0 min
uint8_t Mcp4xxxSetMinStepWiper0()
{
	return Mcp4xxxSetStep(MCP4XXX_WIPER_0, MCP4XXX_POT_MIN_STEPS);
}

// Set wiper 1 max
uint8_t Mcp4xxxSetMaxStepWiper1()
{
	return Mcp4xxxSetStep(MCP4XXX_WIPER_1, MCP4XXX_POT_MAX_STEPS);
}

// Set wiper 1 min
uint8_t Mcp4xxxSetMinStepWiper1()
{
	return Mcp4xxxSetStep(MCP4XXX_WIPER_1, MCP4XXX_POT_MIN_STEPS);
}

// Increment wiper
// Wiper index
// Returns 0 if success
uint8_t Mcp4xxxIncrementStep(uint8_t wiper)
{
	if (wiper < 0 || wiper > 1)
	{
		return 1;
	}

	// Get wiper address
	uint8_t wiperAddress = wiper == 0 ? 0b100 : 0b10100;

	return Mcp4xxxInternalIncrementDecrementStep(wiperAddress);
}

// Increment wiper
// Wiper index
// Returns 0 if success
uint8_t Mcp4xxxDecrementStep(uint8_t wiper)
{
	if (wiper < 0 || wiper > 1)
	{
		return 1;
	}

	// Get wiper address
	uint8_t wiperAddress = wiper == 0 ? 0b1000 : 0b011000;

	return Mcp4xxxInternalIncrementDecrementStep(wiperAddress);
}

uint8_t Mcp4xxxInternalIncrementDecrementStep(uint8_t wiperAddress)
{
	MCP4XXX_SET_SS_CS_LOW;

	Mcp4xxxSpiSendInternal(wiperAddress);

	MCP4XXX_SET_SS_CS_HIGH;

	uint8_t result = SPDR;

	if (!(result & CMDERR_ADDRESS)) // Check 2 bit error CMDERR
	{
		return 1;
	}

	return 0;
}

// TODO: refactor the SPDR 
void Mcp4xxxSpiSendInternal(unsigned char byte)
{
	SPDR = byte; // Shift in some data
	while (!(SPSR & (1 << SPIF)));	//Wait for SPI process to finish
}

// TODO refactr the error result
//uint8_t CheckCMDERR(uint8_t result)
//{
//
//}

// TODO: add SHDN pin implementation (on/off)
// check increment decrement functionality what is the max value 100? use this metho Mcp4xxxSetMaxStepWiper0

// TODO: Refactor those addresses into constants
