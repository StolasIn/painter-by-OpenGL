#include <windows.h>
#include <conio.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <bits/stdc++.h>
#include <gdiplus.h>
#include <string>


/*功能常數*/
#define NULL 0
#define LINE 1
#define RECTANGLE 2
#define TRIANGLE  3
#define POINTS 4
#define TEXT 5
#define POLYGON 6
#define ERASE 7
#define SPRAY 8
#define RUBBER_RECT 9
#define RUBBER_CIR 10
#define DRAW 11
#define BIGGER_D 12
#define SMALLER_D 13
#define COLOR_D 14
#define COLOR_T 15
#define PICK 16
#define MUSIC 17
#define RUBBER_LINE 18
#define BIGGER_E 19
#define SMALLER_E 20


using namespace std;


/*顏色類別*/
struct color_c
{
    double r;
    double g;
    double b;
};


/*多邊形類別*/
struct polygon
{
    double r,g,b;
    bool filled;
    bool used;
    vector<double> x;
    vector<double> y;
};

/*儲存所有多邊形*/
vector<polygon> polygon_v;


/*定義函數*/
void mouse(int, int, int, int);
void key(unsigned char, int, int);
void display(void);
void drawSquare(int, int);
void myReshape(GLsizei, GLsizei);
void myinit(void);
void screen_box(int, int, int);
void right_menu(int);
void middle_menu(int);
void color_menu(int);
void pixel_menu(int);
void fill_menu(int);
int pick(int, int);
void redraw();


/*全域變數*/
GLsizei wh = 500, ww = 500;
GLfloat size = 3.0;
int draw_mode = 0;
int rx, ry;

GLfloat r = 1.0, g = 1.0, b = 1.0;

/*填滿*/
int fill1 = 0;


/*橡皮筋*/
int first=0;
double xm,ym,xmm,ymm;


/*多邊形*/
int one=0;


/*當前狀態*/
int state1=0;


/*畫圓*/
int n=3600;
double PI = 3.1415926;
double eps = 1e-6;
double R;


/*字體選擇*/
int font_id=1;


/*畫筆粗細*/
double draw_r=1.0;


/*橡皮擦粗細*/
double erase_r=10;


/*畫筆與字體顏色選擇*/
int color_cnt1=0;
int color_cnt2=1;
double dr=1,dg=1,db=1;
double tr=0,tg=0,tb=0;
vector<color_c> color_v;

/*抓取多邊形*/
int choose = -1;
int pick_x;
int pick_y;

//截圖
int ScreenShot_cnt = 1;

/*音樂*/
bool music_flag = false;

void ScreenShot()
{
	GLint pView[4];
	glGetIntegerv(GL_VIEWPORT,pView);//得到视图矩阵,pView[2]为宽即width,pView[3]为高即height

	GLsizei numComponet = 3;
	GLsizei bufferSize = pView[2]*pView[3]*sizeof(GLfloat)*numComponet;

	GLfloat* _data = new GLfloat[bufferSize];
	unsigned char*  data = new unsigned char[bufferSize];
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);//设置4字节对齐
	glReadBuffer(GL_FRONT);
	glReadPixels(pView[0],pView[1],pView[2],pView[3],GL_BGR_EXT,GL_FLOAT,_data);//不是GL_RGB的读取方式，而是GL_BGR或者GL_BGR_Ext
	glReadBuffer(GL_BACK);
	for (int i = 0 ; i <bufferSize ; i ++)
	{
		data[i] = _data[i]*255;
	}
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	infoHeader.biSize = 40;
	infoHeader.biWidth = ww;
	infoHeader.biHeight = wh;
	infoHeader.biPlanes = 1;
	infoHeader.biBitCount = 24;
	infoHeader.biCompression = BI_RGB;
	infoHeader.biSizeImage =pView[2]* pView[3] * 3;
	infoHeader.biXPelsPerMeter = 0;
	infoHeader.biYPelsPerMeter = 0;
	infoHeader.biClrUsed = 0;
	infoHeader.biClrImportant = 0;
	fileHeader.bfType = 0x4D42;
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;
	fileHeader.bfOffBits = 54;
	fileHeader.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+pView[2]* pView[3] * 3);
	FILE *fd;
	stringstream str;
	string str1="";
	str<<ScreenShot_cnt;
	str>>str1;
	ScreenShot_cnt++;
	str1 = "temp"+str1+".bmp";
	if( !(fd = fopen(str1.c_str(),"wb+")) )//filepath为你所保存文件的名字
	{
		cout<<"0"<<endl;
		exit(0);
	}
	else
	{
		fwrite(&fileHeader,1,sizeof(BITMAPFILEHEADER),fd);
		fwrite(&infoHeader,1,sizeof(BITMAPINFOHEADER),fd);
		fwrite(data,1,pView[2]* pView[3] * 3,fd);
		fclose(fd);
	}
	delete[] data;
	delete[] _data;
}

/*畫正方形點*/
void drawSquare(int x, int y)
{
    y=wh-y;
    glColor3ub( (char) rand()%256, (char) rand()%256, (char) rand()%256);
    glBegin(GL_POLYGON);
    glVertex2f(x+size, y+size);
    glVertex2f(x-size, y+size);
    glVertex2f(x-size, y-size);
    glVertex2f(x+size, y-size);
    glEnd();
}

