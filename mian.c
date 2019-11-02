/*
This program can process the .bmp image and finish the follow tasks:
1.horizontal mirror
2.vertical mirror
4. ColorToGray
5. smooth
6. sharpening
7. mosaic

Author��Wentao Sun

*/
//λͼ�ļ�ͷBITMAPFILEHEADER��14�ֽ�
//λͼ��ϢͷBITMAPINFOHEASER��40�ֽ�
//��ɫ��Palette(���ͼ����)��RGBֵ
//ʵ�ʵ�λͼ����ImageDate
//�������ص㣬��������ֵ��RGBֵ���Ҷ�ֵ����ֵ��������ֵ���˳�򣺴����½ǣ������ң����¶���
//��ɫ���������ɫͼ������255�ֱ��ȥRGB�����ֽڣ�Ȼ�����ԭ�����������ݡ�24λ���ͼ,256

#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#include<windows.h>
#define M 800*3		//ͼ����������
#define N 1000		//ͼ����������
typedef unsigned char BYTE;




typedef struct tagPICSIZE  {
    int width;//ͼ�����ֽ���
    int height;//ͼ��߶��ֽ���
    int length;//ͼ���С�ֽ���
    int offset;//ǰ�������ֽ���
}PICSIZE;

//����ȫ���������ڴ洢����ͼ��������Ϣ
BYTE PictureIn[M*N];//���ļ���
BYTE PictureOut[M*N];//д���ļ���
FILE *fInput;   //�����ļ�
FILE *fOutput;//����ļ�
BITMAPFILEHEADER FileHeader; //ͼ���һ���ִ洢����
BITMAPINFOHEADER InfoHeader;//ͼ��ڶ����ִ洢����
RGBQUAD Quad[256];//��ɫ��洢������
PICSIZE PiSize;  //�洢ͼ���С��Ϣ�Ľṹ�����

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
	    printf("\n\n**********��ӭʹ�ü򵥵�ͼƬ����ϵͳ**********\n\n");
	    printf("���Ķ��˵�ѡ����Ҫ���еĲ�����\n\n");
	    printf("1����ͼ����ˮƽ�������\n\n");
	    printf("2����ͼ���д�ֱ�������\n\n");
	   // printf("3����24λ���ɫͼ���з�ɫ����\n\n");
	    printf("4����ͼתΪ�Ҷ�ɫͼ\n\n");
	    printf("5����ͼ����ƽ������\n\n");
	    printf("6����ͼ�����񻯲���\n\n");
	    printf("7����ͼ���������˲���\n\n");
	    //printf("8����256ɫͼ�����񻯲���\n\n");
	    printf("9���˳�ϵͳ\n\n");

		printf("����������ѡ��");
	    scanf("%d",&choice);



		if(choice==9)
		{
			printf("\n*****ллʹ�ã�*****\n");
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
	        default:printf("\n�ܱ�Ǹû�д��������������ѡ��\n");
		}



  }
		return 0;
}









/*
pInput() ���̹����Ƕ�ȡָ��λ�õ�BMPͼ���ļ�
S1�ȶ�ȡ BITMAPFILEHEADER ��  BITMAPINFOHEADER ������
�ֱ�����FileHeader �� InfoHeader �����ṹ���������
S2 ���ͼ��ɫ��λ���Ƿ���24λ���ͼ�� ��ȷ�����Ĳ���ImageDateλͼ���ݵ��ֽ���
*/
void pInput()  //BMPͼ��Ķ��벿��
{
    	char FileName[20];
	    printf("������Ҫ�仯��ͼ���ļ���\n");
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
    if(InfoHeader.biBitCount==24)PiSize.width=PiSize.width*3;            //�ж��Ƿ������ͼ������ȡ�������ֵ�ɫ��  Palette �����Quad������
        else fread(Quad,sizeof(RGBQUAD),pow(2,InfoHeader.biBitCount),fInput);
    PiSize.length=PiSize.width*PiSize.height;

    fseek(fInput,PiSize.offset,0);                                      //��ȡImageDateλͼ����
    fread(PictureIn,sizeof(BYTE),PiSize.length,fInput);
}









