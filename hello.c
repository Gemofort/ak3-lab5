

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>
MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");
static uint arg = 1;
module_param(arg, uint, 0);
MODULE_PARM_DESC(arg, "\n\t\tx == 0 or 5 <= x <= 10 - warning\n\t\tx > 10 - error");
struct my_list {
	struct list_head node;
	ktime_t time;
};
static LIST_HEAD(my_list_head);
static int __init hello_init(void)
{
	int i;
	struct my_list *data, *tmp;
	if (arg == 0 || (arg >= 5 && arg <= 10)) {
		printk(KERN_WARNING "Warning!!! arg is 0 or between 5 and 10\n");	
	} else if (arg > 10) {
	printk(KERN_ERR "Error!!! arg > 10\n");
	return -EINVAL;
	}
	BUG_ON(!list_empty(&my_list_head));
	for (i = 0; i < arg; i++) {
		printk(KERN_INFO "Hello, world!\n");
	data = (struct my_list *) kmalloc(sizeof(struct my_list), GFP_KERNEL);
	if (ZERO_OR_NULL_PTR(data))
		goto clean;
	data->time = ktime_get();
	list_add_tail(&data->node, &my_list_head);
	}
	return 0;
	clean:
	list_for_each_entry_safe(data, tmp, &my_list_head, node) {
		list_del(&data->node);
		kfree(data);
	}
	BUG_ON(!list_empty(&my_list_head));

	return -1;
}
static void __exit hello_exit(void)
{
	struct my_list *data, *tmp;
	list_for_each_entry_safe(data, tmp, &my_list_head, node) {
		printk(KERN_INFO "time - %lld\n", data->time);
		list_del(&data->node);
		kfree(data);
	}
	BUG_ON(!list_empty(&my_list_head));
}
module_init(hello_init);
module_exit(hello_exit);
