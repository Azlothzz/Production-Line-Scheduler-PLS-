#include "scheduler.h"
#include "order.h"
#include "util.h"
#include <fcntl.h>
#include <unistd.h>
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) > (y) ? (y) : (x))

#define NUM_PLANTS 3
int CURR_ID = 0;
char GLOBAL_START_DATE[11] = "2000-01-01";
char GLOBAL_END_DATE[11] = "2099-12-31";
// plant schedule order
schedule_order *plant_order_list[NUM_PLANTS];
int plant_order_list_size = 0;
int plant_capacity[3] = {300, 400, 500};
// reject order
list *reject_orders_head = NULL, *reject_orders_tail = NULL;
int reject_orders_counter = 0;
// all order
list *total_order_list_head = NULL, *total_order_list_tail = NULL;
int total_order_counter = 0;

//fcfs scheduler
void fcfs() {
    order *order_arr[total_order_counter];
    list *p = total_order_list_head;
     int i,j;
    for (i = 0; i < total_order_counter; i++) {
        order_arr[i] = p->order;
        p = p->next;
    }
    int cnt[3] = {0, 0, 0}, quantity, days, days1;
    for (i = 0; i < total_order_counter; i++) {
        quantity = order_arr[i]->quantity;
        days = days_diff(GLOBAL_START_DATE, order_arr[i]->due_date);
        days1 = days_diff(order_arr[i]->due_date, GLOBAL_END_DATE);
        if (days < 0 || days1 < 0 || 300 * (days+1 - cnt[0]) + 400 * (days+1 - cnt[1]) + 500 * (days+1 - cnt[2]) < quantity) {
            add_to_list(&reject_orders_head, &reject_orders_tail, &reject_orders_counter, order_arr[i]);
        } else {
            for (j=0; quantity > 0;) {
                if((cnt[0]<cnt[1]&&cnt[0]<cnt[2])||(cnt[0]==cnt[1]&&cnt[0]<cnt[2]||cnt[0]==cnt[2])){
                    j=0;
                }
                if((cnt[1]<cnt[0]&&cnt[1]<cnt[2])||(cnt[1]<cnt[0]&&cnt[1]==cnt[2])){
                    j=1;
                }
                if(cnt[2]<cnt[1]&&cnt[2]<cnt[0]){
                    j=2;
                }
                plant_order_list[j % NUM_PLANTS][cnt[j % NUM_PLANTS]].ord = order_arr[i];
                plant_order_list[j % NUM_PLANTS][cnt[j % NUM_PLANTS]].quantity = MIN(plant_capacity[j % NUM_PLANTS], quantity);
                quantity -= plant_capacity[j % NUM_PLANTS];
                cnt[j % NUM_PLANTS]++;
            }
        }
    }
}
// sjf scheduler
void sjf() {
    order *order_arr[total_order_counter];
    list *p = total_order_list_head;
     int i,j;
    for (i = 0; i < total_order_counter; i++) {
        order_arr[i] = p->order;
        p = p->next;
    }
    // sort by quantity
    for(i=0;i<total_order_counter-1;i++){
        for(j=0;j<total_order_counter-1-i;j++){
            if(order_arr[j]->quantity>order_arr[j+1]->quantity){
                order *tmp=order_arr[j];
                order_arr[j]=order_arr[j+1];
                order_arr[j+1]=tmp;
            }
        }
    }
    int cnt[3] = {0, 0, 0}, quantity, days, days1;
    for (i = 0; i < total_order_counter; i++) {
        quantity = order_arr[i]->quantity;
        days = days_diff(GLOBAL_START_DATE, order_arr[i]->due_date);
        days1 = days_diff(order_arr[i]->due_date, GLOBAL_END_DATE);
        if (days < 0 || days1 < 0 || 300 * (days+1 - cnt[0]) + 400 * (days+1 - cnt[1]) + 500 * (days+1 - cnt[2]) < quantity) {
            add_to_list(&reject_orders_head, &reject_orders_tail, &reject_orders_counter, order_arr[i]);
        } else {
            for (j=0; quantity > 0;) {
                if((cnt[0]<cnt[1]&&cnt[0]<cnt[2])||(cnt[0]==cnt[1]&&cnt[0]<cnt[2]||cnt[0]==cnt[2])){
                    j=0;
                }
                if((cnt[1]<cnt[0]&&cnt[1]<cnt[2])||(cnt[1]<cnt[0]&&cnt[1]==cnt[2])){
                    j=1;
                }
                if(cnt[2]<cnt[1]&&cnt[2]<cnt[0]){
                    j=2;
                }
                plant_order_list[j % NUM_PLANTS][cnt[j % NUM_PLANTS]].ord = order_arr[i];
                plant_order_list[j % NUM_PLANTS][cnt[j % NUM_PLANTS]].quantity = MIN(plant_capacity[j % NUM_PLANTS], quantity);
                quantity -= plant_capacity[j % NUM_PLANTS];
                cnt[j % NUM_PLANTS]++;
            }
        }
    }
}
// add period
void addPERIOD(char *start_date, char *end_date) {
     int i;
    if (is_valid_date(start_date) && is_valid_date(end_date)) {
        strcpy(GLOBAL_START_DATE, start_date);
        strcpy(GLOBAL_END_DATE, end_date);
        plant_order_list_size = days_diff(start_date, end_date)+1;
        for (i = 0; i < NUM_PLANTS; i++) {
            plant_order_list[i] = (schedule_order *) malloc(sizeof(schedule_order) * plant_order_list_size);
            memset(plant_order_list[i],0,sizeof(schedule_order) * plant_order_list_size);
        }
    }
}
// add a order
void addORDER(char *order_num, char *due_date, int quantity, char *product_name) {
    order *new_order = (order *) malloc(sizeof(order));
    strcpy(new_order->order_num, order_num);
    strcpy(new_order->due_date, due_date);
    strcpy(new_order->product_name, product_name);
    new_order->quantity = quantity;
    new_order->id = CURR_ID++;
    add_to_list(&total_order_list_head, &total_order_list_tail, &total_order_counter, new_order);
}
// Execute instructions in file
void addBATCH(char *file) {
    FILE *fp = fopen(file, "r");
    if(fp!=NULL){
        char line[200];
        while (fgets(line, sizeof(line), fp) != NULL) {
            line[strcspn(line, "\n")] = '\0';
//            printf("%s\n",line);
            process(line, false);
        }
        fclose(fp);
    }
}