/*畫笑臉塗鴉*/
void drawSpary(int x,int y)
{
    glColor3ub( (char) rand()%256, (char) rand()%256, (char) rand()%256);
    y=wh-y;
    glBegin(GL_LINE_LOOP);
    for(int i=0; i<n; i++)
    {
        glVertex2f(x+18*cos(2*PI*i/n),y+18*sin(2*PI*i/n));
    }
    glEnd();
    glBegin(GL_LINES);
    glVertex2i(x-10,y+10);
    glVertex2i(x-5,y+2);
    glEnd();
    glBegin(GL_LINES);
    glVertex2i(x-5,y+10);
    glVertex2i(x-10,y+2);
    glEnd();
    glBegin(GL_LINES);
    glVertex2i(x+10,y+10);
    glVertex2i(x+5,y+2);
    glEnd();
    glBegin(GL_LINES);
    glVertex2i(x+5,y+10);
    glVertex2i(x+10,y+2);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2i(x-10,y-6);
    glVertex2i(x-5,y-9);
    glVertex2i(x,y-12);
    glVertex2i(x+5,y-9);
    glVertex2i(x+10,y-6);
    glEnd();
    glBegin(GL_LINES);
    glVertex2i(x-5,y-9);
    glVertex2i(x-5,y-6);
    glEnd();
    glBegin(GL_LINES);
    glVertex2i(x,y-12);
    glVertex2i(x,y-6);
    glEnd();
    glBegin(GL_LINES);
    glVertex2i(x+5,y-9);
    glVertex2i(x+5,y-6);
    glEnd();
}


/*橡皮擦*/
void eraser(int x,int y)
{
    y=wh-y;
    glColor3f(0.8,0.8,0.8);
    glBegin(GL_POLYGON);
    glVertex2f(x+erase_r, y+erase_r);
    glVertex2f(x-erase_r, y+erase_r);
    glVertex2f(x-erase_r, y-erase_r);
    glVertex2f(x+erase_r, y-erase_r);
    glEnd();
    glFlush();
}


