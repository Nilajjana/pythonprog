#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int arr[100];
void fileasc()
{
    FILE* file=fopen(ascending.txt,"w");
    count =70;
	for (int i = 0; i < count; i++){
		fprintf(file, "%d\n", i);
        arr[i]=i;
	}
	fclose(file);
}
void filedec()
{
    FILE* file=fopen(descending.txt,"w");
    count =70;
	for (int i = 0; i < count; i++){count--;
		fprintf(file, "%d\n", count);
        arr[i]=count;
	}
	fclose(file);
}
void filerand(int count) {
	FILE* file = fopen(random.txt, "w");
	srand(time(NULL));
    int a;
	for (int i = 0; i < count; i++){a=rand()%1000;
		fprintf(file, "%d\n", a);
        arr[i]=a;
	}
	fclose(file);
}
int quicksort()
{

}
int partition()
{

}
int main()
{
    printf("enter your choice\n 1. Ascending data\n 2.Decending data\n 3.Random data \n4.Error Exit\n");
    int ch;
    scanf("%d",&ch);
    Switch(ch)
    {
        case 1:printf("the data from ascending order file is");
               fileasc();
               int q=69;
               int p=0;
               quicksort(int p,int q);
               fileascwrt();
        case 2:printf("the data from decending order file is");
               filedec();
               int q=69;
               int p=0;
               quicksort(int p,int q);
               filedecwrt();
        case 3:printf("the data from random order file is but pls enter no of elements");
               int a;
               scanf("%d",&a);
               filerand(a);
               int p=0;
               int q=(a-1);
               quicksort(int p,int q);
        Default:
    }
}
