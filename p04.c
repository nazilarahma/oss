/*
// Sun Jun 21 14:27:37 WIB 2020
// Tue Jun  9 17:46:47 WIB 2020
 */

// adjust DELAY and akunGitHub
#define  DELAY 5
#define  akunGitHub "nethaniasonya"
#include "p00.h"

char*    progs[]={P01, P02, P03, P04, P05};
myshare* mymap;

int init(void) {
    sleep(DELAY);

    struct stat size;
    int ssize=sizeof(myshare);
    int fd   =open(SHAREMEM, O_RDWR, CHMOD);

    fchmod   (fd, CHMOD);
    ftruncate(fd, ssize);
    mymap=mmap(NULL, ssize, MYPROTECTION, MYVISIBILITY, fd, 0);

    if(mymap == MAP_FAILED){
      printf("No \"SharedMemoryFile.bin\" file.\n");
      exit(1);
    }
    close(fd);
    return NOTBOSS;
}

char tmpStr[256]={};  // a temporary string.
void myprint(char* str1, char* str2) {
    printf("%s[%s]\n", str1, str2);
}

int getEntry(void) {
    int entry;
    sem_wait(&(mymap->mutex));
    entry = mymap->entry++;
    mymap->mutexctr++;
    mymap->progs[entry].stamp++;
    sem_post(&(mymap->mutex));
    return entry;
}

void display(int entry) {
    mymap->mutexctr++;
    mymap->progs[entry].stamp++;

    printf("%s[progs[%.2d] TIME[%.2d] MUTEX[%.2d] MMAP[%s] ", akunGitHub, entry, mymap->mutexctr, mymap->progs[entry].stamp, "OPEN");

    for (int i = 0; i < mymap->entry; i++)
    {
       if(mymap->progs[i].akun == NULL){
            printf("[]");
       } else {
          printf("[%s]", mymap->progs[i].akun);
        }
    }

    printf("]\n");
}

void putInfo(char* akun, int entry) {
    sem_wait(&(mymap->mutex));
    strcpy(mymap->progs[entry].akun, akun);
    mymap->mutexctr++;
    mymap->progs[entry].stamp++;
    sem_post(&(mymap->mutex));
}

void checkOpen(void) {
    if(mymap->state == CLOSED){
      printf("CLOSED, BYE BYE ==== ====\n");
      exit(0);
    }
}

int main(void) {
    sprintf(tmpStr, "START PID[%d] PPID[%d]", getpid(), getppid());
    myprint(akunGitHub, tmpStr);
    int boss=init();
    checkOpen();
    sleep (DELAY);
    
    int entry = getEntry();
    display(entry);
    
    putInfo(akunGitHub, entry);
    display(entry);

    sleep(DELAY);

    display(entry);

    myprint(akunGitHub, "BYEBYE =====  ===== =====");
}
