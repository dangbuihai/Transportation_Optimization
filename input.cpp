#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <malloc.h>
#include <math.h>
#include <string>

int actualLength(const char *string) {
//  return the actual lenght of a string (trailling spaces are not counted)
    while(isspace((unsigned char)string[0]))
        string++;
    char *final =(char*)malloc((strlen(string)+1)*sizeof(char));
    strcpy(final,string);
    int length = strlen(final);
    while(length > 0 && isspace((unsigned char)final[length-1]))
        length--;
    final[length-1] = '\0';
    return (strlen(final)+1);
}
/*====================================================================================================================*/
char* readline(int lineNumber,const char *txtFile){
/* return a string of a complete line #lineNumber in the file #txtFile 
with actual length (no spaces character at the end) */   
    char *buf=(char*)malloc(50*sizeof(char));
    int count =1;
    FILE *fileName;
    fileName = fopen(txtFile,"r");//open file #txtFile
    while (fgets(buf,50,fileName) != NULL){
    //get the wanted line by fgets all lines respectively from line #1 to lie #lineNumber
        if (count == lineNumber){
            break;
        }
        else{
            count++;
        }
    }
    
    char *line = (char*)malloc(actualLength(buf)*sizeof(char));
    strcpy(line,buf);
    //so this make 'line' contains a whole line with minimum size
    free (buf);
    fclose(fileName);
    
    return line;
    // delete line;
}
/*===================================================================================================================*/
int *getpos(int line, const char* filename){
//return the ordinate (int x and int y) which has the form [x;y] in a specific line
    static int pos[2]={NULL,NULL};
    int j;
    char*buf=(char*)malloc((strlen(readline(line, filename))+1)*sizeof(char));
    
    strcpy(buf,readline(line,filename));
    for (j=strlen(buf); j>0; j--){
        // find the ';' character in a line then before ';' is x and after ';' is y 
        if (buf[j]==';'){
            pos[0]= buf[j-1]-'0';
            pos[1]= buf[j+1]-'0';
            break;       
        }        
    }  
    free(buf);
    return pos;
}
/*===================================================================================================================*/
int count(const char *obj){
    // return number of object (driver or customer) in the input text file
    FILE *fp; 
    int count=0;  
    char c;  
    fp = fopen(obj, "r"); 
    
    if (fp == NULL) { 
         return 0; 
    } 
    
    for (c = getc(fp); c != EOF; c = getc(fp)){
        if (c == '=') 
        // increment count if this character is '=' 
        //(because I separate the object in text file by '=') 
            count =count + 1; 
    }  
    fclose(fp); 
    return count;
}
/*====================================================================================================================*/
typedef struct{
    char* name;
    char* phone;
    int cip[2];//customer's initial position
    int cfp[2];//customer's final position
}customer;
typedef struct{
    char* name;
    char* phone;
    char* number_plt;
    int dp[2];//driver's present position
}driver;
/*====================================================================================================================*/
customer* construct_c(int num,customer *c){
    //construct all the customer c[i]
    int i;
    for (i=0;i<num;i++){
        // in this session, the term k+5*i is the information in the 
        //line number #linenum congruence with k modulo 5
        //each driver's or customer's information store in 5 lines
        // so all #linenum in the same congruence group containing the same field
        // example for customer: names are in line {1,6,11}, phones are in line {2,7,12}, etc..
        c[i].name=readline((1+5*i),"customer.txt");
        c[i].phone=readline((2+5*i),"customer.txt");
        c[i].cip[0]=*getpos((3+5*i),"customer.txt");
        c[i].cip[1]=*(getpos((3+5*i),"customer.txt")+1);
        c[i].cfp[0]=*getpos((4+5*i),"customer.txt");
        c[i].cfp[1]=*(getpos((4+5*i),"customer.txt")+1);
    }
    return c;
}
/*==================================================================================================================*/
driver* construct_d(int num,driver *d){
    //construct all the driver d[i]
    int i;
    for (i=0;i<num;i++){
        d[i].name=readline((1+5*i),"driver.txt");
        d[i].phone=readline((2+5*i),"driver.txt");
        d[i].number_plt=readline((3+5*i),"driver.txt");
        d[i].dp[0]=*getpos((4+5*i),"driver.txt");
        d[i].dp[1]=*(getpos((4+5*i),"driver.txt")+1);
    }
    return d;
}
//====================================================================================================================
int** allocate(int number_of_row, int number_of_cols) {
	int** arr = (int**)malloc(number_of_row* sizeof(int*));
	for (int i = 0; i < number_of_row; ++i) {
		arr[i] = (int*)calloc(number_of_cols, sizeof(int));
	}
	if (arr == NULL)
	{
		printf_s("can not allocate memory...");
		exit(0);
	}
	return arr;
}
void print_mat(int** arr, int num_of_rows, int num_of_cols)
{
	printf_s("\n");
	for (int i = 0; i < num_of_rows; i++) {
		for (int j = 0; j < num_of_cols; j++) {
			printf("%-4d", arr[i][j]);
		}
		printf("\n");
	}
}
//===================================================================================================================
void get_cus_cip(int**cus_cip_mat,customer *c,int numOfCus){
    //put customers's initial position to cus_cip_mat which is allocated in main function
    for (int i=0;i<numOfCus;i++){
        cus_cip_mat[i][0]=c[i].cip[0];
        cus_cip_mat[i][1]=c[i].cip[1];
    }
}
void get_cus_cfp(int**cus_cfp_mat,customer *c,int numOfCus){
    //same
    for (int i=0;i<numOfCus;i++){
        cus_cfp_mat[i][0]=c[i].cfp[0];
        cus_cfp_mat[i][1]=c[i].cfp[1];
    }
}
void get_drv_dp(int**drv_dp_mat,driver *d,int numOfDrv){
    //same
    for (int i=0;i<numOfDrv;i++){
        drv_dp_mat[i][0]=d[i].dp[0];
        drv_dp_mat[i][1]=d[i].dp[1];
    }
}
//====================================================================================================================

