/*******************************************************************************
**  DAVE App Name : I2C001       App Version: 1.0.28               
**  This file is generated by DAVE, User modification to this file will be    **
**  overwritten at the next code generation.                                  **
*******************************************************************************/


/*CODE_BLOCK_BEGIN[I2C001.c]*/
/******************************************************************************
 Copyright (c) 2014, Infineon Technologies AG                                **
 All rights reserved.                                                        **
                                                                             **
 Redistribution and use in source and binary forms, with or without          **
 modification,are permitted provided that the following conditions are met:  **
                                                                             **
 *Redistributions of source code must retain the above copyright notice,     **
 this list of conditions and the following disclaimer.                       **
 *Redistributions in binary form must reproduce the above copyright notice,  **
 this list of conditions and the following disclaimer in the documentation   **
 and/or other materials provided with the distribution.                      **
 *Neither the name of the copyright holders nor the names of its contributors**
 may be used to endorse or promote products derived from this software       **
 without specific prior written permission.                                  **
                                                                             **
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" **
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   **
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE  **
 ARE  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE  **
 LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR        **
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF        **
 SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS   **
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN     **
 CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)      **
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE  **
 POSSIBILITY OF SUCH DAMAGE.                                                 **
                                                                             **
 To improve the quality of the software, users are encouraged to share       **
 modifications, enhancements or bug fixes with Infineon Technologies AG      **
 dave@infineon.com).                                                         **
                                                                             **
*******************************************************************************
**                                                                           **
**                                                                           **
** PLATFORM : Infineon XMC4000/XMC1000 Series                                **
**                                                                           **
** COMPILER : Compiler Independent                                           **
**                                                                           **
** AUTHOR   : App Developer                                                  **
**                                                                           **
** MAY BE CHANGED BY USER [yes/no]: Yes                                      **
**                                                                           **
** MODIFICATION DATE : Feb 07, 2014                                         **
**                                                                           **
******************************************************************************/
/******************************************************************************
**                      Author(s) Identity                                   **
*******************************************************************************
**                                                                           **
** Initials     Name                                                         **
** --------------------------------------------------------------------------**
** RP         App Developer                                                  **
******************************************************************************/     
/**
 * @file I2C001.c
 *
 * @App Version I2C001 <1.0.28>
 *
 * @brief  USIC_I2C_I2C001 App provides non reentrant APIs for higher level  
 *          Apps which can be used in a Non -RTOS environment. 
 *           Limitations of USIC_I2C_I2C001 App
 *           a) Read/Write functions are non reentrant.
 *           b) No support for DMA mode
 */
/*
 * Revision History
 * 15 Mar 2013 v1.0.22  Changes from 1.0.12 are
 *                       1. Modified as per coding guidelines and MISRA checks.
 *                       2. Modified the macros FUNCTION_ENTRY, FUNCTION_EXIT
 *                       and ERROR.
 *                       3. The hard coded values are replaced with macros in
 *                       I2C001_lConfigureBitRate() function.
 *                       4. The PCTQ value updation in BRG register is removed
 *                       as the value is taken as zero.
 *                       5. The input parameter of I2C001_WriteData() function
 *                       is modified to I2C001_Type from I2C001_DataType.
 *                       6. The input parameters are modified to I2CHandle from
 *                       Handle in all function comments.
 *                       7. Updated for Code optimisation.
 *                       8. Modified for the IO PDRx updation in I2C001_Init()
 *                       function for SDA pin selection as per UTP
 *                       MCSW300000609.
 *                       9. Updated for baud calculation in I2C001_lInit
 *                       function.
 *                       10. Modified for the I2C001_Init() function for
 *                       disabling the clock gate signal for XMC1000 devices.
 *                       11. Updated for the I2C001_Init() API for invoking
 *                       I2C001_lInit() function for USIC0.
 *                       12. Corrected for spelling in revision history.
 * 05 Jun 2013 v1.0.24   1. UTP MCSW300000612 (I2C001 wrong FIFO handling in 
 *                       I2C001_WriteData function) Fix - Removed TDV bit 
 *                       polling from "I2C001_WriteData()" API.
 *                       2. Removed TBIF bit configuration from "I2C001_lInit()" 
 *                       as polling is not used in the I2C001 App. 
 *                       3. Corrected SCTR.TRM configuration (changed from 01b 
 *                       to 11b) in "I2C001_lInit()".
 *                       4. The input parameter of I2C001_WriteData() function
 *                       is modified to I2C001_DataType from I2C001_Type to 
 *                       maintain backward compatibility. 
 * 26 Sep 2013 v1.0.26   1. In I2C001_Init(), SCL & SDA pin configuration 
 *                       modified from "Open-drain" to "Open-drain with 
 *                       General-purpose output" to avoid unintended spikes. 
 * 07 Feb 2014 v1.0.28   1. Removed DBG002_FUNCTION_ENTRY & 
 *                       DBG002_FUNCTION_EXIT calling from APIs as using these
 *                       macros is highly discouraged.                                                           
 */
