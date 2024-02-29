#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"

volatile uint32_t uwTick;


#define GPIO_MODE             0x00000003U
#define EXTI_MODE             0x10000000U
#define GPIO_MODE_IT          0x00010000U
#define GPIO_MODE_EVT         0x00020000U
#define RISING_EDGE           0x00100000U
#define FALLING_EDGE          0x00200000U
#define GPIO_OUTPUT_TYPE      0x00000010U

#define GPIO_NUMBER           16U

static HAL_StatusTypeDef SPI_WaitFlagStateUntilTimeout(SPI_HandleTypeDef *hspi, uint32_t Flag, uint32_t State, uint32_t Timeout, uint32_t Tickstart);
static HAL_StatusTypeDef SPI_CheckFlag_BSY(SPI_HandleTypeDef *hspi, uint32_t Timeout, uint32_t Tickstart);
static void HAL_SPI_MspInit( SPI_HandleTypeDef * hspi);
static void HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init);

Spi::Spi(SPI_HandleTypeDef *hspi,SPI_TypeDef *_instance, uint32_t _mode,uint32_t _direction,uint32_t _dataSize,
				 uint32_t _clkpolarity,uint32_t _clkphase,uint32_t _nss,uint32_t _baudRatePrescaler,uint32_t _firstBit,
				 uint32_t _tiMode,uint32_t _crcCalculation,uint32_t _crcPolynomial)
{
				hspi->Instance  = _instance;
				hspi->Init.Mode  = _mode;
        hspi->Init.Direction = _direction;	
				hspi->Init.DataSize = _dataSize;
				hspi->Init.CLKPolarity = _clkpolarity;
				hspi->Init.CLKPhase = _clkphase;
				hspi->Init.NSS = _nss;
				hspi->Init.BaudRatePrescaler = _baudRatePrescaler;
				hspi->Init.FirstBit = _firstBit;
				hspi->Init.TIMode = _tiMode;
				hspi->Init.CRCCalculation = _crcCalculation;
				hspi->Init.CRCPolynomial = _crcPolynomial;
	
	     this->Spi_Init(hspi);
				
}

HAL_StatusTypeDef  Spi::Spi_Init(SPI_HandleTypeDef *hspi){

 /* Check the SPI handle allocation */
  if (hspi == NULL)
  {
    return HAL_ERROR;
  }
	
	  /* Check the parameters */
  assert_param(IS_SPI_ALL_INSTANCE(hspi->Instance));
  assert_param(IS_SPI_MODE(hspi->Init.Mode));
  assert_param(IS_SPI_DIRECTION(hspi->Init.Direction));
  assert_param(IS_SPI_DATASIZE(hspi->Init.DataSize));
  assert_param(IS_SPI_NSS(hspi->Init.NSS));
  assert_param(IS_SPI_BAUDRATE_PRESCALER(hspi->Init.BaudRatePrescaler));
  assert_param(IS_SPI_FIRST_BIT(hspi->Init.FirstBit));
  assert_param(IS_SPI_TIMODE(hspi->Init.TIMode));
	
	  if (hspi->Init.TIMode == SPI_TIMODE_DISABLE)
  {
    assert_param(IS_SPI_CPOL(hspi->Init.CLKPolarity));
    assert_param(IS_SPI_CPHA(hspi->Init.CLKPhase));
  }
 #if (USE_SPI_CRC != 0U)
  assert_param(IS_SPI_CRC_CALCULATION(hspi->Init.CRCCalculation));
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    assert_param(IS_SPI_CRC_POLYNOMIAL(hspi->Init.CRCPolynomial));
  }
	#else
  hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
 #endif /* USE_SPI_CRC */
	
	  {
    /* Allocate lock resource and initialize it */
    hspi->Lock = HAL_UNLOCKED;

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
    /* Init the SPI Callback settings */
    hspi->TxCpltCallback       = HAL_SPI_TxCpltCallback;       /* Legacy weak TxCpltCallback       */
    hspi->RxCpltCallback       = HAL_SPI_RxCpltCallback;       /* Legacy weak RxCpltCallback       */
    hspi->TxRxCpltCallback     = HAL_SPI_TxRxCpltCallback;     /* Legacy weak TxRxCpltCallback     */
    hspi->TxHalfCpltCallback   = HAL_SPI_TxHalfCpltCallback;   /* Legacy weak TxHalfCpltCallback   */
    hspi->RxHalfCpltCallback   = HAL_SPI_RxHalfCpltCallback;   /* Legacy weak RxHalfCpltCallback   */
    hspi->TxRxHalfCpltCallback = HAL_SPI_TxRxHalfCpltCallback; /* Legacy weak TxRxHalfCpltCallback */
    hspi->ErrorCallback        = HAL_SPI_ErrorCallback;        /* Legacy weak ErrorCallback        */
    hspi->AbortCpltCallback    = HAL_SPI_AbortCpltCallback;    /* Legacy weak AbortCpltCallback    */

    if (hspi->MspInitCallback == NULL)
    {
      hspi->MspInitCallback = HAL_SPI_MspInit; /* Legacy weak MspInit  */
    }

    /* Init the low level hardware : GPIO, CLOCK, NVIC... */
    hspi->MspInitCallback(hspi);
#else
    /* Init the low level hardware : GPIO, CLOCK, NVIC... */
    HAL_SPI_MspInit(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
  }
	  /*----------------------- SPIx CR1 & CR2 Configuration ---------------------*/
  /* Configure : SPI Mode, Communication Mode, Data size, Clock polarity and phase, NSS management,
  Communication speed, First bit and CRC calculation state */
  WRITE_REG(hspi->Instance->CR1, (hspi->Init.Mode | hspi->Init.Direction | hspi->Init.DataSize |
                                  hspi->Init.CLKPolarity | hspi->Init.CLKPhase | (hspi->Init.NSS & SPI_CR1_SSM) |
                                  hspi->Init.BaudRatePrescaler | hspi->Init.FirstBit  | hspi->Init.CRCCalculation));

  /* Configure : NSS management, TI Mode */
  WRITE_REG(hspi->Instance->CR2, (((hspi->Init.NSS >> 16U) & SPI_CR2_SSOE) | hspi->Init.TIMode));

#if (USE_SPI_CRC != 0U)
  /*---------------------------- SPIx CRCPOLY Configuration ------------------*/
  /* Configure : CRC Polynomial */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    WRITE_REG(hspi->Instance->CRCPR, hspi->Init.CRCPolynomial);
  }
