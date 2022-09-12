//�Զ��庯��������
#include"test.h"
//����ȫ�ֱ���
int i, j;
int gameStatus; //��Ϸ״̬
int dwidth, dhigh; //��Ϸ������߽����
int left, right, top, bottom;//��Ϸ�߽��С
//ש��
int brick[BLOCKHANG][BLOCKNUM];
int brick_left, brick_top, brick_right, brick_bottom;
int brick_width, brick_high;
//����
struct BAR {
	int x, y;
	int width, high;
	int left, top, right, bottom;
}Bar;
//С��
struct BALL {
	float vx, vy;
	float x, y;
	int radius;

}Ball;
float Ballv;											//С����ٶ�
//ͼƬ
IMAGE	img_bk, img_bar, img_yb, img_bb, img_rb, img_gb;
//��¼״̬
int isLogin;
//���
MOUSEMSG m;
//С����
int shootball;
//���ֵ÷�
int nowscore;
//Ѫ��
int hp;
//��Ϸ��ʼ
int isgamestart;
//�ļ�����
FILE* fp;
//���
struct PLAYER {
	char ID[30];    //�˻���
	char password[20];   //����
	int maxscore;		 //��߻���
}player;
//��߷���
int Maxscore;
void gameStartUp()                             //��Ŀ��ʼ��
{
	//�������
	srand((unsigned int)time(0));
	//��������
	mciSendString("open .\\Credits.mp3 alias bkmusic", NULL, 0, NULL);
	mciSendString("open .\\hitbrick.mp3 alias hbkmusic", NULL, 0, NULL);
	mciSendString("open .\\Game Over .mp3 alias overmusic", NULL, 0, NULL);
	mciSendString("play bkmusic repeat", NULL, 0, NULL);
	//ȷ������
	initgraph(WIDTH, HIGH);
	//���ô��ڱ�������
	HWND hwnd = GetHWnd();
	SetWindowText(hwnd, _T("������"));
	//������Ϸ�����С
	dwidth = 200;
	left = dwidth, right = WIDTH - dwidth;
	dhigh = 10;
	top = dhigh, bottom = HIGH - dhigh;
	//����ͼƬ
	loadimage(&img_bk, ".\\bk.jpg", WIDTH, HIGH, 1);
	//��ʼ�滭
	BeginBatchDraw();
	//��Ϸ״̬
	gameStatus = 0;
	//�Ƿ��¼
	isLogin = 0;
	//�Ƿ�ʼ
	isgamestart = 1;
}

void mainMenu()                                //������
{
	putimage(0, 0, &img_bk);
	setbkmode(TRANSPARENT);

	settextcolor(GREEN);
	settextstyle(30, 0, _T("����"));
	outtextxy(0, (bottom - top) * 0.1, _T("ID��"));
	if (isLogin == 1)
		outtextxy(45, (bottom - top) * 0.1, player.ID);
	settextcolor(WHITE);
	settextstyle(90, 0, _T("����"));
	outtextxy((right - left) * 0.35 + left, (bottom - top) * 0.2, _T("������"));
	settextstyle(50, 0, _T("����"));
	outtextxy((right - left) * 0.35 + left, (bottom - top) * 0.4, _T(" ������Ϸ"));
	outtextxy((right - left) * 0.35 + left, (bottom - top) * 0.5, _T(" �˳���Ϸ"));
	outtextxy((right - left) * 0.41 + left, (bottom - top) * 0.6, _T("���а�"));
	FlushBatchDraw();
	Sleep(2);
	if (MouseHit())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			setbkmode(OPAQUE);
			if ((m.x >= (right - left) * 0.35 + left) && (m.x <= (right - left) * 0.6 + left) && (m.y >= (bottom - top) * 0.4) && (m.y < (bottom - top) * 0.5))
			{
				gameStatus = 5;			//�����¼����

			}
			if ((m.x >= (right - left) * 0.35 + left) && (m.x <= (right - left) * 0.6 + left) && (m.y >= (bottom - top) * 0.5) && (m.y < (bottom - top) * 0.6))
			{
				gameStatus = -1;			//�˳�
			}
			if ((m.x >= (right - left) * 0.41 + left) && (m.x <= (right - left) * 0.6 + left) && (m.y >= (bottom - top) * 0.6) && (m.y < (bottom - top) * 0.76))
			{
				gameStatus = 6;			//�������а����
			}
		}
	}
}

