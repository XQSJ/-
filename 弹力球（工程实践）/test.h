#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<time.h>
#include<graphics.h>
#include<conio.h>
#include<math.h>
#include<Windows.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include<stdio.h>
#include<string.h>

//定义常量
#define PI 3.1415926
#define WIDTH  1280
#define HIGH 680
#define BLOCKNUM 10     //砖块列数
#define BLOCKHANG 20    //砖块行数

void gameStartUp();
void mainMenu();
void playMenu();
void playStartUp();
void updateWithoutInput();
void updateWithInput();
void playShow();
void pauseMenu();
void againMenu();
void saveMenu();
void saveAccount();
void accountMenu();
int Login();
int Registers();
void listMenu();
void gameOver();
void ballHitBar();
void deleteFile();
void saveFile();
int _ifblank(char judge[]);
