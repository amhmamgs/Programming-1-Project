#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>
#include <math.h>
int boxmax,boxcompall,boxcomp,countstep1,countstep2,countboxcom1,countboxcom2,end,play;


void choose_grid()
{
    char c;
    //Let user choose either a 2x2 grid by entering 'b' or a 5x5 grid by entering 'e' (choosing value for char c)
    //If user enters any other input then display an error message and ask for input again
    do
    {
        printf("Please select either Beginner (b)(2x2) or Expert (e)(5x5)\n");
        scanf(" %c",&c);
        if (c!='b' && c!='e')
        {
            printf("Error: incorrect input format\n");
        }
    }
    while(c!='b' && c!='e');
    //Initialize the number of rows and columns in the 2D array that holds the game interface
    //Then determining the exact values depending on the users choice
    int rownum=0,colnum=0;
    if (c=='b')
    {
        rownum = 4+(1*3)+1;
        colnum = 7+(1*6)+1;
        boxmax = 4;
    }
    else if (c=='e')
    {
        rownum = 4+(4*3)+1;
        colnum = 7+(4*6)+1;
        boxmax = 25;
    }
    //Create a 2D array with the required dimensions then call the 'create_grid' function
    //Sending the parameters for row number, column number, array location, and char c which indicates
    //The size of the grid that the user chose.
    int arr[rownum][colnum];
    int vals[rownum][colnum];
    create_grid(rownum,colnum,arr,c,vals);
}

void create_grid(int y, int x, char arr[y][x],char c,int vals[y][x])
{
    int i,j,player=-1,play=0;
    boxcompall=0;
    countboxcom1=0;
    countboxcom2=0;
    countstep1=0;
    countstep2=0;
    end=0;

    //254 is the ASCII code for the dots used in the interface
    //49 is the ASCII code for the number '1' to be used to write down indexes for rows and columns
    //To show the coordinates of each dot in the grid
    char point=254;
    char gridindexrow=49,gridindexcol=49;
    //Depending on the index, assign a value to the specific index in the array
    for (i=0; i<y; i++)
    {
        for (j=0; j<x; j++)
        {
            if (i==0)
            {
                if (j%6==1)
                {
                    arr[i][j]=gridindexrow++;
                    vals[i][j]=0;
                }
                else
                {
                    arr[i][j]=' ';
                    vals[i][j]=0;
                }
            }
            else if (j==0)
            {
                if (i%3==1)
                {
                    arr[i][j]=gridindexcol++;
                    vals[i][j]=0;
                }
                else
                {
                    arr[i][j]=' ';
                    vals[i][j]=0;
                }
            }
            else if(i%3==1)
            {
                if (j%6==1)
                {
                    arr[i][j]=point;
                    vals[i][j]=9;
                }
                else
                {
                    arr[i][j]=' ';
                    vals[i][j]=0;
                }
            }
            else
            {
                arr[i][j]=' ';
                vals[i][j]=0;
            }
        }

    }
    // Print the newly created grid on the screen
    for (i=0; i<y; i++)
    {
        for (j=0; j<x; j++)
        {
            if (i==0)
            {
                if (j%6==1)
                {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),3);
                    printf("%c",arr[i][j]);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
                }
                else
                {
                    printf("%c",arr[i][j]);
                }

            }
            else if (j==0)
            {
                if (i%3==1)
                {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),3);
                    printf("%c",arr[i][j]);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
                }
                else
                {
                    printf("%c",arr[i][j]);
                }
            }
            else
            {
                printf("%c",arr[i][j]);
            }
        }
        printf("\n");
    }
    // Call the function 'join_grid' while sending it the parameters for row number(y), column number(x), array
    //location and char 'c' which indicates the size of the grid that the user chose.
    do
    {
        player=-player;
        join_grid(y,x,arr,c,player,vals);
    }
    while(!end_check());
    winner();
}