// #include "InOutFiles.h"
int main(){
    int numOfCus=count("customer.txt");//count customers
    int numOfDrv=count("driver.txt");//count drivers
    
    customer *c=(customer*)calloc(numOfCus,sizeof(customer));
    // allocate memory for number of customers counted above
    driver *d=(driver*)calloc(numOfDrv,sizeof(driver));
    //same as above
    construct_c(numOfCus,c);
    construct_d(numOfDrv,d);
    int **cus_cip_mat=allocate(numOfCus,2);//allocate memory for cip mattrix
    get_cus_cip(cus_cip_mat,c,numOfCus);//get values for cip mattrix

    int **cus_cfp_mat=allocate(numOfCus,2);//same
    get_cus_cfp(cus_cfp_mat,c,numOfCus);//same

    int **drv_dp_mat=allocate(numOfDrv,2);//same
    get_drv_dp(drv_dp_mat,d,numOfDrv);//same

    
//print for test
    printf_s("so khach:%d\n",numOfCus);
    for(int i=0;i<numOfCus;i++){
        printf_s("ten:%s\n", c[i].name);
        printf_s("sdt:%s\n",c[i].phone);
        printf_s("don tai:%d\t%d\n",c[i].cip[0],c[i].cip[1]);
        printf_s("tra tai:%d\t%d\n",c[i].cfp[0],c[i].cfp[1]);
    }
    printf_s("so tai xe: %d\n",numOfDrv);
    for(int i=0;i<numOfDrv;i++){
        printf_s("ten:%s\n", d[i].name);
        printf_s("sdt:%s\n",d[i].phone);
        printf_s("bien so:%s\n", d[i].number_plt);
        printf_s("vi tri hien tai:%d\t%d\n",d[i].dp[0],d[i].dp[1]);
    }
    print_mat(cus_cip_mat,numOfCus,2);
    print_mat(cus_cfp_mat,numOfCus,2);
    print_mat(drv_dp_mat,numOfDrv,2);
    
    return 0;
}