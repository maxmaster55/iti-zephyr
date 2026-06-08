#include <stdio.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <say_hi.h>

#define PRI (7)
#define SLEEP_TIME_MS_0 1000
#define SLEEP_TIME_MS_1 800
#define SLEEP_TIME_MS_2 1200

struct k_thread led0_handler;
struct k_thread led1_handler;
struct k_thread led2_handler;

#define STACK_SIZE (1024)
K_THREAD_STACK_DEFINE(thread1_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread2_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread3_stack, STACK_SIZE);

static const struct gpio_dt_spec t0_led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec t1_led = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec t2_led = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);


void thread0(void *arg1, void *arg2, void *arg3);
void thread1(void *arg1, void *arg2, void *arg3);
void thread2(void *arg1, void *arg2, void *arg3);



int main(void) {
  if (!gpio_is_ready_dt(&t0_led) || !gpio_is_ready_dt(&t1_led) ||
      !gpio_is_ready_dt(&t2_led)) {
    printk("GPIO not ready!\n");
    return 0;
  }
  printk("GPIO ready!\n");
  say_hi();

  k_thread_create(&led0_handler, thread1_stack, STACK_SIZE, thread0, NULL, NULL,
                  NULL, PRI, 0, K_NO_WAIT);
  k_thread_create(&led0_handler, thread2_stack, STACK_SIZE, thread1, NULL, NULL,
                  NULL, PRI, 0, K_NO_WAIT);
  k_thread_create(&led0_handler, thread3_stack, STACK_SIZE, thread2, NULL, NULL,
                  NULL, PRI, 0, K_NO_WAIT);

  while (1) {
    // make this thread sleeps forever - its good to leave main thread there
    k_sleep(K_FOREVER);
  }

  return 0;
}



void thread0(void *arg1, void *arg2, void *arg3){
	int ret = gpio_pin_configure_dt(&t0_led, GPIO_OUTPUT_ACTIVE);
	if(ret < 0){
		return;
	}
	while(1){
		gpio_pin_toggle_dt(&t0_led);
		printk("Thread 0\n");
		k_msleep(SLEEP_TIME_MS_0);
	}
}

void thread1(void *arg1, void *arg2, void *arg3){
	int ret = gpio_pin_configure_dt(&t1_led, GPIO_OUTPUT_ACTIVE);
	if(ret < 0){
		return;
	}
	while(1){
		gpio_pin_toggle_dt(&t1_led);
		printk("Thread 1\n");
		k_msleep(SLEEP_TIME_MS_1);
	}
}

void thread2(void *arg1, void *arg2, void *arg3){
	int ret = gpio_pin_configure_dt(&t2_led, GPIO_OUTPUT_ACTIVE);
	if(ret < 0){
		return;
	}
	while(1){
		gpio_pin_toggle_dt(&t2_led);
		printk("Thread 2\n");
		k_msleep(SLEEP_TIME_MS_2);
	}
}