/******************************************************************************
 ** INCLUDE FILES                                                            **
 *****************************************************************************/
#include <DAVE3.h>                

/******************************************************************************
**                      Private Macro Definitions                            **
******************************************************************************/
  /* System Core clock frequency in MHz */
#define USIC_CLOCK                     120.0F 

/* I2C001 Bit rate*/
#define I2C001_BITRATE                 100U

/* Frame Length :  64 Bits */
#define I2C001_SCTR_FLE                (0x3FU)
/* Word Length :  8 Bits */
#define I2C001_SCTR_WLE                (0x07U)
/* Direction set, msb transmitted or received first */
#define I2C001_SCTR_SDIR               (0x01U)
/* Enable I2C mode */
#define I2C_ENABLE                     (0x4U)

#define I2C001_MAX_VALUE               1023.0F

#define I2C001_MAX_STEPVALUE           1024.0F

/* Intermediate step value to calculate Pdiv */
#define I2C001_IM_STEPVALUE            500.0F 

/* 10 timequanta DCTQ value */
#define I2C_DCTQ1_VALUE                (0x09UL)

/* 25 timequanta DCTQ value */
#define I2C_DCTQ2_VALUE                (0x18UL)

/******************************************************************************
**                      Private Type Definitions                             **
******************************************************************************/


/******************************************************************************
**                 Private Function Declarations:
******************************************************************************/

void I2C001_lInit (const I2C001Handle_type* I2CHandle);
/* Give optimized PDIV and STEP value for the given bit rate */ 
void I2C001_lConfigureBitRate\
                   (uint32_t BitRate,uint32_t* PDivValue,uint32_t* StepValue);
/******************************************************************************
**                      Global Constant Definitions                          **
******************************************************************************/

/******************************************************************************
**                      Global Variable Definitions                          **
******************************************************************************/

/******************************************************************************
**                      Private Constant Definitions                         **
******************************************************************************/

/******************************************************************************
**                 Function like macro definitions                           **
******************************************************************************/
/******************************************************************************
**                      Private Function Definitions                         **
******************************************************************************/

