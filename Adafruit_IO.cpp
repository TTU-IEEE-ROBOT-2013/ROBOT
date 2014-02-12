#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>


#include "c_uart.h"
//#include "common.h"
#include "Adafruit_IO.h"

#ifndef notDefined

typedef struct pins_t { 
    const char *name; 
    const char *key; 
    int gpio;
    int pwm_mux_mode;
    int ain;
} pins_t;

//Table generated based on https://raw.github.com/jadonk/bonescript/master/node_modules/bonescript/bone.js
pins_t table[] = {
  { "USR0", "USR0", 53, -1, -1},
  { "USR1", "USR1", 54, -1, -1},
  { "USR2", "USR2", 55, -1, -1},
  { "USR3", "USR3", 56, -1, -1},
  { "DGND", "P8_1", 0, -1, -1},
  { "DGND", "P8_2", 0, -1, -1},
  { "GPIO1_6", "P8_3", 38, -1, -1},
  { "GPIO1_7", "P8_4", 39, -1, -1},
  { "GPIO1_2", "P8_5", 34, -1, -1},
  { "GPIO1_3", "P8_6", 35, -1, -1},
  { "TIMER4", "P8_7", 66, -1, -1},
  { "TIMER7", "P8_8", 67, -1, -1},
  { "TIMER5", "P8_9", 69, -1, -1},
  { "TIMER6", "P8_10", 68, -1, -1},
  { "GPIO1_13", "P8_11", 45, -1, -1},
  { "GPIO1_12", "P8_12", 44, -1, -1},
  { "EHRPWM2B", "P8_13", 23, 4, -1},
  { "GPIO0_26", "P8_14", 26, -1, -1},
  { "GPIO1_15", "P8_15", 47, -1, -1},
  { "GPIO1_14", "P8_16", 46, -1, -1},
  { "GPIO0_27", "P8_17", 27, -1, -1},
  { "GPIO2_1", "P8_18", 65, -1, -1},
  { "EHRPWM2A", "P8_19", 22, 4, -1},
  { "GPIO1_31", "P8_20", 63, -1, -1},
  { "GPIO1_30", "P8_21", 62, -1, -1},
  { "GPIO1_5", "P8_22", 37, -1, -1},
  { "GPIO1_4", "P8_23", 36, -1, -1},
  { "GPIO1_1", "P8_24", 33, -1, -1},
  { "GPIO1_0", "P8_25", 32, -1, -1},
  { "GPIO1_29", "P8_26", 61, -1, -1},
  { "GPIO2_22", "P8_27", 86, -1, -1},
  { "GPIO2_24", "P8_28", 88, -1, -1},
  { "GPIO2_23", "P8_29", 87, -1, -1},
  { "GPIO2_25", "P8_30", 89, -1, -1},
  { "UART5_CTSN", "P8_31", 10, -1, -1},
  { "UART5_RTSN", "P8_32", 11, -1, -1},
  { "UART4_RTSN", "P8_33", 9, -1, -1},
  { "UART3_RTSN", "P8_34", 81, 2, -1},
  { "UART4_CTSN", "P8_35", 8, -1, -1},
  { "UART3_CTSN", "P8_36", 80, 2, -1},
  { "UART5_TXD", "P8_37", 78, -1, -1},
  { "UART5_RXD", "P8_38", 79, -1, -1},
  { "GPIO2_12", "P8_39", 76, -1, -1},
  { "GPIO2_13", "P8_40", 77, -1, -1},
  { "GPIO2_10", "P8_41", 74, -1, -1},
  { "GPIO2_11", "P8_42", 75, -1, -1},
  { "GPIO2_8", "P8_43", 72, -1, -1},
  { "GPIO2_9", "P8_44", 73, -1, -1},
  { "GPIO2_6", "P8_45", 70, 3, -1},
  { "GPIO2_7", "P8_46", 71, 3, -1},
  { "DGND", "P9_1", 0, -1, -1},
  { "DGND", "P9_2", 0, -1, -1},
  { "VDD_3V3", "P9_3", 0, -1, -1},
  { "VDD_3V3", "P9_4", 0, -1, -1},
  { "VDD_5V", "P9_5", 0, -1, -1},
  { "VDD_5V", "P9_6", 0, -1, -1},
  { "SYS_5V", "P9_7", 0, -1, -1},
  { "SYS_5V", "P9_8", 0, -1, -1},
  { "PWR_BUT", "P9_9", 0, -1, -1},
  { "SYS_RESETn", "P9_10", 0, -1, -1},
  { "UART4_RXD", "P9_11", 30, -1, -1},
  { "GPIO1_28", "P9_12", 60, -1, -1},
  { "UART4_TXD", "P9_13", 31, -1, -1},
  { "EHRPWM1A", "P9_14", 50, 6, -1},
  { "GPIO1_16", "P9_15", 48, -1, -1},
  { "EHRPWM1B", "P9_16", 51, 6, -1},
  { "I2C1_SCL", "P9_17", 5, -1, -1},
  { "I2C1_SDA", "P9_18", 4, -1, -1},
  { "I2C2_SCL", "P9_19", 13, -1, -1},
  { "I2C2_SDA", "P9_20", 12, -1, -1},
  { "UART2_TXD", "P9_21", 3, 3, -1},
  { "UART2_RXD", "P9_22", 2, 3, -1},
  { "GPIO1_17", "P9_23", 49, -1, -1},
  { "UART1_TXD", "P9_24", 15, -1, -1},
  { "GPIO3_21", "P9_25", 117, -1, -1},
  { "UART1_RXD", "P9_26", 14, -1, -1},
  { "GPIO3_19", "P9_27", 115, -1, -1},
  { "SPI1_CS0", "P9_28", 113, 4, -1},
  { "SPI1_D0", "P9_29", 111, 1, -1},
  { "SPI1_D1", "P9_30", 112, -1, -1},
  { "SPI1_SCLK", "P9_31", 110, 1, -1},
  { "VDD_ADC", "P9_32", 0, -1, -1},
  { "AIN4", "P9_33", 0, -1, 4},
  { "GNDA_ADC", "P9_34", 0, -1, -1},
  { "AIN6", "P9_35", 0, -1, 6},
  { "AIN5", "P9_36", 0, -1, 5},
  { "AIN2", "P9_37", 0, -1, 2},
  { "AIN3", "P9_38", 0, -1, 3},
  { "AIN0", "P9_39", 0, -1, 0},
  { "AIN1", "P9_40", 0, -1, 1},
  { "CLKOUT2", "P9_41", 20, -1, -1},
  { "GPIO0_7", "P9_42", 7, 0, -1},
  { "DGND", "P9_43", 0, -1, -1},
  { "DGND", "P9_44", 0, -1, -1},
  { "DGND", "P9_45", 0, -1, -1},
  { "DGND", "P9_46", 0, -1, -1},
    { NULL, NULL, 0 }
};
typedef struct uart_t { 
    const char *name; 
    const char *path; 
    const char *dt; 
    const char *rx;
    const char *tx;
} uart_t;

