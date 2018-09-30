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
 * file                   : intc_linflex.c
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
 * Brief Description      : This  file contains  the interrupt service routine  for the Linflex
 *
 ******************************************************************************** 
 *
 * Detail Description     : This file is generated when Linflex function is
 *                         defined in INTC peripheral.This file contains the
 *                         Interrupt handlers routines for Linflex. In Interrupt
 *                         handlers routine respective flags are cleared.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "intc_linflex.h"
#include "UART_drv_api.h" //UART ≈ÎΩ≈ «Ï¥ı∆ƒ¿œ
#include "UART_drv_cfg.h" //

//unsigned char linflex_data[7]; // 0=start, 1~4=data, 5=end, 6=NULL √— 7∞≥
unsigned char linflex_data[9];	//0=start(97), 1~5=data, 6=function(31or32), 7=end(98), 8=NULL √— 9∞≥
//unsigned char linflex_data[11]; // 0=start, 1~8=data, 9=end, 10=NULL
int linflex_cnt=0;
int linflex_status=0;
/*
int linflex_a=0;
int linflex_b=0;
int linflex_c=0;
int linflex_d=0;
int linflex_e=0;
int linflex_f=0;
*/
int RX_flag;
double tem1,tem2,tem3,tem4,tem5,tem6,tem7,tem8=0;
double linflex_output;
//const linflex_size = 6; //data byte = 5byte, function byte 1byte. total = 6 byte
/************************* INTERRUPT HANDLERS ************************/

void TEST_UART (void)
{

	
	LINFLEX_0.LINSR.R = 0xF23F;
	
//    linflex_a++;
    linflex_data[linflex_cnt]=(uint8_t)LINFLEX_0.BDRM.B.DATA4;
    if(linflex_cnt==0){ //if it is first byte,
    	switch(linflex_data[linflex_cnt]){
    	case 97:{//start byte
    		linflex_status=1;//status on
    		linflex_cnt++;//byte count++
//   		linflex_b++;
    		}break;
    		
    	default:{//if it is not start byte, clear status & count
    		linflex_status=0;
    		linflex_cnt=0;
//    		linflex_c++;
    		}break;
    		}//switch end   		
    	}//linflex_cnt==0 end
    else if(linflex_cnt!=0){ //if it is not first byte,
    	if(linflex_status==1){//& status on,
    		switch(linflex_data[linflex_cnt]){
    		case 98:{//end byte
    			linflex_status=0;
    			linflex_cnt=0;//clear status & count 
 //   			linflex_d++;
    			RX_flag=1;
    			}break;
    			default:{//if it is not end byte
    			linflex_cnt++;//byte count++
 //   			linflex_e++;
    			}break;
    			}//end switch
    		}//linflex_cnt==1 end
    		else if(linflex_status==0){ //if status off
    			linflex_status=0;
    			linflex_cnt=0;//clear status & count
 //   			linflex_f++;
    		}
    }//linflex_cnt!=0 end
    
    tem1=linflex_data[0];
    tem2=linflex_data[1];
    tem3=linflex_data[2];
    tem4=linflex_data[3];
    tem5=linflex_data[4];
    tem6=linflex_data[5];
    tem7=linflex_data[6];
    tem8=linflex_data[7];
    
    LINFLEX_0.UARTSR.R = 0x0204; //Clear Data Reception Completed Flag & Release Message Buffer
    //   LINFLEX_0.UARTSR.B.DRF = 1; // Clear Data Reception Completed Flag
    //   LINFLEX_0.UARTSR.B.RMB = 1; // Clear Release Message Buffer 
    }//TEST_UART end
 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