/******************************************************************************
**                      Public Function Definitions                          **
******************************************************************************/
 /* Initializes the App based on User provide configuration. */
 void I2C001_lInit (const I2C001Handle_type* I2CHandle)
{ 
   uint32_t Brg_PDivValue = 0x00U;
   uint32_t Fdr_StepValue = 0x00U;  
   USIC_CH_TypeDef* I2CRegs = I2CHandle->I2CRegs;

   /* <<<DD_I2C001_API_1>>>*/

   /** I2C initialization  */
   /* Disable I2C mode before configuring all USIC registers to avoid unintended edges */ 
   I2CRegs->CCR &= ~( ((uint32_t)(I2C_ENABLE  & USIC_CH_CCR_MODE_Msk)));

   /* Enable the USIC Channel */
   I2CRegs->KSCFG |= (((uint32_t) 0x01 & USIC_CH_KSCFG_MODEN_Msk)) | \
    (((uint32_t)0x01 << USIC_CH_KSCFG_BPMODEN_Pos));
    
   /* Get the optimum PDIV and STEP value for the given bitrate */
   I2C001_lConfigureBitRate(I2CHandle->BitRate,&Brg_PDivValue,&Fdr_StepValue);
   /* Configuration of USIC Channel Fractional Divider */

   /* Fractional divider mode selected */
   I2CRegs->FDR |= \
           ((((uint32_t)USIC_FRACTIONAL_DIV_SEL << USIC_CH_FDR_DM_Pos) & \
             USIC_CH_FDR_DM_Msk) | \
            (((uint32_t)Fdr_StepValue << USIC_CH_FDR_STEP_Pos) & \
            USIC_CH_FDR_STEP_Msk));
   
           
   if(I2CHandle->BitRate <= I2C001_BITRATE)
   {  
     /* The PreDivider for CTQ, PCTQ = 0  */
     /* The Denominator for CTQ, DCTQ = 9 */
     I2CRegs->BRG |= ((I2C_DCTQ1_VALUE <<  USIC_CH_BRG_DCTQ_Pos) | \
                      (((uint32_t)Brg_PDivValue << USIC_CH_BRG_PDIV_Pos) & \
                        USIC_CH_BRG_PDIV_Msk)); 
   }
   else
   {
     /* The PreDivider for CTQ, PCTQ = 0  */
     /* The Denominator for CTQ, DCTQ = 24 */
     I2CRegs->BRG |= ((I2C_DCTQ2_VALUE << USIC_CH_BRG_DCTQ_Pos) | \
                      (((uint32_t)Brg_PDivValue << USIC_CH_BRG_PDIV_Pos) & \
                        USIC_CH_BRG_PDIV_Msk));
   }

   
   /* Configuration of USIC Shift Control */
   
   /* Transmit/Receive MSB first is selected  */
   /* Transmission Mode (TRM) = 3  */
   /* Passive Data Level (PDL) = 1 */
   /* Frame Length (FLE) = 63 (3F) */
   /* Word Length (WLE) = 7  */

   I2CRegs->SCTR |=  \
               ((((uint32_t)I2C001_SCTR_SDIR << USIC_CH_SCTR_SDIR_Pos)&  \
                  USIC_CH_SCTR_SDIR_Msk) | \
                ((uint32_t)(0x01) << USIC_CH_SCTR_PDL_Pos ) | \
                ((uint32_t)(0x03) <<  USIC_CH_SCTR_TRM_Pos) |
                (((uint32_t)I2C001_SCTR_FLE  << USIC_CH_SCTR_FLE_Pos) & \
                  USIC_CH_SCTR_FLE_Msk ) | \
                (((uint32_t)I2C001_SCTR_WLE << USIC_CH_SCTR_WLE_Pos) & \
                  USIC_CH_SCTR_WLE_Msk));

       
   /* Configuration of USIC Transmit Control/Status Register */ 
   /* TBUF Data Enable (TDEN) = 1 */
   /* TBUF Data Single Shot Mode (TDSSM) = 1 */     
   I2CRegs->TCSR |= ((((uint32_t)0x01 << USIC_CH_TCSR_TDEN_Pos) & \
     USIC_CH_TCSR_TDEN_Msk) | \
     (((uint32_t)0x01 << USIC_CH_TCSR_TDSSM_Pos) & USIC_CH_TCSR_TDSSM_Msk));

   /* Configuration of Protocol Control Register */ 
   /* Symbol timing = 10 time quanta */
   /* The start condition interrupt is enabled */
   /* The repeated start condition interrupt is enabled */
   /* The stop condition interrupt is enabled */
   /* Non-Acknowledge interrupt enabled */
   /* Arbitration Lost interrupt enabled */
   /* Slave read request interrupt enabled */
   /* Error interrupt enabled */
   /* Configuration of Protocol Control Register */ 
   if(I2CHandle->BitRate <= I2C001_BITRATE)
   {
	   /* Symbol timing = 10 time quanta */
	   I2CRegs->PCR_IICMode |= (((((uint32_t)0x0U << \
	                             USIC_CH_PCR_IICMode_STIM_Pos)) & \
			                         (uint32_t)USIC_CH_PCR_IICMode_STIM_Msk));
   }
   else
   {
	   /* Symbol timing = 25 time quanta */
  	 I2CRegs->PCR_IICMode |= (((((uint32_t)0x1U << \
                            	USIC_CH_PCR_IICMode_STIM_Pos)) & \
                        			(uint32_t)USIC_CH_PCR_IICMode_STIM_Msk));
   }

   if(I2CHandle->StartCondRecvIntEn)
   {
     /* Enable Start Condition Recv Interrupt*/  
     I2CRegs->PCR_IICMode |=(((uint32_t)0x01  << USIC_CH_PCR_CTR18_Pos) & \
    		  	  	  	  	  USIC_CH_PCR_CTR18_Msk); 
   }
      
   if(I2CHandle->RepStartCondRecvIntEn)
   {
    	/* Enable Repeated Start Condition Interrupt*/  
      I2CRegs->PCR_IICMode |=(((uint32_t)0x01 << USIC_CH_PCR_CTR19_Pos) & \
    		  	  	  	  	  USIC_CH_PCR_CTR19_Msk); 
   }
     
   if(I2CHandle->StopCondRecvIntEn)
   {
     	/* Enable Stop Condition Interrupt*/  
      I2CRegs->PCR_IICMode |=(((uint32_t)0x01  << USIC_CH_PCR_CTR20_Pos) & \
    		  	  	  	  	  USIC_CH_PCR_CTR20_Msk); 
   }
      
   if(I2CHandle->NackDetectIntEn)
   {
     	/* Enable Non Ack Interrupt */  
      I2CRegs->PCR_IICMode |=(((uint32_t)0x01  << USIC_CH_PCR_CTR21_Pos) & \
    		  	  	  	  	  USIC_CH_PCR_CTR21_Msk); 
   }
      
   if(I2CHandle->ArbLostIntEn)
   {
    	/* Enable Arbitration lost Interrupt */  
      I2CRegs->PCR_IICMode |=(((uint32_t)0x01  << USIC_CH_PCR_CTR22_Pos) & \
   		  	  	  	  	  USIC_CH_PCR_CTR22_Msk); 
   }
      
   if(I2CHandle->ErrorIntEn)
   {
    	/* Enable IIC Error Interrupt */ 
      I2CRegs->PCR_IICMode |=(((uint32_t)0x01  << USIC_CH_PCR_CTR24_Pos) & \
   		  	  	  	  	  USIC_CH_PCR_CTR24_Msk); 
   }
      
   if(I2CHandle->AckIntEn)
   {
    	/*  Enable Ack  Interrupt */ 
      I2CRegs->PCR_IICMode |=(((uint32_t)0x01  << USIC_CH_PCR_CTR30_Pos) & \
   		  	  	  	  	  USIC_CH_PCR_CTR30_Msk); 
   }					          
   
   /** FIFO Configuration */
       
   /* Configuration of Transmitter Buffer Control Register */ 
   /* Limit for transmit FIFO interrupt generation is set based on UI */

   I2CRegs->TBCTR |= \
             ((((uint32_t)I2CHandle->TxLimit << USIC_CH_TBCTR_LIMIT_Pos ) & \
	 			                                          USIC_CH_TBCTR_LIMIT_Msk));
        
   /* Configuration of Receiver Buffer Control Register */ 
   /* Limit for receive FIFO interrupt generation is set based on UI*/
   /* Filling level mode is selected */
   I2CRegs->RBCTR |= \
               ((((uint32_t)I2CHandle->RxLimit << USIC_CH_RBCTR_LIMIT_Pos) & \
	 			         USIC_CH_RBCTR_LIMIT_Msk) |  \
                (((uint32_t)0x01 << USIC_CH_RBCTR_LOF_Pos) & \
                USIC_CH_RBCTR_LOF_Msk));
   I2CRegs->CCR |= ((I2C_ENABLE) & USIC_CH_CCR_MODE_Msk);
}


