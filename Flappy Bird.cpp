#include <iostream>
#include <Windows.h>
#include <Shlwapi.h>
#include <conio.h>
#include <string>
#include <fstream>

//=============CAC HAM DO HOA================

HANDLE hConsoleOutput;
HANDLE hConsoleInput;
 
// Ham thay doi kich co man hinh console.
void resizeConsole(SHORT width, SHORT height)
{
    /*HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, width, height, TRUE);*/
 
    COORD crd = { width, height };
    SMALL_RECT rec = { 0, 0, width - 1, height - 1 };
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleWindowInfo(hConsoleOutput, TRUE, &rec);
    SetConsoleScreenBufferSize(hConsoleOutput, crd);
}
 
// Ham dich chuyen con tro den toa do x, y.
void gotoXY(SHORT x, SHORT y)
{
    COORD Cursor_an_Pos = { x, y };
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}
 
// Ham to mau.
void setColor(WORD color)
{
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
 
    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
 
    WORD wAttributes = screen_buffer_info.wAttributes;
    color &= 0x000f;
    wAttributes &= 0xfff0;
    wAttributes |= color;
 
    SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}
 
// Ham thay doi mau nen hien thi.
void setBackgroundColor(WORD color)
{
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
 
    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
 
    WORD wAttributes = screen_buffer_info.wAttributes;
    color &= 0x000f;
    color <<= 4; // Dich trai 3 bit de phu hop voi mau nen
    wAttributes &= 0xff0f; // Cai 0 cho 1 bit chu nhay va 3 bit mau nen
    wAttributes |= color;
 
    SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}
 
WORD textattr()
{
    CONSOLE_SCREEN_BUFFER_INFO ConsoleInfo;
    GetConsoleScreenBufferInfo(hConsoleOutput, &ConsoleInfo);
    return ConsoleInfo.wAttributes;
}
 
void resettextattr()
{
    DWORD Mau_Mac_Dinh = textattr();
    SetConsoleTextAttribute(hConsoleOutput, Mau_Mac_Dinh);
}
 
// Ham in mau chu va thay doi mai nen hien thi theo vi tri x, y.
void setColorBGTextXY(SHORT x, SHORT y, WORD color, WORD background, LPSTR str, ...)
{
    gotoXY(x, y);
    setBackgroundColor(background);
    setColor(color);
 
    /*In duoc nhieu chu hon*/
    va_list args;
    va_start(args, str);
    vprintf(str, args);
    va_end(args);
    /*In duoc nhieu chu hon*/
 
    resettextattr();
    //setColor(7);
}
 
// Ham an hien con tro.
void Cursor(BOOL bVisible, DWORD dwSize)
{
    CONSOLE_CURSOR_INFO ConsoleCursorInfo;
    ConsoleCursorInfo.bVisible = bVisible;
    ConsoleCursorInfo.dwSize = dwSize; // Phan tram bao trum o cua con tro chuot
    SetConsoleCursorInfo(hConsoleOutput, &ConsoleCursorInfo);
}
 
// Xoa so luong dong, SStartPos - Vi tri bat dau xoa, SNumberRow so luong dong can xoa.
void deleteRow(SHORT SStartPos, SHORT SNumberRow)
{
    CONSOLE_SCREEN_BUFFER_INFO  ConsoleInfo;
    COORD Pos = { 0, SStartPos };
    DWORD Tmp;
    GetConsoleScreenBufferInfo(hConsoleOutput, &ConsoleInfo);
    FillConsoleOutputCharacter(hConsoleOutput, ' ', ConsoleInfo.dwSize.X * SNumberRow, Pos, &Tmp);
    FillConsoleOutputAttribute(hConsoleOutput, 15, ConsoleInfo.dwSize.X * SNumberRow, Pos, &Tmp);
    SetConsoleCursorPosition(hConsoleOutput, Pos);
}
 
//Xoa con tro chuot
void hideMouse()
{
    HANDLE hOut;
    CONSOLE_CURSOR_INFO ConCurInf;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    ConCurInf.dwSize = 10;
    ConCurInf.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &ConCurInf);
}



//================ FLAPPY BIRTH==============

using namespace std;
#define consoleWith 120