void playMenu()                                //�������
{
	if (isgamestart == 1)
		playStartUp();
	updateWithoutInput();
	updateWithInput();
	playShow();
}

void playStartUp()
{
	shootball = 0;
	hp = 3;
	nowscore = 0;
	isgamestart = 0;
	Ballv = 10;
	//ש��
	brick_width = (right - left) / BLOCKNUM;
	brick_high = (4 * (bottom - top)) / (5 * BLOCKHANG);
	for (i = 0; i < 3; i++)
		for (j = 0; j < BLOCKNUM; j++)
		{
			if (rand() % 2 == 1)
				brick[i][j] = rand() % 5;
		}

	for (i = 3; i < BLOCKHANG; i++)
		for (j = 0; j < BLOCKNUM; j++)
			brick[i][j] = 0;
	//����
	Bar.high = (bottom - top) / 40;
	Bar.width = (right - left) / 8;
	Bar.x = (right - left) / 2;
	Bar.y = 6 * (bottom - top) / 7;
	Bar.left = Bar.x - Bar.width / 2;
	Bar.right = Bar.x + Bar.width / 2;
	Bar.top = Bar.y - Bar.high / 2;
	Bar.bottom = Bar.y + Bar.high / 2;
	//С��
	Ball.radius = 5;
	Ball.vx = 0;
	Ball.vy = 0;
	Ball.x = Bar.x;
	Ball.y = Bar.top - Ball.radius;
	//����ͼƬ
	loadimage(&img_bar, ".\\bar.jpg", Bar.width, Bar.high, 1);
	loadimage(&img_yb, ".\\yellowbrick.jpg", brick_width - 1, brick_high - 1, 1);
	loadimage(&img_bb, ".\\bluebrick.jpg", brick_width - 1, brick_high - 1, 1);
	loadimage(&img_gb, ".\\greenbrick.jpg", brick_width - 1, brick_high - 1, 1);
	loadimage(&img_rb, ".\\redbrick.jpg", brick_width - 1, brick_high - 1, 1);
}

void updateWithoutInput()
{
	//ש������
	static int brickspeed = 5000;
	if (brickspeed <= 0)
	{
		for (int i = BLOCKHANG - 1; i > 0; i--)
			for (int j = 0; j < BLOCKNUM; j++)
			{
				brick[i][j] = brick[i - 1][j];
				brick[i - 1][j] = 0;

			}
		for (j = 0; j < BLOCKNUM; j++)
		{
			if (rand() % 2 == 1)
				brick[0][j] = rand() % 5;
		}
		brickspeed = 5000;
	}
	else
		brickspeed--;
	//С��ײ��ש��
	for (int i = 0; i < BLOCKHANG; i++)
		for (int j = 0; j < BLOCKNUM; j++)
		{
			int flag = 0;
			brick_left = j * brick_width + left;
			brick_right = brick_left + brick_width;
			brick_top = i * brick_high;
			brick_bottom = brick_top + brick_high;
			if (brick[i][j])
			{
				if (((Ball.x + Ball.radius >= brick_left && Ball.x < brick_right) || (Ball.x - Ball.radius <= brick_right && Ball.x + Ball.radius > brick_right)) && (Ball.y >= brick_top && Ball.y <= brick_bottom))
				{
					Ball.vx = -Ball.vx;
					brick[i][j] = 0;
					flag = 1;

					mciSendString("seek hbkmusic to start", NULL, 0, NULL);
					mciSendString("play hbkmusic ", NULL, 0, NULL);
				}
				if (((Ball.y - Ball.radius <= brick_bottom && Ball.y > brick_top) || (Ball.y + Ball.radius >= brick_top && Ball.y < brick_bottom)) && (Ball.x >= brick_left && Ball.x <= brick_right))
				{
					Ball.vy = -Ball.vy;
					brick[i][j] = 0;
					flag = 1;

					mciSendString("seek hbkmusic to start", NULL, 0, NULL);
					mciSendString("play hbkmusic ", NULL, 0, NULL);
				}
				if (flag == 1)
					nowscore++;
			}
		}
	//С�򷴵���
	if (Ball.x >= Bar.left && Ball.x <= Bar.right && Ball.y >= Bar.top && shootball == 1 && Ball.y < Bar.bottom)
	{
		ballHitBar();
	}
	//С�򷴵�
	if (Ball.x - Ball.radius <= left)
		Ball.vx = fabs(Ball.vx);
	if (Ball.x + Ball.radius >= right)
		Ball.vx = -fabs(Ball.vx);
	if (Ball.y - Ball.radius <= 0)
		Ball.vy = fabs(Ball.vy);
	//С���ƶ�
	static int ballspeed = 10;
	if (ballspeed == 0)
	{
		Ball.x += Ball.vx;
		Ball.y += Ball.vy;

		ballspeed = 10;
	}
	else
		ballspeed--;
	//С��ʼ����Ӷ�
	if (shootball == 0)
		Ball.x = Bar.x;
	//������߷���
	if (Maxscore < nowscore)
		Maxscore = nowscore;
	//��Ϸʧ��
	for (j = 0; j < BLOCKNUM; j++)
		if (brick[BLOCKHANG - 1][j] == 1)
			gameStatus = 3;
	if (Ball.y >= bottom)
	{
		Ball.vx = 0;
		Ball.vy = 0;
		Ball.x = Bar.x;
		Ball.y = Bar.top - Ball.radius;
		shootball = 0;
		hp--;
		if (hp == 0)
			gameStatus = 3;
	}
}

