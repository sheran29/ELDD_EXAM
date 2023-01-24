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
#include<linux/semaphore.h>
 
  dev_t dev;
//static struct class *dev_class;
static struct cdev sema_1;
struct semaphore my_sema;
int op_result=5;
uint8_t kernel_buffer[50];

static int      __init newdriver_init(void);
static void     __exit newdriver_exit(void);
static int      file_open(struct inode *inode, struct file *file);
static int      file_release(struct inode *inode, struct file *file);
static ssize_t  file_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  file_write(struct file *filp, const char *buf, size_t len, loff_t * off);

static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .open           = file_open,
        .release        = file_release,
        .read           = file_read,
        .write          = file_write,
};

/*This function will be called when we open the Device file*/

static int file_open(struct inode *inode, struct file *file)
{
        printk("Device File Opened...!!!\n");
        return 0;
}

/*This function will be called when we close the Device file*/

static int file_release(struct inode *inode, struct file *file)
{
        printk("Device File Closed...!!!\n");
        return 0;
}

/* This function will be called when we read the Device file*/

static ssize_t file_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    printk("Driver Read Function Called...!!!\n");
    
     copy_to_user((char*)buf,(char*)&op_result,sizeof(op_result));
   
     printk("data successfullt created\n");
    
     up(&my_sema);
     
    
        return len;
}

/*This function will be called when we write the Device file*/

static ssize_t file_write(struct file *filp, const char *buf, size_t len, loff_t *off)
{
        printk("Driver Write Function Called...!!!\n");
        down(&my_sema);
      
        //Copy the data to kernel space from the user-space
        copy_from_user((char *)kernel_buffer,buf,len);
         return len;
}

static int __init vishal_init(void)
{

    if((alloc_chrdev_region(&dev, 0, 2, "newdriver_Dev"))<0)
    {
        printk("Cannot allocate major number\n");
        return -1;
    }
    printk("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

     /*Creating cdev structure*/
    cdev_init(&sema_1,&fops);

     /*Adding character device to the system*/
        if((cdev_add(&sema_1,dev,2)) < 0){
        printk("Cannot add the device to the system\n");
        unregister_chrdev_region(dev,3);
        return -1;   
        }


    printk("Charecter file added succesfully\n.........Device Driver inserted successfully\n");
    sema_init(&my_sema,1);

       return 0;
 


}

static void     __exit vishal_exit(void)
{
    cdev_del(&sema_1);
    unregister_chrdev_region(dev, 2);
    printk("Device Driver Remove...Done!!!\n");
}


module_init(vishal_init);
module_exit(vishal_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SHERAN");