/* Give optimized PDIV and STEP value for the given baud rate */ 
void I2C001_lConfigureBitRate\
                    (uint32_t BitRate,uint32_t* PDivValue,uint32_t* StepValue)
{
  float ratio = (float)0.0;
  float TempBitrate = ((float)BitRate * ((float)1000));
  float TempPdiv = (float)0.0;
  float TempStep = (float)0.0;

  if(TempBitrate <= 100000.0)
  {
    ratio = ((((USIC_CLOCK * (float)1000000)/ \
             (((float)1 + (float)I2C_DCTQ1_VALUE)*I2C001_MAX_STEPVALUE)) * \
             I2C001_IM_STEPVALUE)/TempBitrate);
  }
  else
  {
    ratio = ((((USIC_CLOCK * (float)1000000)/ \
             (((float)1 + (float)I2C_DCTQ2_VALUE)*I2C001_MAX_STEPVALUE)) * \
             I2C001_IM_STEPVALUE)/TempBitrate);
  }
  /* Calculating optimum PDIV value */
  if( ratio <= (float)1)
  {
    TempPdiv = (float)0;
  }
  else
  {
    TempPdiv =  ratio - (float)1;
    if( TempPdiv > I2C001_MAX_VALUE)
    {
  	  TempPdiv = I2C001_MAX_VALUE;
    }
  }
  *PDivValue = (uint32_t)(TempPdiv);
  /* Calculating STEP value */
  if(TempBitrate <= 100000.0)
  {
    TempStep = (((((TempBitrate * ((float)1 + (float)I2C_DCTQ1_VALUE) * \
                 I2C001_MAX_STEPVALUE)/(float)1000000)) * \
	               ((float)1+ ((float)*PDivValue)))/(USIC_CLOCK));
  }
  else
  {
    TempStep = (((((TempBitrate * ((float)1 + (float)I2C_DCTQ2_VALUE) * \
                 I2C001_MAX_STEPVALUE)/ (float)1000000)) * \
	               ((float)1+ ((float)*PDivValue)))/(USIC_CLOCK));
  }
  if(TempStep > I2C001_MAX_VALUE)
  {
	  TempStep = I2C001_MAX_VALUE;
  }
  *StepValue = (uint32_t)(TempStep);
}
/******************************************************************************
**                      Public Function Definitions                          **
******************************************************************************/
void I2C001_Init(void)
{
  /* Reset the Peripheral*/
  RESET001_DeassertReset(PER1_USIC2); 
  I2C001_lInit(&I2C001_Handle0);          
     /* Configuration of SCL Pin 5.2 based on User configuration */
  PORT5->PDR0  &= (~(PORT5_PDR0_PD2_Msk));
  PORT5->PDR0  |= (((uint32_t)4 << PORT5_PDR0_PD2_Pos) & PORT5_PDR0_PD2_Msk);       
  PORT5->IOCR0 |= ((uint32_t)24 << 19);
 	 
  PORT5->OMR |= ((uint32_t)0x01 << 2);          
     /* Configuration of SDA Pin 5.0 based on User configuration */
  PORT5->PDR0  &= (~(PORT5_PDR0_PD0_Msk));
  PORT5->PDR0  |= (((uint32_t)4 << PORT5_PDR0_PD0_Pos) & PORT5_PDR0_PD0_Msk);
 	
 	
  PORT5->IOCR0 |= ((uint32_t)24 << 3);
 	 
  PORT5->OMR |= ((uint32_t)0x01 << 0);

  RESET001_DeassertReset(PER0_USIC0);
  I2C001_lInit(&I2C001_Handle1);        
     /* Configuration of SCL Pin 0.8 based on User configuration */
  PORT0->PDR1 &= (~(PORT0_PDR1_PD8_Msk));
  PORT0->PDR1 |= (((uint32_t)0 << PORT0_PDR1_PD8_Pos) & PORT0_PDR1_PD8_Msk);    
 	       
  PORT0->IOCR8 |= ((uint32_t)24 << 3);
 	 
  PORT0->OMR |= ((uint32_t)0x01 << 8);          
     /* Configuration of SDA Pin 1.5 based on User configuration */
  PORT1->PDR0  &= (~(PORT1_PDR0_PD5_Msk));
  PORT1->PDR0  |= (((uint32_t)0 << PORT1_PDR0_PD5_Pos) & PORT1_PDR0_PD5_Msk);
 	
 	
  PORT1->IOCR4 |= ((uint32_t)24 << 11);
 	 
  PORT1->OMR |= ((uint32_t)0x01 << 5);

  /* Reset the Peripheral*/
  RESET001_DeassertReset(PER1_USIC1); 
  I2C001_lInit(&I2C001_Handle2);          
     /* Configuration of SCL Pin 0.11 based on User configuration */
  PORT0->PDR1 &= (~(PORT0_PDR1_PD11_Msk));
  PORT0->PDR1 |= (((uint32_t)0 << PORT0_PDR1_PD11_Pos) & PORT0_PDR1_PD11_Msk);    
 	       
  PORT0->IOCR8 |= ((uint32_t)24 << 27);
 	 
  PORT0->OMR |= ((uint32_t)0x01 << 11);          
     /* Configuration of SDA Pin 0.5 based on User configuration */
  PORT0->PDR0  &= (~(PORT0_PDR0_PD5_Msk));
  PORT0->PDR0  |= (((uint32_t)0 << PORT0_PDR0_PD5_Pos) & PORT0_PDR0_PD5_Msk);
 	
 	
  PORT0->IOCR4 |= ((uint32_t)24 << 11);
 	 
  PORT0->OMR |= ((uint32_t)0x01 << 5);

}
 /* Function provide to reset the App to default values. */
 
