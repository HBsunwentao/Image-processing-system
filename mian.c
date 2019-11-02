/*
This program can process the .bmp image and finish the follow tasks:
1.horizontal mirror
2.vertical mirror
4. ColorToGray
5. smooth
6. sharpening
7. mosaic

Author：Wentao Sun

*/
//位图文件头BITMAPFILEHEADER，14字节
//位图信息头BITMAPINFOHEASER，40字节
//调色板Palette(真彩图不用)设RGB值
//实际的位图数据ImageDate
//划分像素点，赋予像素值（RGB值、灰度值，二值）。像素值存放顺序：从左下角，从左到右，自下而上
//反色操作（真彩色图）：用255分别减去RGB各个字节，然后代替原来的像素数据。24位真彩图,256

#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#include<windows.h>
#define M 800*3		//图像的最大行数
#define N 1000		//图像的最大列数
typedef unsigned char BYTE;




typedef struct tagPICSIZE  {
    int width;//图像宽带字节数
    int height;//图像高度字节数
    int length;//图像大小字节数
    int offset;//前三部分字节数
}PICSIZE;

//定义全局数组用于存储后续图像数据信息
BYTE PictureIn[M*N];//读文件的
BYTE PictureOut[M*N];//写入文件的
FILE *fInput;   //输入文件
FILE *fOutput;//输出文件
BITMAPFILEHEADER FileHeader; //图像第一部分存储变量
BITMAPINFOHEADER InfoHeader;//图像第二部分存储变量
RGBQUAD Quad[256];//调色板存储的数组
PICSIZE PiSize;  //存储图像大小信息的结构体变量

int main()
{
	void LevelOrvertical1();
	void LevelOrvertical2();
	//void TurnOver();
	void ColorToGray();
	void ColorControl(int flag);
	void Mosaic();
	int  choice;


	for(;;)
	{
	    printf("\n\n**********欢迎使用简单的图片处理系统**********\n\n");
	    printf("请阅读菜单选择你要进行的操作：\n\n");
	    printf("1、将图进行水平镜像操作\n\n");
	    printf("2、将图进行垂直镜像操作\n\n");
	   // printf("3、将24位真彩色图进行反色操作\n\n");
	    printf("4、将图转为灰度色图\n\n");
	    printf("5、将图进行平滑操作\n\n");
	    printf("6、将图进行锐化操作\n\n");
	    printf("7、将图进行马赛克操作\n\n");
	    //printf("8、将256色图进行锐化操作\n\n");
	    printf("9、退出系统\n\n");

		printf("请输入您的选择：");
	    scanf("%d",&choice);



		if(choice==9)
		{
			printf("\n*****谢谢使用！*****\n");
			break;
		}
		switch(choice)
		{
			case 1:LevelOrvertical1();break;
			case 2:LevelOrvertical2();break;
	       // case 3:TurnOver();break;
		    case 4:ColorToGray();break;
		    case 5:ColorControl(1);break;
		    case 6:ColorControl(0);break;
		    case 7:Mosaic();break;
		    case 9:return 0;break;
		    //case 8:GrayControl(0);break;
	        default:printf("\n很抱歉没有此项操作，请重新选择：\n");
		}



  }
		return 0;
}









/*
pInput() 过程功能是读取指定位置的BMP图像文件
S1先读取 BITMAPFILEHEADER 和  BITMAPINFOHEADER 两部分
分别存放在FileHeader 和 InfoHeader 两个结构体变量里面
S2 算出图像色彩位数是否是24位真彩图， 并确定第四部分ImageDate位图数据的字节数
*/
void pInput()  //BMP图像的读入部分
{
    	char FileName[20];
	    printf("请输入要变化的图像文件名\n");
	    scanf("%s",FileName);
        while((fInput=fopen(FileName,"rb"))==NULL)
       {
           printf("sorry,we can not open this file,please Re Input\n");
           scanf("%s",FileName);
        }

       fread(&FileHeader,sizeof(BITMAPFILEHEADER),1,fInput);
       fread(&InfoHeader,sizeof(BITMAPINFOHEADER),1,fInput);
	PiSize.width=InfoHeader.biWidth;
	PiSize.height=InfoHeader.biHeight;
	PiSize.offset=FileHeader.bfOffBits;
    if(InfoHeader.biBitCount==24)PiSize.width=PiSize.width*3;            //判断是否是真彩图，并读取第三部分调色板  Palette 存放在Quad数组中
        else fread(Quad,sizeof(RGBQUAD),pow(2,InfoHeader.biBitCount),fInput);
    PiSize.length=PiSize.width*PiSize.height;

    fseek(fInput,PiSize.offset,0);                                      //读取ImageDate位图数据
    fread(PictureIn,sizeof(BYTE),PiSize.length,fInput);
}









