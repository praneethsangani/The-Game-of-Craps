//Praneeth Sangani (PRS79)
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/random.h>
#include <asm/uaccess.h>


static ssize_t dice_read(struct file * file, char * buf, 
			  size_t count, loff_t *ppos)
{
	int i = 0;
	unsigned char* mybuf;

	if(count == 0)
		return 0; 
	mybuf = kmalloc(count, GFP_KERNEL); 
	
	for(i = 0; i < count; i++)
	{
		unsigned char random; 
		get_random_bytes(&random, 1); 
		mybuf[i] = random % 6; 
	}
	if(copy_to_user(buf, mybuf, count)){
  		kfree(mybuf);
  	}
	*ppos = *ppos + count;
	return count;
}


static const struct file_operations dice_fops = {
	.owner		= THIS_MODULE,
	.read		= dice_read,
};

static struct miscdevice dice_dev = {

	MISC_DYNAMIC_MINOR,

	"dice",

	&dice_fops
};

static int __init
dice_init(void)
{
	int ret;

	ret = misc_register(&dice_dev);
	if (ret)
		printk(KERN_ERR
		       "Unable to register \"dice, world!\" misc device\n");

	return ret;
}

module_init(dice_init);

static void __exit
dice_exit(void)
{
	misc_deregister(&dice_dev);
}

module_exit(dice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Praneeth Sangani prs79@pitt.edu");
MODULE_DESCRIPTION("\"dice!\" minimal module");
MODULE_VERSION("dev");
