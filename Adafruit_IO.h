struct PWM_OBJECT
{
	char * PWM_PATH;
	char * PWM_PERIOD;
	char * PWM_DUTY;
	FILE * PWM_PERIOD_F;
	FILE * PWM_DUTY_F;
};
//uart functions
int uart_setup(const char *dt);
void uart_cleanup(void);
//Device Overlay Functions
int load_device_tree(const char *name);
int unload_device_tree(const char *name);
//path functions
int build_path(const char *partial_path, const char *prefix, char *full_path, size_t full_path_len);
//pin lookup functions
int lookup_gpio_by_key(const char *key);
int lookup_gpio_by_name(const char *name);
int lookup_ain_by_key(const char *key);
int lookup_ain_by_name(const char *name);