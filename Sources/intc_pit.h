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
 * file                   : intc_pit.h
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
 *
 *######################################################################
*/

#ifndef  _INTC_PIT_H
#define  _INTC_PIT_H
/********************  Dependent Include files here **********************/

#include "jdp.h"

/**********************  Function Prototype here *************************/

void PIT_CH0_ISR(void);
void PIT_CH1_ISR(void);

#endif  /*_INTC_PIT_H*/

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

