//自定义函数，变量
#include"test.h"
//定义全局变量
int i, j;
int gameStatus; //游戏状态
int dwidth, dhigh; //游戏界面与边界距离
int left, right, top, bottom;//游戏边界大小
//砖块
int brick[BLOCKHANG][BLOCKNUM];
int brick_left, brick_top, brick_right, brick_bottom;
int brick_width, brick_high;
//挡板
struct BAR {
	int x, y;
	int width, high;
	int left, top, right, bottom;
}Bar;
//小球
struct BALL {
	float vx, vy;
	float x, y;
	int radius;

}Ball;
float Ballv;											//小球合速度
//图片
IMAGE	img_bk, img_bar, img_yb, img_bb, img_rb, img_gb;
//登录状态
int isLogin;
//鼠标
MOUSEMSG m;
//小球发射
int shootball;
//本局得分
int nowscore;
//血量
int hp;
//游戏开始
int isgamestart;
//文件变量
FILE* fp;
//玩家
struct PLAYER {
	char ID[30];    //账户名
	char password[20];   //密码
	int maxscore;		 //最高积分
}player;
//最高分数
int Maxscore;
void gameStartUp()                             //项目初始化
{
	//随机种子
	srand((unsigned int)time(0));
	//播放音乐
	mciSendString("open .\\Credits.mp3 alias bkmusic", NULL, 0, NULL);
	mciSendString("open .\\hitbrick.mp3 alias hbkmusic", NULL, 0, NULL);
	mciSendString("open .\\Game Over .mp3 alias overmusic", NULL, 0, NULL);
	mciSendString("play bkmusic repeat", NULL, 0, NULL);
	//确定画幅
	initgraph(WIDTH, HIGH);
	//设置窗口标题文字
	HWND hwnd = GetHWnd();
	SetWindowText(hwnd, _T("弹力球"));
	//设置游戏区域大小
	dwidth = 200;
	left = dwidth, right = WIDTH - dwidth;
	dhigh = 10;
	top = dhigh, bottom = HIGH - dhigh;
	//加载图片
	loadimage(&img_bk, ".\\bk.jpg", WIDTH, HIGH, 1);
	//开始绘画
	BeginBatchDraw();
	//游戏状态
	gameStatus = 0;
	//是否登录
	isLogin = 0;
	//是否开始
	isgamestart = 1;
}

void mainMenu()                                //主界面
{
	putimage(0, 0, &img_bk);
	setbkmode(TRANSPARENT);

	settextcolor(GREEN);
	settextstyle(30, 0, _T("黑体"));
	outtextxy(0, (bottom - top) * 0.1, _T("ID："));
	if (isLogin == 1)
		outtextxy(45, (bottom - top) * 0.1, player.ID);
	settextcolor(WHITE);
	settextstyle(90, 0, _T("黑体"));
	outtextxy((right - left) * 0.35 + left, (bottom - top) * 0.2, _T("弹力球"));
	settextstyle(50, 0, _T("黑体"));
	outtextxy((right - left) * 0.35 + left, (bottom - top) * 0.4, _T(" 进入游戏"));
	outtextxy((right - left) * 0.35 + left, (bottom - top) * 0.5, _T(" 退出游戏"));
	outtextxy((right - left) * 0.41 + left, (bottom - top) * 0.6, _T("排行榜"));
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
				gameStatus = 5;			//进入登录界面

			}
			if ((m.x >= (right - left) * 0.35 + left) && (m.x <= (right - left) * 0.6 + left) && (m.y >= (bottom - top) * 0.5) && (m.y < (bottom - top) * 0.6))
			{
				gameStatus = -1;			//退出
			}
			if ((m.x >= (right - left) * 0.41 + left) && (m.x <= (right - left) * 0.6 + left) && (m.y >= (bottom - top) * 0.6) && (m.y < (bottom - top) * 0.76))
			{
				gameStatus = 6;			//进入排行榜界面
			}
		}
	}
}

void playMenu()                                //游玩界面
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
	//砖块
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
	//挡板
	Bar.high = (bottom - top) / 40;
	Bar.width = (right - left) / 8;
	Bar.x = (right - left) / 2;
	Bar.y = 6 * (bottom - top) / 7;
	Bar.left = Bar.x - Bar.width / 2;
	Bar.right = Bar.x + Bar.width / 2;
	Bar.top = Bar.y - Bar.high / 2;
	Bar.bottom = Bar.y + Bar.high / 2;
	//小球
	Ball.radius = 5;
	Ball.vx = 0;
	Ball.vy = 0;
	Ball.x = Bar.x;
	Ball.y = Bar.top - Ball.radius;
	//加载图片
	loadimage(&img_bar, ".\\bar.jpg", Bar.width, Bar.high, 1);
	loadimage(&img_yb, ".\\yellowbrick.jpg", brick_width - 1, brick_high - 1, 1);
	loadimage(&img_bb, ".\\bluebrick.jpg", brick_width - 1, brick_high - 1, 1);
	loadimage(&img_gb, ".\\greenbrick.jpg", brick_width - 1, brick_high - 1, 1);
	loadimage(&img_rb, ".\\redbrick.jpg", brick_width - 1, brick_high - 1, 1);
}

