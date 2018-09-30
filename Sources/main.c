/*
 *######################################################################
 *                   
             RAppIDJDP
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
 * file                   : main.c
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
 * Brief Description      : This file contains main() function call.
 *
 ******************************************************************************** 
 *
 * Detail Description     : This file contains main() routine which calls system
 *                         initialization routine and interrupt enable routine if defined.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/
#include "stdio.h"
#include "rappid_ref.h"
#include "rappid_utils.h"
#include "sys_init.h"
// 여기서부터 추가
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

/**********************  Function Prototype here *************************/

void main(void);
void init_EMIOS(void);
void init_ENCODER(void);
void init_UART(void);
void TEST_UART(void);
void SetServo(float);
void SetDC(float);
void TaskPriority(void); // Priority : 0x1111(highest) ~ 0x0000(lowest)

extern time;
extern double linflex_output;
extern unsigned char linflex_data[9];
extern unsigned char tx_data[262];
extern linflex_size;
extern RX_flag;
extern lane_left;
extern lane_right;

uint8_t TX_flag;
uint8_t time_flag;

int test1,test2,test3,test4,test5;
/* 
 * PIN CONNECTION
 * 
 * 
 */
void main(void){
/* ----------------------------------------------------------- */
/*	             System Initialization Function                  */
/* ----------------------------------------------------------- */
   sys_init_fnc();
   EnableExternalInterrupts();
   init_EMIOS(); //EMIOS_0.MCR.B.GPRE 및 EMIOS_0.MCR.B.GPREN 설정. 
   init_ENCODER();
   init_UART();
   TaskPriority();
   SetDC(13);
   lane_left=20;
   lane_right=102;
   
   while(1){
	   if(time==100){
		   time_flag=1;
		   time=0;
	   }
	   
	   if(RX_flag==1){
	   		int i;
//	   		test1++;
	   		if(linflex_data[1]==11){//Siginal to TX
	   			TX_flag=1;
	   			linflex_data[1]=0;
//	   			test2++;
	   			
	   		}
	   		else if(linflex_data[1]==31){//function byte 31=servo
	   			linflex_output=0;
	   			for(i=2;i<5;i++){ //i=2,3,4
	   				linflex_output = linflex_output + linflex_data[i]*(pow(10,3-i));
	   			}//linflex_out(return) decision
	   			SetServo(linflex_output);
	   		}
	   		else if(linflex_data[1]==32){
	   			linflex_output=0;
	   			for(i=2;i<5;i++){
	   				linflex_output = linflex_output + linflex_data[i]*(pow(10,3-i));
	   			}
	   			SetDC(linflex_output);
	   			
	   		}
	   		RX_flag=0;
	   }
	   
	  // if(TX_flag==1&&time_flag==1){
		if(time_flag==1){   
		   UartTxMsg(tx_data,262);
//		   test3++;
		   TX_flag=0;
		   time_flag=0;
	   }
	   
   } // while 
} // main
	void TaskPriority(void){ // Priority : 0x1111(highest) ~ 0x0000(lowest)

	INTC.PSR[59].R = 0x01; // 
	INTC.PSR[60].R = 0x02; // 
}

 
    void init_EMIOS(void){
    
    	EMIOS_0.MCR.B.GPRE = 19; // = prescaler , Divide ratio = prescaler + 1
    	EMIOS_0.MCR.B.GPREN = 1; // enable to use prescaler
    	
 //   	EMIOS_0.CH[17].CCR.R = 0x02000658; //PE[1] PCR[65] E0UC[17]
    	
//    	EMIOS_1.MCR.R = 0x14000000;
    	EMIOS_1.MCR.B.GPRE = 19; // = prescaler , Divide ratio = prescaler + 1
    	EMIOS_1.MCR.B.GPREN = 1; // enable to use prescaler
    	EMIOS_1.CH[16].CBDR.R = 0x0000FA00;
    	EMIOS_1.CH[16].CCR.R = 0x02000658;    	
//    	EMIOS_1.CH[30].CCR.R = 0x02000658; //PE[10] PCR[74] E0UC[30]
    }

    void init_UART(void){

    	LINFLEX_0.LINIER.B.DRIE = 1; //Data Reception Complete Interrupt Enable
    	}
    
    /*void init_proto(void){
    	
    	ec_data[0]=':';
    	ca_data[0]=';';
    }*/

      void init_ENCODER(void){
   	   
   	   SIU.PCR[34].R = 0x103;  //phase A : PC[2] , EIRQ[5]
   	   SIU.PCR[35].R = 0x103; //phase B : PC[3] , EIRQ[6]
   	      //SIU.PCR[8].R = 0x103;  //phase Z : PA[8] , EIRQ[2]
   	      
   	   SIU.IREER.B.IREE5 = 1;
   	   SIU.IREER.B.IREE6 = 1;
   	      //SIU.IREER.B.IREE2 = 1;
   	      //443 EIRQ16 17 -> PA11 PA12
   	   SIU.IFEER.B.IFEE5 = 1;
   	   SIU.IFEER.B.IFEE6 = 1;
   	      //SIU.IFEER.B.IFEE2 = 1;
   	      
   	   SIU.IRER.B.EIRE5 = 1;
   	   SIU.IRER.B.EIRE6 = 1;
   	      //SIU.IRER.B.EIRE2 = 1;
   	     
   	  INTC.PSR[41].R=2;

      }

  
   	
 
   	   
  

		
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/