void updateWithInput()
{
	if (MouseHit())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_MOUSEMOVE && m.x >= left + Bar.width / 2 && m.x <= right - Bar.width / 2)
		{
			Bar.x = m.x;
			Bar.left = Bar.x - Bar.width / 2;
			Bar.right = Bar.x + Bar.width / 2;
			Bar.top = Bar.y - Bar.high / 2;
			Bar.bottom = Bar.y + Bar.high / 2;
		}
		else if (m.uMsg == WM_LBUTTONDOWN && shootball == 0)
		{

			Ball.vy = -Ballv;
			shootball = 1;
		}
	}
	char input;
	if (_kbhit())
	{
		input = _getch();
		if (input == 27)
			gameStatus = 2;
	}
}

void playShow()
{
	putimage(0, 0, &img_bk);
	//���Ʒ�Χ
	setcolor(BLACK);
	line(left, top, left, bottom);				//��߽�
	line(right, top, right, bottom);			//�ұ߽�
	line(left, top, right, top);				//�ϱ߽�
	line(left, bottom, right, bottom);			//�±߽�
	line(left, (BLOCKHANG - 1) * brick_high, right, (BLOCKHANG - 1) * brick_high);//ʧ�ܵ���
	//����С��
	setcolor(WHITE);
	setfillcolor(WHITE);
	fillcircle(Ball.x, Ball.y, Ball.radius);
	//���Ƶ���
	putimage(Bar.left, Bar.top, &img_bar);
	//����ש��
	setcolor(RED);
	setfillcolor(GREEN);
	for (int i = 0; i < BLOCKHANG; i++)
		for (int j = 0; j < BLOCKNUM; j++)
		{
			brick_left = j * brick_width + left;
			brick_right = brick_left + brick_width;
			brick_top = i * brick_high + top;
			brick_bottom = brick_top + brick_high;
			if (brick[i][j] == 1)
				putimage(brick_left + 1, brick_top + 1, &img_yb);
			else if (brick[i][j] == 2)
				putimage(brick_left + 1, brick_top + 1, &img_bb);
			else if (brick[i][j] == 3)
				putimage(brick_left + 1, brick_top + 1, &img_gb);
			else if (brick[i][j] == 4)
				putimage(brick_left + 1, brick_top + 1, &img_rb);
		}
	char s[30];
	//�����û���
	settextcolor(GREEN);
	settextstyle(30, 0, _T("����"));
	outtextxy(0, (bottom - top) * 0.1, _T("ID��"));
	if (isLogin == 1)
		outtextxy(45, (bottom - top) * 0.1, player.ID);

	//��������ֵ
	settextcolor(RED);
	settextstyle(30, 0, _T("����"));
	outtextxy((WIDTH - right) * 0.18, (bottom - top) * 0.95, _T("HP��"));
	sprintf_s(s, "%d", hp);
	outtextxy((WIDTH - right) * 0.55, (bottom - top) * 0.95, s);

	//���Ƶ÷�
	settextcolor(YELLOW);
	outtextxy(right + (WIDTH - right) * 0.1, (bottom - top) * 0.85, _T("��߷֣�"));
	outtextxy(right + (WIDTH - right) * 0.1, (bottom - top) * 0.95, _T("�÷֣�"));
	sprintf_s(s, "%d", nowscore);
	outtextxy(right + (WIDTH - right) * 0.55, (bottom - top) * 0.95, s);
	sprintf_s(s, "%d", Maxscore);
	outtextxy(right + (WIDTH - right) * 0.65, (bottom - top) * 0.85, s);

	FlushBatchDraw();
}

