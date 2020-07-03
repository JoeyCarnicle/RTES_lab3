/****************************************************************************/
/* Re-work of the POSIX message queue demo from VxWorks given by Sam Seiwart*/
/*									    */                                                                        
/* Joey Carnicle                                                  	    */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
                                                                    
//#include "msgQLib.h"
//#include "mqueue.h"
//#include "errnoLib.h" 
//#include "ioLib.h" 

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <mqueue.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SNDRCV_MQ "/send_receive_mq"
#define MAX_MSG_SIZE 128
#define ERROR (-1)

struct mq_attr mq_attr;

static char canned_msg[] = "I CAN SEND A MESSAGE THROUGH A MESSAGE QUEUE, WOOOOOOOO";

void receiver(void)
{
  mqd_t mymq;
  char buffer[MAX_MSG_SIZE];
  int prio;
  ssize_t nbytes;

  mymq = mq_open(SNDRCV_MQ, O_RDONLY);

  if(mymq == (mqd_t)ERROR)
    perror("receiver mq_open");
    //printf("receiver mq_open Error: %s.\n", strerror(errno));

  /* read oldest, highest priority msg from the message queue */
  if((nbytes = mq_receive(mymq, buffer, MAX_MSG_SIZE, &prio)) == ERROR)
  {
    perror("receiver mq_receive");
  }
  else
  {
    buffer[nbytes] = '\0';
    printf("receive: msg %s received with priority = %d, length = %d\n",
           buffer, prio, nbytes);
  }
    
}

void sender(void)
{
  mqd_t mymq;
  int prio;
  ssize_t nbytes;

  mymq = mq_open(SNDRCV_MQ, O_CREAT | O_WRONLY, 0644, &mq_attr);

  if(mymq == (mqd_t)ERROR)
    perror("sender mq_open");

  /* send message with priority=30 */
  if((nbytes = mq_send(mymq, canned_msg, sizeof(canned_msg), 30)) == ERROR)
  {
    perror("sender mq_send");
  }
  else
  {
    printf("send: message successfully sent\n");
  }
  
}


void main(void)
{

  /* setup common message q attributes */
  mq_attr.mq_maxmsg = 100;
  mq_attr.mq_msgsize = MAX_MSG_SIZE;

  mq_attr.mq_flags = 0;


  sender();
  receiver();

   
}