void  I2C001_DeInit (void)
{
  /* <<<DD_I2C001_API_2>>>*/
  /* Redundant Api */
}


/* Function which allows changing of bitrate at run time.*/
status_t  I2C001_Configure(const I2C001Handle_type* I2CHandle,uint32_t BitRate)
{
  uint32_t Brg_PDivValue = 0x00U;
  uint32_t Fdr_StepValue = 0x00U;
  USIC_CH_TypeDef* I2CRegs = I2CHandle->I2CRegs;
  status_t Status = (status_t)I2C001_FAIL; 
 
  /* <<<DD_I2C001_API_3>>>*/    
  /* Disable I2C mode before configuring all USIC registers to avoid unintended edges */ 
  I2CRegs->CCR &= ~( ((uint32_t)(I2C_ENABLE  & USIC_CH_CCR_MODE_Msk)));

  /* Configuration of USIC Channel Fractional Divider */
  /* Get the optimum PDIV and STEP value for the given bitrate */
  I2C001_lConfigureBitRate(BitRate,&Brg_PDivValue,&Fdr_StepValue);    
      
  /* Fractional divider mode selected */
  I2CRegs->FDR &= ~(USIC_CH_FDR_STEP_Msk);
  I2CRegs->FDR |= (Fdr_StepValue & USIC_CH_FDR_STEP_Msk);
              
  /* Configure BitRate */
  I2CRegs->BRG &= ~(USIC_CH_BRG_PDIV_Msk | USIC_CH_BRG_DCTQ_Msk | \
	                    USIC_CH_BRG_PCTQ_Msk);
    		                           
  if(BitRate <= I2C001_BITRATE)
  {  
     /* The PreDivider for CTQ, PCTQ = 0  */
     /* The Denominator for CTQ, DCTQ = 9 */
     I2CRegs->BRG |= ((I2C_DCTQ1_VALUE <<  USIC_CH_BRG_DCTQ_Pos) | \
      (((uint32_t)Brg_PDivValue  <<  USIC_CH_BRG_PDIV_Pos) & \
       USIC_CH_BRG_PDIV_Msk)); 
  }
  else
  {
     /* The PreDivider for CTQ, PCTQ = 0  */
     /* The Denominator for CTQ, DCTQ = 24 */
     I2CRegs->BRG |= ((I2C_DCTQ2_VALUE <<  USIC_CH_BRG_DCTQ_Pos) | \
      (((uint32_t)Brg_PDivValue  <<  USIC_CH_BRG_PDIV_Pos) & \
        USIC_CH_BRG_PDIV_Msk));
  }
      	 
  I2CRegs->PCR_IICMode &= ~(USIC_CH_PCR_IICMode_STIM_Msk);    
  /* Configuration of Protocol Control Register */ 
  if(BitRate <= I2C001_BITRATE)
  {
     /* Symbol timing = 10 time quanta */
     I2CRegs->PCR_IICMode |= \
              (((((uint32_t)0x0 << USIC_CH_PCR_IICMode_STIM_Pos)) & \
                                            USIC_CH_PCR_IICMode_STIM_Msk)); 
  }
  else
  {
     /* Symbol timing = 25 time quanta */
     I2CRegs->PCR_IICMode |= \
                 (((((uint32_t)0x1 << USIC_CH_PCR_IICMode_STIM_Pos)) & \
                                             USIC_CH_PCR_IICMode_STIM_Msk));  
  }
      	                            
  Status = (status_t)DAVEApp_SUCCESS;
  /* Enable I2C mode */
  I2CRegs->CCR |= ((I2C_ENABLE) & USIC_CH_CCR_MODE_Msk);
  
  return Status;
}


