#include  <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
/*
 ir_distance_cdev 结构体
*/
typedef struct mycdev_irdistance{
   dev_t devid;
   char * name;
   struct cdev newcdev;
   int test_ma;
   int test_mi;
   struct class* class;
   struct device *device;
   int gpio;
   int irq;
}mycdev_irdistance;


static irqreturn_t gpio_ir_distance_Handler(int irq, void *dev_id)
{
	printk("Interrupt IN\n");
	return IRQ_HANDLED;
}
mycdev_irdistance mycdev_irdistance1={
        .name="mycdev_irdistance1",
};



//文件操作函数
        static	int  mycdev_irdistance1_fop_open (struct inode * inode, struct file * file){


                return 0 ;
        };



static ssize_t mycdev_irdistance1_fop_write(struct file *file, const char __user *data,
		       size_t len, loff_t *ppos)
{ 
   ssize_t a=0;
   return a;
}
static ssize_t mycdev_irdistance1_fop_read(struct file *file, char __user * buf,
		      size_t len, loff_t * ppos)
{
   ssize_t a=0;
   return a;
}

static int mycdev_irdistance1_fop_release(struct inode *inode, struct file *file)
{
  
  printk("release\n");
  return 0;
}

 struct file_operations mycdev_irdistance1_fop={
      .open=mycdev_irdistance1_fop_open,
      .read=mycdev_irdistance1_fop_read,
      .write=mycdev_irdistance1_fop_write,
      .release=mycdev_irdistance1_fop_release,
      .owner=THIS_MODULE,
      };


/*模块入口函数*/
static int __init newcdev_init(void)
{ 
        int ret=0;
        int major=0;

       //申请设备号
        if(major){//给定主设备号
        mycdev_irdistance1.devid= MKDEV(major,0);
           //给定设备号注册cdev 
        ret = register_chrdev_region(mycdev_irdistance1.devid, 0 , mycdev_irdistance1.name);
        if(ret<0)
        {
                printk("fail regist cdev\n");
                goto failregist;
        }
        } 
        else{//没给定主设备号
                ret = alloc_chrdev_region(&mycdev_irdistance1.devid, 0 , 1 , mycdev_irdistance1.name);
                mycdev_irdistance1.test_ma=MAJOR(mycdev_irdistance1.devid);
                mycdev_irdistance1.test_mi=MINOR(mycdev_irdistance1.devid);
                if(ret<0)
                {
                printk("fail register cdev\n");
                goto failregist;
                }
         }
      
              //注册一个cdev
                struct cdev cdev_ir={
                        .owner=THIS_MODULE,
                };
                mycdev_irdistance1.newcdev=cdev_ir;
                //cdev_init
                cdev_init(&mycdev_irdistance1.newcdev,&mycdev_irdistance1_fop);
                //cdev_add
                ret = cdev_add(&mycdev_irdistance1.newcdev,mycdev_irdistance1.devid,1);
                if(ret<0)
                {
                        printk("cdev add fail\n");
                        goto add_fail;
                }


        //创建类
                mycdev_irdistance1.class = class_create(THIS_MODULE,"mycdev_irdistance1");
                if (IS_ERR(mycdev_irdistance1.class)) {
                        return PTR_ERR(mycdev_irdistance1.class);
                }
        //创建设备
        /*struct class *cls, struct device *parent,
                                dev_t devt, void *drvdata,
                                const char *fmt, ...);*/
                mycdev_irdistance1.device = device_create(mycdev_irdistance1.class, NULL, mycdev_irdistance1.devid ,NULL ,"mycdev_irdistance1");
                if (IS_ERR(mycdev_irdistance1.device)) {
                                return PTR_ERR(mycdev_irdistance1.device);
                        }

        //申请gpio中断   PA 20   10 9
        mycdev_irdistance1.gpio=20;
        gpio_is_valid(mycdev_irdistance1.gpio);
        gpio_request(mycdev_irdistance1.gpio,"ir_distance_cdev");
        gpio_direction_input(mycdev_irdistance1.gpio);
        gpio_set_value(mycdev_irdistance1.gpio, 0);
        mycdev_irdistance1.irq= gpio_to_irq(mycdev_irdistance1.gpio);      
        ret = request_irq(mycdev_irdistance1.irq,gpio_ir_distance_Handler ,IRQF_TRIGGER_FALLING, "gpio_ir_distance_interrupt", NULL); 
  	if(ret) {
		printk(KERN_ERR "can not get irq\n");
		return ret;
	} ;




      
        
        //创建成功
        printk("主设备号：%d\n",mycdev_irdistance1.test_ma);
        printk("次设备号 ：%d\n",mycdev_irdistance1.test_mi);
        printk("init ok\n");
        return 0;
   
  
        add_fail:
              unregister_chrdev_region(mycdev_irdistance1.devid, 1);
        failregist:

        return 0;
}

/*模块出口函数*/

static void __exit newcdev_exit(void)
{
        //disable_irq(mycdev_irdistance1.irq);
        free_irq(mycdev_irdistance1.irq, NULL);
        gpio_free(mycdev_irdistance1.gpio);
        cdev_del(&mycdev_irdistance1.newcdev);
	unregister_chrdev_region(mycdev_irdistance1.devid, 1);
        device_destroy(mycdev_irdistance1.class,mycdev_irdistance1.devid);
        class_destroy(mycdev_irdistance1.class);


   printk("rmmod ok\n");             
             
}

/*模块出入口函数*/
module_init(newcdev_init);
module_exit(newcdev_exit);
MODULE_LICENSE("GPL");