/* rehaping routine called whenever window is resized
or moved */
void myReshape(GLsizei w, GLsizei h)
{

    /* adjust clipping box */

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (GLdouble)w, 0.0, (GLdouble)h, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* adjust viewport and  clear */

    glViewport(0,0,w,h);
    glClearColor (0.8, 0.8, 0.8, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    display();
    glFlush();

    /* set global size for use by drawing routine */

    ww = w;
    wh = h;
}


void myinit(void)
{
    glReadBuffer(GL_BACK);
    glViewport(0,0,ww,wh);
    glEnable(GL_COLOR_LOGIC_OP);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    /* Pick 2D clipping window to match size of X window
    This choice avoids having to scale object coordinates
    each time window is resized */

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (GLdouble) ww, 0.0, (GLdouble) wh, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    /* set clear color to black and clear window */

    glClearColor (0.8, 0.8, 0.8, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

/*滑鼠移動*/
void move1(int x,int y)
{
    if(state1 == ERASE)
    {
        if(pick(x,y)==0 && pick(x-erase_r,y-erase_r)==0 &&
                pick(x+erase_r,y-erase_r)==0 && pick(x-erase_r,y+erase_r)==0 &&
                pick(x+erase_r,y+erase_r)==0)
        {

            eraser(x,y);
        }
    }
    else if(state1 == RUBBER_CIR)
    {
        if (first == 1)
        {
            if(fill1)
            {
                glBegin(GL_POLYGON);
            }
            else
            {
                glBegin(GL_LINE_LOOP);
            }
            for(int i=0; i<n; i++)
            {
                glVertex2f(xm+R*cos(2*PI*i/n),ym+R*sin(2*PI*i/n));
            }
            glEnd();
            glFlush();
        }
        xmm=x;
        ymm=(wh-y);
        R=sqrt((xmm-xm)*(xmm-xm)+(ymm-ym)*(ymm-ym));
        if(fill1)
        {
            glBegin(GL_POLYGON);
        }
        else
        {
            glBegin(GL_LINE_LOOP);
        }
        for(int i=0; i<n; i++)
        {
            glVertex2f(xm+R*cos(2*PI*i/n),ym+R*sin(2*PI*i/n));
        }
        glEnd();
        glFlush();
        first = 1;
    }
    else if(state1 == RUBBER_RECT)
    {
        if (first == 1)
        {
            if(fill1)
            {
                glBegin(GL_POLYGON);
            }
            else
            {
                glBegin(GL_LINE_LOOP);
            }
            glVertex2f(xm,ym);
            glVertex2f(xmm,ym);
            glVertex2f(xmm,ymm);
            glVertex2f(xm,ymm);
            glEnd();
            glFlush();
        }
        xmm=x;
        ymm=(wh-y);
        if(fill1)
        {
            glBegin(GL_POLYGON);
        }
        else
        {
            glBegin(GL_LINE_LOOP);
        }
        glVertex2f(xm,ym);
        glVertex2f(xmm,ym);
        glVertex2f(xmm,ymm);
        glVertex2f(xm,ymm);
        glEnd();
        glFlush();
        first = 1;
    }
    else if(state1 == DRAW)
    {
        glColor3f(dr,dg,db);
        glBegin(GL_POLYGON);
        for(int i=0; i<n; i++)
        {
            glVertex2f(x+draw_r*cos(2*PI*i/n),(wh-y)+draw_r*sin(2*PI*i/n));
        }
        glEnd();
        glFlush();
    }
    else if(state1 == PICK)
    {
        if(choose!=-1)
        {
            glColor3f(0.8,0.8,0.8);
            if(polygon_v[choose].filled)
            {
                glBegin(GL_POLYGON);
            }
            else
            {
                glBegin(GL_LINE_LOOP);
            }
            for(int j=0; j<polygon_v[choose].x.size(); j++)
            {
                glVertex2i(polygon_v[choose].x[j],polygon_v[choose].y[j]);
            }
            glEnd();
            for(int i=0; i<polygon_v[choose].x.size(); i++)
            {
                polygon_v[choose].x[i]-=(pick_x-x);
                polygon_v[choose].y[i]+=(pick_y-y);
            }
        }
        pick_x = x;
        pick_y = y;
        redraw();
    }
    else if(state1 == RUBBER_LINE)
    {
        if (first == 1)
        {
            glBegin(GL_LINES);
            glVertex2f(xm,ym);
            glVertex2f(xmm,ymm);
            glEnd();
            glFlush();
        }
        xmm=x;
        ymm=(wh-y);
        glBegin(GL_LINES);
        glVertex2f(xm,ym);
        glVertex2f(xmm,ymm);
        glEnd();
        glFlush();
        first = 1;
    }

}


/*確認點擊是否在多邊形內*/
bool inside_check(int x,int y,polygon p)
{
    int ncross = 0;
    for(int i=0; i<p.x.size(); i++)
    {
        int j=(i+1)%p.x.size();
        if(p.y[i] == p.y[j])
        {
            continue;
        }
        if(y < min(p.y[i],p.y[j]))
        {
            continue;
        }
        if(y >= max(p.y[i],p.y[j]))
        {
            continue;
        }
        double t1 = (double)(y-p.y[i])*(double)(p.x[j]-p.x[i])/(double)(p.y[j] - p.y[i]) + p.x[i];
        if (t1 > x)
        {
            ncross++;
        }
    }
    return ncross%2==1;
}


/*重新繪畫所有多邊形*/
void redraw()
{
    for(int i=0; i<polygon_v.size(); i++)
    {
        if(polygon_v[i].used==true)
        {
            glColor3f(polygon_v[i].r,polygon_v[i].g,polygon_v[i].b);
            if(polygon_v[i].filled)
            {
                glBegin(GL_POLYGON);
            }
            else
            {
                glBegin(GL_LINE_LOOP);
            }
            for(int j=0; j<polygon_v[i].x.size(); j++)
            {
                glVertex2i(polygon_v[i].x[j],polygon_v[i].y[j]);
            }
            glEnd();
        }
    }
    glFlush();
}


/*滑鼠點擊事件*/
void mouse(int btn, int state, int x, int y)
{
    static int count;
    int where;
    static int xp[2],yp[2];
    static vector<double> px,py;
    if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
    {
        for(int i=polygon_v.size()-1; i>=0; i--)
        {
            if(polygon_v[i].used==false)
                continue;
            if(inside_check(x,wh-y,polygon_v[i]))
            {
                glColor3f(0.8,0.8,0.8);
                if(polygon_v[i].filled)
                {
                    glBegin(GL_POLYGON);
                }
                else
                {
                    glBegin(GL_LINE_LOOP);
                }
                for(int j=0; j<polygon_v[i].x.size(); j++)
                {
                    glVertex2i(polygon_v[i].x[j],polygon_v[i].y[j]);
                }
                glEnd();
                glFlush();
                polygon_v[i].used=false;
                redraw();
                break;
            }
        }
    }
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_UP && state1 == PICK)
    {
        state1=0;
        draw_mode = 0;
    }
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        //glPushAttrib(GL_ALL_ATTRIB_BITS);

        where = pick(x,y);
        glColor3f(r, g, b);
        if(where != 0)
        {
            count = 0;
            draw_mode = where;
        }
        switch(draw_mode)
        {
        case(LINE):
            state1 = LINE;
            if(count==0)
            {
                count++;
            }
            else if(count==1)
            {
                count++;
                xp[0] = x;
                yp[0] = y;
            }
            else
            {
                glBegin(GL_LINES);
                glVertex2i(x,wh-y);
                glVertex2i(xp[0],wh-yp[0]);
                glEnd();
                px.push_back(x);
                py.push_back(wh-y);
                px.push_back(xp[0]);
                py.push_back(wh-yp[0]);
                polygon_v.push_back(polygon{r,g,b,false,true,px,py});
                px.clear();
                py.clear();
                draw_mode=0;
                count=0;
            }
            break;
        case(RECTANGLE):
            state1 = RECTANGLE;
            if(count==0)
            {
                count++;
            }
            else if(count == 1)
            {
                count++;
                px.push_back(x);
                py.push_back(wh-y);
            }
            else
            {
                px.push_back(x);
                py.push_back(py[0]);
                px.push_back(x);
                py.push_back(wh-y);
                px.push_back(px[0]);
                py.push_back(wh-y);
                if(fill1)
                {
                    glBegin(GL_POLYGON);
                }
                else
                {
                    glBegin(GL_LINE_LOOP);
                }
                glVertex2i(x,wh-y);
                glVertex2i(x,py[0]);
                glVertex2i(px[0],py[0]);
                glVertex2i(px[0],wh-y);
                glEnd();
                draw_mode=0;
                count=0;
                polygon_v.push_back(polygon{r,g,b,fill1,true,px,py});
                px.clear();
                py.clear();
            }
            break;
        case (TRIANGLE):
            switch(count)
            {
                state1 = TRIANGLE;
            case(0):
                count++;
                break;
            case(1):
                count++;
                px.push_back(x);
                py.push_back(wh-y);
                break;
            case(2):
                count++;
                px.push_back(x);
                py.push_back(wh-y);
                break;
            case(3):
                px.push_back(x);
                py.push_back(wh-y);
                if(fill1)
                {
                    glBegin(GL_POLYGON);
                }
                else
                {
                    glBegin(GL_LINE_LOOP);
                }
                glVertex2i(px[0],py[0]);
                glVertex2i(px[1],py[1]);
                glVertex2i(x,wh-y);
                glEnd();
                draw_mode=0;
                count=0;
                polygon_v.push_back(polygon{r,g,b,fill1,true,px,py});
                px.clear();
                py.clear();
            }
            break;
        case(POINTS):
        {
            state1 = POINTS;
            if(count!=0)
            {
                drawSquare(x,y);
            }
            count++;
        }
        break;
        case(TEXT):
        {
            state1 = TEXT;
            rx=x;
            ry=wh-y;
            glRasterPos2i(rx,ry);
            count=0;
        }
        break;
        case(POLYGON):
        {
            state1 = POLYGON;
            if(where==POLYGON && one!=0)
            {
                if(fill1)
                {
                    glBegin(GL_POLYGON);
                }
                else
                {
                    glBegin(GL_LINE_LOOP);
                }
                for(int i=0; i<px.size(); i++)
                {
                    glVertex2i(px[i],py[i]);
                }
                glEnd();
                polygon_v.push_back(polygon{r,g,b,fill1,true,px,py});
                px.clear();
                py.clear();
                one=0;
            }
            else if(where!=POLYGON)
            {
                px.push_back(x);
                py.push_back(wh-y);
                one=1;
            }
        }
        break;
        case(ERASE):
        {
            state1=ERASE;
        }
        break;
        case(SPRAY):
        {
            state1=SPRAY;
            if(count!=0)
            {
                drawSpary(x,y);
            }
            count++;
        }
        break;
        case(RUBBER_CIR):
        {
            if(count==0)
            {
                count++;
            }
            else if(count==1)
            {
                xm=x;
                ym=(wh-y);
                glLogicOp(GL_XOR);
                first=0;
                state1 = RUBBER_CIR;
            }
        }
        break;
        case(RUBBER_RECT):
        {
            if(count==0)
            {
                count++;
            }
            else if(count==1)
            {
                xm=x;
                ym=(wh-y);
                glLogicOp(GL_XOR);
                first=0;
                state1 = RUBBER_RECT;
            }
        }
        break;
        case(DRAW):
        {
            if(count==0)
            {
                count++;
            }
            else if(count==1)
            {
                state1 = DRAW;
            }
        }
        break;
        case(BIGGER_D):
        {
            draw_r+=1.0;
            draw_mode = 0;
        }
        break;
        case(SMALLER_D):
        {
            draw_r-=1.0;
            draw_r=max(draw_r,1.0);
            draw_mode = 0;
        }
        break;
        case(COLOR_D):
        {
            if(color_cnt1>=color_v.size())
            {
                color_cnt1=0;
            }
            glColor3f(color_v[color_cnt1].r, color_v[color_cnt1].g, color_v[color_cnt1].b);
            dr=color_v[color_cnt1].r;
            dg=color_v[color_cnt1].g;
            db=color_v[color_cnt1].b;
            glBegin(GL_POLYGON);
            glVertex2i(3*ww/10+ww/10, wh-ww/10-ww/12);
            glVertex2i(3*ww/10, wh-ww/10-ww/12);
            glVertex2i(3*ww/10, wh-ww/10-ww/10);
            glVertex2i(3*ww/10+ww/10, wh-ww/10-ww/10);
            glEnd();
            color_cnt1++;
            draw_mode = 0;
        }
        break;
        case(COLOR_T):
        {
            if(color_cnt2>=color_v.size())
            {
                color_cnt2=0;
            }
            glColor3f(color_v[color_cnt2].r, color_v[color_cnt2].g, color_v[color_cnt2].b);
            tr=color_v[color_cnt2].r;
            tg=color_v[color_cnt2].g;
            tb=color_v[color_cnt2].b;
            glBegin(GL_POLYGON);
            glVertex2i(4*ww/10+ww/10, wh-ww/10-ww/12);
            glVertex2i(4*ww/10, wh-ww/10-ww/12);
            glVertex2i(4*ww/10, wh-ww/10-ww/10);
            glVertex2i(4*ww/10+ww/10, wh-ww/10-ww/10);
            glEnd();
            color_cnt2++;
            draw_mode = 0;
        }
        break;
        case(PICK):
        {
            if(count == 0)
            {
                count++;
            }
            else if(count == 1)
            {
                state1 = PICK;
                choose=-1;
                for(int i=polygon_v.size()-1; i>=0; i--)
                {
                    if(polygon_v[i].used == 0)
                    {
                        continue;
                    }
                    if(inside_check(x,wh-y,polygon_v[i]))
                    {
                        pick_x = x;
                        pick_y = y;
                        choose = i;
                        break;
                    }
                }
                count = 0;
            }
        }
        break;
        case(MUSIC):
        {
            state1 = MUSIC;
            if(music_flag==false)
            {
                int shift;
                music_flag = true;
                PlaySoundA("music.wav",NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
                glColor3f(0.8,0.5,0.2);
                screen_box(6*ww/10,wh-ww/10-ww/10,ww/10);
                glColor3f(0,0,0);
                glRasterPos2i(6*ww/10+ww/60,wh-ww/6);
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'S');
                shift=glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'S');
                glRasterPos2i(6*ww/10+ww/60+shift,wh-ww/6);
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'T');
                shift+=glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'T');
                glRasterPos2i(6*ww/10+ww/60+shift,wh-ww/6);
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'O');
                shift+=glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'O');
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'P');
            }
            else
            {
                int shift;
                music_flag = false;
                PlaySound(NULL, NULL, SND_ASYNC);
                glColor3f(0.8,0.5,0.2);
                screen_box(6*ww/10,wh-ww/10-ww/10,ww/10);
                glColor3f(0,0,0);
                glRasterPos2i(6*ww/10+ww/60,wh-ww/6);
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'P');
                shift=glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'P');
                glRasterPos2i(6*ww/10+ww/60+shift,wh-ww/6);
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'L');
                shift+=glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'L');
                glRasterPos2i(6*ww/10+ww/60+shift,wh-ww/6);
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');
                shift+=glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'A');
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'Y');
            }
            draw_mode = 0;
        }
        break;
        case(RUBBER_LINE):
        {
            if(count==0)
            {
                count++;
            }
            else if(count==1)
            {
                xm=x;
                ym=(wh-y);
                glLogicOp(GL_XOR);
                first=0;
                state1 = RUBBER_LINE;
            }
        }
        break;
        case(BIGGER_E):
        {
            erase_r+=2;
            draw_mode = 0;
        }
        break;
        case(SMALLER_E):
        {
            erase_r-=2;
            erase_r=max(erase_r,1.0);
            draw_mode = 0;
        }
        break;
        }
        //glPopAttrib();
    }
    if (btn==GLUT_LEFT_BUTTON && state==GLUT_UP)
    {
        if(state1 == RUBBER_CIR)
        {
            if(fill1)
            {
                glBegin(GL_POLYGON);
            }
            else
            {
                glBegin(GL_LINE_LOOP);
            }
            for(int i=0; i<n; i++)
            {
                glVertex2f(xm+R*cos(2*PI*i/n),ym+R*sin(2*PI*i/n));
            }
            glEnd();
            glFlush();
            glLogicOp(GL_COPY);
            xmm=x;
            ymm=(wh-y);
            glLogicOp(GL_COPY);
            if(fill1)
            {
                glBegin(GL_POLYGON);
            }
            else
            {
                glBegin(GL_LINE_LOOP);
            }
            for(int i=0; i<n; i++)
            {
                px.push_back(xm+R*cos(2*PI*i/n));
                py.push_back(ym+R*sin(2*PI*i/n));
                glVertex2f(xm+R*cos(2*PI*i/n),ym+R*sin(2*PI*i/n));
            }
            polygon_v.push_back(polygon{r,g,b,fill1,true,px,py});
            px.clear();
            py.clear();
            glEnd();
            glFlush();
            state1=0;
        }
        else if(state1 == RUBBER_RECT)
        {
            if(fill1)
            {
                glBegin(GL_POLYGON);
            }
            else
            {
                glBegin(GL_LINE_LOOP);
            }
            glVertex2f(xm,ym);
            glVertex2f(xmm,ym);
            glVertex2f(xmm,ymm);
            glVertex2f(xm,ymm);
            glEnd();
            glFlush();
            glLogicOp(GL_COPY);
            xmm=x;
            ymm=(wh-y);
            glLogicOp(GL_COPY);
            if(fill1)
            {
                glBegin(GL_POLYGON);
            }
            else
            {
                glBegin(GL_LINE_LOOP);
            }
            glVertex2f(xm,ym);
            glVertex2f(xmm,ym);
            glVertex2f(xmm,ymm);
            glVertex2f(xm,ymm);
            px.push_back(xm);
            py.push_back(ym);
            px.push_back(xmm);
            py.push_back(ym);
            px.push_back(xmm);
            py.push_back(ymm);
            px.push_back(xm);
            py.push_back(ymm);
            polygon_v.push_back(polygon{r,g,b,fill1,true,px,py});
            px.clear();
            py.clear();
            glEnd();
            glFlush();
            state1=0;
        }
        else if(state1 == RUBBER_LINE)
        {
            glBegin(GL_LINES);
            glVertex2f(xm,ym);
            glVertex2f(xmm,ymm);
            glEnd();
            glFlush();
            glLogicOp(GL_COPY);
            xmm=x;
            ymm=(wh-y);
            glLogicOp(GL_COPY);
            glBegin(GL_LINES);
            glVertex2f(xm,ym);
            glVertex2f(xmm,ymm);
            px.push_back(xm);
            py.push_back(ym);
            px.push_back(xmm);
            py.push_back(ymm);
            polygon_v.push_back(polygon{r,g,b,false,true,px,py});
            px.clear();
            py.clear();
            glEnd();
            glFlush();
            state1=0;
        }
    }
    glFlush();
}


