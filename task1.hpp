#ifndef TTASK1_HPP
#define TTASK1_HPP

#include "thread.hpp"
#include "mqtt.hpp"
#include "Tpartage.hpp"
#include "screen.hpp"
#include "TCom1.hpp"

class TTask1 : public TThread
{
private:
	TMqtt *mqtt;
	TScreen *screen;
	TPartage *partage;
	TCom1 *com1;
	
public : TTask1(const char *name, void *shared, int32_t policy, int32_t priority, int32_t cpu = -1);
	~TTask1();

	void task(void);
};

#endif // TTASK1_HPP
