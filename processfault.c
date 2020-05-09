// SPDX-License-Identifier: GPL-2.0-only
/*
 * NOTE: This example is works on x86 and powerpc.
 * Here's a sample kernel module showing the use of kprobes to dump a
 * stack trace and selected registers when _do_fork() is called.
 *
 * For more information on theory of operation of kprobes, see
 * Documentation/kprobes.txt
 *
 * You will see the trace data in /var/log/messages and on the console
 * whenever handle_mm_fault is invoked to create a new process.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>

#define MAX_SYMBOL_LEN	64
static char symbol[MAX_SYMBOL_LEN] = "handle_mm_fault";
//module_param_string(symbol, symbol, sizeof(symbol), 0644);

int var;

module_param(var, int, 0);

struct timespec test_of_time;


/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp = {
	.symbol_name	= symbol,
};

/* kprobe pre_handler: called just before the probed instruction is executed */
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{




if(current->pid == var)
    {
		getnstimeofday(&test_of_time);

		//printk("%lu", test_of_time.tv_nsec);
        
        printk(KERN_ALERT "Virtual Address: 0x%lu , Time : %lu",regs->si,test_of_time.tv_nsec);



	}

	/* A dump_stack() here will give a stack backtrace */
	return 0;
}




static int __init kprobe_init(void)
{
	int ret;
	kp.pre_handler = handler_pre;
	

	ret = register_kprobe(&kp);
	if (ret < 0) {
		pr_err("register_kprobe failed, returned %d\n", ret);
		return ret;
	}
	pr_info("Planted kprobe at %p\n", kp.addr);
	return 0;
}

static void __exit kprobe_exit(void)
{
	unregister_kprobe(&kp);
	pr_info("kprobe at %p unregistered\n", kp.addr);
}

module_init(kprobe_init)
module_exit(kprobe_exit)
MODULE_LICENSE("GPL");