/* This function reads out  the content of the USIC receive FIFO Buffer. 
 * Returns true in case FIFO is not empty.else  otherwise.
 *
 */
bool I2C001_ReadData(const I2C001Handle_type* I2CHandle, uint16_t* buffer)
{ 
  bool Result = (bool)FALSE;
  USIC_CH_TypeDef* I2CRegs = I2CHandle->I2CRegs;
  
  /* <<<DD_I2C001_API_4>>>*/
  if(USIC_ubIsRxFIFOempty(I2CRegs))
  {
    Result = (bool)FALSE;
  }
  else
  {
    *buffer = (uint8_t)I2CRegs->OUTR;
    Result = (bool)TRUE;
  }
  
  return Result;
}

/* This function writes out a word to the USIC FIFO transmit buffer register.
 * Returns true in case if the FIFO is not full else otherwise.
 *
 */
bool I2C001_WriteData \
                   (const I2C001Handle_type* I2CHandle,const I2C001_DataType* Data)
{
  bool Result = (bool)FALSE;
  USIC_CH_TypeDef* I2CRegs = I2CHandle->I2CRegs;
  
  /* <<<DD_I2C001_API_5>>>*/
  do
  {
     if(I2CRegs->PSR_IICMode & USIC_CH_PSR_IICMode_WTDF_Msk)
     {
	     Result = (bool)FALSE;
       break;
     }

     if(USIC_IsTxFIFOfull(I2CRegs))
     {
       Result = (bool)FALSE;
     }

     else
     {
       I2CRegs->IN[0] = \
                    (((uint32_t)Data->Data1.TDF_Type << 8) | Data->Data1.Data);
       Result = (bool)TRUE;     
     }
  
  }while(0);
  return Result;

}
/* Checks the specified Flag status bit.
 * The function is used to get the flag status.
 * It returns I2C_SET if the corresponding flag is set else otherwise.
 *
 * */

