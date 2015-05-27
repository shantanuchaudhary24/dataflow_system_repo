                                //***VERIFIED AND CORRECT************

#include "../inc/header.h"

int ps_typical_req(int new_fd,struct book_info * booking_data,struct ser_user_db * user_info,struct resource_node * pass_data)  
{               
        
          //PROCESSING THE REQUEST WHEN DONE WITH REGULAR REQUEST       
          
          //FIRST CHECKING WHETHER THE REQUEST IS TO BE SENT TO THE ADMIN OR NOT
          struct resources req_resource = booking_data ->resource_data ;                                
          
          if(req_resource.audio_recording==true||req_resource.video_recording==true||out_of_time(booking_data)==true)
          {
                        return send_admin(new_fd , booking_data ,user_info , pass_data,TYPICAL_BOOK);                         //SENDING TO ADMIN FOR SPECIAL REQUESTS                 
          }
                                                         
         else
         {
                        return send_ser_ps(new_fd,booking_data,user_info,pass_data);                                                  //SENDING TO THE SERVER FOR NORMAL REQUESTS
         }        
         
}



