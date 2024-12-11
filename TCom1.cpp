#include "TCom1.hpp"

TCom1::TCom1(const char *name, void *shared, int priority, baudrate_t baudRate, parity_t parity, dimData_t dimData, int32_t timeoutRxMs)
	: TCom(name, shared, priority, baudRate, parity, dimData, timeoutRxMs), screen(nullptr)
{
	screen = (TScreen *)shared;
	screen->dispStr(1, 2, name);
	screen->dispStr(1, 3, com);
}

TCom1::~TCom1()
{
}

void TCom1::rxCar(unsigned char car)
{
	static enum { WAIT_START, RECEIVE_DATA, WAIT_END } state = WAIT_START;
	static char buffer[7];
	static int index = 0;

	switch (state)
	{
	case WAIT_START:
		if (car == '<')
		{
			buffer[index++] = car;
			state = RECEIVE_DATA;
		}
		break;

	case RECEIVE_DATA:
		if ((car == '>') && (index == 6))
		{
			buffer[index] = car;
			index = 0;
			state = WAIT_END;
			screen->dispStr(1, 5, buffer);
		}
		else if(index < 6)
		{
			buffer[index++] = car;
		}
		else
		{
			index = 0;
			state = WAIT_START;
		}
			
		break;

	case WAIT_END:
	bool etatA = partage->getEtat('A');
	bool etatB = partage->getEtat('B');
	bool etatC = partage->getEtat('C');

		if(buffer[3] == 'A' && etatA == 1)
		{
			partage->setEtat('A', 0);
		}
		else if(buffer[3] == 'M' && etatA == 0)
		{
			partage->setEtat('A', 1);
		}

		if(buffer[4] == 'A' && etatB == 1)
		{
			partage->setEtat('B', 0);
		}
		else if(buffer[4] == 'M' && etatB == 0)
		{
			partage->setEtat('B', 1);
		}

		if(buffer[5] == 'A' && etatC == 1)
		{
			partage->setEtat('C', 0);
		}
		else if(buffer[5] == 'M' && etatC == 0)
		{
			partage->setEtat('C', 1);
		}

			state = WAIT_START;
		break;
	}
}

void TCom1::rxTimeout(void)
{
	if (screen)
	{
		screen->dispStr(1, 4, "Receive timeout.");
	}
}

std::string TCom1::getComConfig(std::string nameFichierConfig)
{
	TConfiguration config(nameFichierConfig);
	return config.getParametreConfiguration();
}