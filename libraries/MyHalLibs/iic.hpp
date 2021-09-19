#ifndef __IIC_CPP__
#define __IIC_CPP__ 1

#include "main.h"

class soft_iic
{
#define GPI 0
#define GPO 1

#define IIC_TYPE_IIC 0
#define IIC_TYPE_SCCB 1

public:
    int sda_pin;
    int scl_pin;

    uint16_t simiic_delay_time = 100;

    void write_reg(uint8_t dev_addr, uint8_t reg, uint8_t data)
    {
        start();
        send_char((dev_addr << 1) | 0x00);
        send_char(reg);
        send_char(data);
        stop();
    }

    uint8_t read_reg(uint8_t dev_addr, uint8_t reg, uint8_t iic_type)
    {
        uint8_t data;
        start();
        send_char((dev_addr << 1) | 0x00);
        send_char(reg);
        send_char(data);
        stop();
    }

protected:
    void gpio_dir(int pin_num, int dir)
    {
    }

    void gpio_set(int pin_num, int value)
    {
    }

    uint8_t gpio_get(int pin_num)
    {
        return 0;
    }

    void delay(uint16_t time)
    {
        while (time--)
            ;
    }

    void delay()
    {
        while (simiic_delay_time--)
            ;
    }

    void start()
    {
        gpio_set(sda_pin, 1);
        gpio_set(scl_pin, 1);
        delay();
        gpio_set(sda_pin, 0);
        delay();
        gpio_set(scl_pin, 0);
    }

    void stop()
    {
        gpio_set(sda_pin, 0);
        gpio_set(scl_pin, 0);
        delay();
        gpio_set(scl_pin, 1);
        delay();
        gpio_set(sda_pin, 1);
        delay();
    }

    void send_ack(uint8_t ack_x)
    {
        gpio_set(scl_pin, 0);
        delay();

        if (ack_x)
            gpio_set(sda_pin, 0);
        else
            gpio_set(sda_pin, 1);

        gpio_set(scl_pin, 1);
        delay();
        gpio_set(scl_pin, 0);
        delay();
    }

    uint8_t wait_ack()
    {
        gpio_set(scl_pin, 0);
        gpio_dir(sda_pin, GPI);
        delay();

        gpio_set(scl_pin, 1);
        delay();

        if (gpio_get(sda_pin))
        {
            gpio_dir(sda_pin, GPO);
            gpio_set(scl_pin, 0);
            return 0;
        }

        gpio_dir(sda_pin, GPO);
        gpio_set(scl_pin, 0);
        delay();
        return 1;
    }

    void send_char(uint8_t dat)
    {
        uint8_t i = 8;
        while (i--)
        {
            if (dat & 0x80)
                gpio_set(sda_pin, 1);
            else
                gpio_set(sda_pin, 0);

            dat <<= 1;
            delay();
            gpio_set(scl_pin, 1);
            delay();
            gpio_set(scl_pin, 0);
        }

        wait_ack();
    }

    uint8_t read_char(uint8_t ack_x)
    {
        uint8_t c = 0;
        gpio_set(scl_pin, 0);
        delay();
        gpio_set(sda_pin, 1);
        gpio_dir(sda_pin, GPI);

        for (uint8_t i = 0; i < 8; i++)
        {
            delay();
            gpio_set(scl_pin, 0);
            delay();
            gpio_set(scl_pin, 1);
            delay();
            c <<= 1;
            if (gpio_get(sda_pin))
            {
                c += 1;
            }
        }

        gpio_dir(sda_pin, GPO);
        gpio_set(scl_pin, 0);
        delay();
        send_ack(ack_x);
    }
};

#endif // __IIC_CPP__