/*根據點擊區域決定功能*/
int pick(int x, int y)
{
    y = wh - y;
    if(y < wh-ww/5)
        return 0;
    if(y >= wh-ww/10)
    {
        if(x < ww/10)
        {
            return LINE;
        }
        else if(x < 2*ww/10)
        {
            return RECTANGLE;
        }
        else if(x < 3*ww/10)
        {
            return TRIANGLE;
        }
        else if(x < 4*ww/10)
        {
            return POINTS;
        }
        else if(x < 5*ww/10)
        {
            return TEXT;
        }

        /*我的*/
        else if(x < 6*ww/10)
        {
            return POLYGON;
        }
        else if(x < 7*ww/10)
        {
            return ERASE;
        }
        else if(x < 8*ww/10)
        {
            return SPRAY;
        }
        else if(x < 9*ww/10)
        {
            return RUBBER_CIR;
        }
        else if(x < 10*ww/10)
        {
            return RUBBER_RECT;
        }
    }
    else if(y >= wh-ww/5)
    {
        if(x < ww/10)
        {
            return DRAW;
        }
        else if(x < 2*ww/10)
        {
            return BIGGER_D;
        }
        else if(x < 3*ww/10)
        {
            return SMALLER_D;
        }
        else if(x < 4*ww/10)
        {
            return COLOR_D;
        }
        else if(x < 5*ww/10)
        {
            return COLOR_T;
        }
        else if(x < 6*ww/10)
        {
            return PICK;
        }
        else if(x < 7*ww/10)
        {
            return MUSIC;
        }
        else if(x < 8*ww/10)
        {
            return RUBBER_LINE;
        }
        else if(x < 9*ww/10)
        {
            return BIGGER_E;
        }
        else if(x < 10*ww/10)
        {
            return SMALLER_E;
        }
    }
    /*我的*/
    return 0;
}


