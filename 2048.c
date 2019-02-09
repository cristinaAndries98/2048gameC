#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <fcntl.h>
#include <strings.h>
#include <stdio.h>
#include <time.h>


#define FOREVER 1
#define WIN 2048

char *optiuni[] = {
                        "New Game",
                        "Resume",
                        "Quit",     /*Optiunile din meniu*/
                  };
int n_optiuni = sizeof(optiuni) / sizeof(char *);

void init_ncurses(void)
{
        initscr();
        start_color();
        noecho();
        cbreak();
        curs_set(FALSE);
        keypad(stdscr, TRUE);
}



void afisare_meniu(WINDOW *menu_win, int flag)
{
        int a, b, j;

        a = 4,b = 4;
        box(menu_win, 0, 0);
        for(j = 0; j < n_optiuni; ++j)
        {       if(flag == j + 1) /* Facem highlight pe optiuni */
                {       wattron(menu_win, A_REVERSE);
                        mvwprintw(menu_win, b, a, "%s", optiuni[j]);
                        wattroff(menu_win, A_REVERSE);
                }
                else
                        mvwprintw(menu_win, b, a, "%s", optiuni[j]);
                ++b;
        }
        wrefresh(menu_win);
}




int meniu()
{
	int XMAX=44;
	int YMAX=20;
        WINDOW *menu_win;
        int flag = 1, optiune = 0;
	int tasta;


        menu_win = newwin(YMAX/2, XMAX/2, YMAX/5, XMAX/5);
        keypad(menu_win, TRUE);
        mvprintw(1, 2, "Folositi sagetile sus-jos pentru deplasare.\n   Apasati ENTER pentru selectie");
        refresh();
	init_pair(1, COLOR_GREEN, COLOR_WHITE);
	wbkgd(menu_win, COLOR_PAIR(1));
        afisare_meniu(menu_win, flag);
        while(FOREVER)
        {       
		tasta = getch();
		switch(tasta)
                {
                        case KEY_UP:
                                if(flag == 1)
                                {
                                        flag = n_optiuni;
                                        refresh();
					break;
                                }
                                else
                                {
                                        flag--;
                                        refresh();
                                        break;
                                }

                        case KEY_DOWN:
                                if(flag == n_optiuni)
                                {
                                        flag = 1;
                                        refresh();
					break;
                                }
                                else
                                {
                                        flag++;
                                        refresh();
                                        break;
                                }
                        case 10:
                                optiune = flag;
                                break;
              }

              afisare_meniu(menu_win, flag);
              switch(optiune)
              {
                      case 1:
                              mvprintw(15,0,"Ai ales NEW GAME");
                              refresh();
			      break;
                      case 2:
                              mvprintw(15,0,"Ai ales RESUME");
                              refresh();
                              break;
                      case 3:
                              mvprintw(15,0,"Ai ales QUIT");
                              refresh();
                              break;
              }
		//mvprintw(20,0,"-%d-%s",tasta,"ESTI IN AFARA BUCLEI SWITCH");
                refresh();
		if(tasta == 10)
			return optiune;
        }
}


enum {
        Y=5,
        Y0=5,
        X=10,
        X0=10
};


typedef struct {
        int val;
        int y;
        int x;
} celula;

celula cel[4][4];
int scor=0;

int ceas()
{
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        char s[64];
        strftime(s, sizeof(s), "%c", tm);
        mvprintw(2,0,"%s",s);
        mvprintw(0,0,"%s","Pentru deplasare folositi sagetile. Pentru iesire apasati tasta ESC sau Q");
}

