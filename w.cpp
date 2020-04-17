#include<reg51.h>
/*********************显示子函数**************************/

void              Display()

              {

                            char h,l;

                            h=Time_EW/10;

                            l=Time_EW%10;

                    P2=table[l];

                            EW_LED2=0;

                            Delay(2);

                            EW_LED2=1;

                  P2=table[h];

                            EW_LED1=0;

                            Delay(2);

                            EW_LED1=1;



                           

                           

                            h=Time_SN/10;

                            l=Time_SN%10;

                  P2=table[l];

                            SN_LED2=0;

                            Delay(2);

                            SN_LED2=1;

                  P2=table[h];

                            SN_LED1=0;

                            Delay(2);

                            SN_LED1=1;

                           



紧急中断程序，全红灯10s

if(Special_Btton==0)              //紧急情况下，全红灯10S

     {

              uchar i;

              for(i=0;i<50;i++) //循环50次

              {Flag_SN_Yellow=0;

            Flag_EW_Yellow=0;

              P1=0xEE;

              Delay(200);     //延时200ms

            

              }

      }            

}

/**********************外部0中断服务程序************************/



void              EXINT0(void)interrupt 0 using 1

{

   EX0=0; //关中断

   if(Reduces_Button==0) //时间减

       {

         EW1-=5;                  

         SN1-=5;

         if(EW1<=25)

           {

                   EW1=25;

             SN1=15;

           }

        }

   EX0=1;//开中断

}

/**********************外部1中断服务程序************************/



void              EXINT1(void)interrupt 2 using 1

{

   EX1=0; //关中断

   if(Add_Button==0) //时间加

     {

       EW1+=5;                    

       SN1+=5;

       if(EW1>=45)

              {

                EW1=45;

                SN1=35;

        }   

     }                    

                    

   EX1=1;//开中断      

}      

/**********************T0中断服务程序*******************/

void timer0(void)interrupt 1 using 1

{

   static uchar count;

   TH0=(65536-50000)/256;

   TL0=(65536-50000)%256;

   count++;

   if(count==10)

     {

       if(Flag_SN_Yellow==1) //测试南北黄灯标志位

        {SN_Yellow=~SN_Yellow;}

       if(Flag_EW_Yellow==1)  //测试东西黄灯标志位

        {EW_Yellow=~EW_Yellow;}

     }



     if(count==20)

     {

       Time_EW--;

       Time_SN--;

       if(Flag_SN_Yellow==1)//测试南北黄灯标志位

        {SN_Yellow=~SN_Yellow;}

       if(Flag_EW_Yellow==1)//测试东西黄灯标志位

        {EW_Yellow=~EW_Yellow;}

       count=0;

     }            

}

主程序，利用中断调用，将不同的交通灯情况列成s1-s7情况

/*********************主程序开始**********************/

void main(void)

{

  IT0=1;      //INT0负跳变触发

  IT1=1;      //INT1负跳变触发

  TMOD=0x01;  //定时器工作于方式1

  TH0=(65536-50000)/256;//定时器赋初值50ms

  TL0=(65536-50000)%256;

  EA=1; //CPU开中断总允许

  ET0=1;//开定时中断

  EX0=1;//开外部INTO中断

  EX1=1;//开外部INT1中断

  TR0=1;//启动定时                           

  Flag_SN_Yellow=0;                 //SN关黄灯显示信号

  Flag_EW_Yellow=0;                 //EW关黄灯显示信号

  Time_EW=TZ;            

  Time_SN=TZ;

  P1=0xFF;

  while(Time_SN>=0)

   {

     EW_Red=0;              //全红灯3S   0xEE

     SN_Red=0;

     Display();

   }                                         

   while(1)               

    {   /*******S0状态**********/

         Flag_EW_Yellow=0; //EW关黄灯显示信号

              Time_EW=EW1;            

              Time_SN=SN1;



                  while(Time_SN>=0)

                   {P1=S[0];              //SN通行，EW红灯       0xBE

                    Display();}







       /*******S1状态**********/

              /*P1=0xFF;

              while(Time_SN>=0)

                  {Flag_SN_Yellow=1;              //SN开黄灯信号位

                   EW_Red=0;      //SN黄灯亮，等待左拐信号，EW红灯   0xDE

                   Display();}





              /*******S2状态**********/

              Flag_SN_Yellow=0; //SN关黄灯显示信号

              Time_SN=SNL;

              while(Time_SN>=3)

                 {P1=S[2];//SN左拐绿灯和直行绿灯亮，EW红灯       0x3E

                  Display();}







              /*******S3状态**********/

              P1=0xFF;

              while(Time_SN>=0)

                 {Flag_SN_Yellow=1;//SN开黄灯信号位

                  EW_Red=0;        //SN黄灯亮,等待停止信号，EW红灯    0xDE

                  Display();}





              /***********赋值**********/

              EW=EW1;

              SN=SN1;

              EWL=EWL1;

              SNL=SNL1;





              /*******S4状态**********/

              Flag_SN_Yellow=0;  //SN关黄灯显示信号

              Time_EW=SN;

              Time_SN=EW;

                while(Time_EW>=0)

            {P1=S[4];              //EW通行，SN红灯    0xEB

                   Display();}

                                                      

              /*******S5状态**********/

              /*P1=0XFF;

              while(Time_EW>=0)

                {Flag_EW_Yellow=1;//EW开黄灯信号位

                 SN_Red=0;//EW黄灯亮，等待左拐信号，SN红灯    0xED

                 Display();}

                                             

              /*******S6状态**********/

              Flag_EW_Yellow=0;//EW关黄灯显示信号           0xE3

              Time_EW=EWL;

              while(Time_EW>=3)

                 {P1=S[6];//EW左拐绿灯亮，SN红灯

                  Display();}

                                                      



              /*******S7状态**********/

              P1=0xFF;

              while(Time_EW>=0)

                {Flag_EW_Yellow=1; //EN开黄灯信号位

                 SN_Red=0;//EW黄灯亮，等待停止信号，SN红灯    0xE7            

                 Display();}

                                         

              /***********赋值**********/

              EW=EW1;

              SN=SN1;

              EWL=EWL1;

              SNL=SNL1;

   }

}