/*畫功能區域的正方形*/
void screen_box(int x, int y, int s )
{
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x+s, y);
    glVertex2i(x+s, y+s);
    glVertex2i(x, y+s);
    glEnd();
}

void right_menu(int id)
{

}

void middle_menu(int id)
{
    if(id == 1)
    {
        exit(0);
    }
    else if(id == 2)
    {
        polygon_v.clear();
        display();
    }
    else if(id == 3){
        display();
    }
    else if(id == 4){
        redraw();
    }
    else if(id == 5){
        ScreenShot();
    }
}

/*挑選顏色*/
void color_menu(int id)
{
    if(id == 1)
    {
        r = 1.0;
        g = 0.0;
        b = 0.0;
    }
    else if(id == 2)
    {
        r = 0.0;
        g = 1.0;
        b = 0.0;
    }
    else if(id == 3)
    {
        r = 0.0;
        g = 0.0;
        b = 1.0;
    }
    else if(id == 4)
    {
        r = 0.0;
        g = 1.0;
        b = 1.0;
    }
    else if(id == 5)
    {
        r = 1.0;
        g = 0.0;
        b = 1.0;
    }
    else if(id == 6)
    {
        r = 1.0;
        g = 1.0;
        b = 0.0;
    }
    else if(id == 7)
    {
        r = 1.0;
        g = 1.0;
        b = 1.0;
    }
    else if(id == 8)
    {
        r = 0.0;
        g = 0.0;
        b = 0.0;
    }
}


