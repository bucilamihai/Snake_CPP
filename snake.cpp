#include<iostream>
#include<fstream>
#include<cstdlib>
#include<winuser.h>
#include<windows.h>
#include<ctime>
#include<cstring>
using namespace std;

char board[16][32];
bool gameOver;
int iHead,jHead;
int iTail,jTail;
int iFood,jFood;
int time_delay=0,tdelay;
int move_delay=0,mdelay=2;
int pause_delay=0;
bool WPressed,SPressed,APressed,DPressed;
bool isPaused,isReset;

int score=0;
int highscore[6];
int levels[6];
char menu[16][64];
int arrow_pos,menu_delay=0,mmdelay=75;
bool quit=false;
char level[16][64];
int level_pos;
bool quit_level;

struct Date
{
    int year,month,day,hour,minute;
}dates[6];

void ClearScreen()
{
    COORD cursorPosition;
    cursorPosition.X=0;
    cursorPosition.Y=0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),cursorPosition);
}

void ClearScreenV2()
{
    system("cls");
}

void PauseScreen()
{
    system("pause");
}

void GenerateFood()
{
    srand(time(0));
    do
    {
        iFood=rand()%16;
        jFood=rand()%32;
    }
    while(board[iFood][jFood]!=' ');
    board[iFood][jFood]='@';
}

void Food()
{
    board[iHead][jHead]='o';
    if(DPressed)
        jHead++;
    if(APressed)
        jHead--;
    if(SPressed)
        iHead++;
    if(WPressed)
        iHead--;
    board[iHead][jHead]='O';
    GenerateFood();
    score=score+(9-tdelay);
}

void Date(int j)
{
    time_t now=time(0);
    char* date_time=ctime(&now);
    strcpy(date_time,date_time+4);
    char *p=strtok(date_time," ");
    int i=1;
    while(p)
    {
        if(i==1)
        {
            if(strcmp(p,"Jan")==0)
                dates[j].month=1;
            if(strcmp(p,"Feb")==0)
                dates[j].month=2;
            if(strcmp(p,"Mar")==0)
                dates[j].month=3;
            if(strcmp(p,"Apr")==0)
                dates[j].month=4;
            if(strcmp(p,"May")==0)
                dates[j].month=5;
            if(strcmp(p,"Jun")==0)
                dates[j].month=6;
            if(strcmp(p,"Jul")==0)
                dates[j].month=7;
            if(strcmp(p,"Aug")==0)
                dates[j].month=8;
            if(strcmp(p,"Sep")==0 || strcmp(p,"Sept")==0)
                dates[j].month=9;
            if(strcmp(p,"Oct")==0)
                dates[j].month=10;
            if(strcmp(p,"Nov")==0)
                dates[j].month=11;
            if(strcmp(p,"Dec")==0)
                dates[j].month=12;
        }
        if(i==2)
        {
            int a,b;
            a=p[0]-'0';
            b=p[1]-'0';
            dates[j].day=a*10+b;
        }
        if(i==3)
        {
            int a,b,c,d;
            a=p[0]-'0';
            b=p[1]-'0';
            c=p[3]-'0';
            d=p[4]-'0';
            dates[j].hour=a*10+b;
            dates[j].minute=c*10+d;
        }
        if(i==4)
        {
            int a,b,c,d;
            a=p[0]-'0';
            b=p[1]-'0';
            c=p[2]-'0';
            d=p[3]-'0';
            dates[j].year=a*1000+b*100+c*10+d;
        }
        p=strtok(NULL," ");
        i++;
    }
}

void DisplayDate(int i)
{
    cout<<dates[i].day<<'.'<<dates[i].month<<'.'<<dates[i].year<<" ";
    if(dates[i].hour<10)
        cout<<0;
    cout<<dates[i].hour<<':';
    if(dates[i].minute<10)
        cout<<0;
    cout<<dates[i].minute;
}

