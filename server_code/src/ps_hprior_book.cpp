                                        //********VERIFIED AND CORRECTNESS PROVED*****

#include "../inc/header.h"

int ps_hprior_req(int new_fd , struct book_info * booking_data ,struct ser_user_db * user_info , struct resource_node * pass_data )
{
                    
                     //PROCESSING THE REQUEST WHEN DONE WITH HIGH PRIORITY
              return send_admin(new_fd , booking_data ,user_info , pass_data,HIGH_PRIOR_BOOK);                         //SENDING TO ADMIN FOR HIGH PRIORITY REQUESTS        
}