//----------Data---------------------------
int score = 0; // diem nguoi choi
string user;
void xuLyChuoi(string &name);
struct Node
{
	int data;   //luu vi tri khoang trong tu tren xuong
	Node* link;
};
struct nguoiChoi
{
	string hoTen;
	int diem;	
};
struct Tnode 
{
	nguoiChoi nc;
	Tnode *left;
	Tnode *right;
};
Tnode *getTNode (nguoiChoi nc)
{
	Tnode *g = new Tnode;
	g->nc = nc;
	
	g->left = NULL;
	g->right = NULL;
	return g;
}
//Bien toan cuc
Tnode *tree = NULL;
void themNguoiChoi(nguoiChoi nc)
{
	xuLyChuoi(nc.hoTen);
	if(nc.hoTen.length() == 1)
		return;
	Tnode *temp = getTNode(nc);
	if(tree == NULL)
	{
		tree = temp;
	}
	else
	{
		Tnode *current = tree;
		Tnode *parent;
		while(current != NULL)
		{
			parent = current;
			if(nc.diem > current->nc.diem)
			{
				current = current->right;
			}
			else
				current = current->left;
		}
		if(nc.diem > parent->nc.diem)
			parent->right = temp;
		else
			parent->left = temp;
	}
}

Node* getNode(int x)
{
	Node* p = new Node;
	if (p==NULL)
	{
		cout<<"Loi Bo Nho!!!";
		return NULL;
	}
	p->data = x;
	p->link = NULL;
	return p;
}
struct List
{
	Node* first;
	Node* last;
};
void init(List &l)
{
	l.first = NULL;
	l.last = NULL;
}
void addLast(List &l,int x)
{
	Node* p = getNode(x);
	if (l.first==NULL)
	{
		l.first=p;
		l.last=p;
	}
	else
	{
		l.last->link = p;
		l.last = p;
	}
	
}
void xoaFirst(List &l)
{
	Node* p = l.first;
	l.first = l.first->link;
	delete(p);
}


//-----Menu----------------------------------
void veMenuChinh(int index);
void tieuDe();
int menuChinh();
int menuTenNguoiChoi(int vt, int index);
void nhapTenNguoiChoi();
void xuLyMenuChinh();
void thoat();
void xuLyMenuAndGame();
void veMenuAndGame(int index);
int menuAndGame();


//----XuLy------------------------------------
void khoiTaoMap(List &l);
void veMap(List &l,int &kt);
void veChim(int x,int y);
void diChuyenChim(int &y);
void endgame(); //Ket thuc game
bool GameOver(List l,int vtX, int vtY);
void diChuyen(bool &flap, int vtX, int &vtY);
void Diem(int k, int vtY);
void tocDoGame(int score);
void Game();
void GhiDuLieu();
void DocDuLieu();
void BangXepHang();
void xuatBangXepHang(Tnode *tree, int tdX, int tdY);


int main()
{
	hideMouse();
	tieuDe();
	xuLyMenuChinh();
	return 0;
}

void veMenuChinh(int index)
{
	LPSTR s;
	s = " TRO CHOI MOI   ";
	setColorBGTextXY((consoleWith/2)- (strlen(s)/2),5,15,index==0?1:0,s);
	s = " BANG XEP HANG  ";
	setColorBGTextXY((consoleWith/2)- (strlen(s)/2),6,15,index==1?1:0,s);
	s = " THOAT TRO CHOI ";
	setColorBGTextXY((consoleWith/2)- (strlen(s)/2),7,15,index==2?1:0,s);
}
void tieuDe()
{
	LPSTR s;
	s = "=======================================";
	setColorBGTextXY((consoleWith/2)- (strlen(s)/2),1,11,16,s);
	s = "=             FLAPPY BIRD             =";
	setColorBGTextXY((consoleWith/2)- (strlen(s)/2),2,11,16,s);
	s = "=======================================";
	setColorBGTextXY((consoleWith/2)- (strlen(s)/2),3,11,16,s);
}
int menuChinh()
{
	int index = 0;
	do
	{
		veMenuChinh(index);
		char key = getch();
		if (key == 13)
			return index;
		if (key == 80)
			index++;
		if (key == 72)
			index--;
		if (index == -1)
			index = 2;
		if (index == 3)
			index =0;
	}
	while (true);
} 
int menuTenNguoiChoi(int vt, int index)
{
	LPSTR s;
	s = "<<           BAT DAU             >>";
	setColorBGTextXY(vt,5,15,index==0?3:17,s);
	s = "<<           QUAY LAI            >>";
	setColorBGTextXY(vt,6,15,index==1?3:17,s);
	s = "<<            THOAT              >>";
	setColorBGTextXY(vt,7,15,index==2?3:17,s);
	
}

