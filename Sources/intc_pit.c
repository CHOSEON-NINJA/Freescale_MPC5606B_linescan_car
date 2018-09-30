/*
 *######################################################################
 *                                RAppIDJDP
 *           Rapid Application Initialization and Documentation Tool
 *                         Freescale Semiconductor Inc.
 *
 *######################################################################
 *
 * Project Name           : NEWPROJECT
 *
 * Project File           : NEWPROJECT.rsp
 *
 * Revision Number        : 1.0
 *
 * Tool Version           : 1.2.1.5
 *
 * file                   : intc_pit.c
 *
 * Target Compiler        : Codewarrior
 *
 * Target Part            : MPC5606B
 *
 * Part Errata Fixes      : none
 *
 * Project Last Save Date : 08-Feb-2018 17:16:35
 *
 * Created on Date        : 08-Feb-2018 17:16:35
 *
 * Brief Description      : This  file contains  the interrupt service routine  for the Periodic Interrupt Timer
 *
 ******************************************************************************** 
 *
 * Detail Description     : This file is generated when PIT(Periodic Interrupt
 *                         Timer) function is defined in INTC peripheral.This
 *                         file contains the Interrupt handlers routines for PIT.
 *                         In Interrupt handlers routine respective flags are cleared.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/
#include "UART_drv_api.h" //UART 통신 헤더파일
#include "UART_drv_cfg.h" 

#include "intc_pit.h"
#include "gpio_drv.h"

#include "sbc_hld.h"
#include "adc_drv.h" // ADC 헤더파일 포함
#include "gpio_drv.h"
#include "pot_hld.h"
#include "photo_sensor_hld.h"
#include "freemaster.h"
#include "UART_drv_api.h" //UART 통신 헤더파일
#include "UART_drv_cfg.h" 
#include "emios_init.h" // EMIOS 헤더파일 
#include "math.h"

#define DC_STEP		6400
#define MIN_DC		6400
#define MAX_DC		64000

void binary();
void getway();
void LKAS(float);
void SetServo(float);
void SetDC(float);
void delay(int);
void camera(void);
void infrared(void);
void encoder(void);
//----------------camera
unsigned char ca_data[128]; //data for camera
unsigned char ca_threshold[128];
unsigned char ca_binary[128];
//----------------infrared
int distance;
int distance_LPF=0;
//----------------encoder
int ec_r=0;	// rotation
int ec_ct=0; // 2000(ec_ct) = 1(ec_r)
int ec_angle=0;	//angle
//----------------
unsigned char tx_data[262];
int lane_left;
int lane_right;
float servo_duty;
int time;



/************************* INTERRUPT HANDLERS ************************/

void PIT_CH0_ISR (void)
{
	time++;
    PIT.CH[0].TFLG.R = 0x00000001;
    
    binary();
	getway();
	LKAS((lane_left+lane_right)/2);
}
void PIT_CH1_ISR (void){ //Camera 20ms 마다 실행
	
	//PIT.CH[1].TFLG.R = 0x00000001;
	PIT.CH[1].TFLG.B.TIF = 1;
	
	camera();
	
}

void delay(int j){
	int i;
    for(i=0;i<j;i++){ 
	   
    }
}

