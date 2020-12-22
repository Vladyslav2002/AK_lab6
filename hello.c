#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>


#include "linux/ktime.h"
#include "linux/slab.h"

MODULE_AUTHOR("Bohdanenko Vladyslav IV-81");
MODULE_DESCRIPTION("Hello world printing Linux Kernel Training for Lab6");
MODULE_LICENSE("Dual BSD/GPL");

static uint count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "How many times hello world will be repeated");

struct time_list {
	struct time_list *next;
	ktime_t time;
};

static struct time_list *head;
static int __init hello_init(void)
{
	struct time_list *tail;
	uint i;
	if (count == 0) {
		printk(KERN_WARNING "count=%i is 0\n", count);
		return 0;
	} 
	else if (count >= 5 && count <= 10) {
		printk(KERN_WARNING "count=%i in range of 5,10\n", count);
	} 
	else if (count > 10) {
		printk(KERN_ERR "count=%i is bigger than 10\n", count);
		return -EINVAL;
	}

	head = kmalloc(sizeof(struct time_list *), GFP_KERNEL);
	head->next = NULL;
	head->time = ktime_get();
	tail = head;
	printk(KERN_INFO "Hello world\n");

	for (i = 1; i < count; i++) {
		tail->next = kmalloc(sizeof(struct time_list *), GFP_KERNEL);
		if (tail->next == NULL) {
			while (head != NULL) {
				tail = head;
				head = tail->next;
				kfree(tail);
			}
			printk(KERN_ERR "Out of memory");
			return -ENOMEM;
		}
		tail = tail->next;
		tail->next = NULL;
		tail->time = ktime_get();
		printk(KERN_INFO "Hello world\n");
	}
	return 0;

}

static void __exit hello_exit(void)
{
	struct time_list *tail;
	while (head != NULL) {
		tail = head;
		pr_info("Time: %lld\n", tail->time);
		head = tail->next;
		kfree(tail);
	}
}

module_init(hello_init);
module_exit(hello_exit);
