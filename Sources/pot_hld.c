/*
 *########################################################################
 *                (c) Copyright 2013 Freescale Semiconductor, Inc.
 *                         ALL RIGHTS RESERVED. 
 *########################################################################
 * 
 * Brief Description     : Potentiometer high level driver file
 *
 **************************************************************************
*/
/******************************************************************************
* Includes
******************************************************************************/
#include "pot_hld.h"
#include "adc_drv.h"

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Macros 
******************************************************************************/
#define POT_CHANNEL	0

/******************************************************************************
* Types
******************************************************************************/

/******************************************************************************
* Local Functions
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Static variables
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

/******************************************************************************
*   Function: Init_SBC_DBG
*
*   Description: Returns ADC value of Potentiometer channel 
*
******************************************************************************/
uint16_t Pot_Get_Value(void)
{
	return (A2D_GetSingleCh_10bit(POT_CHANNEL));
}
 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/