void updateWithoutInput()
{
	//砖块下移
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
	//小球撞击砖块
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
	//小球反弹板
	if (Ball.x >= Bar.left && Ball.x <= Bar.right && Ball.y >= Bar.top && shootball == 1 && Ball.y < Bar.bottom)
	{
		ballHitBar();
	}
	//小球反弹
	if (Ball.x - Ball.radius <= left)
		Ball.vx = fabs(Ball.vx);
	if (Ball.x + Ball.radius >= right)
		Ball.vx = -fabs(Ball.vx);
	if (Ball.y - Ball.radius <= 0)
		Ball.vy = fabs(Ball.vy);
	//小球移动
	static int ballspeed = 10;
	if (ballspeed == 0)
	{
		Ball.x += Ball.vx;
		Ball.y += Ball.vy;

		ballspeed = 10;
	}
	else
		ballspeed--;
	//小球开始随板子动
	if (shootball == 0)
		Ball.x = Bar.x;
	//更新最高分数
	if (Maxscore < nowscore)
		Maxscore = nowscore;
	//游戏失败
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
	//绘制范围
	setcolor(BLACK);
	line(left, top, left, bottom);				//左边界
	line(right, top, right, bottom);			//右边界
	line(left, top, right, top);				//上边界
	line(left, bottom, right, bottom);			//下边界
	line(left, (BLOCKHANG - 1) * brick_high, right, (BLOCKHANG - 1) * brick_high);//失败底线
	//绘制小球
	setcolor(WHITE);
	setfillcolor(WHITE);
	fillcircle(Ball.x, Ball.y, Ball.radius);
	//绘制挡板
	putimage(Bar.left, Bar.top, &img_bar);
	//绘制砖块
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
	//绘制用户名
	settextcolor(GREEN);
	settextstyle(30, 0, _T("黑体"));
	outtextxy(0, (bottom - top) * 0.1, _T("ID："));
	if (isLogin == 1)
		outtextxy(45, (bottom - top) * 0.1, player.ID);

	//绘制生命值
	settextcolor(RED);
	settextstyle(30, 0, _T("黑体"));
	outtextxy((WIDTH - right) * 0.18, (bottom - top) * 0.95, _T("HP："));
	sprintf_s(s, "%d", hp);
	outtextxy((WIDTH - right) * 0.55, (bottom - top) * 0.95, s);

	//绘制得分
	settextcolor(YELLOW);
	outtextxy(right + (WIDTH - right) * 0.1, (bottom - top) * 0.85, _T("最高分："));
	outtextxy(right + (WIDTH - right) * 0.1, (bottom - top) * 0.95, _T("得分："));
	sprintf_s(s, "%d", nowscore);
	outtextxy(right + (WIDTH - right) * 0.55, (bottom - top) * 0.95, s);
	sprintf_s(s, "%d", Maxscore);
	outtextxy(right + (WIDTH - right) * 0.65, (bottom - top) * 0.85, s);

	FlushBatchDraw();
}

void pauseMenu()								//暂停界面
{
	putimage(0, 0, &img_bk);
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(90, 0, _T("黑体"));
	outtextxy((right - left) * 0.38 + left, (bottom - top) * 0.2, _T("暂停"));
	settextstyle(50, 0, _T("黑体"));
	outtextxy((right - left) * 0.35 + left, (bottom - top) * 0.4, _T(" 继续游戏"));
	outtextxy((right - left) * 0.35 + left, (bottom - top) * 0.5, _T(" 返回首页"));
	FlushBatchDraw();
	Sleep(2);
	if (MouseHit())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if ((m.x >= (right - left) * 0.35 + left) && (m.x <= (right - left) * 0.6 + left) && (m.y >= (bottom - top) * 0.4) && (m.y < (bottom - top) * 0.5)) //继续游戏
			{
				gameStatus = 1;
			}
			if ((m.x >= (right - left) * 0.35 + left) && (m.x <= (right - left) * 0.6 + left) && (m.y >= (bottom - top) * 0.5) && (m.y < (bottom - top) * 0.6))  //退出游戏
			{
				gameStatus = 4;
			}
		}
	}
}