#endif /* USE_SPI_CRC */

#if defined(SPI_I2SCFGR_I2SMOD)
  /* Activate the SPI mode (Make sure that I2SMOD bit in I2SCFGR register is reset) */
  CLEAR_BIT(hspi->Instance->I2SCFGR, SPI_I2SCFGR_I2SMOD);
#endif /* SPI_I2SCFGR_I2SMOD */

  hspi->ErrorCode = HAL_SPI_ERROR_NONE;
  hspi->State     = HAL_SPI_STATE_READY;

  return HAL_OK;
}


/**
  * @brief  Transmit an amount of data in blocking mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  pData pointer to data buffer
  * @param  Size amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef Spi::SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  uint32_t tickstart = 0U;
  HAL_StatusTypeDef errorcode = HAL_OK;

  /* Check Direction parameter */
  assert_param(IS_SPI_DIRECTION_2LINES_OR_1LINE(hspi->Init.Direction));

  /* Process Locked */
  __HAL_LOCK(hspi);

  /* Init tickstart for timeout management*/
  tickstart = HAL_GetTick();

  if(hspi->State != HAL_SPI_STATE_READY)
  {
    errorcode = HAL_BUSY;
    goto error;
  }

  if((pData == NULL ) || (Size == 0))
  {
    errorcode = HAL_ERROR;
    goto error;
  }

  /* Set the transaction information */
  hspi->State       = HAL_SPI_STATE_BUSY_TX;
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
  hspi->pTxBuffPtr  = (uint8_t *)pData;
  hspi->TxXferSize  = Size;
  hspi->TxXferCount = Size;

  /*Init field not used in handle to zero */
  hspi->pRxBuffPtr  = (uint8_t *)NULL;
  hspi->RxXferSize  = 0U;
  hspi->RxXferCount = 0U;
  hspi->TxISR       = NULL;
  hspi->RxISR       = NULL;

  /* Configure communication direction : 1Line */
  if(hspi->Init.Direction == SPI_DIRECTION_1LINE)
  {
    SPI_1LINE_TX(hspi);
  }

#if (USE_SPI_CRC != 0U)
  /* Reset CRC Calculation */
  if(hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    SPI_RESET_CRC(hspi);
  }