/*更改色點大小*/
void pixel_menu(int id)
{
    if (id == 1)
    {
        size = 2 * size;
    }
    else if (size > 1)
    {
        size = size/2;
    }
}


/*要不要塗滿*/
void fill_menu(int id)
{
    if (id == 1)
    {
        fill1 = 1;
    }
    else
    {
        fill1 = 0;
    }
}


/*字形選擇*/
void font_menu(int id)
{
    font_id=id;
}


/*打字*/
void key(unsigned char k, int xx, int yy)
{
    if(draw_mode!=TEXT)
    {
        return;
    }
    glColor3f(tr,tg,tb);
    glRasterPos2i(rx,ry);
    switch(font_id)
    {
    case 1:
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13,k);
        rx+=glutBitmapWidth(GLUT_BITMAP_8_BY_13,k);
        break;
    case 2:
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15,k);
        rx+=glutBitmapWidth(GLUT_BITMAP_9_BY_15,k);
        break;
    case 3:
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,k);
        rx+=glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_10,k);
        break;
    case 4:
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,k);
        rx+=glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24,k);
        break;
    case 5:
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,k);
        rx+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_10,k);
        break;
    case 6:
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,k);
        rx+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_12,k);
        break;
    case 7:
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,k);
        rx+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_18,k);
        break;
    }
    glFlush();
    /*glutStrokeCharacter(GLUT_STROKE_ROMAN,i); */
}


