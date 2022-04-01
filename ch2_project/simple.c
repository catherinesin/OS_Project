#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/list.h>

static LIST_HEAD(birthday_list);

struct birthday{
       int day;
       int month;
       int year;
       struct list_head list;
};

void set_birthday(struct birthday *person, int day, int month, int year);

/* This function is called when the module is loaded. */
int simple_init(void){
       struct birthday *amy, *betty, *cathy, *daniel, *ethan;
       struct birthday *ptr;
       printk(KERN_INFO "Loading Module\n");

       amy = kmalloc(sizeof(*amy), GFP_KERNEL);
       betty = kmalloc(sizeof(*betty), GFP_KERNEL);
       cathy = kmalloc(sizeof(*cathy), GFP_KERNEL);
       daniel = kmalloc(sizeof(*daniel), GFP_KERNEL);
       ethan = kmalloc(sizeof(*ethan), GFP_KERNEL);

       set_birthday(amy, 31, 7, 1998);
       set_birthday(betty, 22, 6, 1985);
       set_birthday(cathy, 1, 2, 2000);
       set_birthday(daniel, 15, 12, 2002);
       set_birthday(ethan, 6, 8, 1995);

       list_for_each_entry(ptr, &birthday_list, list){
            printk(KERN_INFO "person day: %d, month: %d, year: %d \n", ptr->day, ptr->month, ptr->year);
       }
       return 0;
}

void set_birthday(struct birthday *person, int day, int month, int year){
       person->day = day;
       person->month = month;
       person->year = year;
       INIT_LIST_HEAD(&person->list);
       list_add_tail(&person->list, &birthday_list);
}

/* This function is called when the module is removed. */
void simple_exit(void){
       struct birthday *ptr, *next;
       printk(KERN_INFO "Removing Module\n");

       list_for_each_entry_safe(ptr, next, &birthday_list, list){
              printk(KERN_INFO "Removing day: %d, month: %d, year: %d \n", ptr->day, ptr->month, ptr->year);
              list_del(&ptr->list);
              kfree(ptr);
       }
}

/* Macros for registering module entry and exit points. */
module_init(simple_init); 
module_exit(simple_exit); 

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