void desen_tabla()              //Desenam tabla
{
    int XMAX=4*X;
    int YMAX=4*Y;
    int M = X0-1;
    int N = Y0;

    int i, j;
    for(i=0; i<=YMAX; i++)
        for(j=0; j<=XMAX; j++)
        {
            if (i == 0)   // Primul rand.
            {
                if (!(j % (XMAX / 4) == 0))   //Linii 
                    mvaddch(N+i, M+j, ACS_S1);
            }

            else if (i % (YMAX / 4) == 0 && i != YMAX)
            {
                if (!(j % (XMAX / 4) == 0))
                    mvaddch(N+i, M+j, ACS_S1);//Linii
            }

            else if (i == YMAX)
            {
                if (!(j % (XMAX / 4) == 0))
                    mvaddch(N+i, M+j, ACS_S1);//Linii
            }

            else if (j % (XMAX / 4) == 0)   // Coloane
            {
                mvaddch(N+i-1, M+j, ACS_VLINE);
                mvaddch(N+i, M+j, ACS_VLINE);
            }
        }
}



int end()
{
     clrtoeol();
     refresh();
     resetty();
     endwin();
     return 0;
}

int rand_a_b(int a, int b)
{
        int fd;
        int i;

        fd = open("/dev/urandom", O_RDONLY);
        read(fd, &i, sizeof(i));
        close(fd);
        if (i < 0 && a >= 0)
                i *= -1;
        i = i % (b - a) + a;
        return (i);
}

void deseneaza_celula(celula *cel)
{
        init_ncurses();
        WINDOW *a_win;
        a_win = newwin(Y, X, cel->y, cel->x);
	refresh();

        init_pair(1, COLOR_BLACK, COLOR_WHITE);
        init_pair(2, COLOR_BLACK, COLOR_YELLOW);
        init_pair(3, COLOR_BLACK, COLOR_BLUE);
        init_pair(4, COLOR_BLACK, COLOR_MAGENTA);
        init_pair(5, COLOR_BLACK, COLOR_RED);
        init_pair(6, COLOR_BLACK, COLOR_CYAN);
        init_pair(7, COLOR_BLACK, COLOR_GREEN);
        init_pair(8, COLOR_BLACK, 45);
        init_pair(9, COLOR_BLACK, 42);
        init_pair(10, COLOR_BLACK, 41);
        init_pair(11, COLOR_BLACK, 179);

        switch(cel->val)
        {
                case 2:
			wbkgd(a_win,COLOR_PAIR(1));
			mvwprintw(a_win,2,4,"%d",cel->val);
			wrefresh(a_win);
			break;
                case 4:
                        wbkgd(a_win,COLOR_PAIR(2));
			mvwprintw(a_win,2,4,"%d",cel->val);
                        wrefresh(a_win);
                        break;
                case 8:
                        wbkgd(a_win,COLOR_PAIR(3));
			mvwprintw(a_win,2,4,"%d",cel->val);
                        wrefresh(a_win);
                        break;
                case 16:
                        wbkgd(a_win,COLOR_PAIR(4));
			mvwprintw(a_win,2,4,"%d",cel->val);
                        wrefresh(a_win);
                        break;
                case 32:
                        wbkgd(a_win,COLOR_PAIR(5));
			mvwprintw(a_win,2,4,"%d",cel->val);
                        wrefresh(a_win);
                        break;
                case 64:
                        wbkgd(a_win,COLOR_PAIR(6));
			mvwprintw(a_win,2,4,"%d",cel->val);
                        wrefresh(a_win);
                        break;
                case 128:
                        wbkgd(a_win,COLOR_PAIR(7));
			mvwprintw(a_win,2,4,"%d",cel->val);
                        wrefresh(a_win);
                        break;
                case 256:
                        wbkgd(a_win,COLOR_PAIR(8));
			mvwprintw(a_win,2,4,"%d",cel->val);
                        wrefresh(a_win);
                        break;
                case 512:
                        wbkgd(a_win,COLOR_PAIR(9));
			mvwprintw(a_win,2,4,"%d",cel->val);
                        wrefresh(a_win);
                        break;
                case 1024:
                        wbkgd(a_win,COLOR_PAIR(10));
			mvwprintw(a_win,2,4,"%d",cel->val);
                        wrefresh(a_win);
                        break;
                case 2048:
                        wbkgd(a_win,COLOR_PAIR(11));
			mvwprintw(a_win,2,4,"%d",cel->val);
                        wrefresh(a_win);
                        break;




        }
}