#endif /* USE_SPI_CRC */

  /* Check if the SPI is already enabled */
  if((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
  {
    /* Enable SPI peripheral */
    __HAL_SPI_ENABLE(hspi);
  }

  /* Transmit data in 16 Bit mode */
  if(hspi->Init.DataSize == SPI_DATASIZE_16BIT)
  {
    if((hspi->Init.Mode == SPI_MODE_SLAVE) || (hspi->TxXferCount == 0x01))
    {
      hspi->Instance->DR = *((uint16_t *)pData);
      pData += sizeof(uint16_t);
      hspi->TxXferCount--;
    }
    /* Transmit data in 16 Bit mode */
    while (hspi->TxXferCount > 0U)
    {
      /* Wait until TXE flag is set to send data */
      if(__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE))
      {
          hspi->Instance->DR = *((uint16_t *)pData);
          pData += sizeof(uint16_t);
          hspi->TxXferCount--;
      }
      else
      {
        /* Timeout management */
        if((Timeout == 0U) || ((Timeout != HAL_MAX_DELAY) && ((HAL_GetTick()-tickstart) >=  Timeout)))
        {
          errorcode = HAL_TIMEOUT;
          goto error;
        }
      }
    }
  }
  /* Transmit data in 8 Bit mode */
  else
  {
    if((hspi->Init.Mode == SPI_MODE_SLAVE)|| (hspi->TxXferCount == 0x01))
    {
      *((__IO uint8_t*)&hspi->Instance->DR) = (*pData);
      pData += sizeof(uint8_t);
      hspi->TxXferCount--;
    }
    while (hspi->TxXferCount > 0U)
    {
      /* Wait until TXE flag is set to send data */
      if(__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE))
      {
        *((__IO uint8_t*)&hspi->Instance->DR) = (*pData);
        pData += sizeof(uint8_t);
        hspi->TxXferCount--;
      }
      else
      {
        /* Timeout management */
        if((Timeout == 0U) || ((Timeout != HAL_MAX_DELAY) && ((HAL_GetTick()-tickstart) >=  Timeout)))
        {
          errorcode = HAL_TIMEOUT;
          goto error;
        }
      }
    }
  }

  /* Wait until TXE flag */
  if(SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_TXE, SET, Timeout, tickstart) != HAL_OK)
  {
    errorcode = HAL_TIMEOUT;
    goto error;
  }
  
  /* Check Busy flag */
  if(SPI_CheckFlag_BSY(hspi, Timeout, tickstart) != HAL_OK)
  {
    errorcode = HAL_ERROR;
    hspi->ErrorCode = HAL_SPI_ERROR_FLAG;
    goto error;
  }

  /* Clear overrun flag in 2 Lines communication mode because received is not read */
  if(hspi->Init.Direction == SPI_DIRECTION_2LINES)
  {
    __HAL_SPI_CLEAR_OVRFLAG(hspi);
  }
#if (USE_SPI_CRC != 0U)
  /* Enable CRC Transmission */
  if(hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
     SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
  }
#endif /* USE_SPI_CRC */

  if(hspi->ErrorCode != HAL_SPI_ERROR_NONE)
  {
    errorcode = HAL_ERROR;
  }

error:
  hspi->State = HAL_SPI_STATE_READY;
  /* Process Unlocked */
  __HAL_UNLOCK(hspi);
  return errorcode;
}


/**
  * @brief Handle SPI Communication Timeout.
  * @param hspi pointer to a SPI_HandleTypeDef structure that contains
  *              the configuration information for SPI module.
  * @param Flag SPI flag to check
  * @param State flag state to check
  * @param Timeout Timeout duration
  * @param Tickstart tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef SPI_WaitFlagStateUntilTimeout(SPI_HandleTypeDef *hspi, uint32_t Flag, uint32_t State, uint32_t Timeout, uint32_t Tickstart)
{
  while((((hspi->Instance->SR & Flag) == (Flag)) ? SET : RESET) != State)
  {
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0U) || ((HAL_GetTick()-Tickstart) >= Timeout))
      {
        /* Disable the SPI and reset the CRC: the CRC value should be cleared
        on both master and slave sides in order to resynchronize the master
        and slave for their respective CRC calculation */

        /* Disable TXE, RXNE and ERR interrupts for the interrupt process */
        __HAL_SPI_DISABLE_IT(hspi, (SPI_IT_TXE | SPI_IT_RXNE | SPI_IT_ERR));

        if((hspi->Init.Mode == SPI_MODE_MASTER)&&((hspi->Init.Direction == SPI_DIRECTION_1LINE)||(hspi->Init.Direction == SPI_DIRECTION_2LINES_RXONLY)))
        {
          /* Disable SPI peripheral */
          __HAL_SPI_DISABLE(hspi);
        }

        /* Reset CRC Calculation */
        if(hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
        {
          SPI_RESET_CRC(hspi);
        }

        hspi->State= HAL_SPI_STATE_READY;

        /* Process Unlocked */
        __HAL_UNLOCK(hspi);

        return HAL_TIMEOUT;
      }
    }
  }

  return HAL_OK;
}


