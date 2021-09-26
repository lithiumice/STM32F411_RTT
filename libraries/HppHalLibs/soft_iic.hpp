#ifndef __IIC_CPP__
#define __IIC_CPP__ 1

#include "main.h"
#include "drv_common.h"
#include <rtdevice.h>
#include <board.h>

class soft_iic
{
#define GPI 0
#define GPO 1

    // #define PIN_LOW 0
    // #define PIN_HIGH 1

#define IIC_TYPE_IIC 0  //slave
#define IIC_TYPE_SCCB 1 //master

#define no_ack 0 //slave
#define ack 1    //master

private:
    void gpio_dir(int pin_num, int dir)
    {
        if (GPO == dir)
            rt_pin_mode(pin_num, PIN_MODE_OUTPUT);
        else
            rt_pin_mode(pin_num, PIN_MODE_INPUT);
    }

    void gpio_set(int pin_num, int value)
    {
        if (value)
            rt_pin_write(pin_num, PIN_HIGH);
        else
            rt_pin_write(pin_num, PIN_LOW);
    }

    uint8_t gpio_get(int pin_num)
    {
        return rt_pin_read(pin_num);
    }

    void delay(uint16_t time)
    {
        rt_hw_us_delay(time);
    }

    void delay()
    {
        //4us,4.7us --> 100kHz
        rt_hw_us_delay(5);
    }

public:
    int sda_pin;
    int scl_pin;
    uint8_t dev_addr;

    uint8_t iic_speed_type = 0; //100kHz
    uint8_t delay_us_time = 4;
    uint16_t simiic_delay_time = 100;

    soft_iic() {}

    //0x010_0000
    //0x0100_0000
    soft_iic(int sda_pin, int scl_pin, uint8_t dev_addr) : sda_pin(sda_pin), scl_pin(scl_pin), dev_addr(dev_addr)
    {
    }

    void init()
    {
        gpio_dir(scl_pin, GPO);
        gpio_dir(sda_pin, GPO);
        gpio_set(sda_pin, 1);
        gpio_set(scl_pin, 1);
    }

    void start()
    {
        gpio_dir(sda_pin, GPO);
        gpio_set(sda_pin, 1);
        gpio_set(scl_pin, 1);
        delay();
        gpio_set(sda_pin, 0);
        delay();
        gpio_set(scl_pin, 0);
    }

    void stop()
    {
        gpio_dir(sda_pin, GPO);

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
        gpio_dir(sda_pin, GPO);

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

    void send_ack()
    {

        gpio_set(scl_pin, 0);
        gpio_dir(sda_pin, GPO);
        gpio_set(sda_pin, 0);
        delay();

        gpio_set(scl_pin, 1);
        delay();

        gpio_set(scl_pin, 0);
        delay();
    }

    void send_nack()
    {

        gpio_set(scl_pin, 0);
        gpio_dir(sda_pin, GPO);
        gpio_set(sda_pin, 1);
        delay();

        gpio_set(scl_pin, 1);
        delay();

        gpio_set(scl_pin, 0);
        delay();
    }

    //return: 0:fail; 1:sucess
    uint8_t wait_ack()
    {
        int err_time = 0;

        gpio_set(scl_pin, 0);

        gpio_dir(sda_pin, GPI);
        delay();

        gpio_set(scl_pin, 1);
        delay();

        // if (gpio_get(sda_pin))
        // {
        //     gpio_dir(sda_pin, GPO);
        //     gpio_set(scl_pin, 0);
        //     return 0;
        // }
        while (gpio_get(sda_pin))
        {
            err_time++;
            if (err_time > 250)
            {
                stop();
                return 0;
            }
        }

        gpio_dir(sda_pin, GPO);
        gpio_set(scl_pin, 0);
        delay();
        return 1;
    }

    //with wait ack
    void send_char(uint8_t dat)
    {
        gpio_dir(sda_pin, GPO);
        gpio_set(scl_pin, 0);
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
            delay(); //TODO
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
        return c;
    }

    uint8_t read_char()
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
        send_ack(no_ack);
        return c;
    }

    void write_data(uint8_t data)
    {
        start();
        send_char((dev_addr << 1) | 0x00);
        send_char(data);
        stop();
    }

    //include sending addr in function
    void write_datas(uint8_t *src_data, uint16_t len)
    {
        start();
        send_char((dev_addr << 1) | 0x00);
        for (int i = 0; i < len; i++)
        {
            send_char(src_data[i]);
        }
        stop();
    }

    void write_reg(uint8_t reg, uint8_t data)
    {
        start();
        send_char((dev_addr << 1) | 0x00);
        send_char(reg);
        send_char(data);
        stop();
    }

    uint8_t read_reg(uint8_t reg, uint8_t iic_type)
    {
        uint8_t data;
        start();
        send_char((dev_addr << 1) | 0x00);
        send_char(reg);

        if (iic_type == IIC_TYPE_SCCB)
            stop();

        start();
        send_char((dev_addr << 1) | 0x01);
        data = read_char();
        stop();
        return data;
    }

    void read_regs(uint8_t reg, uint8_t *data_ret, uint8_t num, uint8_t iic_type)
    {
        start();
        send_char((dev_addr << 1) | 0x00);
        send_char(reg);

        if (iic_type == IIC_TYPE_SCCB)
            stop();

        start();
        send_char((dev_addr << 1) | 0x01);
        while (--num)
        {
            *data_ret = read_char(ack);
            data_ret++;
        }
        *data_ret = read_char(no_ack);
        stop();
    }
};

#endif // __IIC_CPP__
