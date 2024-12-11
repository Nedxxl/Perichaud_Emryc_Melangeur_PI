#include "task1.hpp"

TTask1::TTask1(const char *name, void *shared, int32_t policy, int32_t priority, int32_t cpu) : TThread(name, shared, policy, priority, cpu)
{
	std::string nameMqtt;

	screen = (TScreen *)shared;

	partage = TPartage::getInstance();

	nameMqtt = TMqtt::getMqttConfig();
	mqtt = new TMqtt(screen, "MqttMelangeur", nameMqtt.c_str());

	com1 = new TCom1(TCom1::getComConfig("com.def").c_str(), screen, 10, TCom::b9600, TCom::pNONE, TCom::dS8, 10000);

	screen->dispStr(1, 10, "Task MQTT (CPU :  ) :");
	screen->dispStr(30, 10, nameMqtt.c_str());
}

TTask1::~TTask1()
{
	if (mqtt)
		delete mqtt;

	if (com1)
		delete com1;
}

void TTask1::task(void)
{
	// variable locale
	char strCar[2] = {'-', '\0'};
	char trameMoteur[5] = {'\0'};
	char marcheA[5] = {'<', 'M', 'M', 'A', '>'};
	char stopA[5] = {'<', 'M', 'A', 'A', '>'};
	char marcheB[5] = {'<', 'M', 'M', 'B', '>'};
	char stopB[5] = {'<', 'M', 'A', 'B', '>'};
	char marcheC[5] = {'<', 'M', 'M', 'C', '>'};
	char stopC[5] = {'<', 'M', 'A', 'C', '>'};
	bool changeA = 0;
	bool changeB = 0;
	bool changeC = 0;
	bool mode = 0;
	bool recetteGo = 0;
	double poids = 0.0;
	double poidsStart = 0.0;
	double poidAvoulu = 0.0;
	double poidBvoulu = 0.0;
	double poidCvoulu = 0.0;
	double poidAreel = 0.0;
	double poidBreel = 0.0;
	double poidCreel = 0.0;
	int vis = 0;
	bool stop = 0;

	// synchronisation démarrage tâche
	signalStart();

	sleep(2);

	while (1)
	{
		// traitement
		if (strCar[0] == '|')
			strCar[0] = '-';
		else
			strCar[0] = '|';
		screen->dispStr(23, 11, strCar);

		mode = partage->getMode();
		poids = partage->getPoidBalance() * 1000;
		screen->dispStr(1, 5, std::to_string(poids).c_str());
		screen->dispStr(1, 4, std::to_string(mode).c_str());
		if (mode == 0)
		{
			changeA = partage->isChange('A');
			changeB = partage->isChange('B');
			changeC = partage->isChange('C');
			if (changeA)
			{
				bool moteurA = partage->getMoteur('A');
				if (moteurA)
				{
					trameMoteur[0] = '<';
					trameMoteur[1] = 'M';
					trameMoteur[2] = 'M';
					trameMoteur[3] = 'A';
					trameMoteur[4] = '>';
					com1->sendTx(trameMoteur, 5);
					mqtt->publish(NULL, "RAM/melangeur/etats/motA", 2, "on", 0, false);
				}
				else
				{
					trameMoteur[0] = '<';
					trameMoteur[1] = 'M';
					trameMoteur[2] = 'A';
					trameMoteur[3] = 'A';
					trameMoteur[4] = '>';
					com1->sendTx(trameMoteur, 5);
					mqtt->publish(NULL, "RAM/melangeur/etats/motA", 3, "off", 0, false);
				}
			}
			if (changeB)
			{
				bool moteurB = partage->getMoteur('B');
				if (moteurB)
				{
					trameMoteur[0] = '<';
					trameMoteur[1] = 'M';
					trameMoteur[2] = 'M';
					trameMoteur[3] = 'B';
					trameMoteur[4] = '>';
					com1->sendTx(trameMoteur, 5);
					mqtt->publish(NULL, "RAM/melangeur/etats/motB", 2, "on", 0, false);
				}
				else
				{
					trameMoteur[0] = '<';
					trameMoteur[1] = 'M';
					trameMoteur[2] = 'A';
					trameMoteur[3] = 'B';
					trameMoteur[4] = '>';
					com1->sendTx(trameMoteur, 5);
					mqtt->publish(NULL, "RAM/melangeur/etats/motB", 3, "off", 0, false);
				}
			}
			if (changeC)
			{
				bool moteurC = partage->getMoteur('C');
				if (moteurC)
				{
					trameMoteur[0] = '<';
					trameMoteur[1] = 'M';
					trameMoteur[2] = 'M';
					trameMoteur[3] = 'C';
					trameMoteur[4] = '>';
					com1->sendTx(trameMoteur, 5);
					mqtt->publish(NULL, "RAM/melangeur/etats/motC", 2, "on", 0, false);
				}
				else
				{
					trameMoteur[0] = '<';
					trameMoteur[1] = 'M';
					trameMoteur[2] = 'A';
					trameMoteur[3] = 'C';
					trameMoteur[4] = '>';
					com1->sendTx(trameMoteur, 5);
					mqtt->publish(NULL, "RAM/melangeur/etats/motC", 3, "off", 0, false);
				}
			}
		}
		else if (mode == 1)
		{
			recetteGo = partage->getRecetteAuto();
			if (recetteGo)
			{
				if (partage->isChangeRecetteValue())
				{
					poidsStart = (partage->getPoidBalance() * 1000);
					poidAvoulu = partage->getPoid('A');
					poidBvoulu = partage->getPoid('B');
					poidCvoulu = partage->getPoid('C');
					changeA = false;
					changeB = false;
					changeC = false;
					vis = 0;
					mqtt->publish(NULL, "RAM/melangeur/etats/recetteStatut", 10, "PROCESSING", 0, false);
				}
				switch (vis)
				{
				case 0:
					if (poidAvoulu > (poids - poidsStart))
					{
						if (!changeA)
						{
							changeA = true;
							com1->sendTx(marcheA, 5);
							sleep(1);
							mqtt->publish(NULL, "RAM/melangeur/etats/motA", 2, "on", 0, false);
						}
						poidAreel = poids - poidsStart;
						mqtt->publish(NULL, "RAM/balance/etats/poidsA", sizeof(poidAreel), std::to_string(poidAreel).c_str(), 0, false);
					}
					else
					{

						changeA = false;
						com1->sendTx(stopA, 5);
						sleep(1);
						mqtt->publish(NULL, "RAM/melangeur/etats/motA", 3, "off", 0, false);

						poidAreel = poids - poidsStart;
						mqtt->publish(NULL, "RAM/balance/etats/poidsA", sizeof(poidAreel), std::to_string(poidAreel).c_str(), 0, false);
						vis = 1;
					}
					break;
				case 1:
					if ((poidBvoulu + poidAreel) > (poids - poidsStart))
					{
						if (!changeB)
						{
							changeB = true;
							com1->sendTx(marcheB, 5);
							sleep(1);
							mqtt->publish(NULL, "RAM/melangeur/etats/motB", 2, "on", 0, false);
						}
						poidBreel = poids - (poidsStart + poidAreel);
						mqtt->publish(NULL, "RAM/balance/etats/poidsB", sizeof(poidBreel), std::to_string(poidBreel).c_str(), 0, false);
					}
					else
					{

						changeB = false;
						com1->sendTx(stopB, 5);
						sleep(1);
						mqtt->publish(NULL, "RAM/melangeur/etats/motB", 3, "off", 0, false);

						poidBreel = poids - (poidsStart + poidAreel);
						mqtt->publish(NULL, "RAM/balance/etats/poidsB", sizeof(poidBreel), std::to_string(poidBreel).c_str(), 0, false);
						vis = 2;
					}
					break;
				case 2:
					if ((poidCvoulu + poidBreel + poidAreel) > (poids - poidsStart))
					{
						if (!changeC)
						{
							changeC = true;
							com1->sendTx(marcheC, 5);
							sleep(1);
							mqtt->publish(NULL, "RAM/melangeur/etats/motC", 2, "on", 0, false);
						}
						poidCreel = poids - (poidsStart + poidAreel + poidBreel);
						mqtt->publish(NULL, "RAM/balance/etats/poidsC", sizeof(poidCreel), std::to_string(poidCreel).c_str(), 0, false);
					}
					else
					{

						changeC = false;
						com1->sendTx(stopC, 5);
						sleep(1);
						mqtt->publish(NULL, "RAM/melangeur/etats/motC", 3, "off", 0, false);

						poidCreel = poids - (poidsStart + poidAreel + poidBreel);
						mqtt->publish(NULL, "RAM/balance/etats/poidsC", sizeof(poidCreel), std::to_string(poidCreel).c_str(), 0, false);
						vis = 3;
					}
					break;

				default:
					stop = true;
					partage->setRecetteAuto(false);
					mqtt->publish(NULL, "RAM/melangeur/etats/recetteStatut", 8, "FINISHED", 0, false);
					break;
				}
				screen->dispStr(1, 6, std::to_string(partage->getPoid('A')).c_str());
				screen->dispStr(1, 7, std::to_string(partage->getPoid('B')).c_str());
				screen->dispStr(1, 8, std::to_string(partage->getPoid('C')).c_str());
			}
			else
			{
				if (stop == true)
				{
					stop = false;
					com1->sendTx(stopA, 5);
					sleep(1);
					mqtt->publish(NULL, "RAM/melangeur/etats/motA", 3, "off", 0, false);
					com1->sendTx(stopB, 5);
					sleep(1);
					mqtt->publish(NULL, "RAM/melangeur/etats/motB", 3, "off", 0, false);
					com1->sendTx(stopC, 5);
					sleep(1);
					mqtt->publish(NULL, "RAM/melangeur/etats/motC", 3, "off", 0, false);
					changeA = false;
					changeB = false;
					changeC = false;
					vis = 0;
				}
			}
		}

		if (mqtt->loop(100) != 0)
			mqtt->reconnect();

		// usleep(250000); // 250 ms
	}
}