/**
  * @brief Handle to check BSY flag before start a new transaction.
  * @param hspi pointer to a SPI_HandleTypeDef structure that contains
  *              the configuration information for SPI module.
  * @param Timeout Timeout duration
  * @param Tickstart tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef SPI_CheckFlag_BSY(SPI_HandleTypeDef *hspi, uint32_t Timeout, uint32_t Tickstart)
{
  /* Control the BSY flag */
  if(SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_BSY, RESET, Timeout, Tickstart) != HAL_OK)
  {
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
    return HAL_TIMEOUT;
  }
  return HAL_OK;
}


/**
  * @brief  Receive an amount of data in blocking mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  pData pointer to data buffer
  * @param  Size amount of data to be received
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef Spi::SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
#if (USE_SPI_CRC != 0U)
  __IO uint16_t tmpreg = 0U;
#endif /* USE_SPI_CRC */
  uint32_t tickstart = 0U;
  HAL_StatusTypeDef errorcode = HAL_OK;

  if((hspi->Init.Mode == SPI_MODE_MASTER) && (hspi->Init.Direction == SPI_DIRECTION_2LINES))
  {
     hspi->State = HAL_SPI_STATE_BUSY_RX;
     /* Call transmit-receive function to send Dummy data on Tx line and generate clock on CLK line */
    return (this->SPI_TransmitReceive(hspi,pData,pData,Size,Timeout));
  }

  /* Process Locked */
  __HAL_LOCK(hspi);

  /* Init tickstart for timeout management*/
  tickstart = HAL_GetTick();

  if(hspi->State != HAL_SPI_STATE_READY)
  {
    errorcode = HAL_BUSY;
    goto error;
  }

  if((pData == NULL ) || (Size == 0))
  {
    errorcode = HAL_ERROR;
    goto error;
  }

  /* Set the transaction information */
  hspi->State       = HAL_SPI_STATE_BUSY_RX;
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
  hspi->pRxBuffPtr  = (uint8_t *)pData;
  hspi->RxXferSize  = Size;
  hspi->RxXferCount = Size;

  /*Init field not used in handle to zero */
  hspi->pTxBuffPtr  = (uint8_t *)NULL;
  hspi->TxXferSize  = 0U;
  hspi->TxXferCount = 0U;
  hspi->RxISR       = NULL;
  hspi->TxISR       = NULL;

#if (USE_SPI_CRC != 0U)
  /* Reset CRC Calculation */
  if(hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    SPI_RESET_CRC(hspi);
    /* this is done to handle the CRCNEXT before the latest data */
    hspi->RxXferCount--;
  }
#endif /* USE_SPI_CRC */

  /* Configure communication direction: 1Line */
  if(hspi->Init.Direction == SPI_DIRECTION_1LINE)
  {
    SPI_1LINE_RX(hspi);
  }

  /* Check if the SPI is already enabled */
  if((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
  {
    /* Enable SPI peripheral */
    __HAL_SPI_ENABLE(hspi);
  }

    /* Receive data in 8 Bit mode */
  if(hspi->Init.DataSize == SPI_DATASIZE_8BIT)
  {
    /* Transfer loop */
    while(hspi->RxXferCount > 0U)
    {
      /* Check the RXNE flag */
      if(__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE))
      {
        /* read the received data */
        (* (uint8_t *)pData)= *(__IO uint8_t *)&hspi->Instance->DR;
        pData += sizeof(uint8_t);
        hspi->RxXferCount--;
      }
      else
      {
        /* Timeout management */
        if((Timeout == 0U) || ((Timeout != HAL_MAX_DELAY) && ((HAL_GetTick()-tickstart) >=  Timeout)))
        {
          errorcode = HAL_TIMEOUT;
          goto error;
        }
      }
    }
  }
  else
  {
    /* Transfer loop */
    while(hspi->RxXferCount > 0U)
    {
      /* Check the RXNE flag */
      if(__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE))
      {
        *((uint16_t*)pData) = hspi->Instance->DR;
        pData += sizeof(uint16_t);
        hspi->RxXferCount--;
      }
      else
      {
        /* Timeout management */
        if((Timeout == 0U) || ((Timeout != HAL_MAX_DELAY) && ((HAL_GetTick()-tickstart) >=  Timeout)))
        {
          errorcode = HAL_TIMEOUT;
          goto error;
        }
      }
    }
  }