/*
生成变化完成的BMP格式图像文件
*/
void pOutput(char OutName[20],int length)
{
    while((fOutput=fopen(OutName,"wb"))==NULL)
    {
        printf("sorry we can not build a file,please check your computer");
        return;
    }
	fwrite(&FileHeader,sizeof(BITMAPFILEHEADER),1,fOutput);
	fwrite(&InfoHeader,sizeof(BITMAPINFOHEADER),1,fOutput);
	if(InfoHeader.biBitCount!=24)fwrite(Quad,sizeof(RGBQUAD),pow(2,InfoHeader.biBitCount),fOutput);
	fwrite(PictureOut,sizeof(BYTE),length,fOutput);
	fclose(fOutput);
	fclose(fInput);
	printf("we have changed it sucessfully!   %d",InfoHeader.biBitCount);
}













/*
水平镜像变化
将Imagedate位图数据（存储在PictureIn中）左右交交换
*/
void LevelOrvertical1()
{
    void pInput();
    void pOutput();
    int i,j;
    pInput();
    if(InfoHeader.biBitCount==24)      //如果是真彩色图（因为一个像素点三个字节）
    for(j=0;j<PiSize.height;j++)
    {
        for(i=0;i<PiSize.width/3;i++)   //将左右像素点镜像 交换
        {
		        PictureOut[j*PiSize.width+i*3]=PictureIn[j*PiSize.width+(PiSize.width/3-i)*3];
		        PictureOut[j*PiSize.width+i*3+1]=PictureIn[j*PiSize.width+(PiSize.width/3-i)*3+1];
		        PictureOut[j*PiSize.width+i*3+2]=PictureIn[j*PiSize.width+(PiSize.width/3-i)*3+2];
        }
    }
    else                            //如果不是真彩色图 左右像素点交换
    {
        for(j=0;j<PiSize.height;j++)
        {
            for(i=0;i<PiSize.width;i++)
            {
                PictureOut[j*PiSize.width+i]=PictureIn[j*PiSize.width+PiSize.width-i];
            }
        }
    }

    pOutput("Horizontal.bmp",PiSize.length);
}














/*
垂直镜像变化
即上下像素点变化位置
*/
void LevelOrvertical2(int flag)
{
    void pInput();
    void pOutput();
    int i,j;
    pInput();



    for(j=0;j<PiSize.height;j++)
    {
        for(i=0;i<PiSize.width;i++)
        {
            PictureOut[j*PiSize.width+i]=PictureIn[(PiSize.height-j)*PiSize.width+i];

        }
    }



    pOutput("mirror vertically.bmp",PiSize.length);
}



















/*
灰度图变化
对真彩色：算每个像素点的灰度值求出，然后使RGB三色全部赋值为此灰度
对非真彩色图：直接修改对调色板RGB三色的数值为三者和的平均数
*/
void ColorToGray()
{
    void pOutput();
    int i,j;
    pInput();
    if(InfoHeader.biBitCount==24)
	for(j=0;j<PiSize.height;j++)
	{
	  for(i=0;i<PiSize.width/3;i++)
	  {
		int sum;//求每个像素点的灰度值
		sum=PictureIn[j*PiSize.width+i*3]*0.299+PictureIn[j*PiSize.width+i*3+1]*0.578+PictureIn[j*PiSize.width+i*3+2]*0.114;
	    PictureOut[j*PiSize.width+i*3]=sum;//将RGB三通道 全部赋值为 同一灰度值 实现照片变黑白
		PictureOut[j*PiSize.width+i*3+1]=sum;
		PictureOut[j*PiSize.width+i*3+2]=sum;
	  }
	}
	else
    {
        for(i=0;i<PiSize.length;i++)PictureOut[i]=PictureIn[i];
        BYTE sum;
        for(i=0;i<pow(2,InfoHeader.biBitCount);i++)//将调色板不同颜色转化为灰度
        {

           sum=(Quad[i].rgbBlue+Quad[i].rgbGreen+Quad[i].rgbRed)/3;
            Quad[i].rgbBlue=sum;
            Quad[i].rgbGreen=sum;
            Quad[i].rgbRed=sum;
        }
    }
    pOutput("color to grey.bmp",PiSize.length);

}