/*
���ɱ仯��ɵ�BMP��ʽͼ���ļ�
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
ˮƽ����仯
��Imagedateλͼ���ݣ��洢��PictureIn�У����ҽ�����
*/
void LevelOrvertical1()
{
    void pInput();
    void pOutput();
    int i,j;
    pInput();
    if(InfoHeader.biBitCount==24)      //��������ɫͼ����Ϊһ�����ص������ֽڣ�
    for(j=0;j<PiSize.height;j++)
    {
        for(i=0;i<PiSize.width/3;i++)   //���������ص㾵�� ����
        {
		        PictureOut[j*PiSize.width+i*3]=PictureIn[j*PiSize.width+(PiSize.width/3-i)*3];
		        PictureOut[j*PiSize.width+i*3+1]=PictureIn[j*PiSize.width+(PiSize.width/3-i)*3+1];
		        PictureOut[j*PiSize.width+i*3+2]=PictureIn[j*PiSize.width+(PiSize.width/3-i)*3+2];
        }
    }
    else                            //����������ɫͼ �������ص㽻��
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
��ֱ����仯
���������ص�仯λ��
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
�Ҷ�ͼ�仯
�����ɫ����ÿ�����ص�ĻҶ�ֵ�����Ȼ��ʹRGB��ɫȫ����ֵΪ�˻Ҷ�
�Է����ɫͼ��ֱ���޸ĶԵ�ɫ��RGB��ɫ����ֵΪ���ߺ͵�ƽ����
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
		int sum;//��ÿ�����ص�ĻҶ�ֵ
		sum=PictureIn[j*PiSize.width+i*3]*0.299+PictureIn[j*PiSize.width+i*3+1]*0.578+PictureIn[j*PiSize.width+i*3+2]*0.114;
	    PictureOut[j*PiSize.width+i*3]=sum;//��RGB��ͨ�� ȫ����ֵΪ ͬһ�Ҷ�ֵ ʵ����Ƭ��ڰ�
		PictureOut[j*PiSize.width+i*3+1]=sum;
		PictureOut[j*PiSize.width+i*3+2]=sum;
	  }
	}
	else
    {
        for(i=0;i<PiSize.length;i++)PictureOut[i]=PictureIn[i];
        BYTE sum;
        for(i=0;i<pow(2,InfoHeader.biBitCount);i++)//����ɫ�岻ͬ��ɫת��Ϊ�Ҷ�
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
���flag=0 ����ƽ������
��ԭͼ�е�ÿһ��ĻҶȺ�����Χ�˸���ĻҶ���ӣ�Ȼ�����9����Ϊ��ͼ�ж�Ӧ��ĻҶ�
���flag=1 �����񻯲���
��ģ����������˹(Laplacian)ģ�壬���Ƚ���������Χ��8�������������ʾ��������Χ��
�صĲ���ٽ����������������Ϊ�����صĻҶȡ��ɼ������һƬ����������һ�����㣬
��ô�񻯴���Ľ������������ø�����������ͼ���������
*/
void ColorControl(int flag)
{
    void pOutput();
    void pInput();
    int i,j;
    pInput();
    if(InfoHeader.biBitCount==24)             //�ж��Ƿ���24ɫ���ͼ
    {
	if(flag==1)//flag=1����ƽ������          //24ɫ���ͼ�������
	{
		for(j=1;j<PiSize.height;j++)
	    {
			for(i=3;i<(PiSize.width-3);i++)
	        {
	            int y=0;

			y+=PictureIn[(j-1)*PiSize.width+i-3];    //����ģ��
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
	else//flag=0 ������
	{
		for(j=1;j<PiSize.height-1;j++)
	    {
			for(i=3;i<(PiSize.width-3);i++)   //�����񻯹���ģ�壬ʹ��������˹ģ��
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
    else                                    //�����24λ���ͼ
    {
if(flag==1)//flag=1����ƽ������
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
	else//flag=0 �����񻯴���
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
�����˹���ʵ��
��ͼ��ֳ��ڶ�20*20�ķ���
ͨ����20*20���صķ���ɫ��ȥȡƽ��ֵ
����Ƿ�24λ���ͼ��ֱ��ʹ��20*20ͼ�񷽿���м�ɫ��
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
    printf("����������ı���������½����� �� ���Ͻ����꣬���������½�Ϊԭ�㣬���ȺͿ��Ĭ��100\n");
    scanf("%d %d %d %d",&lx,&ly,&rx,&ry);
    for(i=0;i<PiSize.length;i++)PictureOut[i]=PictureIn[i];
    if(InfoHeader.biBitCount==24)
    for(j=PiSize.height*ly/2000;j<PiSize.height*ry/2000;j++)
    {
        for(i=PiSize.width*lx/6000;i<PiSize.width*rx/6000;i++)
        {
            Mosaicchange(j,i,PiSize.width);                 //����ÿһ��������ɫ��ƽ��ֵ���������������ص�ɫ��ȫ����ֵΪ ƽ��ֵ
                    }
    }
    else//�����24λ���ͼ
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
24λ���ͼ ����ɫ����ɫ�����ȡ
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
��24ɫͼ��ȥ�����м���ɫ��Ϊ�����������ɫ
*/
void Mosaicchange2(int row,int col,int width) //��24ɫ���ͼ�� С���鴦�����
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