// run algorithm scheduler
void runPLS(char *algorithm) {
    release_list(reject_orders_head);
    reject_orders_counter = 0;
     int i;
    for (i = 0; i < NUM_PLANTS; i++) {
        free(plant_order_list[i]);
        plant_order_list[i] = (schedule_order *) malloc(sizeof(schedule_order) * plant_order_list_size);
        memset(plant_order_list[i],0,sizeof(schedule_order) * plant_order_list_size);
    }
    if (strcmp(algorithm, "FCFS") == 0) {
        fcfs();
    } else if (strcmp(algorithm, "SJF") == 0) {
        sjf();
    } else {
    }
}
// analyze module
void report(char *algorithm) {
    char date_str[11];// "yyyy-mm-dd" + '\0'
    char plant_s[]={'X','Y','Z'};
    int produced[] = {0,0,0}, days_used[3]={0,0,0};
     int i,j,k,quantity,days;
    for (i = 0; i < NUM_PLANTS; i++) {
        printf("Plant_%c (%d per day)\n", plant_s[i], plant_capacity[i]);
        printf("%s to %s\n", GLOBAL_START_DATE, GLOBAL_END_DATE);
        printf("Date          Product Name   Order Number   Quantity(Produced)   Due Date\n");
        time_t start = date_to_timestamp(GLOBAL_START_DATE);
        strftime(date_str, sizeof(date_str), "%Y-%m-%d", localtime(&start));
        for (j = 0; j < plant_order_list_size; j++) {
            time_t now=start+j*60*60*24;
            strftime(date_str, sizeof(date_str), "%Y-%m-%d", localtime(&now));
            if (plant_order_list[i][j].ord == NULL) {
                printf("%-11s   NA\n", date_str);
            } else {
                printf("%-11s   %-12s   %-12s   %18d   %s\n", date_str,
                       plant_order_list[i][j].ord->product_name,
                       plant_order_list[i][j].ord->order_num,
                       plant_order_list[i][j].quantity,
                       plant_order_list[i][j].ord->due_date);
                produced[i]+=plant_order_list[i][j].quantity;
                days_used[i]++;
            }
        }
        printf("\n");
    }
    printf("\n");
    printf("***PLS Schedule Analysis Report***\n\n");
    printf("Algorithm used: %s\n\n", algorithm);
    printf("There are %d Orders ACCEPTED.  Details are as follows:\n\n", total_order_counter - reject_orders_counter);
    printf("ORDER NUMBER   START         END             DAYS     QUANTITY     PLANT\n");
    printf("===========================================================================\n");
    i = 0, j = 0, k = 0, quantity, days;
    char start_date[11], end_date[11];
    time_t start = date_to_timestamp(GLOBAL_START_DATE), st, ed;
    while (i < plant_order_list_size ||
           j < plant_order_list_size ||
           k < plant_order_list_size) {

        while(i < plant_order_list_size&&plant_order_list[0][i].ord==NULL){
            i++;
        }
        if(i<plant_order_list_size){
            quantity = plant_order_list[0][i].quantity;
            days = 1;
            for (i++; i < plant_order_list_size&&plant_order_list[0][i].ord == plant_order_list[0][i - 1].ord; i++, days++) {
                quantity += plant_order_list[0][i].quantity;
            }
            if(i<=plant_order_list_size&&plant_order_list[0][i - 1].ord!=NULL){
                st = start + (i - days - 1) * 60 * 60 * 24;
                ed = start + (i - 1) * 60 * 60 * 24;
                strftime(start_date, sizeof(start_date), "%Y-%m-%d", localtime(&(st)));
                strftime(end_date, sizeof(end_date), "%Y-%m-%d", localtime(&(ed)));
                printf("%-11s   %11s   %11s  %8d     %8d     %s\n", plant_order_list[0][i - 1].ord->order_num, start_date, end_date, days, quantity, "PLANT_X");
                if(i==plant_order_list_size){
                    i++;
                }
            }
        }


        while(j < plant_order_list_size&&plant_order_list[0][i].ord==NULL){
            j++;
        }
        if(j<plant_order_list_size){
            quantity = plant_order_list[1][j].quantity;
            days = 1;
            for (j++; j < plant_order_list_size&&plant_order_list[1][j].ord == plant_order_list[1][j - 1].ord; j++, days++) {
                quantity += plant_order_list[1][j].quantity;
            }
            if(j<=plant_order_list_size&&plant_order_list[0][j - 1].ord!=NULL) {
                st = start + (j - days - 1) * 60 * 60 * 24;
                ed = start + (j - 1) * 60 * 60 * 24;
                strftime(start_date, sizeof(start_date), "%Y-%m-%d", localtime(&(st)));
                strftime(end_date, sizeof(end_date), "%Y-%m-%d", localtime(&(ed)));
                printf("%-11s   %11s   %11s  %8d     %8d     %s\n", plant_order_list[1][j - 1].ord->order_num, start_date, end_date, days, quantity, "PLANT_Y");
                if(j==plant_order_list_size){
                    j++;
                }
            }
        }

        while(k < plant_order_list_size&&plant_order_list[0][i].ord==NULL){
            k++;
        }
        if(k<plant_order_list_size){
            quantity = plant_order_list[2][k].quantity;
            days = 1;
            for (k++; k < plant_order_list_size&&plant_order_list[2][k].ord == plant_order_list[2][k - 1].ord; k++, days++) {
                quantity += plant_order_list[2][k].quantity;
            }
            if(k<=plant_order_list_size&&plant_order_list[0][k - 1].ord!=NULL) {
                st = start + (k - days - 1) * 60 * 60 * 24;
                ed = start + (k - 1) * 60 * 60 * 24;
                strftime(start_date, sizeof(start_date), "%Y-%m-%d", localtime(&(st)));
                strftime(end_date, sizeof(end_date), "%Y-%m-%d", localtime(&(ed)));
                printf("%-11s   %11s   %11s  %8d     %8d     %s\n", plant_order_list[2][k - 1].ord->order_num, start_date, end_date, days, quantity, "PLANT_Z");
                if(k==plant_order_list_size){
                    k++;
                }
            }
        }
    }
    printf("\n- End -\n\n");
    printf("===========================================================================\n");


    printf("\n\n");
    printf("There are %d Orders REJECTED.  Details are as follows: \n\n", reject_orders_counter);
    printf("ORDER NUMBER   PRODUCT NAME         Due Date             QUANTITY\n");
    printf("===========================================================================\n");
    list *p = reject_orders_head;
    while (p != NULL) {
        printf("%-12s   %-12s         %-11s          %d\n", p->order->order_num, p->order->product_name, p->order->due_date, p->order->quantity);
        p = p->next;
    }
    printf("\n- End -\n\n");
    printf("===========================================================================\n\n");
    printf("***PERFORMANCE \n\n");
    printf("Plant_X:\n");
    printf("        Number of days in use:               %7d days\n", days_used[0]);
    printf("        Number of products produced:         %7d (in total)\n", produced[0]);
    printf("        Utilization of the plant:            %5.1lf %%\n\n", produced[0] * 100. / plant_order_list_size / plant_capacity[0]);
    printf("Plant_Y:\n");
    printf("        Number of days in use:               %7d days\n", days_used[1]);
    printf("        Number of products produced:         %7d (in total)\n", produced[1]);
    printf("        Utilization of the plant:            %5.1lf %%\n\n", produced[1] * 100. / plant_order_list_size / plant_capacity[1]);
    printf("Plant_Z:\n");
    printf("        Number of days in use:               %7d days\n", days_used[2]);
    printf("        Number of products produced:         %7d (in total)\n", produced[2]);
    printf("        Utilization of the plant:            %5.1lf %%\n\n", produced[2] * 100. / plant_order_list_size / plant_capacity[2]);

    int total = produced[0] + produced[1] + produced[2];
    printf("Overall of utilization:                      %5.1lf %%\n", total * 100. / plant_order_list_size / (plant_capacity[0] + plant_capacity[1] + plant_capacity[2]));


}
// exec command
void process(char *line, bool add_batch) {

    const char delim[] = " ";
    char *tokens[10]={NULL};        
    int count = 0;

    // use strtok() split string
    char *token = strtok(line, delim);

    // get all split string
    while (token != NULL && count < 10) {
        tokens[count] = token;
        count++;
        token = strtok(NULL, delim);
    }

    if (strcmp(tokens[0], "addPERIOD") == 0) {
        addPERIOD(tokens[1], tokens[2]);
    } else if (strcmp(tokens[0], "addORDER") == 0) {
        int quantity=strtol(tokens[3], NULL, 10);
        addORDER(tokens[1], tokens[2], quantity, tokens[4]);
    } else if (strcmp(tokens[0], "addBATCH") == 0 && add_batch) {
        addBATCH(tokens[1]);
    } else if (strcmp(tokens[0], "runPLS") == 0) {
        runPLS(tokens[1]);
        if (strcmp(tokens[3], "printREPORT") == 0) {
            pid_t pid=0;
            pid = fork();
            if (pid == 0) {
                if (tokens[5] != NULL) {
                    freopen(tokens[5],"w",stdout);
                    report(tokens[1]);
                    fclose(stdout);
                }else{
                    report(tokens[1]);
                };
                release();
                exit(EXIT_SUCCESS);
            }
        }
    } else {
        printf("Command not found\n");
    }
}
// release memory
void release() {
    list *p = total_order_list_head;
    if (p == NULL) {
        return;
    }
    list *p1 = p->next;
    while (p1 != NULL) {
        free(p->order);
        free(p);
        p = p1;
        p1 = p1->next;
    }
    free(p->order);
    free(p);
    release_list(reject_orders_head);
     int i;
    for (i = 0; i < NUM_PLANTS; i++) {
        free(plant_order_list[i]);
    }
}

void scheduler_main(int *pipefd) {
    char line[120];
    close(pipefd[1]);
    while (true) {
        memset(line,0,120*sizeof(char));
        read(pipefd[0], line, sizeof(line));
        if (strcmp(line, "exitPLS") == 0) {
            printf(" Bye-bye! \n");
            break;
        }
        process(line, true);
    }
    close(pipefd[0]);
    release();
    // wait children proc end
    while ((waitpid(-1, NULL, WNOHANG)) > 0)
        ;
    exit(EXIT_SUCCESS);
}