/*
如果flag=0 进行平滑操作
将原图中的每一点的灰度和它周围八个点的灰度相加，然后除以9，作为新图中对应点的灰度
如果flag=1 进行锐化操作
锐化模板是拉普拉斯(Laplacian)模板，：先将自身与周围的8个象素相减，表示自身与周围象
素的差别；再将这个差别加上自身作为新象素的灰度。可见，如果一片暗区出现了一个亮点，
那么锐化处理的结果是这个亮点变得更亮，增加了图象的噪声。
*/
void ColorControl(int flag)
{
    void pOutput();
    void pInput();
    int i,j;
    pInput();
    if(InfoHeader.biBitCount==24)             //判断是否是24色真彩图
    {
	if(flag==1)//flag=1进行平滑处理          //24色真彩图处理代码
	{
		for(j=1;j<PiSize.height;j++)
	    {
			for(i=3;i<(PiSize.width-3);i++)
	        {
	            int y=0;

			y+=PictureIn[(j-1)*PiSize.width+i-3];    //套用模板
			y+=PictureIn[(j-1)*PiSize.width+i];
			y+=PictureIn[(j-1)*PiSize.width+i+3];
			y+=PictureIn[j*PiSize.width+i-3];
			y+=PictureIn[j*PiSize.width+i];
			y+=PictureIn[j*PiSize.width+i+3];
			y+=PictureIn[(j+1)*PiSize.width+i-3];
			y+=PictureIn[(j+1)*PiSize.width+i];
			y+=PictureIn[(j+1)*PiSize.width+i+3];
			PictureOut[j*PiSize.width+i]=y/9;
			}
        }
        pOutput("smooth.bmp",PiSize.length);
	}
	else//flag=0 进行锐化
	{
		for(j=1;j<PiSize.height-1;j++)
	    {
			for(i=3;i<(PiSize.width-3);i++)   //套用锐化功能模板，使用拉普拉斯模板
	        {
	            int sum;
	            sum=9*PictureIn[j*PiSize.width+i];
				sum-=PictureIn[(j-1)*PiSize.width+(i-3)];
				sum-=PictureIn[(j-1)*PiSize.width+i];
				sum-=PictureIn[(j-1)*PiSize.width+i+3];
				sum-=PictureIn[j*PiSize.width+i-3];
				sum-=PictureIn[j*PiSize.width+i+3];
				sum-=PictureIn[(j+1)*PiSize.width+(i-3)];
				sum-=PictureIn[(j+1)*PiSize.width+i];
				sum-=PictureIn[(j+1)*PiSize.width+i+3];
				if(sum>255)
					PictureOut[j*PiSize.width+i]=255;
				else if(sum<0)
					PictureOut[j*PiSize.width+i]=0;
		        else
                    PictureOut[j*PiSize.width+i]=sum;
	        }
	    }
        pOutput("sharpening.bmp",PiSize.length);
	}
    }
    else                                    //如果非24位真彩图
    {
if(flag==1)//flag=1进行平滑处理
	{
		for(j=1;j<PiSize.height;j++)
	    {
			for(i=1;i<(PiSize.width-1);i++)
	        {
            int sum=0;
            sum+=PictureIn[(j-1)*PiSize.width+i-1];
			sum+=PictureIn[(j-1)*PiSize.width+i];
			sum+=PictureIn[(j-1)*PiSize.width+i+1];
			sum+=PictureIn[j*PiSize.width+i-1];
			sum+=PictureIn[j*PiSize.width+i];
			sum+=PictureIn[j*PiSize.width+i+1];
			sum+=PictureIn[(j+1)*PiSize.width+i-1];
			sum+=PictureIn[(j+1)*PiSize.width+i];
			sum+=PictureIn[(j+1)*PiSize.width+i+1];
			PictureOut[j*PiSize.width+i]=sum/9;
			}
        }
        pOutput("smooth.bmp",PiSize.length);
	}
	else//flag=0 进行锐化处理
	{
		for(j=1;j<PiSize.height-1;j++)
	    {
			for(i=1;i<(PiSize.width-1);i++)
	        {
	            int sum;
	            sum=9*PictureIn[j*PiSize.width+i];
				sum-=PictureIn[(j-1)*PiSize.width+(i-1)];
				sum-=PictureIn[(j-1)*PiSize.width+i];
				sum-=PictureIn[(j-1)*PiSize.width+i+1];
				sum-=PictureIn[j*PiSize.width+i-1];
				sum-=PictureIn[j*PiSize.width+i+1];
				sum-=PictureIn[(j+1)*PiSize.width+(i-1)];
				sum-=PictureIn[(j+1)*PiSize.width+i];
				sum-=PictureIn[(j+1)*PiSize.width+i+1];
				if(sum>255)
					PictureOut[j*PiSize.width+i]=255;
				else if(sum<0)
					PictureOut[j*PiSize.width+i]=0;
		        else
                    PictureOut[j*PiSize.width+i]=sum;
	        }
	    }
        pOutput("sharpening.bmp",PiSize.length);
	}
    }


}






