#if (USE_SPI_CRC != 0U)
  /* Handle the CRC Transmission */
  if(hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    /* freeze the CRC before the latest data */
    SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);

    /* Read the latest data */
    if(SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_RXNE, SET, Timeout, tickstart) != HAL_OK)
    {
      /* the latest data has not been received */
      errorcode = HAL_TIMEOUT;
      goto error;
    }

    /* Receive last data in 16 Bit mode */
    if(hspi->Init.DataSize == SPI_DATASIZE_16BIT)
    {
      *((uint16_t*)pData) = hspi->Instance->DR;
    }
    /* Receive last data in 8 Bit mode */
    else
    {
      (*(uint8_t *)pData) = *(__IO uint8_t *)&hspi->Instance->DR;
    }

    /* Wait the CRC data */
    if(SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_RXNE, SET, Timeout, tickstart) != HAL_OK)
    {
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);
      errorcode = HAL_TIMEOUT;
      goto error;
    }

    /* Read CRC to Flush DR and RXNE flag */
    tmpreg = hspi->Instance->DR;
    /* To avoid GCC warning */
    UNUSED(tmpreg);
  }
#endif /* USE_SPI_CRC */

  /* Check the end of the transaction */
  if((hspi->Init.Mode == SPI_MODE_MASTER)&&((hspi->Init.Direction == SPI_DIRECTION_1LINE)||(hspi->Init.Direction == SPI_DIRECTION_2LINES_RXONLY)))
  {
    /* Disable SPI peripheral */
    __HAL_SPI_DISABLE(hspi);
  }

#if (USE_SPI_CRC != 0U)
  /* Check if CRC error occurred */
  if(__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_CRCERR))
  {
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);
    __HAL_SPI_CLEAR_CRCERRFLAG(hspi);
  }
#endif /* USE_SPI_CRC */

  if(hspi->ErrorCode != HAL_SPI_ERROR_NONE)
  {
    errorcode = HAL_ERROR;
  }

error :
  hspi->State = HAL_SPI_STATE_READY;
  __HAL_UNLOCK(hspi);
  return errorcode;
}



/**
  * @brief  Transmit and Receive an amount of data in blocking mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  pTxData pointer to transmission data buffer
  * @param  pRxData pointer to reception data buffer
  * @param  Size amount of data to be sent and received
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef Spi::SPI_TransmitReceive(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout)
{
  uint32_t tmp = 0U, tmp1 = 0U;
#if (USE_SPI_CRC != 0U)
  __IO uint16_t tmpreg1 = 0U;
#endif /* USE_SPI_CRC */
  uint32_t tickstart = 0U;
  /* Variable used to alternate Rx and Tx during transfer */
  uint32_t txallowed = 1U;
  HAL_StatusTypeDef errorcode = HAL_OK;

  /* Check Direction parameter */
  assert_param(IS_SPI_DIRECTION_2LINES(hspi->Init.Direction));

  /* Process Locked */
  __HAL_LOCK(hspi);

  /* Init tickstart for timeout management*/
  tickstart = HAL_GetTick();
  
  tmp  = hspi->State;
  tmp1 = hspi->Init.Mode;
  
  if(!((tmp == HAL_SPI_STATE_READY) || \
    ((tmp1 == SPI_MODE_MASTER) && (hspi->Init.Direction == SPI_DIRECTION_2LINES) && (tmp == HAL_SPI_STATE_BUSY_RX))))
  {
    errorcode = HAL_BUSY;
    goto error;
  }

  if((pTxData == NULL) || (pRxData == NULL) || (Size == 0))
  {
    errorcode = HAL_ERROR;
    goto error;
  }

  /* Don't overwrite in case of HAL_SPI_STATE_BUSY_RX */
  if(hspi->State == HAL_SPI_STATE_READY)
  {
    hspi->State = HAL_SPI_STATE_BUSY_TX_RX;
  }

  /* Set the transaction information */
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
  hspi->pRxBuffPtr  = (uint8_t *)pRxData;
  hspi->RxXferCount = Size;
  hspi->RxXferSize  = Size;
  hspi->pTxBuffPtr  = (uint8_t *)pTxData;
  hspi->TxXferCount = Size;
  hspi->TxXferSize  = Size;

  /*Init field not used in handle to zero */
  hspi->RxISR       = NULL;
  hspi->TxISR       = NULL;

