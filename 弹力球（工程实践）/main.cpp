//������
#include"test.h"
extern int gameStatus; //���ñ���
int main()									//������
{
	gameStartUp();
	while (gameStatus!=-1)
	{
		while (gameStatus == 0)
			mainMenu();
		while (gameStatus == 1)
			playMenu();
		while (gameStatus == 2)
			pauseMenu();
		while ( gameStatus == 3)
			againMenu();
		while (gameStatus == 4)
			saveMenu();
		while (gameStatus == 5)
			accountMenu();
		while (gameStatus == 6)
			listMenu();
	}
	gameOver();
	return 0;
}
