#include "bsp_usart.h"
#include <stdarg.h>
#include "cmsis_os.h"

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;

extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;

usart_manage_obj_t usart1_manage_obj = {0};
usart_manage_obj_t usart2_manage_obj = {0};
usart_manage_obj_t usart3_manage_obj = {0};

static void usart_rec_to_buff(usart_manage_obj_t *m_obj, interrput_type int_type);
static void usart_transmit_hook(usart_manage_obj_t *m_obj);

/********************** Custom API **********************************************/
void debug_raw_printf(char *fmt, ...)
{
    va_list arg;
    uint8_t buff[USART1_PRINTF_BUFF_SIZE];
    uint8_t printf_len;

    va_start(arg, fmt);
    printf_len = vsnprintf((char *)buff, USART1_PRINTF_BUFF_SIZE, fmt, arg);
    va_end(arg);

    if (printf_len > USART1_PRINTF_BUFF_SIZE)
    {
        printf_len = USART1_PRINTF_BUFF_SIZE;
    }

    HAL_UART_Transmit(&huart1, (uint8_t *)buff, printf_len, 0xFFFF);
}

/********************** Custom API **********************************************/

void usart1_idle_callback(void)
{
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
        usart_rec_to_buff(&usart1_manage_obj, INTERRUPT_TYPE_UART);
    }
}

void usart2_idle_callback(void)
{
    if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE))
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart2);
        usart_rec_to_buff(&usart2_manage_obj, INTERRUPT_TYPE_UART);
    }
}

void usart3_idle_callback(void)
{
    if (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE))
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart3);
        usart_rec_to_buff(&usart3_manage_obj, INTERRUPT_TYPE_UART);
    }
}

void usart1_manage_init(void)
{
    uint8_t *usart1_rx_buff = pvPortMalloc(USART1_RX_BUFFER_SIZE);
    uint8_t *usart1_tx_fifo_buff = pvPortMalloc(USART1_TX_FIFO_SIZE);
    uint8_t *usart1_tx_buff = pvPortMalloc(USART1_TX_BUFFER_SIZE);
    
    usart1_manage_obj.rx_buffer = usart1_rx_buff;
    usart1_manage_obj.rx_buffer_size = USART1_RX_BUFFER_SIZE;
    usart1_manage_obj.dma_h = &hdma_usart1_rx;
    usart1_manage_obj.uart_h = &huart1;
    usart1_manage_obj.tx_fifo_buffer = usart1_tx_fifo_buff;
    usart1_manage_obj.tx_fifo_size = USART1_TX_FIFO_SIZE;
    usart1_manage_obj.tx_buffer_size = USART1_TX_BUFFER_SIZE;
    usart1_manage_obj.tx_buffer = usart1_tx_buff;
    usart1_manage_obj.is_sending = 0;

    fifo_s_init(&(usart1_manage_obj.tx_fifo), usart1_tx_fifo_buff, USART1_TX_FIFO_SIZE);

    HAL_UART_Receive_DMA(&huart1, usart1_rx_buff, USART1_RX_BUFFER_SIZE);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
}

void usart2_manage_init(void)
{
    uint8_t *usart2_rx_buff = pvPortMalloc(USART2_RX_BUFFER_SIZE);
    uint8_t *usart2_tx_fifo_buff = pvPortMalloc(USART2_TX_FIFO_SIZE);
    uint8_t *usart2_tx_buff = pvPortMalloc(USART2_TX_BUFFER_SIZE);
    
    usart2_manage_obj.rx_buffer = usart2_rx_buff;
    usart2_manage_obj.rx_buffer_size = USART2_RX_BUFFER_SIZE;
    usart2_manage_obj.dma_h = &hdma_usart2_rx;
    usart2_manage_obj.uart_h = &huart2;
    usart2_manage_obj.tx_fifo_buffer = usart2_tx_fifo_buff;
    usart2_manage_obj.tx_fifo_size = USART2_TX_FIFO_SIZE;
    usart2_manage_obj.tx_buffer_size = USART2_TX_BUFFER_SIZE;
    usart2_manage_obj.tx_buffer = usart2_tx_buff;
    usart2_manage_obj.is_sending = 0;

    fifo_s_init(&(usart2_manage_obj.tx_fifo), usart2_tx_fifo_buff, USART2_TX_FIFO_SIZE);

    HAL_UART_Receive_DMA(&huart2, usart2_rx_buff, USART2_RX_BUFFER_SIZE);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
}