#if (USE_SPI_CRC != 0U)
  /* Reset CRC Calculation */
  if(hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    SPI_RESET_CRC(hspi);
  }
#endif /* USE_SPI_CRC */

  /* Check if the SPI is already enabled */
  if((hspi->Instance->CR1 &SPI_CR1_SPE) != SPI_CR1_SPE)
  {
    /* Enable SPI peripheral */
    __HAL_SPI_ENABLE(hspi);
  }

  /* Transmit and Receive data in 16 Bit mode */
  if(hspi->Init.DataSize == SPI_DATASIZE_16BIT)
  {
    if((hspi->Init.Mode == SPI_MODE_SLAVE) || (hspi->TxXferCount == 0x01U))
    {
      hspi->Instance->DR = *((uint16_t *)pTxData);
      pTxData += sizeof(uint16_t);
      hspi->TxXferCount--;
    }
    while ((hspi->TxXferCount > 0U) || (hspi->RxXferCount > 0U))
    {
      /* Check TXE flag */
      if(txallowed && (hspi->TxXferCount > 0U) && (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE)))
      {
        hspi->Instance->DR = *((uint16_t *)pTxData);
        pTxData += sizeof(uint16_t);
        hspi->TxXferCount--;
        /* Next Data is a reception (Rx). Tx not allowed */ 
        txallowed = 0U;

#if (USE_SPI_CRC != 0U)
        /* Enable CRC Transmission */
        if((hspi->TxXferCount == 0U) && (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE))
        {
          SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
        }
#endif /* USE_SPI_CRC */
      }

      /* Check RXNE flag */
      if((hspi->RxXferCount > 0U) && (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE)))
      {
        *((uint16_t *)pRxData) = hspi->Instance->DR;
        pRxData += sizeof(uint16_t);
        hspi->RxXferCount--;
        /* Next Data is a Transmission (Tx). Tx is allowed */ 
        txallowed = 1U;
      }
      if((Timeout != HAL_MAX_DELAY) && ((HAL_GetTick()-tickstart) >=  Timeout))
      {
        errorcode = HAL_TIMEOUT;
        goto error;
      }
    }
  }
  /* Transmit and Receive data in 8 Bit mode */
  else
  {
    if((hspi->Init.Mode == SPI_MODE_SLAVE) || (hspi->TxXferCount == 0x01U))
    {
      *((__IO uint8_t*)&hspi->Instance->DR) = (*pTxData);
      pTxData += sizeof(uint8_t);
      hspi->TxXferCount--;
    }
    while((hspi->TxXferCount > 0U) || (hspi->RxXferCount > 0U))
    {
      /* check TXE flag */
      if(txallowed && (hspi->TxXferCount > 0U) && (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE)))
      {
        *(__IO uint8_t *)&hspi->Instance->DR = (*pTxData++);
        hspi->TxXferCount--;
        /* Next Data is a reception (Rx). Tx not allowed */ 
        txallowed = 0U;

#if (USE_SPI_CRC != 0U)
        /* Enable CRC Transmission */
        if((hspi->TxXferCount == 0U) && (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE))
        {
          SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
        }
#endif /* USE_SPI_CRC */
      }

      /* Wait until RXNE flag is reset */
      if((hspi->RxXferCount > 0U) && (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE)))
      {
        (*(uint8_t *)pRxData++) = hspi->Instance->DR;
        hspi->RxXferCount--;
        /* Next Data is a Transmission (Tx). Tx is allowed */ 
        txallowed = 1U;
      }
      if((Timeout != HAL_MAX_DELAY) && ((HAL_GetTick()-tickstart) >=  Timeout))
      {
        errorcode = HAL_TIMEOUT;
        goto error;
      }
    }
  }