/*display 功能區域與畫布*/
void display(void)
{
    int shift=0;
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glClearColor (0.8, 0.8, 0.8, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    //選擇圖標部分
    glColor3f(1.0, 1.0, 1.0);
    screen_box(0,wh-ww/10,ww/10);
    glColor3f(1.0, 0.0, 0.0);
    screen_box(ww/10,wh-ww/10,ww/10);
    glColor3f(0.0, 1.0, 0.0);
    screen_box(2*ww/10,wh-ww/10,ww/10);
    glColor3f(0.0, 0.0, 1.0);
    screen_box(3*ww/10,wh-ww/10,ww/10);
    glColor3f(1.0, 1.0, 0.0);
    screen_box(4*ww/10,wh-ww/10,ww/10);

    /*我的*/
    glColor3f(0.0, 1.0, 1.0);
    screen_box(5*ww/10,wh-ww/10,ww/10);
    glColor3f(1.0, 0.0, 1.0);
    screen_box(6*ww/10,wh-ww/10,ww/10);
    glColor3f(1.0, 0.0, 0.5);
    screen_box(7*ww/10,wh-ww/10,ww/10);
    glColor3f(0.5, 0.5, 0.5);
    screen_box(8*ww/10,wh-ww/10,ww/10);
    glColor3f(0.5, 1, 0.5);
    screen_box(9*ww/10,wh-ww/10,ww/10);

    //待更新
    glColor3f(1, 0.2, 0.5);
    screen_box(0,wh-ww/10-ww/10,ww/10);
    glColor3f(0.5, 0.2, 0.5);
    screen_box(ww/10,wh-ww/10-ww/10,ww/10);
    glColor3f(0.5, 0.2, 1);
    screen_box(2*ww/10,wh-ww/10-ww/10,ww/10);
    glColor3f(0.5, 0.5, 0.2);
    screen_box(3*ww/10,wh-ww/10-ww/10,ww/10);
    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2i(3*ww/10+ww/10, wh-ww/10-ww/12);
    glVertex2i(3*ww/10, wh-ww/10-ww/12);
    glVertex2i(3*ww/10, wh-ww/10-ww/10);
    glVertex2i(3*ww/10+ww/10, wh-ww/10-ww/10);
    glEnd();
    glColor3f(0.7,0.5,0.7);
    screen_box(4*ww/10,wh-ww/10-ww/10,ww/10);
    glColor3f(0, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2i(4*ww/10+ww/10, wh-ww/10-ww/12);
    glVertex2i(4*ww/10, wh-ww/10-ww/12);
    glVertex2i(4*ww/10, wh-ww/10-ww/10);
    glVertex2i(4*ww/10+ww/10, wh-ww/10-ww/10);
    glEnd();
    glColor3f(0.5,0.5,0.5);
    screen_box(5*ww/10,wh-ww/10-ww/10,ww/10);
    glColor3f(0.8,0.5,0.2);
    screen_box(6*ww/10,wh-ww/10-ww/10,ww/10);
    glColor3f(0.2,0.5,0.8);
    screen_box(7*ww/10,wh-ww/10-ww/10,ww/10);

    glColor3f(1, 1, 0.5);
    screen_box(8*ww/10,wh-ww/10-ww/10,ww/10);
    glColor3f(0.2, 1, 0.5);
    screen_box(9*ww/10,wh-ww/10-ww/10,ww/10);
    /*我的*/

    //選擇部分結束


    //畫圖標上的圖形
    glColor3f(0.0, 0.0, 0.0);
    screen_box(ww/10+ww/40,wh-ww/10+ww/40,ww/20);
    glBegin(GL_LINES);
    glVertex2i(wh/40,wh-ww/20);
    glVertex2i(wh/40+ww/20,wh-ww/20);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2i(ww/5+ww/40,wh-ww/10+ww/40);
    glVertex2i(ww/5+ww/20,wh-ww/40);
    glVertex2i(ww/5+3*ww/40,wh-ww/10+ww/40);
    glEnd();
    glPointSize(3.0);
    glBegin(GL_POINTS);
    glVertex2i(3*ww/10+ww/20, wh-ww/20);
    glEnd();

    glRasterPos2i(4*ww/10+ww/40,wh-ww/15);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');
    shift=glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'A');
    glRasterPos2i(4*ww/10+ww/40+shift,wh-ww/15);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'B');
    shift+=glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'B');
    glRasterPos2i(4*ww/10+ww/40+shift,wh-ww/15);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'C');

    //我的
    //5邊形
    glBegin(GL_POLYGON);
    glVertex2i(5*ww/10+ww/40,wh-ww/10+ww/20);
    glVertex2i(5*ww/10+ww/20,wh-ww/40);
    glVertex2i(5*ww/10+3*ww/40,wh-ww/10+ww/20);
    glVertex2i(5*ww/10+2.5*ww/40,wh-ww/10+ww/40);
    glVertex2i(5*ww/10+ww/30,wh-ww/10+ww/40);
    glEnd();

    //橡皮擦
    glBegin(GL_POLYGON);
    glVertex2i(6*ww/10+ww/40,wh-ww/10+ww/20);
    glVertex2i(6*ww/10+3*ww/40,wh-ww/10+ww/20);
    glVertex2i(6*ww/10+3*ww/40,wh-ww/10+ww/40);
    glVertex2i(6*ww/10+ww/40,wh-ww/10+ww/40);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2i(6*ww/10+ww/40+1,wh-ww/10+ww/15);
    glVertex2i(6*ww/10+3*ww/40,wh-ww/10+ww/15);
    glVertex2i(6*ww/10+3*ww/40,wh-ww/10+ww/20);
    glVertex2i(6*ww/10+ww/40+1,wh-ww/10+ww/20);
    glEnd();

    //塗鴉
    glBegin(GL_LINE_LOOP);
    for(int i=0; i<n; i++)
    {
        glVertex2f(7*ww/10+ww/20+ww/28*cos(2*PI*i/n),wh-ww/15+ww/100+ww/28*sin(2*PI*i/n));
    }
    glEnd();
    glBegin(GL_LINES);
    glVertex2i(7*ww/10+ww/25,wh-ww/10+ww/16);
    glVertex2i(7*ww/10+ww/33,wh-ww/10+ww/21);
    glEnd();
    glBegin(GL_LINES);
    glVertex2i(7*ww/10+ww/25,wh-ww/10+ww/21);
    glVertex2i(7*ww/10+ww/33,wh-ww/10+ww/16);
    glEnd();
    glBegin(GL_LINES);
    glVertex2i(7*ww/10+ww/17,wh-ww/10+ww/16);
    glVertex2i(7*ww/10+ww/14,wh-ww/10+ww/21);
    glEnd();
    glBegin(GL_LINES);
    glVertex2i(7*ww/10+ww/17,wh-ww/10+ww/21);
    glVertex2i(7*ww/10+ww/14,wh-ww/10+ww/16);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2i(7*ww/10+ww/33,wh-ww/10+ww/30);
    glVertex2i(7*ww/10+ww/25,wh-ww/10+ww/40);
    glVertex2i(7*ww/10+ww/21,wh-ww/10+ww/50);
    glVertex2i(7*ww/10+ww/17,wh-ww/10+ww/40);
    glVertex2i(7*ww/10+ww/14,wh-ww/10+ww/30);
    glEnd();
    glBegin(GL_LINES);
    glVertex2i(7*ww/10+ww/25,wh-ww/10+ww/40);
    glVertex2i(7*ww/10+ww/25,wh-ww/10+ww/30);
    glEnd();
    glBegin(GL_LINES);
    glVertex2i(7*ww/10+ww/20,wh-ww/10+ww/50);
    glVertex2i(7*ww/10+ww/20,wh-ww/10+ww/30);
    glEnd();
    glBegin(GL_LINES);
    glVertex2i(7*ww/10+ww/17,wh-ww/10+ww/40);
    glVertex2i(7*ww/10+ww/17,wh-ww/10+ww/30);
    glEnd();
    //橡皮筋圓
    glBegin(GL_LINE_LOOP);
    for(int i=0; i<n; i++)
    {
        glVertex2f(8*ww/10+ww/20+ww/28*cos(2*PI*i/n),wh-ww/15+ww/100+ww/28*sin(2*PI*i/n));
    }
    glEnd();
    //橡皮筋方塊
    glBegin(GL_LINE_LOOP);
    glVertex2i(9*ww/10+ww/40,wh-ww/10+ww/15);
    glVertex2i(9*ww/10+3*ww/40,wh-ww/10+ww/15);
    glVertex2i(9*ww/10+3*ww/40,wh-ww/10+ww/50);
    glVertex2i(9*ww/10+ww/40,wh-ww/10+ww/50);
    glEnd();
    //筆刷
    glBegin(GL_LINE_LOOP);
    glVertex2i(wh/40+ww/100,wh-ww/5.5);
    glVertex2i(wh/40+ww/20,wh-ww/7);
    glVertex2i(wh/40+ww/30,wh-ww/8);
    glVertex2i(wh/40-ww/95,wh-ww/6);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2i(wh/40+ww/20,wh-ww/7);
    glVertex2i(wh/40+ww/16,wh-ww/9);
    glVertex2i(wh/40+ww/30,wh-ww/8);
    glEnd();

    //加大
    glRasterPos2i(ww/10+ww/25,wh-ww/6);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '+');
    //縮小
    glRasterPos2i(2*ww/10+ww/27,wh-ww/6);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '-');
    //字體顏色
    glRasterPos2i(3*ww/10+ww/27,wh-ww/6);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'D');
    glRasterPos2i(4*ww/10+ww/27,wh-ww/6);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'A');

    //抓取多邊形
    glRasterPos2i(5*ww/10+ww/60,wh-ww/6);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'P');
    shift=glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'P');
    glRasterPos2i(5*ww/10+ww/60+shift,wh-ww/6);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'I');
    shift+=glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'I');
    glRasterPos2i(5*ww/10+ww/60+shift,wh-ww/6);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'C');
    shift+=glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'C');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'K');


    //MUSIC
    glRasterPos2i(6*ww/10+ww/60,wh-ww/6);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'P');
    shift=glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'P');
    glRasterPos2i(6*ww/10+ww/60+shift,wh-ww/6);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'L');
    shift+=glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'L');
    glRasterPos2i(6*ww/10+ww/60+shift,wh-ww/6);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');
    shift+=glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'A');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'Y');

    //RUBBER_LINE
    glBegin(GL_LINES);
    glVertex2i(7*ww/10+wh/40,wh-ww/5.5);
    glVertex2i(7*ww/10+wh/40+ww/20,wh-ww/8);
    glEnd();
    //bigger_e
    glBegin(GL_POLYGON);
    glVertex2i(8*ww/10+ww/40,wh-ww/5+ww/20);
    glVertex2i(8*ww/10+3*ww/40,wh-ww/5+ww/20);
    glVertex2i(8*ww/10+3*ww/40,wh-ww/5+ww/40);
    glVertex2i(8*ww/10+ww/40,wh-ww/5+ww/40);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2i(8*ww/10+ww/40+1,wh-ww/5+ww/15);
    glVertex2i(8*ww/10+3*ww/40,wh-ww/5+ww/15);
    glVertex2i(8*ww/10+3*ww/40,wh-ww/5+ww/20);
    glVertex2i(8*ww/10+ww/40+1,wh-ww/5+ww/20);
    glEnd();

    glRasterPos2i(8*ww/10+ww/15,wh-ww/7.5);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '+');
    //small_e
    glBegin(GL_POLYGON);
    glVertex2i(9*ww/10+ww/40,wh-ww/5+ww/20);
    glVertex2i(9*ww/10+3*ww/40,wh-ww/5+ww/20);
    glVertex2i(9*ww/10+3*ww/40,wh-ww/5+ww/40);
    glVertex2i(9*ww/10+ww/40,wh-ww/5+ww/40);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2i(9*ww/10+ww/40+1,wh-ww/5+ww/15);
    glVertex2i(9*ww/10+3*ww/40,wh-ww/5+ww/15);
    glVertex2i(9*ww/10+3*ww/40,wh-ww/5+ww/20);
    glVertex2i(9*ww/10+ww/40+1,wh-ww/5+ww/20);
    glEnd();

    glRasterPos2i(9*ww/10+ww/15,wh-ww/7.5);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '-');
    //我的

    //繪畫圖型結束

    //畫出來
    glFlush();
    glPopAttrib();
}