uart_t uart_table[] = {
  { "UART1", "/dev/ttyO1", "ADAFRUIT-UART1", "P9_26", "P9_24"},
  { "UART2", "/dev/ttyO2", "ADAFRUIT-UART2", "P9_22", "P9_21"},
  { "UART3", "/dev/ttyO3", "ADAFRUIT-UART3", "P9_42", ""},
  { "UART4", "/dev/ttyO4", "ADAFRUIT-UART4", "P9_11", "P9_13"},
  { "UART5", "/dev/ttyO5", "ADAFRUIT-UART5", "P8_38", "P8_37"},
  { NULL, NULL, 0 }
};
#endif

int uart_setup(const char *dt)
{
    if (load_device_tree(dt)) {
        return 1;
    }

    return 0;
}
void uart_cleanup(void)
{
    unload_device_tree("ADAFRUIT-UART1");
    unload_device_tree("ADAFRUIT-UART2");
    unload_device_tree("ADAFRUIT-UART3");
    unload_device_tree("ADAFRUIT-UART4");
    unload_device_tree("ADAFRUIT-UART5");
}
int load_device_tree(const char *name)
{

    FILE *file = NULL;
    char slots[40];
    char line[256];

    build_path("/sys/devices", "bone_capemgr", ctrl_dir, sizeof(ctrl_dir));
    snprintf(slots, sizeof(slots), "%s/slots", ctrl_dir);

    file = fopen(slots, "r+");
    if (!file) {
        return 0;
    }

    while (fgets(line, sizeof(line), file)) {
        //the device is already loaded, return 1
        if (strstr(line, name)) {
            fclose(file);
            return 1;
        }
    }

    //if the device isn't already loaded, load it, and return
    fprintf(file, name);
    fclose(file);

    return 1;
}
int unload_device_tree(const char *name)
{
    FILE *file = NULL;
    char slots[40];
    char line[256];
    char *slot_line;

    snprintf(slots, sizeof(slots), "%s/slots", ctrl_dir);

    file = fopen(slots, "r+");
    if (!file) {
        return 0;
    }

    while (fgets(line, sizeof(line), file)) {
        //the device is loaded, let's unload it
        if (strstr(line, name)) {
            slot_line = strtok(line, ":");
            //remove leading spaces
            while(*slot_line == ' ')
                slot_line++;

            fprintf(file, "-%s", slot_line);
            fclose(file);
            return 1;
        }
    }

    //not loaded, close file
    fclose(file);

    return 1;
}
int lookup_uart_by_name(const char *input_name, char *dt)
{
    uart_t *p;
    for (p = uart_table; p->name != NULL; ++p) {
        if (strcmp(p->name, input_name) == 0) {
            strncpy(dt, p->dt, FILENAME_BUFFER_SIZE);
            dt[FILENAME_BUFFER_SIZE - 1] = '\0';
            return 1;                
        }
    }
    fprintf(stderr, "return 0 lookup_uart_by_name");
    return 0;
}