void againMenu()								//重试界面
{
	putimage(0, 0, &img_bk);
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(90, 0, _T("黑体"));
	outtextxy((right - left) * 0.2 + left, (bottom - top) * 0.2, _T("是否重新开始？"));
	settextstyle(50, 0, _T("黑体"));
	outtextxy((right - left) * 0.45 + left, (bottom - top) * 0.4, _T(" 是"));
	outtextxy((right - left) * 0.45 + left, (bottom - top) * 0.5, _T(" 否"));
	FlushBatchDraw();
	Sleep(2);
	if (MouseHit())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if ((m.x >= (right - left) * 0.4 + left) && (m.x <= (right - left) * 0.5 + left) && (m.y >= (bottom - top) * 0.4) && (m.y < (bottom - top) * 0.5)) //是
			{
				isgamestart = 1;
				gameStatus = 1;
			}
			if ((m.x >= (right - left) * 0.4 + left) && (m.x <= (right - left) * 0.5 + left) && (m.y >= (bottom - top) * 0.5) && (m.y < (bottom - top) * 0.6))  //否
			{
				gameStatus = 4;
			}
		}
	}
}

void saveMenu()									//保存界面
{
	putimage(0, 0, &img_bk);
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(90, 0, _T("黑体"));
	outtextxy((right - left) * 0.25 + left, (bottom - top) * 0.2, _T(" 是否保存？"));
	settextstyle(50, 0, _T("黑体"));
	outtextxy((right - left) * 0.45 + left, (bottom - top) * 0.4, _T(" 是"));
	outtextxy((right - left) * 0.45 + left, (bottom - top) * 0.5, _T(" 否"));
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
	//更新数据
	player.maxscore = Maxscore;
	//删除原本数据
	deleteFile();
	//存入数据
	saveFile();
}