void distruge_celula(celula *cel)
{
        cel->y=0;
        cel->x=0;
        cel->val=0;
}

int naste_celula()
{
        int i,j,k=0;
        int cnt=0,p;
	typedef struct {
		int a;
		int b;
	} free;

	free K[16];

	for (i=0;i<4;i++)
		for(j=0;j<4;j++)
			if(cel[i][j].val == 0)
			{
				K[k].a=i;
				K[k].b=j;
				k++;
			}
	if(k>0)
	{
		p=rand_a_b(0,k);
        	while (1)
        	{
            		k=rand_a_b(2,5);
            		if(k != 3)
                	break;
        	}
		i=K[p].a;
		j=K[p].b;
        	cel[i][j].val = k;
        	cel[i][j].y=Y*j+Y0;
        	cel[i][j].x=X*i+X0;
        	return 0;
	}
	else
		return 1;

}


void deplasare(char d)
{
    int i,j;
    int n;
    if(d == 'd')
    {
        n=3;
        while(n)
        {
            for(j=0;j<4;j++)
            {
                for(i=3;i>0;i--)
                {
                        if(cel[i][j].x == 0)
                                if(cel[i-1][j].x != 0)
                                {
                                        cel[i][j].x = X0 + i*X;
                                        cel[i][j].y = Y0 + j*Y;
                                        cel[i][j].val=cel[i-1][j].val;
                                        distruge_celula(&cel[i-1][j]);
                                }
                }
            }
        n--;
        }
    }

    if(d == 's')
    {
        n=3;
        while(n)
        {
            for(j=0;j<4;j++)
            {
              for(i=0;i<4;i++)
                {

                        if(cel[i][j].x == 0)
                        {
                                if(cel[i+1][j].x != 0)
                                {
                                        cel[i][j].x = X0 + i*X;
                                        cel[i][j].y = Y0 + j*Y;
                                        cel[i][j].val=cel[i+1][j].val;
                                        distruge_celula(&cel[i+1][j]);
                                }
                        }
               }
            }
        n--;
       }
    }

    if(d == 'u')
    {
        n=3;
        while(n)
        {
              for(i=0;i<4;i++)
            {
              for(j=0;j<3;j++)
                {
                        if(cel[i][j].y == 0)
                        {
                                if(cel[i][j+1].y != 0)
                                {
                                        cel[i][j].x = X0 + i*X;
                                        cel[i][j].y = Y0 + j*Y;
                                        cel[i][j].val=cel[i][j+1].val;
                                        distruge_celula(&cel[i][j+1]);
                                }
                        }
               }
            }
        n--;
       }
    }

   if(d == 'j')
    {
        n=3;
        while(n)
        {
            for(i=0;i<4;i++)
            {
                for(j=3;j>0;j--)
                {
                        if(cel[i][j].y == 0)
                                if(cel[i][j-1].x != 0)
                                {
                                        cel[i][j].x = X0 + i*X;
                                        cel[i][j].y = Y0 + j*Y;
                                        cel[i][j].val=cel[i][j-1].val;
                                        distruge_celula(&cel[i][j-1]);
                                }
                }
            }
        n--;
        }
    }

}