void MainMenuSetup()
{
    strcpy(menu[0],"           Snake");
    strcpy(menu[1],"        - the game -");
    strcpy(menu[4],"        Main Menu");
    strcpy(menu[5],"-> 1)Play");
    strcpy(menu[6],"   2)Level");
    strcpy(menu[7],"   3)Instructions");
    strcpy(menu[8],"   4)Highscore");
    strcpy(menu[9],"   5)Quit");
    arrow_pos=5;
}

void LevelSetup()
{
    strcpy(level[0],"           Snake");
    strcpy(level[1],"        - the game -");
    strcpy(level[3],"        Levels");
    strcpy(level[4],"A higher level means a higher snake speed");
    strcpy(level[5],"-> Level 1");
    strcpy(level[6],"   Level 2");
    strcpy(level[7],"   Level 3");
    strcpy(level[8],"   Level 4");
    strcpy(level[9],"   Level 5");
    strcpy(level[10],"   Level 6");
    strcpy(level[11],"   Level 7");
    strcpy(level[12],"   Level 8");
    level_pos=5;
}

void Setup()
{
    int i,j;
    for(i=0;i<16;i++)
        for(j=0;j<32;j++)
            board[i][j]=' ';
    for(i=0;i<16;i++)
        board[i][0]=board[i][31]='|';
    for(j=0;j<32;j++)
        board[0][j]=board[15][j]='-';
    iHead=8,jHead=16;
    iTail=8,jTail=14;
    board[8][16]='O';
    board[8][15]='o';
    board[8][14]='o';
    GenerateFood();
}

void Display()
{
    int i,j;
    cout<<"           Score: "<<score<<"\n";
    for(i=0;i<16;i++,cout<<"\n")
        for(j=0;j<32;j++)
            cout<<board[i][j];
}

void Tail()
{
    board[iTail][jTail]=' ';
    if(board[iTail][jTail+1]=='o' && (board[iTail][jTail+2]=='o' || board[iTail][jTail+2]=='O'))
        jTail++;
    else if(board[iTail][jTail-1]=='o' && (board[iTail][jTail-2]=='o' || board[iTail][jTail-2]=='O'))
        jTail--;
    else if(board[iTail+1][jTail]=='o' && (board[iTail+2][jTail]=='o' || board[iTail+2][jTail]=='O'))
        iTail++;
    else if(board[iTail-1][jTail]=='o' && (board[iTail-2][jTail]=='o' || board[iTail-2][jTail]=='O'))
        iTail--;

    else if((board[iTail][jTail+1]=='o' && (board[iTail-1][jTail+1]=='o' || board[iTail-1][jTail+1]=='O') && board[iTail+1][jTail+1]!='o' && board[iTail+1][jTail+1]!='O')
            || (board[iTail][jTail+1]=='o' && (board[iTail+1][jTail+1]=='o' || board[iTail+1][jTail+1]=='O') && board[iTail-1][jTail+1]!='o' && board[iTail-1][jTail+1]!='O'))
        jTail++;
    else if((board[iTail][jTail-1]=='o' && (board[iTail-1][jTail-1]=='o' || board[iTail-1][jTail-1]=='O') && board[iTail+1][jTail-1]!='o' && board[iTail+1][jTail-1]!='O')
            || (board[iTail][jTail-1]=='o' && (board[iTail+1][jTail-1]=='o' || board[iTail+1][jTail-1]=='O') && board[iTail-1][jTail-1]!='o' && board[iTail-1][jTail-1]!='O'))
        jTail--;
    else if((board[iTail+1][jTail]=='o' && (board[iTail+1][jTail-1]=='o' || board[iTail+1][jTail-1]=='O') && board[iTail+2][jTail]!='o' && board[iTail+2][jTail]!='O')
            || (board[iTail+1][jTail]=='o' && (board[iTail+1][jTail+1]=='o' || board[iTail+1][jTail+1]=='O') && board[iTail+2][jTail]!='o' && board[iTail+2][jTail]!='O'))
        iTail++;
    else if((board[iTail-1][jTail]=='o' && (board[iTail-1][jTail-1]=='o' || board[iTail-1][jTail-1]=='O') && board[iTail-2][jTail]!='o' && board[iTail-2][jTail]!='O')
            || (board[iTail-1][jTail]=='o' && (board[iTail-1][jTail+1]=='o' || board[iTail-1][jTail+1]=='O') && board[iTail-2][jTail]!='O' && board[iTail-2][jTail]!='O'))
        iTail--;
}

