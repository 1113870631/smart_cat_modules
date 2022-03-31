#include  <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/delay.h>
typedef struct SR04_cdev{

     int devid;
     int gpio_echo;
     int gpio_trige;
     int major;
     int minor;
     struct cdev * sr04_cdev;
     int irp;
     char * name;
     struct class* class;
     struct device *device;


}SR04_cdev;

/*
  SR04 结构体
  cdev 结构体
*/
struct timeval tstart, tend;
SR04_cdev SR04_cdev1={
    .name="SR04_cdev1"
};
struct cdev sr04_cdev1={
    .owner=THIS_MODULE,
};


irqreturn_t SR04_cdev1_irq_handler_t(int tmp_int, void * tmp_void){

    printk("irq\n");
   if(gpio_get_value(SR04_cdev1.gpio_echo)){
       do_gettimeofday(&tstart);
   }
   else{
       do_gettimeofday(&tend);
        printk("%s, stime: %lds, utime: %ldus\n", __func__, tstart.tv_sec, tstart.tv_usec);
        printk("%s, before mtime: %ldus\n", __func__, 1000 * 1000 * tstart.tv_sec + tstart.tv_usec);
        printk("%s, stime: %lds, utime: %ldus\n", __func__, tend.tv_sec, tend.tv_usec);
        printk("%s, after mtime: %ldus\n", __func__, 1000 * 1000 * tend.tv_sec + tend.tv_usec);
        int tmp=tend.tv_usec-tstart.tv_usec;
        if(tmp>0){
            printk("%d\n",tmp);
        }
        else{
            printk("error\n");
        }
        
   }
    
   return IRQ_HANDLED;

};

//file_ops
int SR04_cdev1_file_operations_open (struct inode * inode, struct file * file){

    printk("SR04_cdev1_file_operations_open\n");
    gpio_direction_output(SR04_cdev1.gpio_trige,0);
    gpio_set_value(SR04_cdev1.gpio_trige,1);
    mdelay(3);//延时3ms
    gpio_set_value(SR04_cdev1.gpio_trige,0); 
   
   
   return 0;
};

static int SR04_cdev1_file_operations_release(struct inode *inode, struct file *file)
{
  
  printk("release\n");
  return 0;
}

ssize_t SR04_cdev1_file_operations_read (struct file *file, char __user * user, size_t size, loff_t *  loff_t){


    

  return 0;
};




struct file_operations SR04_cdev1_file_operations={
    .owner=THIS_MODULE,
    .open=SR04_cdev1_file_operations_open,
    .release=SR04_cdev1_file_operations_release,
    .read=SR04_cdev1_file_operations_read,
 };



/**
 * @brief 
 * 
 * 模块入口函数
 * 
 * @return int 
 */
static int __init newcdev_init(void)
{
    int major;
    int ret;
    printk("module init\n");

    SR04_cdev1.sr04_cdev=&sr04_cdev1;
  
    /*
    *注册cdev
    */

    //申请设备号
        if(major){//给定主设备号
        SR04_cdev1.devid= MKDEV(major,0);
           //给定设备号注册cdev 
        ret = register_chrdev_region(SR04_cdev1.devid, 0 , SR04_cdev1.name);
        if(ret<0)
        {
                printk("fail regist cdev\n");
                goto failregist;
        }
        } 
        else{//没给定主设备号
                ret = alloc_chrdev_region(&SR04_cdev1.devid, 0 , 1 , SR04_cdev1.name);
                SR04_cdev1.major=MAJOR(SR04_cdev1.devid);
                SR04_cdev1.minor=MINOR(SR04_cdev1.devid);
                if(ret<0)
                {
                printk("fail register cdev\n");
                goto failregist;
                }
         }

        //cdev_init;
          cdev_init(SR04_cdev1.sr04_cdev,& SR04_cdev1_file_operations);
        //cdev_add
           ret = cdev_add(SR04_cdev1.sr04_cdev,SR04_cdev1.devid,1);
           if(ret){
               printk("cdev_add fail\n");
               goto  failregist;
           }
        //class
        SR04_cdev1.class=class_create(THIS_MODULE,"SR04_cdev1_class");
               if (IS_ERR(SR04_cdev1.class)) {
                        return PTR_ERR(SR04_cdev1.class);
                }
        SR04_cdev1.device = device_create(SR04_cdev1.class,NULL,SR04_cdev1.devid,NULL,"SR04_cdev1");
         if (IS_ERR(SR04_cdev1.device)) {
                        return PTR_ERR(SR04_cdev1.device);
                }


                SR04_cdev1.gpio_echo=10;
                SR04_cdev1.gpio_trige=9;
                /*
                申请GPIO 和中断
                */
                //检查GPIO 是否可用
                if(!(gpio_is_valid(SR04_cdev1.gpio_echo)&&  gpio_is_valid(SR04_cdev1.gpio_trige))){
                    printk("gpio is not valid\n");
                    return 0;
                }
                //申请gpio
                gpio_request(SR04_cdev1.gpio_echo,"SR04_cdev1.gpio_echo");
                gpio_request(SR04_cdev1.gpio_trige,"SR04_cdev1.gpio_trige");
                //申请irq
                gpio_direction_input(SR04_cdev1.gpio_echo);
                gpio_set_value(SR04_cdev1.gpio_echo,0);
              SR04_cdev1.irp= gpio_to_irq(SR04_cdev1.gpio_echo);
                ret =request_irq(SR04_cdev1.irp,SR04_cdev1_irq_handler_t,IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING,"SR04_cdev1_irq",NULL);
                if(ret){
                    printk("request_irq fail\n");
                    return ret;
                }

                printk("init ok\n");
                
    failregist:
        unregister_chrdev_region(SR04_cdev1.devid,1);


    return 0;
}
/**
 * @brief 
 * 
 * 模块出口函数
 * 
 */
static void __exit newcdev_exit(void)
{
        free_irq(SR04_cdev1.irp,NULL);
        gpio_free(SR04_cdev1.gpio_echo);
        gpio_free(SR04_cdev1.gpio_trige);

        cdev_del(SR04_cdev1.sr04_cdev);
        device_destroy(SR04_cdev1.class, SR04_cdev1.devid);
        class_destroy( SR04_cdev1.class);
        unregister_chrdev_region(SR04_cdev1.devid,1);
        printk("exit ok\n");
}






/*
模块出入口函数
*/
module_init(newcdev_init);
module_exit(newcdev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("wh");