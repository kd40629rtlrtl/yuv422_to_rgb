#include<stdio.h>
#include<stdlib.h> 

#define MAX_CAM_NUM 4
#define MAX_WIDTH 640	
#define MAX_HEIGHT 480

typedef struct yuv422_struct
{
	unsigned char Y;
	unsigned char C;
}yuv422, *yuv422_ptr;

int main()
{
	FILE *fp[MAX_CAM_NUM], *output_fp;
	int yuv_file_num;
	char *filename[MAX_CAM_NUM] = {"test0.yuv\0", "test1.yuv\0", "test2.yuv\0", "test3.yuv\0"};
	yuv422_ptr yuv_data[MAX_CAM_NUM];  
	yuv422_ptr output_yuv_file = (yuv422_ptr)malloc(MAX_WIDTH*MAX_HEIGHT * MAX_CAM_NUM * sizeof(yuv422));
	int y,x;
	
	for(yuv_file_num = 0; yuv_file_num<MAX_CAM_NUM; yuv_file_num++)
	{
		fp[yuv_file_num] = fopen(filename[yuv_file_num], "rb");
		if(!fp[yuv_file_num])
		{
			printf("file open error\n");
			return -1;
		}
	}
	printf("fopen over\n");
	
	for(yuv_file_num = 0; yuv_file_num<MAX_CAM_NUM; yuv_file_num++)
	{
		yuv_data[yuv_file_num] = (yuv422_ptr)malloc(MAX_WIDTH*MAX_HEIGHT * sizeof(yuv422)); //yuv_data¬°¤@ºû¯x°} 
		fread(yuv_data[yuv_file_num], (MAX_WIDTH*MAX_HEIGHT * sizeof(yuv422)), 1, fp[yuv_file_num]);
	}
	
	printf("fread over\n");
	output_fp = fopen("output_test.yuv\0", "w");
	
	for(yuv_file_num = 0; yuv_file_num<MAX_CAM_NUM; yuv_file_num++)
	{
		for(y=0; y<MAX_HEIGHT; y++)
		{
			for(x=0; x<MAX_WIDTH; x++)
			{
				output_yuv_file[(y*MAX_WIDTH*MAX_CAM_NUM) + x + yuv_file_num*MAX_WIDTH] = yuv_data[yuv_file_num][y*MAX_WIDTH + x];
			}
		}
	}
	
	fwrite(output_yuv_file, (MAX_WIDTH*MAX_HEIGHT * MAX_CAM_NUM * sizeof(yuv422)), 1, output_fp);
	
	for(yuv_file_num = 0; yuv_file_num<MAX_CAM_NUM; yuv_file_num++)
	{
		fclose(fp[yuv_file_num]);
	}
	fclose(output_fp);
	
	return 0;
}
