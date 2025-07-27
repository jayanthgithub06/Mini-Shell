#include "main.h"

//linking of the global variables
extern sll *head;
extern int status;
//global variable
int tstp_pid = 0;


//check for the commands which wants to resume the stopped processes
int check_for_commands_resume(char *input_string)
{
    //check for the fg command execute the first stopped process
    if(!strcmp(input_string,"fg"))
    {
        //if list is empty no process stopped available
        if(head == NULL)
        {
            printf("bash: fg: current: no such job\n");
            return SUCCESS;
        }
        //if available execute it
        sll *temp = head;
        head = head -> link;
        
        //resumable child pid
        tstp_pid = temp -> child_pid;
        strcpy(input_string,temp -> command);

        //continue the child process which was stopped first
        kill(tstp_pid,SIGCONT);
        
        //giving time for child switch its state
        sleep(2);

        //parent is waiting for the stopped child gets complete
        waitpid(tstp_pid,&status,WUNTRACED);

        tstp_pid = 0;
        return SUCCESS;

    }//check for the fg command execute the first stopped process
    else if(!strcmp(input_string,"bg"))
    {
        //if list is empty no process stopped available
        if(head == NULL)
        {
            printf("bash: fg: current: no such job\n");
            return SUCCESS;
        }
        
        //if available execute it
        sll *temp = head;
        head = head -> link;
        
        //continue the child process which was stopped first
        kill(tstp_pid,SIGCONT);

        return SUCCESS;
    }else if(!strcmp(input_string,"jobs"))
    {
        //for jobs we just have to print the stopped processes
        sll *temp = head;
        int l = 1;
        while(temp != NULL)
        {
            printf("[%d] stopped             %s\n",l,temp -> command);
            temp = temp -> link;
        }
        return SUCCESS;
    }
    return FAILURE;
}