int lookup_gpio_by_key(const char *key)
{
  pins_t *p;
  for (p = table; p->key != NULL; ++p) {
      if (strcmp(p->key, key) == 0) {
          return p->gpio;
      }
  }
  return 0;
}

int lookup_gpio_by_name(const char *name)
{
  pins_t *p;
  for (p = table; p->name != NULL; ++p) {
      if (strcmp(p->name, name) == 0) {
          return p->gpio;
      }
  }
  return 0;
}

int lookup_ain_by_key(const char *key)
{
  pins_t *p;
  for (p = table; p->key != NULL; ++p) {
      if (strcmp(p->key, key) == 0) {
        if (p->ain == -1) {
          return -1;
        } else {
          return p->ain;
        }
      }
  }
  return -1;
}

int lookup_ain_by_name(const char *name)
{
  pins_t *p;
  for (p = table; p->name != NULL; ++p) {
      if (strcmp(p->name, name) == 0) {
        if (p->ain == -1) {
          return -1;
        } else {
          return p->ain;
        }
      }
  }
  return -1;
}

#ifdef USE_ADA_PWM
//pwm functions (simplified)
//first, a 'pwm' structure
int pwm_initialized = 0;

int initialize_pwm(void)
{
    if  (!pwm_initialized && load_device_tree("am33xx_pwm")) {
        build_path("/sys/devices", "ocp", ocp_dir, sizeof(ocp_dir));
        pwm_initialized = 1;
        return 1;
    }

    return 0;   
}

PWM_OBJECT * pwm_open(const char *key)
{
    char fragment[18];
    char pwm_test_fragment[20];
    char pwm_test_path=(char*)malloc(45);//[45];
    char period_path=(char*)malloc(50);//[50];
    char duty_path=(char*)malloc(50);//[50];
    //char polarity_path[55];
    int period_fd, duty_fd, polarity_fd;
    struct pwm_exp *new_pwm, *pwm;
	PWM_OBJECT * pwm_op=new PWM_OBJECT;
	
    if(!pwm_initialized) {
        initialize_pwm();
    }

    snprintf(fragment, sizeof(fragment), "bone_pwm_%s", key);
    

    if (!load_device_tree(fragment)) {
        //error enabling pin for pwm
		//free(pwm_test_fragment);
		free(pwm_test_path);
		free(period_path);
		free(duty_path);
		delete pwm_op;
        return 0;
    }

    //creates the fragment in order to build the pwm_test_filename, such as "pwm_test_P9_13"
    snprintf(pwm_test_fragment, sizeof(pwm_test_fragment), "pwm_test_%s", key);

    //finds and builds the pwm_test_path, as it can be variable...
    build_path(ocp_dir, pwm_test_fragment, pwm_test_path, sizeof(pwm_test_path));

    //create the path for the period and duty
    snprintf(period_path, sizeof(period_path), "%s/period", pwm_test_path);
    snprintf(duty_path, sizeof(duty_path), "%s/duty", pwm_test_path);
    snprintf(polarity_path, sizeof(polarity_path), "%s/polarity", pwm_test_path);

    //add period and duty fd to pwm list    
	FILE * period_fd;
	FILE * duty_fd;
    if ((period_fd = fopen(period_path, O_RDWR)) < 0)
        {		free(pwm_test_path);
		free(period_path);
		free(duty_path);
		delete pwm_op;
        return 0;}


    if ((duty_fd = fopen(duty_path, O_RDWR)) < 0) {
        //error, close already opened period_fd.
        close(period_fd);
        free(pwm_test_path);
		free(period_path);
		free(duty_path);
		delete pwm_op;
        return 0;
    }
	pwm_op->PWM_PATH=pwm_test_path;
	pwm_op->PWM_PERIOD=period_path;
	pwm_op->PWM_DUTY=duty_path;
	pwm_op->PWM_PERIOD_F=period_fd;
	pwm_op->PWM_DUTY_F=duty_fd;
    return pwm_op;
}

//add PwmDuty(...)
//add PwmPeriod(...)
#endif