void usart3_manage_init(void)
{
    uint8_t *usart3_rx_buff = pvPortMalloc(USART3_RX_BUFFER_SIZE);
    uint8_t *usart3_tx_fifo_buff = pvPortMalloc(USART3_TX_FIFO_SIZE);
    uint8_t *usart3_tx_buff = pvPortMalloc(USART3_TX_BUFFER_SIZE);
    
    usart3_manage_obj.rx_buffer = usart3_rx_buff;
    usart3_manage_obj.rx_buffer_size = USART3_RX_BUFFER_SIZE;
    usart3_manage_obj.dma_h = &hdma_usart3_rx;
    usart3_manage_obj.uart_h = &huart3;
    usart3_manage_obj.tx_fifo_buffer = usart3_tx_fifo_buff;
    usart3_manage_obj.tx_fifo_size = USART3_TX_FIFO_SIZE;
    usart3_manage_obj.tx_buffer_size = USART3_TX_BUFFER_SIZE;
    usart3_manage_obj.tx_buffer = usart3_tx_buff;
    usart3_manage_obj.is_sending = 0;

    fifo_s_init(&(usart3_manage_obj.tx_fifo), usart3_tx_fifo_buff, USART3_TX_FIFO_SIZE);

    HAL_UART_Receive_DMA(&huart3, usart3_rx_buff, USART3_RX_BUFFER_SIZE);
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
}

int usart1_printf(char *fmt, ...)
{

    va_list arg;
    uint8_t buff[USART1_PRINTF_BUFF_SIZE];
    uint8_t printf_len;

    va_start(arg, fmt);
    printf_len = vsnprintf((char *)buff, USART1_PRINTF_BUFF_SIZE, fmt, arg);
    va_end(arg);

    if (printf_len > USART1_PRINTF_BUFF_SIZE)
    {
        printf_len = USART1_PRINTF_BUFF_SIZE;
    }

    usart_transmit(&usart1_manage_obj, buff, printf_len);

    return 0;
}

void usart1_transmit(uint8_t *buff, uint16_t len)
{
    usart_transmit(&usart1_manage_obj, buff, len);
}

void usart2_transmit(uint8_t *buff, uint16_t len)
{
    usart_transmit(&usart2_manage_obj, buff, len);
}

void usart3_transmit(uint8_t *buff, uint16_t len)
{
    usart_transmit(&usart3_manage_obj, buff, len);
}

void usart1_rx_callback_register(usart_call_back fun)
{
    usart_rx_callback_register(&usart1_manage_obj, fun);
    return;
}

void usart2_rx_callback_register(usart_call_back fun)
{
    usart_rx_callback_register(&usart2_manage_obj, fun);
    return;
}

void usart3_rx_callback_register(usart_call_back fun)
{
    usart_rx_callback_register(&usart3_manage_obj, fun);
    return;
}

/********************** Usual API **********************************************/
/**
  * @brief  register uart callback function.
  * @param
  * @retval void
  */
void usart_rx_callback_register(usart_manage_obj_t *m_obj, usart_call_back fun)
{
    m_obj->call_back_f = fun;
    return;
}

/**
  * @brief  rx dma half complete interupt
  * @param
  * @retval void
  */
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1)
    {
        usart_rec_to_buff(&usart1_manage_obj, INTERRUPT_TYPE_DMA_HALF);
    }
    
    if (huart == &huart2)
    {
        usart_rec_to_buff(&usart2_manage_obj, INTERRUPT_TYPE_DMA_HALF);
    }
    
    if (huart == &huart3)
    {
        usart_rec_to_buff(&usart3_manage_obj, INTERRUPT_TYPE_DMA_HALF);
    }

    return;
}

