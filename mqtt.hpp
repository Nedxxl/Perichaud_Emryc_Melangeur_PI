#ifndef MQTT_HPP
#define MQTT_HPP

#include <mosquittopp.h>
#include <unordered_map>
#include <functional>
#include <string>
#include "screen.hpp"
#include "Tpartage.hpp"

class TMqtt : public mosqpp::mosquittopp
{
private:
	TPartage *partage;
	TScreen *screen;
	std::unordered_map<std::string, std::function<void(TMqtt *, std::string)>> mqttTable;

	// void messageModule1(TMqtt * this,std::string val);
	void moteurA(std::string val);
	void moteurB(std::string val);
	void moteurC(std::string val);
	void mode(std::string val);
	void poidBalance(std::string val);
	void recetteAuto(std::string val);
	void recetteValue(std::string val);

public:
	TMqtt(void *shared, const char *id, const char *host, int port = 1883);
	virtual ~TMqtt();

	void on_connect(int rc);
	void on_message(const struct mosquitto_message *message);
	void on_subscribe(int mid, int qos_count, const int *granted_pos);

	static std::string getMqttConfig(std::string nameFichierConfig = "./mqtt.def");
};

#endif // MQTT_HPP
