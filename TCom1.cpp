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
	static enum { WAIT_START,
				  RECEIVE_DATA } state = WAIT_START;
	static char buffer[5];
	static int index = 0;

	switch (state)
	{
	case WAIT_START:
		if (car == '<')
		{
			memset(buffer, '\0', sizeof(buffer));
			index = 0;
			buffer[index++] = car;
			state = RECEIVE_DATA;
		}
		break;

	case RECEIVE_DATA:
		if ((car == '>') && (index == 4))
		{
			buffer[index] = car;
			index = 0;
			if (buffer[2] == 'M')
			{
				switch (buffer[3])
				{
				case 'A':
					partage->setEtat('A', 1);
					screen->dispStr(1, 15, "Marche moteur A.");
					break;
				case 'B':
					partage->setEtat('B', 1);
					screen->dispStr(1, 15, "Marche moteur B.");
					break;
				case 'C':
					partage->setEtat('C', 1);
					screen->dispStr(1, 15, "Marche moteur C.");
					break;
				}
			}
			else if (buffer[2] == 'A')
			{
				switch (buffer[3])
				{
				case 'A':
					partage->setEtat('A', 0);
					screen->dispStr(1, 15, "Arret moteur A. ");
					break;
				case 'B':
					partage->setEtat('B', 0);
					screen->dispStr(1, 15, "Arret moteur B. ");
					break;
				case 'C':
					partage->setEtat('C', 0);
					screen->dispStr(1, 15, "Arret moteur C. ");
					break;
				}
			}
			screen->dispStr(1, 11, buffer);
		}
		else if (index < 4)
		{
			buffer[index++] = car;
		}
		else
		{
			index = 0;
			state = WAIT_START;
		}
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