/**
  * @brief  rx dma complete interupt
  * @param
  * @retval void
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1)
    {
        usart_rec_to_buff(&usart1_manage_obj, INTERRUPT_TYPE_DMA_ALL);
    }
    
    if (huart == &huart2)
    {
        usart_rec_to_buff(&usart2_manage_obj, INTERRUPT_TYPE_DMA_ALL);
    }
    
    if (huart == &huart3)
    {
        usart_rec_to_buff(&usart3_manage_obj, INTERRUPT_TYPE_DMA_ALL);
    }

    return;
}

/**
  * @brief  tx complete interupt
  * @param
  * @retval void
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1)
    {
        usart_transmit_hook(&usart1_manage_obj);
    }
    
    if (huart == &huart2)
    {
        usart_transmit_hook(&usart2_manage_obj);
    }
    
    if (huart == &huart3)
    {
        usart_transmit_hook(&usart3_manage_obj);
    }

    return;
}

/**
  * @brief  uart fifo transmit
  * @param
  * @retval error code
  */
int32_t usart_transmit(usart_manage_obj_t *m_obj, uint8_t *buf, uint16_t len)
{
    uint16_t to_send_len;
    uint16_t to_tx_fifo_len;

    if (m_obj->is_sending == 0)
    {
        if (len < m_obj->tx_buffer_size)
        {
            to_send_len = len;
            to_tx_fifo_len = 0;
        }
        else if (len < m_obj->tx_buffer_size + m_obj->tx_fifo_size)
        {
            to_send_len = m_obj->tx_buffer_size;
            to_tx_fifo_len = len - m_obj->tx_buffer_size;
        }
        else
        {
            to_send_len = m_obj->tx_buffer_size;
            to_tx_fifo_len = m_obj->tx_fifo_size;
        }
    }
    else
    {
        if (len < m_obj->tx_fifo_size)
        {
            to_send_len = 0;
            to_tx_fifo_len = len;
        }
        else
        {
            to_send_len = 0;
            to_tx_fifo_len = m_obj->tx_fifo_size;
        }
    }

    if (to_send_len > 0)
    {
        memcpy(m_obj->tx_buffer, buf, to_send_len);
        m_obj->is_sending = 1;
        HAL_UART_Transmit_DMA(m_obj->uart_h, m_obj->tx_buffer, to_send_len);
    }
    if (to_tx_fifo_len > 0)
    {
        uint8_t len;
        len = fifo_s_puts(&(m_obj->tx_fifo), (char *)(buf) + to_send_len, to_tx_fifo_len);

        if (len != to_tx_fifo_len)
        {
            return -1;
        }
    }

    return 0;
}

static void usart_transmit_hook(usart_manage_obj_t *m_obj)
{
    uint16_t fifo_data_num = 0;
    uint16_t send_num = 0;

    fifo_data_num = m_obj->tx_fifo.used_num;

    if (fifo_data_num != 0)
    {
        if (fifo_data_num < m_obj->tx_buffer_size)
        {
            send_num = fifo_data_num;
        }
        else
        {
            send_num = m_obj->tx_buffer_size;
        }
        fifo_s_gets(&(m_obj->tx_fifo), (char *)(m_obj->tx_buffer), send_num);
        m_obj->is_sending = 1;
        HAL_UART_Transmit_DMA(m_obj->uart_h, m_obj->tx_buffer, send_num);
    }
    else
    {
        m_obj->is_sending = 0;
    }
    return;
}

/**
  * @brief  rx to fifo
  * @param
  * @retval void
  */
static void usart_rec_to_buff(usart_manage_obj_t *m_obj, interrput_type int_type)
{
    uint16_t read_end_ptr = 0;
    uint16_t read_length = 0;
    uint16_t read_success_length = 0;
    uint16_t read_start_ptr = m_obj->read_start_index;
    uint8_t *pdata = m_obj->rx_buffer;

    UNUSED(read_success_length);

    uint16_t buff_left = m_obj->dma_h->Instance->NDTR;

    if (int_type == INTERRUPT_TYPE_UART)
    {
        read_end_ptr = m_obj->rx_buffer_size - buff_left;
    }

    if (int_type == INTERRUPT_TYPE_DMA_HALF)
    {
        read_end_ptr = m_obj->rx_buffer_size / 2;
    }

    if (int_type == INTERRUPT_TYPE_DMA_ALL)
    {
        read_end_ptr = m_obj->rx_buffer_size;
    }

    read_length = read_end_ptr - m_obj->read_start_index;

    if (m_obj->call_back_f != NULL)
    {
        uint8_t *read_ptr = pdata + read_start_ptr;
        read_success_length = m_obj->call_back_f(read_ptr, read_length);
    }

    m_obj->read_start_index = (m_obj->read_start_index + read_length) % (m_obj->rx_buffer_size);

    return;
}