void D()
{
    jHead++;
    board[iHead][jHead]='O';
    board[iHead][jHead-1]='o';
    Tail();
}


void A()
{
    jHead--;
    board[iHead][jHead]='O';
    board[iHead][jHead+1]='o';
    Tail();
}

void W()
{
    iHead--;
    board[iHead][jHead]='O';
    board[iHead+1][jHead]='o';
    Tail();
}

void S()
{
    iHead++;
    board[iHead][jHead]='O';
    board[iHead-1][jHead]='o';
    Tail();
}

void Check()
{
    int i,j;
    for(i=0;i<16;i++)
        if(board[i][0]=='O' || board[i][31]=='O')
            gameOver=true;
    for(j=0;j<32;j++)
        if(board[0][j]=='O' || board[15][j]=='O')
            gameOver=true;
    if(board[iHead+1][jHead]=='o' && SPressed)
        gameOver=true;
    if(board[iHead-1][jHead]=='o' && WPressed)
        gameOver=true;
    if(board[iHead][jHead-1]=='o' && APressed)
        gameOver=true;
    if(board[iHead][jHead+1]=='o' && DPressed)
        gameOver=true;
    if(gameOver)
        board[iHead][jHead]='X';
}

void CheckHighscore()
{
    if(score>highscore[0])
        highscore[4]=highscore[3],highscore[3]=highscore[2],highscore[2]=highscore[1],highscore[1]=highscore[0],highscore[0]=score,
        levels[4]=levels[3],levels[3]=levels[2],levels[2]=levels[1],levels[1]=levels[0],levels[0]=level_pos-4,
        dates[4]=dates[3],dates[3]=dates[2],dates[2]=dates[1],dates[1]=dates[0],Date(0);
    else
        if(score>highscore[1])
            highscore[4]=highscore[3],highscore[3]=highscore[2],highscore[2]=highscore[1],highscore[1]=score,
            levels[4]=levels[3],levels[3]=levels[2],levels[2]=levels[1],levels[1]=level_pos-4,
            dates[4]=dates[3],dates[3]=dates[2],dates[2]=dates[1],Date(1);
        else
            if(score>highscore[2])
                highscore[4]=highscore[3],highscore[3]=highscore[2],highscore[2]=score,
                levels[4]=levels[3],levels[3]=levels[2],levels[2]=level_pos-4,
                dates[4]=dates[3],dates[3]=dates[2],Date(2);
            else
                if(score>highscore[3])
                    highscore[4]=highscore[3],highscore[3]=score,
                    levels[4]=levels[3],levels[3]=level_pos-4,
                    dates[4]=dates[3],Date(3);
                else
                    if(score>highscore[4])
                        highscore[4]=score,
                        levels[4]=level_pos-4,
                        Date(4);
}

void LoadHighscores()
{
    ifstream fin("highscore.txt");
    int i;
    for(i=0;i<5;i++)
        fin>>highscore[i];
    fin.close();
}

void LoadLevels()
{
    ifstream fin("level.txt");
    int i;
    for(i=0;i<5;i++)
        fin>>levels[i];
    fin.close();
}

void LoadDates()
{
    ifstream fin("date.txt");
    int i;
    for(i=0;i<5;i++)
        fin>>dates[i].day>>dates[i].month>>dates[i].year>>dates[i].hour>>dates[i].minute;
    fin.close();
}

