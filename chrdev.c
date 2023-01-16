#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>

static char *kbuf;
static dev_t first;
static unsigned count = 1;
static int major = 500, minor = 0;
static struct cdev *my_cdev;
#define MYDEV_NAME "chardev"
#define KBUF_SIZE (size_t) ((10) * PAGE_SIZE)

static int mychrdev_open(struct inode *inode, struct file *file){
	printk(KERN_INFO "opening device %s\n", MYDEV_NAME);
	return 0;
}

static int mychrdev_release(struct inode *inode, struct file *file){
	printk(KERN_INFO "closing device %s\n", MYDEV_NAME);
	return 0;
}

static ssize_t mychrdev_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos){
	int nbytes = lbuf - copy_to_user(buf, kbuf + *ppos, lbuf);
	*ppos += nbytes;
	printk(KERN_INFO "reading device %s nbytes %d\n", MYDEV_NAME, nbytes);
	return nbytes;
}

static ssize_t mychrdev_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos){
	int nbytes = lbuf - copy_from_user(kbuf + *ppos, buf, lbuf);
	*ppos += nbytes;
	printk(KERN_INFO "writing devise %s nbytes %d\n", MYDEV_NAME, nbytes);
	return nbytes;
}

static const struct file_operations mycdev_fops = {
	.owner = THIS_MODULE,
	.read = mychrdev_read,
	.write = mychrdev_write,
	.open = mychrdev_open,
	.release = mychrdev_release
};

static int __init init_chrdev(void)
{
    printk(KERN_INFO "init module\n");
    kbuf = kmalloc(KBUF_SIZE, GFP_KERNEL);
    first = MKDEV(major, minor);
    register_chrdev_region(first, count, MYDEV_NAME);
    my_cdev = cdev_alloc();
    cdev_init(my_cdev, &mycdev_fops);
    cdev_add(my_cdev, first, count);
    return 0;
} 

static void __exit cleanup_chrdev(void)
{
    printk(KERN_INFO "cleaup module.\n");
}

module_init(init_chrdev);
module_exit(cleanup_chrdev);

MODULE_LICENSE("GPL");
