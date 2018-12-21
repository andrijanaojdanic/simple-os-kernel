#ifndef _LOCK_H_
#define _LOCK_H_


#define lock asm cli;
#define unlock asm sti;

class Lock{
public:
static volatile unsigned lock_flag;
static volatile unsigned switch_on_demand;
};

#endif