int contopire(char d, int e)
{
    int i,j,mark = 0;
    int n;
    if(d == 'd')
    {
            for(j=0;j<4;j++)
            {
                for(i=3;i>0;i--)
                {
                        if(cel[i][j].x != 0)
                                if(cel[i-1][j].x != 0 && cel[i-1][j].val == cel[i][j].val)
                                {
                                        if (e==1)
					{
						cel[i][j].val=cel[i][j].val+cel[i-1][j].val;
                                        	distruge_celula(&cel[i-1][j]);
                                        	scor = scor + cel[i][j].val;
					}
					else
						mark = 1;			
                                }
                }
            }
    }

    if(d == 's')
    {
            for(j=0;j<4;j++)
            {
              for(i=0;i<4;i++)
                {
                        if(cel[i][j].x != 0 && cel[i+1][j].val == cel[i][j].val)
                        {
                                if(cel[i+1][j].x != 0)
                                {
                                        if (e==1)
                                        {
						cel[i][j].val=cel[i][j].val+cel[i+1][j].val;
                                        	distruge_celula(&cel[i+1][j]);
                                        	scor = scor + cel[i][j].val;
					}
					else
						mark = 1;
                                }
                        }
               }
            }
    }

    if(d == 'u')
    {
           for(i=0;i<4;i++)
            {
              for(j=0;j<3;j++)
                {
                        if(cel[i][j].y != 0)
                        {
                                if(cel[i][j+1].y != 0 && cel[i][j+1].val == cel[i][j].val)
                                {
                                        if (e==1)
					{
                                        	cel[i][j].val=cel[i][j].val+cel[i][j+1].val;
                                        	distruge_celula(&cel[i][j+1]);
                                        	scor = scor + cel[i][j].val;
					}
					else
						mark = 1;
                                }
                        }
               }
            }
    }

   if(d == 'j')
    {
            for(i=0;i<4;i++)
            {
                for(j=3;j>0;j--)
                {
                        if(cel[i][j].y != 0)
                                if(cel[i][j-1].x != 0 && cel[i][j-1].val == cel[i][j].val)
                                {
                                        if (e==1)
					{
                                        	cel[i][j].val=cel[i][j].val+cel[i][j-1].val;
                                        	distruge_celula(&cel[i][j-1]);
                                        	scor = scor + cel[i][j].val;
					}
					else
						mark = 1;
                                }
                }
            }
    }
    return mark;
}

int game_over();

