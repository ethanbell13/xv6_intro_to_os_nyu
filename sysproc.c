#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
int sys_fork(void)
{
  return fork();
}

int sys_exit(void)
{
  exit();
  return 0; // not reached
}

int sys_wait(void)
{
  return wait();
}

int sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int sys_getpid(void)
{
  return proc->pid;
}

int sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

int sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (proc->killed)
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
int sys_changenice(void)
{
  int pid, val;
  if (argint(0, &pid) < 0)
    return -1;
  if (argint(1, &val) < 0)
    return -1;
  return changenice(pid, val);
}
int sys_getnice(void)
{
  int pid;
  if (argint(0, &pid) < 0)
    return -1;
  return getnice(pid);
}
int sys_lock(void)
{
  int id;
  if (argint(0, &id) < 0)
    return -1;
  return lock(id);
}
int sys_resourcerelease(void)
{
  int id;
  if (argint(0, &id) < 0)
    return -1;
  return resourcerelease(id);
}
int sys_lockstate(void)
{
  int id;
  if (argint(0, &id) < 0)
    return -1;
  return lockstate(id);
}
int sys_pilock(void)
{
  int id;
  if (argint(0, &id) < 0)
    return -1;
  return pilock(id);
}
int sys_printtable(void)
{
  return printtable();
}