void SaveHighscores()
{

    std::ofstream ofs;
    ofs.open("highscore.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ofstream fout("highscore.txt");
    int i;
    for(i=0;i<5;i++)
        fout<<highscore[i]<<" ";
    fout.close();
}

void SaveLevels()
{

    std::ofstream ofs;
    ofs.open("level.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ofstream fout("level.txt");
    int i;
    for(i=0;i<5;i++)
        fout<<levels[i]<<" ";
    fout.close();
}

void SaveDates()
{
    std::ofstream ofs;
    ofs.open("date.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ofstream fout("date.txt");
    int i;
    for(i=0;i<5;i++)
        fout<<dates[i].day<<" "<<dates[i].month<<" "<<dates[i].year<<" "<<dates[i].hour<<" "<<dates[i].minute<<"\n";
    fout.close();
}


/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~MAIN WINDOW~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void MainMenu()
{
    int i,j;
    menu_delay++;
    if(GetAsyncKeyState('S') && arrow_pos<9 && menu_delay>mmdelay)
    {
        menu[arrow_pos][0]=menu[arrow_pos][1]=' ';
        arrow_pos++;
        menu[arrow_pos][0]='-';
        menu[arrow_pos][1]='>';
        menu_delay=0;
    }
    if(GetAsyncKeyState('W') && arrow_pos>5 && menu_delay>mmdelay)
    {
        menu[arrow_pos][0]=menu[arrow_pos][1]=' ';
        arrow_pos--;
        menu[arrow_pos][0]='-';
        menu[arrow_pos][1]='>';
        menu_delay=0;
    }
    for(i=0;i<12;i++)
        cout<<menu[i]<<"\n";
    ClearScreen();
}

void Play()
{
    score=0;
    ClearScreen();
    Setup();
    Display();
    isPaused=false;
    isReset=false;
    gameOver=false;
    WPressed=false;
    SPressed=false;
    APressed=false;
    DPressed=true;
    while(!gameOver)
    {
        if(GetAsyncKeyState('P'))
        {
            isPaused=true;
            do
            {
                pause_delay++;
                if(GetAsyncKeyState('P') && pause_delay>10000)
                    isPaused=false;
            }
            while(isPaused);
            pause_delay=0;
        }
        if(!isPaused)
        {
            time_delay++;
            move_delay++;
            ClearScreen();
            Display();
            Check();
            if(time_delay>tdelay)
            {
                time_delay=0;
                if(DPressed)
                    D();
                if(WPressed)
                    W();
                if(SPressed)
                    S();
                if(APressed)
                    A();
            }
            if(GetAsyncKeyState('W') && !WPressed && !SPressed && move_delay>mdelay)
            {
                move_delay=0;
                WPressed=true;
                SPressed=false;
                APressed=false;
                DPressed=false;
            }
            if(GetAsyncKeyState('S') && !SPressed && !WPressed && move_delay>mdelay)
            {
                move_delay=0;
                WPressed=false;
                SPressed=true;
                APressed=false;
                DPressed=false;
            }
            if(GetAsyncKeyState('D') && !DPressed && !APressed && move_delay>mdelay)
            {
                move_delay=0;
                WPressed=false;
                SPressed=false;
                APressed=false;
                DPressed=true;
            }
            if(GetAsyncKeyState('A') && !APressed && !DPressed && move_delay>mdelay)
            {
                move_delay=0;
                WPressed=false;
                SPressed=false;
                APressed=true;
                DPressed=false;
            }
            if(iHead==iFood && jHead==jFood)
                Food();
        }
        if(GetAsyncKeyState('R') && !isReset)
            isReset=true;
        if(isReset)
            gameOver=true;
    }
    CheckHighscore();
    ClearScreen();
    Display();
    menu_delay=0;
    if(isReset)
    {
        ClearScreenV2();
        Play();
    }
    else
    {
        PauseScreen();
        ClearScreenV2();
    }
}

void Level()
{
    int i;
    menu_delay++;
    if(GetAsyncKeyState('S') && level_pos<12 && menu_delay>mmdelay)
    {
        level[level_pos][0]=level[level_pos][1]=' ';
        level_pos++;
        level[level_pos][0]='-';
        level[level_pos][1]='>';
        menu_delay=0;
    }
    if(GetAsyncKeyState('W') && level_pos>5 && menu_delay>mmdelay)
    {
        level[level_pos][0]=level[level_pos][1]=' ';
        level_pos--;
        level[level_pos][0]='-';
        level[level_pos][1]='>';
        menu_delay=0;
    }
    for(i=0;i<14;i++)
        cout<<level[i]<<"\n";
    ClearScreen();
}

void Instruction()
{
    ClearScreenV2();
    cout<<"           Snake"<<"\n";
    cout<<"        - the game -"<<"\n"<<"\n";
    cout<<"       Instructions"<<"\n";
    cout<<"   You play the well-known game - snake"<<"\n"<<"\n";
    cout<<"Use W and S to go through menu and ENTER to proceed"<<"\n";
    cout<<"You can play, change your snake level and view highscores"<<"\n";
    cout<<"Use WASD to control the snake"<<"\n";
    cout<<"Use P while you are playing to pause the game"<<"\n";
    cout<<"Use R while you are playing to reset the game"<<"\n";
    cout<<"Have fun!"<<"\n"<<"\n";
    PauseScreen();
    ClearScreenV2();
    menu_delay=0;
}

void Highscore()
{
    ClearScreenV2();
    cout<<"           Snake"<<"\n";
    cout<<"        - the game -"<<"\n"<<"\n";
    cout<<"     Your highscores"<<"\n";
    int i;
    for(i=0;i<5;i++)
    {
        cout<<i+1<<") ";
        if(highscore[i])
            cout<<"Score "<<highscore[i]<<" ~ "<<"Level "<<levels[i]<<" ~ ",DisplayDate(i),cout<<"\n";
        else
            cout<<"NaN"<<"\n";
    }
    cout<<"\n"<<"Press Y to reset the highscores or N otherwise"<<"\n";
    int reset=0;
    while(!reset)
    {
        if(GetAsyncKeyState('Y'))
            reset=1;
        else
            if(GetAsyncKeyState('N'))
                reset=-1;
    }
    if(reset==1)
        for(i=0;i<5;i++)
            highscore[i]=0;
    ClearScreenV2();
    menu_delay=0;
}

int main()
{
    LoadLevels();
    LoadHighscores();
    LoadDates();
    MainMenuSetup();
    while(!quit)
    {
        MainMenu();
        if(GetAsyncKeyState(VK_RETURN) && menu_delay>mmdelay)
        {
            menu_delay=0;
            if(arrow_pos==5)
            {
                if(tdelay)
                    Play();
                else
                {
                    ClearScreenV2();
                    cout<<"Please set your snake level"<<"\n";
                    PauseScreen();
                    ClearScreenV2();
                    menu_delay=0;
                }
            }
            if(arrow_pos==6)
            {
                LevelSetup();
                quit_level=false;
                ClearScreenV2();
                while(!quit_level)
                {
                    Level();
                    if(GetAsyncKeyState(VK_RETURN) && menu_delay>mmdelay)
                    {
                        quit_level=true;
                        tdelay=9-(level_pos-4);
                    }
                }
                ClearScreenV2();
                menu_delay=0;
            }
            if(arrow_pos==7)
                Instruction();
            if(arrow_pos==8)
                Highscore();
            if(arrow_pos==9)
                quit=true;
        }
    }
    SaveLevels();
    SaveHighscores();
    SaveDates();
    return 0;
}
