#include <rtthread.h>
#include "main.h"

#define NTC_ADC_BUF_SIZE 8
#define NTC_TEMP_RES_MAP_LEN 126 //0C - 125C
int ntc_temp_res_map[NTC_TEMP_RES_MAP_LEN] = {
    3004, //0 C
    2966, //1 C
    2931,
    2895,
    2858,
    2821,
    2784,
    2746,
    2709,
    2670,
    2633, //10 C
    2593,
    2555,
    2516,
    2477,
    2437,
    2398,
    2359,
    2320,
    2281,
    2241, //20 C
    2202,
    2163,
    2125,
    2086,
    2048, //25 C
    2009,
    1971,
    1934,
    1896,
    1859,
    1822,
    1786,
    1750,
    1714,
    1679,
    1644,
    1609,
    1576,
    1542,
    1509,
    1476,
    1444,
    1413,
    1381,
    1351,
    1321,
    1291,
    1262,
    1233,
    1204,
    1178,
    1151,
    1124,
    1098,
    1073,
    1048,
    1023,
    999,
    976,
    953,
    930,
    908,
    887,
    866,
    845,
    825,
    805,
    786,
    767,
    749,
    731,
    714,
    697,
    680,
    664,
    648,
    632,
    617,
    602,
    588,
    574,
    561,
    547,
    534,
    522,
    509,
    497,
    485,
    474,
    463,
    452,
    441,
    431,
    421,
    411,
    402,
    392,
    383,
    375,
    366,
    358,
    349,
    341,
    334,
    326,
    319,
    312,
    305,
    298,
    291,
    285,
    279,
    272,
    267,
    261,
    255,
    250,
    244,
    239,
    234,
    229,
    224,
    220,
    215,
    211, //125 C

};
ADC_HandleTypeDef hadc1;
uint16_t temp_adc_get = 0;

static void MX_ADC1_Init(void)
{

    /* USER CODE BEGIN ADC1_Init 0 */

    /* USER CODE END ADC1_Init 0 */

    ADC_ChannelConfTypeDef sConfig = {0};

    /* USER CODE BEGIN ADC1_Init 1 */

    /* USER CODE END ADC1_Init 1 */
    /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
    sConfig.Channel = ADC_CHANNEL_3;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */
}

uint16_t Get_Adc()
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    ADC1_ChanConf.Channel = ADC_CHANNEL_3;                 //通道
    ADC1_ChanConf.Rank = 1;                                //第1个序列，序列1
    ADC1_ChanConf.SamplingTime = ADC_SAMPLETIME_480CYCLES; //采样时间
    ADC1_ChanConf.Offset = 0;
    HAL_ADC_ConfigChannel(&hadc1, &ADC1_ChanConf); //通道配置
    HAL_ADC_Start(&hadc1);                         //开启ADC
    HAL_ADC_PollForConversion(&hadc1, 10);         //轮询转换
    return (u16)HAL_ADC_GetValue(&hadc1);          //返回最近一次ADC1规则组的转换结果
}

void ntc_init()
{
    MX_ADC1_Init();
}

int ntc_get_temp()
{
    temp_adc_get = Get_Adc();
    for (int i = 0; i < NTC_TEMP_RES_MAP_LEN; i++)
    {
        if (temp_adc_get < ntc_temp_res_map[i])
        {
            return i;
        }
    }
    return -1;
}

float ntc_get_float()
{
    temp_adc_get = Get_Adc();
    for (int i = 0; i < NTC_TEMP_RES_MAP_LEN; i++)
    {
        if (temp_adc_get < ntc_temp_res_map[i])
        {
            return ((float)(temp_adc_get - ntc_temp_res_map[i]) /
                    (ntc_temp_res_map[i + 1] - ntc_temp_res_map[i])) +
                   i;
        }
    }
    return -1;
}