/*main*/
int main(int argc, char** argv)
{
    int c_menu, p_menu, f_menu, t_menu;
    color_v.clear();
    color_v.push_back(color_c{0,0,0});
    color_v.push_back(color_c{1,0,0});
    color_v.push_back(color_c{0,1,0});
    color_v.push_back(color_c{0,0,1});
    color_v.push_back(color_c{0,1,1});
    color_v.push_back(color_c{1,0,1});
    color_v.push_back(color_c{1,1,0});
    color_v.push_back(color_c{1,1,1});
    glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("square");
    glutDisplayFunc(display);
    c_menu = glutCreateMenu(color_menu);
    glutAddMenuEntry("Red",1);
    glutAddMenuEntry("Green",2);
    glutAddMenuEntry("Blue",3);
    glutAddMenuEntry("Cyan",4);
    glutAddMenuEntry("Magenta",5);
    glutAddMenuEntry("Yellow",6);
    glutAddMenuEntry("White",7);
    glutAddMenuEntry("Black",8);
    p_menu = glutCreateMenu(pixel_menu);
    glutAddMenuEntry("increase pixel size", 1);
    glutAddMenuEntry("decrease pixel size", 2);
    f_menu = glutCreateMenu(fill_menu);
    glutAddMenuEntry("fill on", 1);
    glutAddMenuEntry("fill off", 2);
    t_menu = glutCreateMenu(font_menu);
    glutAddMenuEntry("8 by 13",1);
    glutAddMenuEntry("9 by 15",2);
    glutAddMenuEntry("Times Roman 10",3);
    glutAddMenuEntry("Times Roman 24",4);
    glutAddMenuEntry("Helvetica 10",5);
    glutAddMenuEntry("Helvetica 12",6);
    glutAddMenuEntry("Helvetica 18",7);
    glutCreateMenu(middle_menu);
    glutAddSubMenu("Colors", c_menu);
    glutAddSubMenu("Pixel Size", p_menu);
    glutAddSubMenu("Fill", f_menu);
    glutAddSubMenu("Font", t_menu);
    glutAddMenuEntry("quit",1);
    glutAddMenuEntry("clear",2);
    glutAddMenuEntry("hide",3);
    glutAddMenuEntry("display",4);
    glutAddMenuEntry("save",5);
    glutAttachMenu(GLUT_MIDDLE_BUTTON);
    myinit();
    glutReshapeFunc (myReshape);
    glutKeyboardFunc(key);
    glutMouseFunc (mouse);
    glutMotionFunc(move1);
    glutMainLoop();
}
