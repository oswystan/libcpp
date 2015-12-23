#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define KEY_FILE "/tmp/msgkey"
#define KEY_PJID 0xFF

static msgid = -1;

struct st_msg
{
    int value;
    char desc[64];
};
#if 0
struct msgbuf
{
    int msgtype;
    struct st_msg msg;
};
#endif

void sig_handler(int signo)
{
    switch(signo)
    {
        case SIGTERM:
        {
            printf("SIGTERM received\n");
            break;
        }
        case SIGINT:
        {
            printf("SIGINT received\n");
            break;
        }
    }

    if(msgid > 0)
    {
        msgctl(msgid, IPC_RMID, NULL);
        msgid = -1;
    }
}

void dump_msg(struct msgbuf* buf)
{
    if(!buf)
    {
        return;
    }

    printf("msgtype=[%d], value=[%d] desc=[%s]\n", buf->msgtype, buf->msg.value, buf->msg.desc);
}

static void server()
{
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
    key_t msgkey = ftok(KEY_FILE, KEY_PJID);
    if(-1 == msgkey)
    {
        perror("ftok");
        return;
    }
    msgid = msgget(msgkey, IPC_CREAT | IPC_EXCL | 0600);
    if(msgid < 0)
    {
        perror("msgget");
    }

    struct msgbuf buf;
    while(1)
    {
        memset(&buf, 0x00, sizeof(buf));
        if(msgrcv(msgid, &buf, sizeof(buf.msg), 0, 0) >= 0)
        {
            printf("Server: recevice msg:");
            dump_msg(&buf);
        }
        else
        {
            perror("fail to receive msg");
            break;
        }
    }
    if(msgid > 0)
    {
        msgctl(msgid, IPC_RMID, NULL);
        msgid = -1;
    }
    return;
}

static void client()
{
    key_t msgkey = ftok(KEY_FILE, KEY_PJID);
    if(-1 == msgkey)
    {
        perror("ftok\n");
        return;
    }
    msgid = msgget(msgkey, 0);
    if(msgid < 0)
    {
        perror("msgget");
    }

    struct msgbuf buf;
    int i = 0;
    while(i++<10)
    {
        memset(&buf, 0x00, sizeof(buf));
        sprintf(buf.msg.desc, "this is msg %d", i);
        buf.msg.value = i;
        buf.msgtype = 1;
        if(msgsnd(msgid, &buf, sizeof(buf.msg), 0) >= 0)
        {
        }
        else
        {
            perror("fail to snd msg");
            break;
        }
        sleep(1);
    }
    return;
}


int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s c|s\n", argv[0]);
        return 0;
    }
    switch(*argv[1])
    {
        case 'c':
        {
            client();
            printf("Client EXIT !!\n");
            break;
        }
        case 's':
        {
            server();
            printf("Server EXIT !!\n");
            break;
        }
        default:
        {
            printf("Unknow argument !\n");
            break;
        }
    }

    return 0;
}


