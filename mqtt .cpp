#include "mqtt.hpp"
#include "Tconfig.hpp"
#include "Tpartage.hpp"

using namespace std;

TMqtt::TMqtt(void *shared, const char *id, const char *host, int port) : mosqpp::mosquittopp(id)
{
	screen = (TScreen *)shared;
	partage = TPartage::getInstance();

	mqttTable["RAM/melangeur/cmd/motA"] = &TMqtt::moteurA;
	mqttTable["RAM/melangeur/cmd/motB"] = &TMqtt::moteurB;
	mqttTable["RAM/melangeur/cmd/motC"] = &TMqtt::moteurC;
	mqttTable["RAM/melangeur/cmd/mode"] = &TMqtt::mode;
	mqttTable["RAM/balance/etats/poids"] = &TMqtt::poidBalance;
	mqttTable["RAM/melangeur/cmd/recetteGo"] = &TMqtt::recetteAuto;
	mqttTable["RAM/melangeur/cmd/recette"] = &TMqtt::recetteValue;

	mosqpp::lib_init();

	connect(host, port, 120);
}

TMqtt::~TMqtt()
{
	mosqpp::lib_cleanup();
}

void TMqtt::on_connect(int rc)
{
	if (rc == 0)
	{
		screen->dispStr(1, 13, "Connected : OK  ");

		subscribe(NULL, "RAM/#", 0);
	}
	else
		screen->dispStr(1, 13, "Connected : Fail");
}

void TMqtt::on_message(const struct mosquitto_message *message)
{
	string topic = message->topic;
	string payload = (char *)message->payload;

	screen->dispStr(1, 12, (char *)message->topic);
	screen->dispStr(15, 12, "    ");
	screen->dispStr(15, 12, (char *)message->payload);

	auto it = mqttTable.find(topic);
	if (it != mqttTable.end())
		it->second(this, payload);
	/*
	#if 0
		if(topic.substr(0,7) == "module/")
			{
			if(topic.substr(7,8) == "1")
				{
				partage.setModule1( (payload == "on")? true : false );messageModule1
				}
			else if(topic.substr(7,8) == "2")
				{
				partage.setModule2( (payload == "on")? true : false );
				}
			else if(topic.substr(7,8) == "3")
				{
				partage.setModule3( (payload == "on")? true : false );
				}
			else if(topic.substr(7,8) == "4")
				{
				partage.setModule4( (payload == "on")? true : false );
				}
			else if(topic.substr(7,8) == "5")
				{
				partage.setModule5( (payload == "on")? true : false );
				}
			else if(topic.substr(7,8) == "6")bool
				{
				partage.setModule6( (payload == "on")? true : false );
				}
			}
	#endif
	*/
}

void TMqtt::on_subscribe(int mid, int qos_count, const int *granted_pos)
{
	screen->dispStr(1, 14, "on_subcribe");
}

void TMqtt::moteurA(string val)
{
	if (val == "on")
		partage->setMoteur('A', 1, 1);
	else if (val == "off")
		partage->setMoteur('A', 0, 1);
}

void TMqtt::moteurB(string val)
{
	if (val == "on")
		partage->setMoteur('B', 1, 1);
	else if (val == "off")
		partage->setMoteur('B', 0, 1);
}

void TMqtt::moteurC(string val)
{
	if (val == "on")
		partage->setMoteur('C', 1, 1);
	else if (val == "off")
		partage->setMoteur('C', 0, 1);
}

void TMqtt::mode(string val)
{
	if (val == "auto")
		partage->setMode(1);
	else if (val == "manuel")
		partage->setMode(0);
}

void TMqtt::poidBalance(string val)
{
	partage->setPoidBalance(stod(val));
}

void TMqtt::recetteAuto(string val)
{
	if (val == "go")
	{
		TPartage::recette_t recette = partage->getRecetteValue();
		partage->setPoid('A', recette.poidA);
		partage->setPoid('B', recette.poidB);
		partage->setPoid('C', recette.poidC);

		partage->setRecetteAuto(1);
	}
	else if (val == "stop")
	{
		partage->setRecetteAuto(0);
	}
	else if (val == "flush")
	{
		partage->removeRecette();
		partage->setRecetteAuto(0);
	}
}

void TMqtt::recetteValue(string val)
{
	char valRecived[3][4];
	int i = 0;
	char *ptr = strtok((char *)val.c_str(), "-");
	while ((ptr != NULL) && (i < 4))
	{
		strcpy(valRecived[i], ptr);
		ptr = strtok(NULL, "-");
		i++;
	}
	partage->addRecette(atoi(valRecived[0]), atoi(valRecived[1]), atoi(valRecived[2]));
}

string TMqtt::getMqttConfig(string nameFichierConfig)
{
	TConfiguration config(nameFichierConfig);

	return config.getParametreConfiguration();
}