void join_grid(int y, int x, char arr[y][x],char t,int player,int vals[y][x])
{
    int i,j,a,b,c,d,rowdiff,coldiff,p1x,p1y,p2x,p2y;
    int lastindex;
    /*boxcomp=0;*/
    //196 and 179 are the ASCII codes for the characters used to draw lines in the game horizontally
    //or vertically respectively
    char rowline = 196, colline = 179, aa, bb, cc, dd;
    //If the user chose 'b' then the largest row and column value for a dot is 3,3
    //If the user chose 'b' then the largest row and column value for a dot is 6,6
    if (t=='b')
    {
        lastindex=3;
    }
    else if (t=='e')
    {
        lastindex=6;
    }
    //Scan the coordinates of the two points to be joined as characters then subtract 48 from their ASCII
    //codes and store them as integers (e.g: if user enters '1' for char aa, the ASCII code for '1' is 49
    //so we subtract 48 to get 1 then store in integer a.
    //This is done to avoid crashing the program if (scanf) was used to scan an integer and the user entered
    //a character that is not numeric.
    do
    {
        play=0;
        printf("\nPlease enter the required points:");
        scanf(" %c",&aa);
        scanf(" %c",&bb);
        scanf(" %c",&cc);
        scanf(" %c",&dd);
        a = aa-48, b = bb-48;
        c = cc-48;
        d = dd-48;

        //If the coordinates of the points were outside the limits of the grid then display an error message.
        if (a<1 || a>lastindex || b<1 || b>lastindex || c<1 || c>lastindex || d<1 || d>lastindex)
        {
            printf("\nError: these points do not exist\n");
        }
        //Calculate difference in rows and difference in columns and square the values in case the user enter the
        //coordinates of a dot which has a higher row or column index first
        //Then check for possible errors such as: entering the same point twice or entering points
        //that are not directly adjacent to each other horizontally or vertically
        else
        {
            rowdiff = (a-c)*(a-c);
            coldiff = (b-d)*(b-d);
            if (rowdiff==0)
            {
                if (coldiff==0)
                {
                    printf("\nError: cannot enter the same point twice\n");
                }
                else if (coldiff == 1)
                {
                    p1y = (a-1)*3+1;
                    p1x = (b-1)*6+1;
                    p2y = (c-1)*3+1;
                    p2x = (d-1)*6+1;

                    if (p2x<p1x)
                    {
                        p1x = p1x ^ p2x;
                        p2x = p1x ^ p2x;
                        p1x = p1x ^ p2x;
                    }
                    if (arr[p1y][p1x+2]!=rowline)
                    {
                        for (i=p1x+2; i<p2x-1; i++)
                        {
                            arr[p1y][i]=rowline;
                            vals[p1y][i]=player;
                            play=1;
                        }
                    }
                    else
                    {
                        printf("\nError: there is a line already drawn in this position\n");
                    }
                }
                else
                {
                    printf("\nError: cannot join these two points\n");
                }
            }
            else if (coldiff==0)
            {
                if (rowdiff==0)
                {
                    printf("\nError: cannot enter the same point twice\n");
                }
                else if (rowdiff == 1)
                {
                    p1y = (a-1)*3+1;
                    p1x = (b-1)*6+1;
                    p2y = (c-1)*3+1;
                    p2x = (d-1)*6+1;
                    if (p2y<p1y)
                    {
                        p1y = p1y ^ p2y;
                        p2y = p1y ^ p2y;
                        p1y = p1y ^ p2y;
                    }
                    if (arr[p1y+1][p1x]!=colline)
                    {
                        for (i=p1y+1; i<p2y; i++)
                        {
                            arr[i][p1x]=colline;
                            vals[i][p1x]=player;
                            play=1;
                        }
                    }
                    else
                    {
                        printf("\nError: there is a line already drawn in this position\n");
                    }
                }
                else
                {
                    printf("\nError: cannot join these two points\n");
                }

            }
            else
            {
                printf("\nError: cannot join these two points\n");
            }
        }

        check_box(y,x,player,vals);
        // Print the grid on screen after a line is drawn or an error occurred
        if(play==1)
        {
            for (i=0; i<y; i++)
            {
                for (j=0; j<x; j++)
                {
                    if (i==0)
                    {
                        if (j%6==1)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),3);
                            printf("%c",arr[i][j]);
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
                        }
                        else
                        {
                            printf("%c",arr[i][j]);
                        }

                    }
                    else if (j==0)
                    {
                        if (i%3==1)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),3);
                            printf("%c",arr[i][j]);
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
                        }
                        else
                        {
                            printf("%c",arr[i][j]);
                        }
                    }
                    else if (i!=0&&j!=0&&(i%3!=1||j%6!=1))
                    {
                        if(vals[i][j]==4)
                        {
                            if(vals[i+1][j]==1)
                            {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),2);
                                arr[i][j]='A';
                                printf("%c",arr[i][j]);
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
                            }
                            else if (vals[i+1][j]==-1)
                            {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),4);
                                arr[i][j]='B';
                                printf("%c",arr[i][j]);
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
                            }
                        }
                        else if (vals[i][j]==1)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),2);
                            printf("%c",arr[i][j]);
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
                        }

                        else if (vals[i][j]==-1)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),4);
                            printf("%c",arr[i][j]);
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
                        }
                        else
                        {
                            printf("%c",arr[i][j]);
                        }
                    }
                    else
                    {
                        printf("%c",arr[i][j]);
                    }
                }
                printf("\n");
            }
            if(play==1)
            {
                if(player==1)
                {
                    countstep1++;
                }
                else if (player==-1)
                {
                    countstep2++;
                }
            }
            printf("\nScore:\tPlayer A:\t%d\tPlayer B:\t%d\n",countboxcom1,countboxcom2);
            printf("Steps:\tPlayer A:\t%d\t\Player B:\t%d\n",countstep1,countstep2);
        }
    }
    while((play==0||boxcomp!=0)&&!end_check());
}

void check_box(int y, int x,int player,int vals[y][x])
{
    int i,j;
    boxcomp=0;
    for(i=2; i<y; i+=3)
    {
        for(j=4; j<x; j+=6)
        {

            vals[i][j]=abs(vals[i-1][j])+abs(vals[i+2][j])+abs(vals[i][j-3])+abs(vals[i][j+3]);
            if (vals[i][j]==4&&vals[i+1][j]==0)
            {
                boxcomp++;
                vals[i+1][j]=player;
                if(player==1)
                    countboxcom1++;
                else if (player==-1)
                    countboxcom2++;
            }
        }
    }
    boxcompall+=boxcomp;
}

int end_check()
{
    int flag;
    if(boxcompall==boxmax)
        flag=1;
    else
        flag=0;
    return flag;

}

void winner()
{
    if(countboxcom1>countboxcom2)
        printf("The winner is Player A");
    else if (countboxcom2>countboxcom1)
        printf("The winner is Player B");
    else
        printf("Draw");
}


int main()
{
    //Call the 'choose_grid' function which starts the game
    choose_grid();


    return 0;
}
