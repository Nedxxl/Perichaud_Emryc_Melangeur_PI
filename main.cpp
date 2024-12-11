
//============================================================================
// Name        : TemplateThreadCom.cpp
// Author      : SG
// Version     :
// Copyright   : Your copyright notice
// Description : Template Thread Com
//============================================================================

#include "clavier.hpp"
#include "screen.hpp"
#include "Tpartage.hpp"
#include "TCom1.hpp"
#include "task1.hpp"

int main(int argc, char *argv[])
{
  char car;

  // Initialisation task Principal
  TThread::initTaskMain(SCHED_FIFO, 0);

  // Création Clavier et console
  TClavier *clavier = TClavier::getInstance();
  TScreen *screen = new TScreen();
  TTask1 *task1 = new TTask1("Task1", screen, SCHED_FIFO, 10);
  screen->start();

  // Création tâches
  TCom1 *com1 = new TCom1(TCom1::getComConfig("com.def").c_str(), screen, 10, TCom::b9600, TCom::pNONE, TCom::dS8, 10000);

  // Démarrage tâches
  com1->start();
  task1->start();
  // com1->setSignalTimeout(1);
  //  Traitement tâche principale
  screen->dispStr(1, 1, "Test Com (SG  09/09/2024)");
  char buffer[5];

  do
  {
    // Traitement

    if (clavier->kbhit())
    {
      car = clavier->getch();
      if (car == '1')
      {
        buffer[0] = '<';
        buffer[1] = 'M';
        buffer[2] = 'M';
        buffer[3] = 'A';
        buffer[4] = '>';

        com1->sendTx(buffer, 5);
      }
      if (car == '2')
      {
        buffer[0] = '<';
        buffer[1] = 'M';
        buffer[2] = 'M';
        buffer[3] = 'B';
        buffer[4] = '>';

        com1->sendTx(buffer, 5);
      }
      if (car == '3')
      {
        buffer[0] = '<';
        buffer[1] = 'M';
        buffer[2] = 'M';
        buffer[3] = 'C';
        buffer[4] = '>';

        com1->sendTx(buffer, 5);
      }
      if (car == '4')
      {
        buffer[0] = '<';
        buffer[1] = 'M';
        buffer[2] = 'A';
        buffer[3] = 'A';
        buffer[4] = '>';

        com1->sendTx(buffer, 5);
      }
      if (car == '5')
      {
        buffer[0] = '<';
        buffer[1] = 'M';
        buffer[2] = 'A';
        buffer[3] = 'B';
        buffer[4] = '>';

        com1->sendTx(buffer, 5);
      }
      if (car == '6')
      {
        buffer[0] = '<';
        buffer[1] = 'M';
        buffer[2] = 'A';
        buffer[3] = 'C';
        buffer[4] = '>';

        com1->sendTx(buffer, 5);
      }
    }
  } while ((car != '|') && (car != '~'));

  // Destruction tâches
  if (com1)
    delete com1;

  if (task1)
    delete task1;

  if (screen)
    delete screen;

  return 0;

  // Destru    char str;
}