void camera(void){
	
   	uint8_t j;
   	
   	GPIO_SetState(70,1); //PE6 70=SI, PE5 69=CLK
   	delay(200);
   	GPIO_SetState(69,1);
   	delay(200);
   	GPIO_SetState(70,0);
   	delay(200);
   	GPIO_SetState(69,0);
   	for(j=0;j<128;j++){
   		
   		delay(400);
   		GPIO_SetState(69,1);
   		delay(400);
   		GPIO_SetState(69,0); 
   		ca_data[j]=A2D_GetSingleCh_10bit(0)/4; // PB4
   		tx_data[j]=ca_data[j];
   	}
   }
   
   void infrared(void){
   	distance=A2D_GetSingleCh_10bit(2)/4; //PB6
  /* 	if(distance_LPF==0){
   		distance_LPF=distance;
   	}else{
   		distance_LPF=0.9*distance_LPF+0.1*distance;
   	}
   	*/
 //  	tx_data[128]=distance;//distance_LPF;
   }

   void LKAS(float way){
   	
   	
   	
//   	integral=time_integral*0.001*(way-61)+integral;
//   	time_integral=0;
   	
   	servo_duty=8.5+(way-61)*0.2;//+integral*3;
   	
   	
   	if(servo_duty>10.5){
   		servo_duty=10.5;
   	}
   	if(servo_duty<6.5){
   		servo_duty=6.5;
   	}
   	
   	SetServo(servo_duty);
   	
   	tx_data[258]=servo_duty;
 
   }
   
   void binary(void){
   	
   	int i,j=0;
   	int temp=0;
   	int temp1=0;
   	int temp2=0;

   	
   	for(i=0;i<15;i++){
   			ca_binary[i]=1;
   			ca_binary[127-i]=1;
   	}
   	for(i=15;i<113;i++){
   		if((ca_data[i-3]+ca_data[i-2]+ca_data[i-1]+ca_data[i+1]+ca_data[i+2]+ca_data[i+3])/6-30>ca_data[i]){
   			ca_binary[i]=0;
   		}else if((ca_data[i-3]+ca_data[i-2]+ca_data[i-1]+ca_data[i+1]+ca_data[i+2]+ca_data[i+3])/6-30<=ca_data[i]){
   			ca_binary[i]=1;
   		}
   	}
   	for(i=0;i<128;i++){
   		tx_data[128+i]=ca_binary[i];
   	}
   	
   	
   	
   }
   
   void getway(void){
   	int i;
   	int left_flag,right_flag=0;
   	int left_gap,right_gap=0;
   	
   	
   	
   	for(i=0;i<25;i++){
   		if(lane_left+i<=128){
   			if(ca_binary[(lane_left)+i]==0){
   				lane_left=lane_left+i;
   				left_gap=i;
   				left_flag=1;
   				break;
   			}
   		}
   		if(lane_left-i>=0){
   			if(ca_binary[(lane_left)-i]==0){
   				lane_left=lane_left-i;
   				left_gap=i*(-1);
   				left_flag=1;
   				break;
   			}
   		}
   	}
   	
   	for(i=0;i<25;i++){
   		if(lane_right+i<=128){
   			if(ca_binary[(lane_right)+i]==0){
   				lane_right=lane_right+i;
   				right_gap=i;
   				right_flag=1;
   				break;
   			}
   		}
   		if(lane_right-i>=0){
   			if(ca_binary[(lane_right)-i]==0){
   			
   				lane_right=lane_right-i;
   				right_gap=i*(-1);
   				right_flag=1;
   				break;
   			}
   		}	
   	}
   	
   	if(left_flag==0&&right_flag==1){
   		
   		lane_left=lane_left+right_gap;
   		
   	}else if(left_flag==1&&right_flag==0){
   		
   		lane_right=lane_right+left_gap;
   	}
   	
   	if(lane_left==lane_right){
   		lane_left=20;
   		lane_right=102;
   	}
   	
   	tx_data[256]=lane_left;//130번째 데이터
   	tx_data[257]=lane_right;//131번째 데이터
   	tx_data[259]=left_flag; //DATA(1,260)
   	tx_data[260]=right_flag; //DATA(1,261)
       	
   }
  
	void SetServo(float duty){
		
	   EMIOS_0.CH[23].CADR.R=640*duty; // PE7
	   // 5%duty=-90degree, 7.5%duty=0degree, 10%duty=90degree
	}
   void SetDC(float duty){
   	
      if(duty>=0){//시계방향
   	   
         EMIOS_1.CH[16].CADR.R = 640*(duty); //input 1, PWM 제어, PG7
  //       GPIO_SetState(30,1); // EN1, 항상 high
         GPIO_SetState(65,1); // input2,  PB 15
      }else if(duty<0){
   	   
         EMIOS_1.CH[16].CADR.R = 640*(-duty); //input 1, PWM 제어, PG7
   //      GPIO_SetState(30,1); // EN1, 항상 high 
         GPIO_SetState(65,0); // input2
      }
    }

  void encoder(void){
	   
	   if(SIU.ISR.B.EIF5 == 1)  // Interrupt Status Flag Register
	  	    {
	  		
	  		   if(GPIO_GetState(34)==1){
	  			   if(GPIO_GetState(35)==1){
	  				   ec_ct++;
	  			   }else if(GPIO_GetState(34)==0){
	  				   ec_ct--;
	  			   }
	  		   }else if(GPIO_GetState(34)==0){
	  			   if(GPIO_GetState(35)==0){
	  				   ec_ct++;
	  			   }else if(GPIO_GetState(35)==1){
	  				   ec_ct--;
	  			   }
	  		   }
	  	//	   TX_flag=1;
	  		   SIU.ISR.B.EIF5 = 1; // Clear the status
	  		           
	  	   } 
	  	   else if(SIU.ISR.B.EIF6 == 1)
	  	   {
	  		   if(GPIO_GetState(34)==1){
	  			   if(GPIO_GetState(35)==0){
	  				   ec_ct++;
	  			   }else if(GPIO_GetState(35)==1){
	  				   ec_ct--;
	  			   }
	  		   }else if(GPIO_GetState(34)==0){
	  			   if(GPIO_GetState(35)==1){
	  				   ec_ct++;
	  			   }else if(GPIO_GetState(35)==0){
	  				   ec_ct--;
	  			   }
	  		   }   
	  	//	   TX_flag=1;
	  		   SIU.ISR.B.EIF6=1;

	   }
	
	
  }
/*void PIT_CH2_ISR (void){
	
}*/

 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