void pauseMenu()								//��ͣ����
{
	putimage(0, 0, &img_bk);
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(90, 0, _T("����"));
	outtextxy((right - left) * 0.38 + left, (bottom - top) * 0.2, _T("��ͣ"));
	settextstyle(50, 0, _T("����"));
	outtextxy((right - left) * 0.35 + left, (bottom - top) * 0.4, _T(" ������Ϸ"));
	outtextxy((right - left) * 0.35 + left, (bottom - top) * 0.5, _T(" ������ҳ"));
	FlushBatchDraw();
	Sleep(2);
	if (MouseHit())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if ((m.x >= (right - left) * 0.35 + left) && (m.x <= (right - left) * 0.6 + left) && (m.y >= (bottom - top) * 0.4) && (m.y < (bottom - top) * 0.5)) //������Ϸ
			{
				gameStatus = 1;
			}
			if ((m.x >= (right - left) * 0.35 + left) && (m.x <= (right - left) * 0.6 + left) && (m.y >= (bottom - top) * 0.5) && (m.y < (bottom - top) * 0.6))  //�˳���Ϸ
			{
				gameStatus = 4;
			}
		}
	}
}

void againMenu()								//���Խ���
{
	putimage(0, 0, &img_bk);
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(90, 0, _T("����"));
	outtextxy((right - left) * 0.2 + left, (bottom - top) * 0.2, _T("�Ƿ����¿�ʼ��"));
	settextstyle(50, 0, _T("����"));
	outtextxy((right - left) * 0.45 + left, (bottom - top) * 0.4, _T(" ��"));
	outtextxy((right - left) * 0.45 + left, (bottom - top) * 0.5, _T(" ��"));
	FlushBatchDraw();
	Sleep(2);
	if (MouseHit())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if ((m.x >= (right - left) * 0.4 + left) && (m.x <= (right - left) * 0.5 + left) && (m.y >= (bottom - top) * 0.4) && (m.y < (bottom - top) * 0.5)) //��
			{
				isgamestart = 1;
				gameStatus = 1;
			}
			if ((m.x >= (right - left) * 0.4 + left) && (m.x <= (right - left) * 0.5 + left) && (m.y >= (bottom - top) * 0.5) && (m.y < (bottom - top) * 0.6))  //��
			{
				gameStatus = 4;
			}
		}
	}
}

