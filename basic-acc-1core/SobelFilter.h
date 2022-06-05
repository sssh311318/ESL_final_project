#ifndef SOBEL_FILTER_H_
#define SOBEL_FILTER_H_
#include <systemc>
#include <cmath>
#include <iomanip>
using namespace sc_core;
#include "time.h"
#include <tlm>
#include <tlm_utils/simple_target_socket.h>

#include "filter_def.h"

struct SobelFilter : public sc_module {
  tlm_utils::simple_target_socket<SobelFilter> tsock;

  sc_fifo<int> i_r;

  sc_fifo<int> o_result;

  SC_HAS_PROCESS(SobelFilter);

  SobelFilter(sc_module_name n): 
    sc_module(n), 
    tsock("t_skt"), 
    base_offset(0) 
  {
    tsock.register_b_transport(this, &SobelFilter::blocking_transport);
    SC_THREAD(do_filter);
  }

  ~SobelFilter() {
	}

  int temp[41] = {0};
  unsigned int base_offset;
  void cocktail_sort(int arr[], int len) {
	int i, left = 0, right = len - 1;
	int temp;
	while (left < right) {

		for (i = left; i < right; i++){
			if (arr[i] > arr[i + 1]) {
				temp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = temp;
      }
    }
		right--;
		for (i = right; i > left; i--){
			if (arr[i - 1] > arr[i]) {
				temp = arr[i];
				arr[i] = arr[i - 1];
				arr[i - 1] = temp;
      }
		}
		left++;
	}
  wait(10*CLOCK_PERIOD, SC_NS);
	}
  int binary_serach(int arr[],int key){
		int first, last, middle;
		int size = 40;
   first = 0;
   last = size - 1;
   middle = (first+last)/2;
   while (first <= last) {
      if (arr[middle] < key){
         first = middle + 1;
      }    
      else if (arr[middle] == key) {
         printf("Element found at index %d.\n",middle);
         break;
      }
      else {
         last = middle - 1;
      }
      middle = (first + last)/2;
   }
   wait(10*CLOCK_PERIOD, SC_NS);
   if (first > last)
      printf("Element Not found in the list.\n");
   
  return key;
	}
  void do_filter(){
    { wait(CLOCK_PERIOD, SC_NS); }
    while (true) {
        for (int i = 0; i < 41; i ++) {
          int value = i_r.read();
          temp[i] = value;
          wait(10*CLOCK_PERIOD, SC_NS);
        }
        printf("before sorted\n");
        for (int i = 0 ; i < 40 ; i++){
          printf(" %d ",temp[i]);
        }
        
        /*
        srand( time(NULL) );

  	//指定亂數範圍 
  	    int min = 0;
  	    int max = 19;

  //產生 [min , max] 的整數亂數 
  	    int index = rand() % (max - min + 1) + min;
	      int key = temp[index];
        */
        int key = temp[40];
        cocktail_sort(temp, 40);
        printf("\n after the sorted \n");
      for (int i = 0 ; i < 40 ; i++){
        printf(" %d ",temp[i]);
      }
      printf("\n");
    
      int result = binary_serach(temp, key);
      // cout << (int)result << endl;

      o_result.write(result);
    }
  }

  void blocking_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay){
    wait(delay);
    // unsigned char *mask_ptr = payload.get_byte_enable_ptr();
    // auto len = payload.get_data_length();
    tlm::tlm_command cmd = payload.get_command();
    sc_dt::uint64 addr = payload.get_address();
    unsigned char *data_ptr = payload.get_data_ptr();

    addr -= base_offset;


    // cout << (int)data_ptr[0] << endl;
    // cout << (int)data_ptr[1] << endl;
    // cout << (int)data_ptr[2] << endl;
    word buffer;

    switch (cmd) {
      case tlm::TLM_READ_COMMAND:
        // cout << "READ" << endl;
        switch (addr) {
          case SOBEL_FILTER_RESULT_ADDR:
            buffer.uint = o_result.read();
            break;
          default:
            std::cerr << "READ Error! SobelFilter::blocking_transport: address 0x"
                      << std::setfill('0') << std::setw(8) << std::hex << addr
                      << std::dec << " is not valid" << std::endl;
          }
        data_ptr[0] = buffer.uc[0];
        data_ptr[1] = buffer.uc[1];
        data_ptr[2] = buffer.uc[2];
        data_ptr[3] = buffer.uc[3];
        break;
      case tlm::TLM_WRITE_COMMAND:
        // cout << "WRITE" << endl;
        switch (addr) {
          case SOBEL_FILTER_R_ADDR:
            i_r.write(data_ptr[0]);

            break;
          default:
            std::cerr << "WRITE Error! SobelFilter::blocking_transport: address 0x"
                      << std::setfill('0') << std::setw(8) << std::hex << addr
                      << std::dec << " is not valid" << std::endl;
        }
        break;
      case tlm::TLM_IGNORE_COMMAND:
        payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
        return;
      default:
        payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
        return;
      }
      payload.set_response_status(tlm::TLM_OK_RESPONSE); // Always OK
  }
};
#endif