status_t I2C001_GetFlagStatus \
               (const I2C001Handle_type* I2CHandle,I2C001_FlagStatusType Flag)
{
  status_t Status = (status_t)I2C001_RESET;
  uint32_t TempValue = 0x00U;
  USIC_CH_TypeDef* I2CRegs = I2CHandle->I2CRegs;
  
  /* <<<DD_I2C001_API_6>>>*/
  if(Flag <= I2C001_FLAG_RIF)
  {
    TempValue = I2CRegs->PSR_IICMode;
    TempValue &= ((uint32_t)0x01 << ((uint32_t)Flag + 10U ));
  }
  else if(Flag == I2C001_FLAG_NACK_RECEIVED)
  {
    TempValue = I2CRegs->PSR_IICMode;
    TempValue &= ((uint32_t)0x01 << (uint32_t)I2C001_FLAG_WRONG_TDF);
  }
  else if(Flag == I2C001_FLAG_WRONG_TDF)
  {
    TempValue = I2CRegs->PSR_IICMode;
    TempValue &= ((uint32_t)0x01 << (uint32_t)I2C001_FLAG_DLIF);	
  }
  else if(Flag <= I2C001_FLAG_RBERI)
  {
    TempValue = I2CRegs->TRBSR;
    TempValue &= ((uint32_t)0x01 << ((uint32_t)Flag - \
                                               (uint32_t)I2C001_FLAG_SRBI));		
  }
  else
  {
    TempValue = I2CRegs->TRBSR;
    TempValue &= ((uint32_t)0x01 << (((uint32_t)Flag - \
                                  (uint32_t)I2C001_FLAG_STBI) + 8U));		
  }
  if(TempValue != 0x00U)
  {
    Status = (status_t)I2C001_SET;
  }
  return Status;
}

/* Clears the specified flag status.*/
void I2C001_ClearFlag\
                (const I2C001Handle_type* I2CHandle,I2C001_FlagStatusType Flag)
{
  USIC_CH_TypeDef* I2CRegs = I2CHandle->I2CRegs;

  /* <<<DD_SIMPLE_I2C_API_7>>>*/
  if(Flag <= I2C001_FLAG_RIF)
  {
    I2CRegs->PSCR = ((uint32_t)0x01 << (((uint32_t)Flag - \
                                  (uint32_t)I2C001_FLAG_RSIF) + 10U));
  }
  else if(Flag == I2C001_FLAG_NACK_RECEIVED)
  {
    I2CRegs->PSCR = ((uint32_t)0x01 << (uint32_t)I2C001_FLAG_WRONG_TDF);
  }
  else if(Flag == I2C001_FLAG_WRONG_TDF)
  {
    I2CRegs->PSCR = ((uint32_t)0x01 << (uint32_t)I2C001_FLAG_DLIF);	
  }
  else if(Flag <= I2C001_FLAG_RBERI)
  {
    I2CRegs->PSCR = ((uint32_t)0x01 << ((uint32_t)Flag - \
                                        (uint32_t)I2C001_FLAG_SRBI));		
  }
  else
  {
    I2CRegs->PSCR = ((uint32_t)0x01 << (((uint32_t)Flag - \
                                          (uint32_t)I2C001_FLAG_STBI) + 8U));
  }
}

/*CODE_BLOCK_END*/ 