/*
马赛克功能实现
将图像分成众多20*20的方块
通过对20*20像素的方框色彩去取平均值
如果是非24位真彩图，直接使用20*20图像方块的中间色彩
*/
void Mosaic(int flag)
{
    void pInput();
    void Mosaicchange();
    void Mosaicchange2();
    void pOutput();
    int i,j;
    int lx,ly;
    int rx,ry;
    pInput();
    printf("请输入你想改变的区域，左下角坐标 和 右上角坐标，坐标以左下角为原点，长度和宽度默认100\n");
    scanf("%d %d %d %d",&lx,&ly,&rx,&ry);
    for(i=0;i<PiSize.length;i++)PictureOut[i]=PictureIn[i];
    if(InfoHeader.biBitCount==24)
    for(j=PiSize.height*ly/2000;j<PiSize.height*ry/2000;j++)
    {
        for(i=PiSize.width*lx/6000;i<PiSize.width*rx/6000;i++)
        {
            Mosaicchange(j,i,PiSize.width);                 //计算每一个方块颜色的平均值，并将方块中像素点色彩全部赋值为 平均值
                    }
    }
    else//如果非24位真彩图
    {
       for(j=PiSize.height*ly/2000;j<PiSize.height*ry/2000;j++)
      {
        for(i=PiSize.width*lx/2000;i<PiSize.width*rx/2000;i++)
        {
            Mosaicchange2(j,i,PiSize.width);
        }
      }
    }
    pOutput("mosaic.bmp",PiSize.length);

}

/*
24位真彩图 方块色彩颜色计算获取
*/
void Mosaicchange(int row,int col,int width)
{
    int i,j;
    int averRed=0;
    int averBlue=0;
    int averGreen=0;
    int sum=0;
    for(i=row*20;i<row*20+20;i++)
    {
        for(j=col*20;j<col*20+20;j++)
        {
            averRed+=PictureIn[i*width+j*3];
            averGreen+=PictureIn[i*width+j*3+1];
            averBlue+=PictureIn[i*width+j*3+2];
            ++sum;
        }
    }
    averRed/=sum;
    averGreen/=sum;
    averBlue/=sum;
    for(i=row*20;i<row*20+20;i++)
    {
        for(j=col*20;j<col*20+20;j++)
        {
            PictureOut[i*width+j*3]=averRed;
            PictureOut[i*width+j*3+1]=averGreen;
            PictureOut[i*width+j*3+2]=averBlue;
        }
    }
}

/*
非24色图像，去方块中间颜色作为整个方块的颜色
*/
void Mosaicchange2(int row,int col,int width) //非24色真彩图的 小方块处理过程
{
    int i,j;
    for(i=row*20;i<row*20+20;i++)
    {
        for(j=col*20;j<col*20+20;j++)
        {
            PictureOut[i*width+j]=PictureIn[(row*20+10)*width+col*20+10];
        }
    }
}
