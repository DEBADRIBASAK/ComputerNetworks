// implementation of the tick-tock question


#include <linux/module.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#define DEVICE "TickTock"
static int Major;
static int __init tick_init(void);
static void __exit tick_exit(void);
static int open_dev(struct inode* i,struct file* f);
static int close_dev(struct inode* i,struct file* f);
static ssize_t read_dev(struct file* f,char* buf,size_t length,loff_t* offset);
static ssize_t write_dev(struct file* f,const char* buf,size_t length,loff_t* offset);
static struct file_operations fops = {
	.read = read_dev,
	.write = write_dev,
	.open = open_dev,
	.release = close_dev
};

static int __init tick_init(void)
{
	Major = register_chrdev(0,DEVICE,&fops);
	if(Major>0)
	{
		printk("TickTock initialized with major no = %d\n",Major);
		return 0;
	}
	else
	{
		printk("TickTock not initialized\n");
		return -1;
	}
}

module_init(tick_init);

static void __exit tick_exit(void)
{
	unregister_chrdev(Major,DEVICE);
	printk("TickTock removed\n");
}
module_exit(tick_exit);

static int open_dev(struct inode* i,struct file* f)
{
	printk("TickTock has been opened!");
	return 0;
}

static int close_dev(struct inode* i,struct file* f)
{
	printk("TickTock has been closed\n");
	return 0;
}

MODULE_LICENSE("GPL");

static ssize_t read_dev(struct file* f,char* buf,size_t length,loff_t* offset)
{
	int len;
	struct timeval t;
	char *b;
	do_gettimeofday(&t);
	b = kmalloc(sizeof(char)*40,GFP_KERNEL);
	len = snprintf(b,sizeof(char)*40,"%ld",(long)(t.tv_sec%60));
	copy_to_user(buf,b,len);
	printk("The time elapsed = %s\n",b);
	return length;
}

static ssize_t write_dev(struct file* f,const char* buf,size_t length,loff_t* offset)
{
	long i;
	struct timespec ab;
	kstrtol(buf,10,&i);
	ab.tv_sec=i;
	ab.tv_nsec=0;
	do_settimeofday(&ab);
	return length;
}

MODULE_AUTHOR("Debadri Basak");
MODULE_VERSION("printk");