#if (USE_SPI_CRC != 0U)
  /* Read CRC from DR to close CRC calculation process */
  if(hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    /* Wait until TXE flag */
    if(SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_RXNE, SET, Timeout, tickstart) != HAL_OK)
    {
      /* Error on the CRC reception */
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);
      errorcode = HAL_TIMEOUT;
      goto error;
    }
    /* Read CRC */
    tmpreg1 = hspi->Instance->DR;
    /* To avoid GCC warning */
    UNUSED(tmpreg1);
  }

  /* Check if CRC error occurred */
  if(__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_CRCERR))
  {
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);
    /* Clear CRC Flag */
    __HAL_SPI_CLEAR_CRCERRFLAG(hspi);

    errorcode = HAL_ERROR;
  }
#endif /* USE_SPI_CRC */

  /* Wait until TXE flag */
  if(SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_TXE, SET, Timeout, tickstart) != HAL_OK)
  {
    errorcode = HAL_TIMEOUT;
    goto error;
  }
  
  /* Check Busy flag */
  if(SPI_CheckFlag_BSY(hspi, Timeout, tickstart) != HAL_OK)
  {
    errorcode = HAL_ERROR;
    hspi->ErrorCode = HAL_SPI_ERROR_FLAG;
    goto error;
  }

  /* Clear overrun flag in 2 Lines communication mode because received is not read */
  if(hspi->Init.Direction == SPI_DIRECTION_2LINES)
  {
    __HAL_SPI_CLEAR_OVRFLAG(hspi);
  }
  
error :
  hspi->State = HAL_SPI_STATE_READY;
  __HAL_UNLOCK(hspi);
  return errorcode;
}



 static void HAL_SPI_MspInit( SPI_HandleTypeDef * hspi){
	 
	 GPIO_InitTypeDef GPIO_InitStruct;
	 
	 if(hspi->Instance ==  SPI1){
	 

    /* Peripheral clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
 
    /**SPI1 GPIO Configuration    
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI1_MspInit 1 */

  /* USER CODE END SPI1_MspInit 1 */
	 
	 }
	 else if(hspi->Instance ==  SPI2){
	 
	 }
	 
	 else if(hspi->Instance ==  SPI3){
	 
	 }
		else if(hspi->Instance ==  SPI4){
	 
	 }
			 
	 else if(hspi->Instance ==  SPI5){
	 
	 }
 
	 
 }
 
 
 
void systick_init(void){
    __disable_irq();                /* global disable IRQs */

    /* Configure SysTick */
    SysTick->LOAD = 16000-1;     /* reload with number of clocks per second */
    SysTick->VAL = 0;
    SysTick->CTRL = 7;              /* enable SysTick interrupt, use system clock */
    __enable_irq();
}

void SysTick_Handler(void)
{
  HAL_IncTick();

}


/**
  * @brief This function is called to increment  a global variable "uwTick"
  *        used as application time base.
  * @note In the default implementation, this variable is incremented each 1ms
  *       in SysTick ISR.
 * @note This function is declared as __weak to be overwritten in case of other 
  *      implementations in user file.
  * @retval None
  */
 void HAL_IncTick(void)
{
  uwTick +=1;
}


uint32_t HAL_GetTick(void){

   return uwTick;


}


/**
  * @brief  Initializes the GPIOx peripheral according to the specified parameters in the GPIO_Init.
  * @param  GPIOx where x can be (A..K) to select the GPIO peripheral for STM32F429X device or
  *                      x can be (A..I) to select the GPIO peripheral for STM32F40XX and STM32F427X devices.
  * @param  GPIO_Init pointer to a GPIO_InitTypeDef structure that contains
  *         the configuration information for the specified GPIO peripheral.
  * @retval None
  */
