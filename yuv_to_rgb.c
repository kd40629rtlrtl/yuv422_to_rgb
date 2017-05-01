#include<stdio.h>
#include<stdlib.h> 

#define MAX_CAM_NUM 4
#define MAX_WIDTH 640	
#define MAX_HEIGHT 480

#define YUV_2_B(y, u) ((int)(y + 1.732446 * (u - 128))) 
#define YUV_2_G(y, u, v)((int)(y - 0.698001 * (u - 128) - 0.703125 * (v - 128)))
#define YUV_2_R(y, v)((int)(y + 1.370705  * (v - 128)))

int YUYVToBGR24_Native(unsigned char* pYUV,unsigned char* pBGR24,int width,int height)
{
    if (width < 1 || height < 1 || pYUV == NULL || pBGR24 == NULL)
        return 0;
    const long len = width * height;
    unsigned char* yData = pYUV;
    unsigned char* vData = pYUV;
    unsigned char* uData = pYUV;
    int y, x, k;

    int bgr[3];
    int yIdx,uIdx,vIdx,idx;
    for (y=0; y < height; y++)
	{
        for (x=0; x < width;x++)
		{
           	yIdx = 2*((y*width) + x);
            uIdx = 4*(((y*width) + x)>>1) + 1;
            vIdx = 4*(((y*width) + x)>>1) + 3;
            
            bgr[0] = YUV_2_B(yData[yIdx], uData[uIdx]); // b分量
			bgr[1] = YUV_2_G(yData[yIdx], uData[uIdx], vData[vIdx]); // g分量                                  
			bgr[2] = YUV_2_R(yData[yIdx], vData[vIdx]); // r分量 
            
			for (k = 0;k < 3;k++)
			{
                idx = (y * width + x) * 3 + k;
                if(bgr[k] >= 0 && bgr[k] <= 255)
                    pBGR24[idx] = bgr[k];
                else
                    pBGR24[idx] = (bgr[k] < 0)?0:255;
            }
        }
    }
    return 1;
}
void bmp_write(unsigned char *image_in, int width, int height)
{
	long file_size=(long)height*(long)width * 3 + 54;
	FILE *fp_out=fopen("output_bmp.bmp", "wb");
		
	unsigned char header[54]={
		0x42,
		0x4d,
		0, 0, 0, 0,
		0, 0,
		0, 0,
		54, 0, 0, 0,
		40, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		1, 0,
		24, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		};
	
	header[2]=(unsigned char)(file_size & 0x000000ff);
	header[3]=(file_size >> 8) & 0x000000ff;
	header[4]=(file_size >> 16) & 0x000000ff;
	header[5]=(file_size >> 24) & 0x000000ff;
	
	header[18]=width & 0x000000ff;
	header[19]=(width >> 8) & 0x000000ff;
	header[20]=(width >> 16) & 0x000000ff;
	header[21]=(width >> 24) & 0x000000ff;
	
	header[22]=height & 0x000000ff;
	header[23]=(height >> 8) & 0x000000ff;
	header[24]=(height >> 16) & 0x000000ff;
	header[25]=(height >> 24) & 0x000000ff;
	
	printf("%d %d\n", height, width);
	
	fwrite(header, sizeof(unsigned char), 54, fp_out);
	fwrite(image_in, sizeof(unsigned char), (size_t)(long)height*width*3, fp_out);
	
	fclose(fp_out);
}

int main()
{
	FILE *fp, *output_test_1_fp;
	fp = fopen("output_test.yuv", "rb");
	//output_test_1_fp = fopen("output_test_1.yuv\0", "w");
	
	//bgr_data and yuv_data init
	unsigned char *bgr_data = (unsigned char*)malloc(MAX_WIDTH*MAX_HEIGHT*MAX_CAM_NUM*3 * sizeof(unsigned char));
	
	unsigned char *yuv_data = (unsigned char*)malloc(MAX_WIDTH*MAX_HEIGHT*MAX_CAM_NUM*2 * sizeof(unsigned char)); //yuv_data為一維矩陣 
	fread(yuv_data, (MAX_WIDTH*MAX_HEIGHT*MAX_CAM_NUM*2 * sizeof(unsigned char)), 1, fp);//fill in yuv_data
	
	//convert YUYV to RGB24
	if(!(YUYVToBGR24_Native(yuv_data, bgr_data, MAX_WIDTH*MAX_CAM_NUM, MAX_HEIGHT))) return -1;
	
	//output RGB24 to bmp
	bmp_write(bgr_data, MAX_WIDTH*MAX_CAM_NUM, MAX_HEIGHT);
	
	//fwrite(yuv_data, (MAX_WIDTH*MAX_HEIGHT * MAX_CAM_NUM * 2), 1, output_test_1_fp);
	
	//fclose(output_test_1_fp);
	fclose(fp);
	return 0;
}