void saveMenu()									//�������
{
	putimage(0, 0, &img_bk);
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(90, 0, _T("����"));
	outtextxy((right - left) * 0.25 + left, (bottom - top) * 0.2, _T(" �Ƿ񱣴棿"));
	settextstyle(50, 0, _T("����"));
	outtextxy((right - left) * 0.45 + left, (bottom - top) * 0.4, _T(" ��"));
	outtextxy((right - left) * 0.45 + left, (bottom - top) * 0.5, _T(" ��"));
	FlushBatchDraw();
	Sleep(2);
	if (MouseHit())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if ((m.x >= (right - left) * 0.35 + left) && (m.x <= (right - left) * 0.55 + left) && (m.y >= (bottom - top) * 0.4) && (m.y < (bottom - top) * 0.5))
			{
				saveAccount();
				isgamestart = 1;
				gameStatus = 0;
				Maxscore = player.maxscore;

			}
			if ((m.x >= (right - left) * 0.35 + left) && (m.x <= (right - left) * 0.55 + left) && (m.y >= (bottom - top) * 0.5) && (m.y < (bottom - top) * 0.6))
			{
				isgamestart = 1;
				Maxscore = player.maxscore;
				gameStatus = 0;

			}
		}
	}
}

void saveAccount()
{
	//��������
	player.maxscore = Maxscore;
	//ɾ��ԭ������
	deleteFile();
	//��������
	saveFile();
}

void saveFile()
{
	struct PLAYER read;
	FILE* fp1;
	int flag = 0;
	fp = fopen(".\\gameRecord.txt", "r+");
	fp1 = fopen(".\\linshi.txt", "w+");//��д�½�һ����ʱ�ļ�
	while (fread(&read, sizeof(struct PLAYER), 1, fp))//��ԭ�ļ���һ�����
	{
		if ((player.maxscore >= read.maxscore) && flag == 0)
		{
			fwrite(&player, sizeof(struct PLAYER), 1, fp1);
			flag = 1;
		}
		fwrite(&read, sizeof(struct PLAYER), 1, fp1);
	}
	fclose(fp);
	fclose(fp1);
	remove(".\\gameRecord.txt");
	rename(".\\linshi.txt", ".\\gameRecord.txt");
}

void accountMenu()								//��¼ע��ע������
{
	putimage(0, 0, &img_bk);
	setbkmode(TRANSPARENT);
	settextcolor(GREEN);
	settextstyle(30, 0, _T("����"));
	outtextxy(0, (bottom - top) * 0.1, _T("ID��"));
	if (isLogin == 1)
		outtextxy(45, (bottom - top) * 0.1, player.ID);
	settextcolor(WHITE);
	settextstyle(50, 0, _T("����"));
	if (isLogin == 0)
	{
		outtextxy((right - left) * 0.4 + left, (bottom - top) * 0.35, _T("��¼"));
		outtextxy((right - left) * 0.4 + left, (bottom - top) * 0.45, _T("ע��"));
	}
	else
	{
		outtextxy((right - left) * 0.4 + left, (bottom - top) * 0.35, _T("��ʼ"));
		outtextxy((right - left) * 0.4 + left, (bottom - top) * 0.45, _T("ע��"));
	}
	outtextxy((right - left) * 0.4 + left, (bottom - top) * 0.55, _T("����"));
	if (isLogin)
		outtextxy((right - left) * 0.4 + left, (bottom - top) * 0.65, _T("����"));
	settextcolor(GREEN);
	settextstyle(30, 0, _T("����"));
	outtextxy(0, (bottom - top) * 0.1, _T("ID��"));
	if (isLogin == 1)
		outtextxy(45, (bottom - top) * 0.1, player.ID);
	FlushBatchDraw();
	Sleep(2);
	if (MouseHit())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if ((m.x >= (right - left) * 0.4 + left) && (m.x <= (right - left) * 0.5 + left) && (m.y >= (bottom - top) * 0.35) && (m.y < (bottom - top) * 0.45))
			{
				if (isLogin == 0)
				{
					Login();

				}
				else
				{
					isgamestart = 1;
					gameStatus = 1;
				}

			}
			if ((m.x >= (right - left) * 0.4 + left) && (m.x <= (right - left) * 0.5 + left) && (m.y >= (bottom - top) * 0.45) && (m.y < (bottom - top) * 0.55))
			{
				if (isLogin == 0)
				{
					Registers();

				}
				else
				{
					isgamestart = 1;
					isLogin = 0;
					gameStatus = 5;
				}
			}
			if ((m.x >= (right - left) * 0.4 + left) && (m.x <= (right - left) * 0.5 + left) && (m.y >= (bottom - top) * 0.55) && (m.y < (bottom - top) * 0.65))
			{
				gameStatus = 0;
			}
			if ((m.x >= (right - left) * 0.4 + left) && (m.x <= (right - left) * 0.5 + left) && (m.y >= (bottom - top) * 0.65) && (m.y < (bottom - top) * 0.75))
			{

				deleteFile();
				isLogin = 0;
				isgamestart = 1;
			}

		}
	}
}

