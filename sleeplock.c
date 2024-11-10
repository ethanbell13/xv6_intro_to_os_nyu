// Sleeping locks

#include "types.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "sleeplock.h"

// #define RESOURCE_NAME_SIZE 10

void initsleeplock(struct sleeplock *lk, char *name)
{
  initlock(&lk->lk, "sleep lock");
  lk->name = name;
  lk->locked = 0;
  lk->pid = 0;
}

void acquiresleep(struct sleeplock *lk)
{
  // int i;
  acquire(&lk->lk);
  while (lk->locked)
  {
    // cprintf("%d is waiting for lock %s", lk->lk.cpu->proc->pid);
    // for (i = 0; i < RESOURCE_NAME_SIZE; i++)
    //   cprintf("%c", lk->name[i]);
    // cprintf("\n");
    sleep(lk, &lk->lk);
  }
  lk->locked = 1;
  lk->pid = proc->pid;
  release(&lk->lk);
}

void releasesleep(struct sleeplock *lk)
{
  acquire(&lk->lk);
  lk->locked = 0;
  lk->pid = 0;
  wakeup(lk);
  release(&lk->lk);
}

int holdingsleep(struct sleeplock *lk)
{
  int r;

  acquire(&lk->lk);
  r = lk->locked;
  release(&lk->lk);
  return r;
}
