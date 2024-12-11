#ifndef TTCOM1_HPP
#define TTCOM1_HPP

#include "thread.hpp"
#include "Tpartage.hpp"
#include "screen.hpp"
#include "com.hpp"
#include "Tconfig.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string>

class TCom1 : public TCom
{
private:
	TScreen *screen;
	TPartage *partage = TPartage::getInstance();
public:
	TCom1(const char *name, void *shared, int priority, baudrate_t baudRate = b115200, parity_t parity = pNONE, dimData_t dimData = dS8, int32_t timeoutRxMs = 25);
	virtual ~TCom1();

	virtual void rxCar(unsigned char car);
	virtual void rxTimeout(void);

	static std::string getComConfig(std::string nameFichierConfig = "./com1.def");
};
#endif // TTASK1_HPP