int joc()
{
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        int tasta=0;
        int rezultat=0,i=0,j=0,k;

        // Deseneaza tot tabloul 
        //clear();
	
        desen_tabla();
        for (i=0;i<4;i++)
                for(j=0;j<4;j++)
                        if(! (cel[i][j].y == 0 || cel[i][j].x == 0))
                                deseneaza_celula(&cel[i][j]);
        //desen_tabla();

        mvprintw(1,2,"Scor = %d",scor);
        ceas();
	refresh();

        while(rezultat == 0)
        {
                tasta=getch();
                switch(tasta)
                {
                        case KEY_RIGHT:
                                deplasare('d');
                                contopire('d',1);
                                deplasare('d');
                                naste_celula();
                                clear();//Redeseneaza tot
                                desen_tabla();
                                for (i=0;i<4;i++)
                                      for(j=0;j<4;j++)
					{
                                        	if(! (cel[i][j].y == 0 || cel[i][j].x == 0))
                                                	deseneaza_celula(&cel[i][j]);
						if( cel[i][j].val == WIN )
						{
							mvprintw( 10, 2, "Ai reusit sa formezi %d\n AI CASTIGAT JOCUL cu scorul: %d",WIN,scor);
							refresh();
							sleep(4);
							clear();
							return 0;
						}
					}
                                        mvprintw(1,2,"Scor = %d",scor);
                                        ceas();
					game_over();
                                        break;

                        case KEY_LEFT:
                                deplasare('s');
                                contopire('s',1);
                                deplasare('s');
                                naste_celula();
                                clear();//Redeseneaza tot
                                desen_tabla();
                                for (i=0;i<4;i++)
                                      for(j=0;j<4;j++)
                                        {
                                                if(! (cel[i][j].y == 0 || cel[i][j].x == 0))
                                                        deseneaza_celula(&cel[i][j]);
                                                if( cel[i][j].val == WIN )
                                                {
                                                        mvprintw( 10, 2, "Ai reusit sa formezi %d\n AI CASTIGAT JOCUL cu scorul: %d",WIN,scor);
                                                        refresh();
                                                        sleep(4);
                                                        clear();
                                                        return 0;
                                                }
                                        }

                                mvprintw(1,2,"Scor = %d",scor);
                                ceas();
				game_over();
                                break;

                        case KEY_UP:
                                deplasare('u');
                                contopire('u',1);
                                deplasare('u');
                                naste_celula();
                                clear();//Redeseneaza tot
                                desen_tabla();
                                for (i=0;i<4;i++)
                                      for(j=0;j<4;j++)
                                        {
                                                if(! (cel[i][j].y == 0 || cel[i][j].x == 0))
                                                        deseneaza_celula(&cel[i][j]);
                                                if( cel[i][j].val == WIN )
                                                {
                                                        mvprintw( 10, 2, "Ai reusit sa formezi %d\n AI CASTIGAT JOCUL cu scorul: %d",WIN,scor);
                                                        refresh();
                                                        sleep(4);
                                                        clear();
                                                        return 0;
                                                }
                                        }

                                mvprintw(1,2,"Scor = %d",scor);
                                ceas();
				game_over();
                                break;

                        case KEY_DOWN:
                                deplasare('j');
                                contopire('j',1);
                                deplasare('j');
                                naste_celula();
                                clear();//Redeseneaza tot
                                desen_tabla();
                                for (i=0;i<4;i++)
                                      for(j=0;j<4;j++)
                                        {
                                                if(! (cel[i][j].y == 0 || cel[i][j].x == 0))
                                                        deseneaza_celula(&cel[i][j]);
                                                if( cel[i][j].val == WIN )
                                                {
                                                        mvprintw( 10, 2, "Ai reusit sa formezi %d\n AI CASTIGAT JOCUL cu scorul: %d",WIN,scor);
                                                        refresh();
                                                        sleep(4);
                                                        clear();
                                                        return 0;
                                                }
                                        }

                                mvprintw(1,2,"Scor = %d",scor);
                                ceas();
				game_over();
                                break;

                        case 113: // Tasta q
                                rezultat = 113;
                                break;
                        case 81: // Tasta Q
                                rezultat = 113;
                                break;
                        case 27: // Tasta ESC
                                rezultat = 27;
                                break;
                }
        }
        return rezultat;
}

int game_over() //va returna YES(=1) sau NO(=0)
{
	int i,j;
        for (i=0;i<4;i++) //verificam daca toate celulele sunt ocupate
             for(j=0;j<4;j++)
		if( cel[i][j].val == 0 )
			return 0;
		   //daca toate celulele sunt ocupate verificam daca mai e vreo miscare posibila

	if(contopire('d',0) == 0 && contopire('s',0) == 0 && contopire('u',0) == 0 && contopire('j',0) == 0) 
	{	
		
		mvprintw(14,20,"GAME OVER.");
		mvprintw(16,15, "Nu mai sunt mutari posibile"); 
		refresh();
		return 1;
	}	
}


int main ()
{
	int rezultat;
	int alegere;
        int i,j,cnt=0,k;

	init_ncurses();
	while (FOREVER)
	{
		alegere = meniu();
		if (alegere == 1)
		{
		        for (i=0;i<4;i++) //initializam celulele 
                		for(j=0;j<4;j++)
					distruge_celula(&cel[i][j]);
			scor = 0;
		        naste_celula();
        		naste_celula();
	
			clear();
			rezultat=joc();
			if (rezultat == 27 || rezultat == 113 || rezultat == 81) //S-a iesit prin ESC, q sau Q
			{	
				clear();
				continue;
			}
		}

		if (alegere == 2)
		{
                        clear();
                        rezultat=joc();
                        if (rezultat == 27 || rezultat == 113 || rezultat == 81) //S-a iesit prin ESC, q sau Q
                        {
                                clear();
                                continue;
                        }

		}
		
		if (alegere == 3)
		{
                        mvprintw(16,0,"La revedere !");
                        refresh();
			sleep(1);
			end();
			break;
		}
	}
	end();
	return 0;
}