int Login()
{
	struct PLAYER read;
	putimage(0, 0, &img_bk);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(90, 0, _T("����"));
	outtextxy((right - left) * 0.4 + left, (bottom - top) * 0.2, _T("��¼"));
	FlushBatchDraw();
	fp = fopen(".\\gameRecord.txt", "a+");
	fread(&read, sizeof(struct PLAYER), 1, fp);
	InputBox(player.ID, 30, "�������˺�");
	while (strcmp(read.ID, player.ID))
	{
		if (!feof(fp))
			fread(&read, sizeof(struct PLAYER), 1, fp);
		else
		{
			//�˻�������
			putimage(0, 0, &img_bk);
			setbkmode(TRANSPARENT);
			settextcolor(RED);
			settextstyle(90, 0, _T("����"));
			outtextxy((right - left) * 0.15 + left, (bottom - top) * 0.4, _T("  �˻�������"));
			FlushBatchDraw();
			Sleep(500);
			fclose(fp);
			return 0;
		}
	}
	InputBox(player.password, 30, "����������");
	if (strcmp(read.password, player.password) == 0)
	{
		fclose(fp);
		putimage(0, 0, &img_bk);
		setbkmode(TRANSPARENT);
		settextcolor(GREEN);
		settextstyle(90, 0, _T("����"));
		outtextxy((right - left) * 0.3 + left, (bottom - top) * 0.4, _T("��½�ɹ�"));
		isLogin = 1;
		player = read;
		//��ȡ��������
		Maxscore = player.maxscore;
		FlushBatchDraw();
		Sleep(500);
		return 1;
	}
	else
	{
		putimage(0, 0, &img_bk);
		setbkmode(TRANSPARENT);
		settextcolor(RED);
		settextstyle(90, 0, _T("����"));
		outtextxy((right - left) * 0.3 + left, (bottom - top) * 0.4, _T("�������"));
		FlushBatchDraw();
		Sleep(500);
		return 0;
	}
}

int Registers()
{
	struct PLAYER newaccount;
	struct PLAYER read;
	putimage(0, 0, &img_bk);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(90, 0, _T("����"));
	outtextxy((right - left) * 0.4 + left, (bottom - top) * 0.2, _T("ע��"));
	FlushBatchDraw();
	fp = fopen(".\\gameRecord.txt", "a+");
	fread(&read, sizeof(struct PLAYER), 1, fp);
	InputBox(newaccount.ID, 30, "ע�ᣬ�뽨�����˺�");
	if (_ifblank(newaccount.ID) == 0)
	{
		putimage(0, 0, &img_bk);
		setbkmode(TRANSPARENT);
		settextcolor(RED);
		settextstyle(90, 0, _T("����"));
		outtextxy((right - left) * 0.3 + left, (bottom - top) * 0.4, _T("�������"));
		FlushBatchDraw();
		Sleep(500);
		return 0;
	}
	while (1)
	{
		if (strcmp(newaccount.ID, read.ID))
		{
			if (!feof(fp))
			{
				fread(&read, sizeof(struct PLAYER), 1, fp);
			}
			else
				break;
		}
		else
		{
			//���ʧ��
			putimage(0, 0, &img_bk);
			setbkmode(TRANSPARENT);
			settextcolor(RED);
			settextstyle(90, 0, _T("����"));
			outtextxy((right - left) * 0.15 + left, (bottom - top) * 0.4, _T("  �˻��Ѵ���"));
			FlushBatchDraw();
			Sleep(500);
			return 0;
		}
	}
	InputBox(newaccount.password, 30, "����������");
	//����ע��ɹ�
	putimage(0, 0, &img_bk);
	setbkmode(TRANSPARENT);
	settextcolor(GREEN);
	settextstyle(90, 0, _T("����"));
	outtextxy((right - left) * 0.25 + left, (bottom - top) * 0.4, _T("ע��ɹ�"));
	FlushBatchDraw();
	Sleep(500);
	newaccount.maxscore = 0;                    //��ʼ������
	fwrite(&newaccount, sizeof(struct PLAYER), 1, fp);
	fclose(fp);
	return 1;
}

