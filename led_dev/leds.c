/* a sample device driver for testing the device driver function with leds */

// WARNING! Never run this module! Very much dangerous!!!


#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/delay.h>
#define DEVICE "led"

static int __init led_init(void);
static void __exit led_exit(void);
static int led_open(struct inode* i,struct file* f);
static int led_close(struct inode* i,struct file* f);
static ssize_t led_read(struct file* f,char* buf,size_t length,loff_t* offset);
static ssize_t led_write(struct file* f,const char* buf,size_t length,loff_t* offset);
static struct file_operations fops = {
	.read = led_read,
	.write = led_write,
	.open = led_open,
	.release = led_close
};

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Debadri");
MODULE_VERSION("printk");

static int Major;

static int __init led_init(void)
{
	Major = register_chrdev(0,DEVICE,&fops);
	if(Major>0)
	{
		printk("LED installed successfully with major no = %d\n",Major);
		return 0;
	}
	else
	{
		printk("Installation failed\n");
		return -1;
	}
}

module_init(led_init);

static void __exit led_exit(void)
{
	unregister_chrdev(Major,DEVICE);
	printk("LED removed\n");
}

module_exit(led_exit);

static int led_open(struct inode* i,struct file* f)
{
	printk("LED opened\n");
	return 0;
}

static int led_close(struct inode* i,struct file* f)
{
	printk("LED closed\n");
	return 0;
}

static ssize_t led_read(struct file* f,char* buf,size_t length,loff_t* offset)
{
	printk("Sorry! This operation is not supported\n");
	return 0;
}

static ssize_t led_write(struct file* f,const char* buffer,size_t length,loff_t* offset)
{
	int i,retries = 5;
	unsigned long delay = 1000;
	unsigned short n = 0x00;
	for(i=0;i<(int)length;i++)
	{
		if(buffer[i]=='1')
			n = n&(1<<0);
		else if(buffer[i]=='2')
			n = n&(1<<1);
		else if(buffer[i]=='3')
			n = n&(1<<2);
	}
	outb(0xed,0x60);
	udelay(delay);
	while(retries!=0&&inb(0x60)!=0xfa)
	{
		retries--;
		udelay(delay);
	}
	if(retries!=0)
	{
		outb(n,0x60);
		printk("successfully wrote = %s\n",buffer);
		return 1;
	}
	else
	{
		printk("writing failed\n");
		return 0;
	}
}