void xuLyMenuChinh()
{
	int key = menuChinh();
	switch (key)
	{
		case 0:
			deleteRow(5,3);
			nhapTenNguoiChoi();
			break;
		case 1:
			system("cls");
			tieuDe();
			BangXepHang();
			break;
		case 2:
			thoat();

	}
		
}
void nhapTenNguoiChoi()
{
	LPSTR s;
	int index = 0;
	s = "<< TEN NGUOI CHOI:               >>";
	int vt = 43;
	setColorBGTextXY(vt,5,15,17,s);
	gotoXY(vt+19,5);
	getline(cin,user);
	char key;
	do
	{
		menuTenNguoiChoi(vt,index);
		key = getch();
		if (key == 80)
			index++;
		if (key == 72)
			index--;
		if (index == -1)
			index = 2;
		if (index == 3)
			index = 0;
	}
	while (key!=13);
	if (index == 1)
	{
		deleteRow(5,4);
		xuLyMenuChinh();
	}
	else 
		if (index == 2)
			thoat();
		else
		{
			Game();
		}
	
}
void thoat()
{
	
	setBackgroundColor(0);
	system("cls");
	cout<<"                                   ***  *     *   *     ***   * *** ***"<<endl;
	cout<<"                                    *  *-*   * * * *    ** *  * **   *"<<endl;
	cout<<"                                    * *   * *   *   *   ***   * ***  *" ;
	exit(0);
}


void veCot(int x, int y, int k)
{
	if (k!=0)
	{
		
		LPSTR s = "=";
		for (int i = y; i<=k; i++)
			setColorBGTextXY(x,i,2,2,s);
		for (int i = k+5; i<=20; i++)
			setColorBGTextXY(x,i,2,2,s);
	}
}

void khoiTaoMap(List &l)
{
	for (int i=0; i<=90; i++)
		addLast(l,0);
}
void veMap(List &l,int &kt)
{
		Node* p =l.first;
		gotoXY(30,21);
		cout<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^";
		for (int x =30;  x<=119; x+=1)
		{
			veCot(x,5,p->data);
			p = p->link;
			if (p==NULL)
				p = l.first;
		}
		if (kt==0)
		{
			int x = rand()%9+6;
			addLast(l,x);
			addLast(l,x);
			kt++;
		}
		else
		{
			kt++;
			addLast(l,0);
			addLast(l,0);
			if (kt==7)
				kt=0;
		}
		xoaFirst(l);
		xoaFirst(l);
}
void veChim(int x,int y)
{
	LPSTR s="=}>";
	setColorBGTextXY(x,y,1,0,s); 
      
}
void diChuyenChim(int &y)
{
	if (kbhit())
	{
		y--;
		getch();
		if (kbhit())
		{
			y--;
			getch();
			if (kbhit())
			{
				y--;
				getch();
			}
		}
	}
	else y++;
	//Giu chim o vi tri gioi han
	if(y < 5) y = 5;
	veChim(40,y);
}
void endgame() //Ket thuc game
{
    system("cls");   
    setColor(12);
    cout<<""<<endl<<endl;
    cout<<"\t\t\t ------------------------------------------------------------------------- "<<endl;
    cout<<"\t\t\t|    *****      *     *       * ******       ****  *       ****** ****    |"<<endl;
    cout<<"\t\t\t|   *          * *    * *   * * *           *    *  *     * *     *   *   |"<<endl;
    cout<<"\t\t\t|   *  ****   *   *   *  * *  * *****       *    *   *   *  ****  ****    |"<<endl;
    cout<<"\t\t\t|   *  *  *  *******  *   *   * *           *    *    * *   *     * *     |"<<endl;
    cout<<"\t\t\t|    *****  *       * *       * ******       ****      *    ***** *   *   |"<<endl;
    cout<<"\t\t\t ------------------------------------------------------------------------- "<<endl;
    cout<<""<<endl<<endl;
    // in ten  va diem
    gotoXY(48,9);
    cout<<" -------------------------";
    gotoXY(48,10);
    cout<<"|   User:                 |";
    gotoXY(48,11);
    cout<<"|   Score:                |";
    gotoXY(48,12);
    cout<<" -------------------------";
    gotoXY(60,10);
    cout<<user;
    gotoXY(60,11);
    cout<<score;
    // goi Menu andGame;
    xuLyMenuAndGame();
    
}
bool GameOver(List l,int vtX, int vtY)
{
	/*Day la vi tri khoang trong cho chim qua*/
	Node *temp = l.first;
	for (int i =0; i<8; i++)
	{
		temp = temp->link;
	}
	int cotY = temp->data;
	int cotY1 = temp->link->link->data;
	/*Tinh diem*/
	Diem(cotY, vtY);
	/*Cham cot*/
	if(cotY != 0)
	{
	 	if((vtY<=cotY || vtY>=cotY+4))
	 	{
	 		return false;
		}
	}
	else if(cotY1 != 0)
	{
	 	if((vtY<=cotY1 || vtY>=cotY1+4))
	 	{
	 		return false;
		}
	}
	/*Rot xuong vach*/
	if(vtY >= 20)
		 {
		 	return false;
		 }	
	//khong vi pham dieu kien tra ve true
	return true;	
}
void diChuyen(bool &flap, int vtX, int &vtY)
{
	if (flap)
			diChuyenChim(vtY);
		else
			veChim(40,vtY);
	if (kbhit())
	{
		flap = true;
		getch();
	}
}
/*Ghi diem vao TEXT*/