void listMenu()									//���а����
{
	struct PLAYER read;
	putimage(0, 0, &img_bk);
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(70, 0, _T("����"));
	outtextxy((right - left) * 0.37 + left, (bottom - top) * 0.01, _T("���а�"));
	settextstyle(30, 0, _T("����"));
	outtextxy(right, (bottom - top) * 0.01, _T("����"));
	settextcolor(BLACK);
	settextstyle(40, 0, _T("����"));
	outtextxy(1 * WIDTH - right + left - 20, (bottom - top) * 0.1 + (bottom - top) * 0.04, _T("����"));
	outtextxy(2 * (WIDTH - right) + left - 20, (bottom - top) * 0.1 + (bottom - top) * 0.04, _T("�û���"));
	outtextxy(3 * (WIDTH - right) + left - 20, (bottom - top) * 0.1 + (bottom - top) * 0.04, _T("��߷�"));
	fp = fopen(".\\gameRecord.txt", "a+");
	char s[30];
	fread(&read, sizeof(struct PLAYER), 1, fp);
	i = 0;
	settextstyle(20, 0, _T("����"));
	settextcolor(YELLOW);
	while (!feof(fp) && i < 10)
	{
		sprintf_s(s, "%d", i + 1);
		outtextxy(1 * WIDTH - right + left, (bottom - top) * 0.2 + (bottom - top) * 0.05 * i, s);
		outtextxy(2 * (WIDTH - right) + left, (bottom - top) * 0.2 + (bottom - top) * 0.05 * i, read.ID);
		sprintf_s(s, "%d", read.maxscore);
		outtextxy(3 * (WIDTH - right) + left, (bottom - top) * 0.2 + (bottom - top) * 0.05 * i, s);
		fread(&read, sizeof(struct PLAYER), 1, fp);
		i++;
	}
	fclose(fp);
	FlushBatchDraw();
	if (MouseHit())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if ((m.x >= right && m.x <= right + 80) && (m.y >= 0 && m.y < 40))
			{
				gameStatus = 0;
			}
		}
	}
}

void gameOver()									//��Ϸ����
{
	EndBatchDraw();
	closegraph();
}

void ballHitBar()								//С�򷴵��Ƕȼ���
{
	float distance = Ball.x - Bar.x;
	float angle = (90 * distance / Bar.width);
	Ball.vx = Ballv * sin(angle * PI / 180.0);
	Ball.vy = -Ballv * cos(angle * PI / 180.0);
}

void deleteFile()								//ɾ������
{
	struct PLAYER read;
	FILE* fp1;
	fp = fopen(".\\gameRecord.txt", "r+");
	fp1 = fopen(".\\linshi.txt", "w+");//��д�½�һ����ʱ�ļ�
	while (fread(&read, sizeof(struct PLAYER), 1, fp))//��ԭ�ļ���һ�����
	{
		if (strcmp(player.ID, read.ID) != 0)//����Ҫɾ��������
		{
			fwrite(&read, sizeof(struct PLAYER), 1, fp1);
		}
	}
	fclose(fp);
	fclose(fp1);
	remove(".\\gameRecord.txt");
	rename(".\\linshi.txt", ".\\gameRecord.txt");
}

int _ifblank(char judge[])					//���������ַ���
{
	char ch;
	char true_[20];
	int i, j;
	for (i = 0, j = 0; i < strlen(judge); i++) {
		if (judge[i] == ' ')
			continue;
		else {
			true_[j] = judge[i];
			j++;
		}
	}
	true_[j] = '\0';
	return strlen(true_);					//���������ַ�������������ո�
}