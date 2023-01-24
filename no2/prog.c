/*Verifying Fileoperations through internel memory with userspace application*/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 //kmalloc()
#include <linux/uaccess.h>              //copy_to/from_user()
#include <linux/err.h>
#include<linux/ioctl.h>
#include "ioctl.h"
 

#define mem_size        1024           //Memory Size
#define WR_DATA _IOW('a','a',int32_t *)
#define RD_DATA _IOR('a','b',int32_t *)

dev_t dev = 0;
static struct class *dev_class;
static struct cdev driver2_cdev;
int32_t kernel_buffer;

/*Function Prototypes*/

static int      __init driver2_init(void);
static void     __exit driver2_exit(void);
static int      file_open(struct inode *inode, struct file *file);
static int      file_release(struct inode *inode, struct file *file);
/*static ssize_t  file_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  file_write(struct file *filp, const char *buf, size_t len, loff_t * off);*/
extern long file_ioctl(struct file *file, unsigned int cmd, unsigned long arg);


/*File Operations structure*/

static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        // .read           = file_read,
        // .write          = file_write,
        .open           = file_open,
        .release        = file_release,
        .unlocked_ioctl = file_ioctl,
};
 
/*This function will be called when we open the Device file*/

static int file_open(struct inode *inode, struct file *file)
{
        pr_info("Device File Opened...!!!\n");
        return 0;
}

/*This function will be called when we close the Device file*/

static int file_release(struct inode *inode, struct file *file)
{
        pr_info("Device File Closed...!!!\n");
        return 0;
}

/*This function will be called when we read the Device file

static ssize_t file_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        //Copy the data from the kernel space to the user-space
       /* if( copy_to_user(buf, kernel_buffer, mem_size) )
        {
                pr_err("Data Read : Err!\n");
        } */
        // pr_info("Data Read : Done!\n");
        // return mem_size;
//}

/*This function will be called when we write the Device file*/

/*static ssize_t file_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        //Copy the data to kernel space from the user-space
      /*  if( copy_from_user(kernel_buffer, buf, len) )
        {
                pr_err("Data Write : Err!\n");
        } */
   //     pr_info("Data Write : Done!\n");
 //       return len;
//}

extern long file_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	switch(cmd)
	{
		case WR_DATA:	if(copy_from_user(&kernel_buffer,(int32_t *)arg,sizeof(kernel_buffer)))
		               {
                                   pr_err("Data Write : Err!\n");
                               }
				printk("Writing ioctl data\n");
				break;
				
		case RD_DATA:	if(copy_to_user((int32_t *)arg,&kernel_buffer,sizeof(kernel_buffer)))
		                {
                                pr_err("Data Write : Err!\n");
                               }
				printk("Reading data\n");
				break;
		default :
			printk("DEFAULT\n");
			break;			
	}
	return 0;
}

/*Module Init function*/

static int __init driver2_init(void)
{		
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "driver2_Dev")) <0){
                pr_info("Cannot allocate major number\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        /*Creating cdev structure*/
        cdev_init(&driver2_cdev,&fops);
 
        /*Adding character device to the system*/
        if((cdev_add(&driver2_cdev,dev,1)) < 0){
            pr_info("Cannot add the device to the system\n");
            goto r_class;
        }
 
        /*Creating struct class
        if(IS_ERR(dev_class = class_create(THIS_MODULE,"driver2_cs"))){
            pr_info("Cannot create the struct class\n");
            goto r_class;
        }*/
 
        /*Creating device
        if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"driver2_device"))){
            pr_info("Cannot create the Device 1\n");
            goto r_device;
        }*/
        
        /*Creating Physical memory*/
      /*  if((kernel_buffer = kmalloc(mem_size , GFP_KERNEL)) == 0){
            pr_info("Cannot allocate memory in kernel\n");
            goto r_device;
        }
        
        strcpy(kernel_buffer, "Hello_World");
      */  
        pr_info("Device Driver Insert...Done!!!\n");
        return 0;
 
r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}

/*Module exit function*/

static void __exit driver2_exit(void)
{
	kfree(kernel_buffer);
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&driver2_cdev);
        unregister_chrdev_region(dev, 1);
        pr_info("Device Driver Remove...Done!!!\n");
}
 
module_init(driver2_init);
module_exit(driver2_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("CDAC");
MODULE_DESCRIPTION("IOCTL");
