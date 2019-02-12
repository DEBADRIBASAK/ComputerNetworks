// this is a driver program for hello world problem

#include <linux/init.h>
#include <linux/module.h>

static int __init hello_init(void)
{
	printk("Hello World\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk("Goodbye Cruel World!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Debadri");
MODULE_DESCRIPTION("\"Hello World\" Minimal module");
MODULE_VERSION("printk");