void saveFile()
{
	struct PLAYER read;
	FILE* fp1;
	int flag = 0;
	fp = fopen(".\\gameRecord.txt", "r+");
	fp1 = fopen(".\\linshi.txt", "w+");//读写新建一个临时文件
	while (fread(&read, sizeof(struct PLAYER), 1, fp))//从原文件读一个结点
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

void accountMenu()								//登录注册注销界面
{
	putimage(0, 0, &img_bk);
	setbkmode(TRANSPARENT);
	settextcolor(GREEN);
	settextstyle(30, 0, _T("黑体"));
	outtextxy(0, (bottom - top) * 0.1, _T("ID："));
	if (isLogin == 1)
		outtextxy(45, (bottom - top) * 0.1, player.ID);
	settextcolor(WHITE);
	settextstyle(50, 0, _T("黑体"));
	if (isLogin == 0)
	{
		outtextxy((right - left) * 0.4 + left, (bottom - top) * 0.35, _T("登录"));
		outtextxy((right - left) * 0.4 + left, (bottom - top) * 0.45, _T("注册"));
	}
	else
	{
		outtextxy((right - left) * 0.4 + left, (bottom - top) * 0.35, _T("开始"));
		outtextxy((right - left) * 0.4 + left, (bottom - top) * 0.45, _T("注销"));
	}
	outtextxy((right - left) * 0.4 + left, (bottom - top) * 0.55, _T("返回"));
	if (isLogin)
		outtextxy((right - left) * 0.4 + left, (bottom - top) * 0.65, _T("销户"));
	settextcolor(GREEN);
	settextstyle(30, 0, _T("黑体"));
	outtextxy(0, (bottom - top) * 0.1, _T("ID："));
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
	settextstyle(90, 0, _T("黑体"));
	outtextxy((right - left) * 0.4 + left, (bottom - top) * 0.2, _T("登录"));
	FlushBatchDraw();
	fp = fopen(".\\gameRecord.txt", "a+");
	fread(&read, sizeof(struct PLAYER), 1, fp);
	InputBox(player.ID, 30, "请输入账号");
	while (strcmp(read.ID, player.ID))
	{
		if (!feof(fp))
			fread(&read, sizeof(struct PLAYER), 1, fp);
		else
		{
			//账户不存在
			putimage(0, 0, &img_bk);
			setbkmode(TRANSPARENT);
			settextcolor(RED);
			settextstyle(90, 0, _T("黑体"));
			outtextxy((right - left) * 0.15 + left, (bottom - top) * 0.4, _T("  账户不存在"));
			FlushBatchDraw();
			Sleep(500);
			fclose(fp);
			return 0;
		}
	}
	InputBox(player.password, 30, "请输入密码");
	if (strcmp(read.password, player.password) == 0)
	{
		fclose(fp);
		putimage(0, 0, &img_bk);
		setbkmode(TRANSPARENT);
		settextcolor(GREEN);
		settextstyle(90, 0, _T("黑体"));
		outtextxy((right - left) * 0.3 + left, (bottom - top) * 0.4, _T("登陆成功"));
		isLogin = 1;
		player = read;
		//读取各项数据
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
		settextstyle(90, 0, _T("黑体"));
		outtextxy((right - left) * 0.3 + left, (bottom - top) * 0.4, _T("密码错误"));
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
	settextstyle(90, 0, _T("黑体"));
	outtextxy((right - left) * 0.4 + left, (bottom - top) * 0.2, _T("注册"));
	FlushBatchDraw();
	fp = fopen(".\\gameRecord.txt", "a+");
	fread(&read, sizeof(struct PLAYER), 1, fp);
	InputBox(newaccount.ID, 30, "注册，请建立新账号");
	if (_ifblank(newaccount.ID) == 0)
	{
		putimage(0, 0, &img_bk);
		setbkmode(TRANSPARENT);
		settextcolor(RED);
		settextstyle(90, 0, _T("黑体"));
		outtextxy((right - left) * 0.3 + left, (bottom - top) * 0.4, _T("输入错误"));
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
			//输出失败
			putimage(0, 0, &img_bk);
			setbkmode(TRANSPARENT);
			settextcolor(RED);
			settextstyle(90, 0, _T("黑体"));
			outtextxy((right - left) * 0.15 + left, (bottom - top) * 0.4, _T("  账户已存在"));
			FlushBatchDraw();
			Sleep(500);
			return 0;
		}
	}
	InputBox(newaccount.password, 30, "请输入密码");
	//输入注册成功
	putimage(0, 0, &img_bk);
	setbkmode(TRANSPARENT);
	settextcolor(GREEN);
	settextstyle(90, 0, _T("黑体"));
	outtextxy((right - left) * 0.25 + left, (bottom - top) * 0.4, _T("注册成功"));
	FlushBatchDraw();
	Sleep(500);
	newaccount.maxscore = 0;                    //初始化数据
	fwrite(&newaccount, sizeof(struct PLAYER), 1, fp);
	fclose(fp);
	return 1;
}

void listMenu()									//排行榜界面
{
	struct PLAYER read;
	putimage(0, 0, &img_bk);
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(70, 0, _T("黑体"));
	outtextxy((right - left) * 0.37 + left, (bottom - top) * 0.01, _T("排行榜"));
	settextstyle(30, 0, _T("黑体"));
	outtextxy(right, (bottom - top) * 0.01, _T("返回"));
	settextcolor(BLACK);
	settextstyle(40, 0, _T("黑体"));
	outtextxy(1 * WIDTH - right + left - 20, (bottom - top) * 0.1 + (bottom - top) * 0.04, _T("排名"));
	outtextxy(2 * (WIDTH - right) + left - 20, (bottom - top) * 0.1 + (bottom - top) * 0.04, _T("用户名"));
	outtextxy(3 * (WIDTH - right) + left - 20, (bottom - top) * 0.1 + (bottom - top) * 0.04, _T("最高分"));
	fp = fopen(".\\gameRecord.txt", "a+");
	char s[30];
	fread(&read, sizeof(struct PLAYER), 1, fp);
	i = 0;
	settextstyle(20, 0, _T("黑体"));
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

void gameOver()									//游戏结束
{
	EndBatchDraw();
	closegraph();
}

void ballHitBar()								//小球反弹角度计算
{
	float distance = Ball.x - Bar.x;
	float angle = (90 * distance / Bar.width);
	Ball.vx = Ballv * sin(angle * PI / 180.0);
	Ball.vy = -Ballv * cos(angle * PI / 180.0);
}

void deleteFile()								//删除数据
{
	struct PLAYER read;
	FILE* fp1;
	fp = fopen(".\\gameRecord.txt", "r+");
	fp1 = fopen(".\\linshi.txt", "w+");//读写新建一个临时文件
	while (fread(&read, sizeof(struct PLAYER), 1, fp))//从原文件读一个结点
	{
		if (strcmp(player.ID, read.ID) != 0)//不是要删除的内容
		{
			fwrite(&read, sizeof(struct PLAYER), 1, fp1);
		}
	}
	fclose(fp);
	fclose(fp1);
	remove(".\\gameRecord.txt");
	rename(".\\linshi.txt", ".\\gameRecord.txt");
}

int _ifblank(char judge[])					//检验输入字符数
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
	return strlen(true_);					//返回输入字符数量，不计入空格
}