static void HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
  uint32_t position;
  uint32_t ioposition = 0x00U;
  uint32_t iocurrent = 0x00U;
  uint32_t temp = 0x00U;

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Init->Pin));
  assert_param(IS_GPIO_MODE(GPIO_Init->Mode));
  assert_param(IS_GPIO_PULL(GPIO_Init->Pull));

  /* Configure the port pins */
  for(position = 0U; position < GPIO_NUMBER; position++)
  {
    /* Get the IO position */
    ioposition = 0x01U << position;
    /* Get the current IO position */
    iocurrent = (uint32_t)(GPIO_Init->Pin) & ioposition;

    if(iocurrent == ioposition)
    {
      /*--------------------- GPIO Mode Configuration ------------------------*/
      /* In case of Output or Alternate function mode selection */
      if((GPIO_Init->Mode == GPIO_MODE_OUTPUT_PP) || (GPIO_Init->Mode == GPIO_MODE_AF_PP) ||
         (GPIO_Init->Mode == GPIO_MODE_OUTPUT_OD) || (GPIO_Init->Mode == GPIO_MODE_AF_OD))
      {
        /* Check the Speed parameter */
        assert_param(IS_GPIO_SPEED(GPIO_Init->Speed));
        /* Configure the IO Speed */
        temp = GPIOx->OSPEEDR; 
        temp &= ~(GPIO_OSPEEDER_OSPEEDR0 << (position * 2U));
        temp |= (GPIO_Init->Speed << (position * 2U));
        GPIOx->OSPEEDR = temp;

        /* Configure the IO Output Type */
        temp = GPIOx->OTYPER;
        temp &= ~(GPIO_OTYPER_OT_0 << position) ;
        temp |= (((GPIO_Init->Mode & GPIO_OUTPUT_TYPE) >> 4U) << position);
        GPIOx->OTYPER = temp;
       }

      /* Activate the Pull-up or Pull down resistor for the current IO */
      temp = GPIOx->PUPDR;
      temp &= ~(GPIO_PUPDR_PUPDR0 << (position * 2U));
      temp |= ((GPIO_Init->Pull) << (position * 2U));
      GPIOx->PUPDR = temp;

      /* In case of Alternate function mode selection */
      if((GPIO_Init->Mode == GPIO_MODE_AF_PP) || (GPIO_Init->Mode == GPIO_MODE_AF_OD))
      {
        /* Check the Alternate function parameter */
        assert_param(IS_GPIO_AF(GPIO_Init->Alternate));
        /* Configure Alternate function mapped with the current IO */
        temp = GPIOx->AFR[position >> 3U];
        temp &= ~(0xFU << ((uint32_t)(position & 0x07U) * 4U)) ;
        temp |= ((uint32_t)(GPIO_Init->Alternate) << (((uint32_t)position & 0x07U) * 4U));
        GPIOx->AFR[position >> 3U] = temp;
      }

      /* Configure IO Direction mode (Input, Output, Alternate or Analog) */
      temp = GPIOx->MODER;
      temp &= ~(GPIO_MODER_MODER0 << (position * 2U));
      temp |= ((GPIO_Init->Mode & GPIO_MODE) << (position * 2U));
      GPIOx->MODER = temp;

      /*--------------------- EXTI Mode Configuration ------------------------*/
      /* Configure the External Interrupt or event for the current IO */
      if((GPIO_Init->Mode & EXTI_MODE) == EXTI_MODE)
      {
        /* Enable SYSCFG Clock */
        __HAL_RCC_SYSCFG_CLK_ENABLE();

        temp = SYSCFG->EXTICR[position >> 2U];
        temp &= ~(0x0FU << (4U * (position & 0x03U)));
        temp |= ((uint32_t)(GPIO_GET_INDEX(GPIOx)) << (4U * (position & 0x03U)));
        SYSCFG->EXTICR[position >> 2U] = temp;

        /* Clear EXTI line configuration */
        temp = EXTI->IMR;
        temp &= ~((uint32_t)iocurrent);
        if((GPIO_Init->Mode & GPIO_MODE_IT) == GPIO_MODE_IT)
        {
          temp |= iocurrent;
        }
        EXTI->IMR = temp;

        temp = EXTI->EMR;
        temp &= ~((uint32_t)iocurrent);
        if((GPIO_Init->Mode & GPIO_MODE_EVT) == GPIO_MODE_EVT)
        {
          temp |= iocurrent;
        }
        EXTI->EMR = temp;

        /* Clear Rising Falling edge configuration */
        temp = EXTI->RTSR;
        temp &= ~((uint32_t)iocurrent);
        if((GPIO_Init->Mode & RISING_EDGE) == RISING_EDGE)
        {
          temp |= iocurrent;
        }
        EXTI->RTSR = temp;

        temp = EXTI->FTSR;
        temp &= ~((uint32_t)iocurrent);
        if((GPIO_Init->Mode & FALLING_EDGE) == FALLING_EDGE)
        {
          temp |= iocurrent;
        }
        EXTI->FTSR = temp;
      }
    }
  }
}