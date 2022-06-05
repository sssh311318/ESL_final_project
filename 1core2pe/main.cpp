#include "string"
#include "string.h"
#include "cassert"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include <iostream>
#include "time.h"
using namespace std;

int buff[41] = {0};
int buff0[21] = {0};
int buff1[21] = {0};
union word {
  int sint;
  unsigned int uint;
  unsigned char uc[4];
};
// Sobel Filter ACC
static char* const SOBELFILTER0_START_ADDR = reinterpret_cast<char* const>(0x73000000);
static char* const SOBELFILTER0_READ_ADDR  = reinterpret_cast<char* const>(0x73000004);
static char* const SOBELFILTER1_START_ADDR = reinterpret_cast<char* const>(0x74000000);
static char* const SOBELFILTER1_READ_ADDR  = reinterpret_cast<char* const>(0x74000004);

// DMA 
static volatile uint32_t * const DMA_SRC_ADDR  = (uint32_t * const)0x70000000;
static volatile uint32_t * const DMA_DST_ADDR  = (uint32_t * const)0x70000004;
static volatile uint32_t * const DMA_LEN_ADDR  = (uint32_t * const)0x70000008;
static volatile uint32_t * const DMA_OP_ADDR   = (uint32_t * const)0x7000000C;
static volatile uint32_t * const DMA_STAT_ADDR = (uint32_t * const)0x70000010;
static const uint32_t DMA_OP_MEMCPY = 1;

bool _is_using_dma = false;
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
void write_data_to_ACC(char* ADDR, unsigned char* buffer, int len){
  if(_is_using_dma){  
    // Using DMA 
    *DMA_SRC_ADDR = (uint32_t)(buffer);
    *DMA_DST_ADDR = (uint32_t)(ADDR);
    *DMA_LEN_ADDR = len;
    *DMA_OP_ADDR  = DMA_OP_MEMCPY;
  }else{
    // Directly Send
    memcpy(ADDR, buffer, sizeof(unsigned char)*len);
  }
}
void read_data_from_ACC(char* ADDR, unsigned char* buffer, int len){
  if(_is_using_dma){
    // Using DMA 
    *DMA_SRC_ADDR = (uint32_t)(ADDR);
    *DMA_DST_ADDR = (uint32_t)(buffer);
    *DMA_LEN_ADDR = len;
    *DMA_OP_ADDR  = DMA_OP_MEMCPY;
  }else{
    // Directly Read
    memcpy(buffer, ADDR, sizeof(unsigned char)*len);
  }
}

int main(int argc, char *argv[]) {
  printf("open the file\n");
  FILE *infp;
  infp = fopen( "stimulus.txt", "r" );
	  // Read stimulus values from file and send to DUT
    for( int i = 0; i < 40; i++ )
	    {      
    		fscanf(infp, "%d\n", &buff[i]);	
	    }
  fclose(infp);
    for( int i = 0 ; i < 20 ; i++){
      buff0[i] = buff[i];
      buff1[i] = buff[i + 20];
    }
  unsigned char  buffer0[4] = {0};
  unsigned char  buffer1[4] = {0};
  word data0,data1;
    printf("produce the key\n");
    srand( time(NULL) );

  	/* 指定亂數範圍 */
  	    int min = 0;
  	    int max = 39;

  /* 產生 [min , max] 的整數亂數 */
  	    int index = rand() % (max - min + 1) + min;
	      int key = buff[index];
        buff0[20] = key;
        buff1[20] = key;
        printf("the key is %d\n",key);
    printf("start transporting\n");
    /*for (int i = 0 ; i < 20 ; i++){
      printf(" %d ",buff[i]);
    }
    */

      for(int i = 0 ; i < 21 ; i++ ){
        buffer0[0] = buff0[i];
        buffer0[1] = 0;
        buffer0[2] = 0;
        buffer0[3] = 0;
        buffer1[0] = buff1[i];
        buffer1[1] = 0;
        buffer1[2] = 0;
        buffer1[3] = 0;
        //printf("times is %d",times);
        write_data_to_ACC(SOBELFILTER0_START_ADDR, buffer0, 4);
        write_data_to_ACC(SOBELFILTER1_START_ADDR, buffer1, 4);
        //times++;
        
      }
    read_data_from_ACC(SOBELFILTER0_READ_ADDR, buffer0, 4);
    read_data_from_ACC(SOBELFILTER1_READ_ADDR, buffer1, 4);

    memcpy(data0.uc, buffer0, 4);
    memcpy(data1.uc, buffer1, 4);
    return 0;
}