/*Tinh Diem*/
void Diem(int k, int vtY)
{
	if(k != 0)
		score++;
	gotoXY(10,5);
	cout << "Score: " << score;
	gotoXY(10,6);
	cout << "User:  "<<user;
	//Ten nguoi choi cout <<
}
void tocDoGame(int score)
{
	if(score < 5)
		Sleep(200);
	else if(score < 10)
		Sleep(140);
	else if(score < 15)
		Sleep(120);
	else 
		Sleep(100); 
}
void Game()
{
	List l;
	//nguoiChoi nv;
	init(l);
	khoiTaoMap(l);
	int kt=0; //2 cot den + 1 cot mau
	int vtX = 40; //Toa do x cua chim
	int vtY= 10; //Chim duy chuyen le xuong 1 cho
	bool flap = false;
	while(GameOver(l, vtX, vtY))
	{
		hideMouse();
		veMap(l,kt);
		diChuyen(flap, vtX, vtY);
		tocDoGame(score);
		setBackgroundColor(0);
		system("cls");
	}
	GhiDuLieu();	 
	endgame();	
}
void xuLyMenuAndGame()
{
	int key = menuAndGame();
	if (key==0)
	{
		system("cls");
		tieuDe();
		xuLyMenuChinh();
	}
	else
		thoat();
}
int menuAndGame()
{
	int index = 0;
	while (true)
	{
		veMenuAndGame(index);
		char key = getch();
		if (key == 13)
			return index;
		if (key == 80)
			index++;
		if (key == 72)
			index--;
		if (index == -1)
			index = 1;
		if (index == 2)
			index =0;
	}
}
void veMenuAndGame(int index)
{
	LPSTR s = "<<<       QUAY LAI      >>>";
	setColorBGTextXY(48,13,15,index==0?1:0,s);
	      s = "<<<      THOAT GAME     >>>";
	setColorBGTextXY(48,14,15,index==1?1:0,s);
}
void xuLyChuoi(string &name)
{
	if(name[0] == '\n'){
		for(int i = 0; i < name.length()-1; i++)
		{
			
			name[i] = name[i + 1];
		}
		name[name.length() - 1] = '\0';
	}
}
void GhiDuLieu()
{
	ofstream ghiDL;
	ghiDL.open("bangxephang.txt", ios::app);
	ghiDL << user << "," << score << endl;
	ghiDL.close();
}
void xoatatCaNode(Tnode *&t)
{
	if(t != NULL)
	{
		xoatatCaNode(t->left);
		xoatatCaNode(t->right);
		delete t;
	}
}
void DocDuLieu()
{
	xoatatCaNode(tree);
	tree = NULL;
	ifstream docDL;
	docDL.open("bangxephang.txt", ios::in);
	nguoiChoi nc;
	while(!docDL.eof())
	{
		getline(docDL, nc.hoTen, ',');
		docDL >> nc.diem;
		themNguoiChoi(nc);
	}
	docDL.close();
}
void BangXepHang()
{
	DocDuLieu();
	gotoXY(49, 5);
	cout << ">>>> BANG XEP HANG <<<<";
	gotoXY(53, 7);
	cout << "TEN";		
	gotoXY(64, 7);
	cout << "DIEM" << endl;
	xuatBangXepHang(tree, 53, 9);
	getch();
	system("cls");
	tieuDe();
	xuLyMenuChinh();
}
void xuatBangXepHang(Tnode *tree, int tdX, int tdY)
{
	if(tree != NULL){
		xuatBangXepHang(tree->right, tdX, tdY - 2);
		gotoXY(tdX, tdY);
		cout << tree->nc.hoTen;
		gotoXY(tdX+11, tdY);
		cout << tree->nc.diem;
		xuatBangXepHang(tree->left, tdX, tdY + 2);
	}
}








