#include "../inc/global_header.h"
#include "../inc/admin_functions_header.h"
#include "../inc/aux_functions.h"
int room_counter = INITIALIZED;

void database_writer_pool( struct general_pool object )
{
	FILE *fp;
	fp=fopen("/home/shantanu/admin_files/database_pool.bin","w");
	fwrite( &object,sizeof(int), sizeof(struct general_pool), fp);
	fclose(fp);

}

void database_writer_config(struct book_info object)
{
	room_counter++;
	object.resources_data.room_ID = room_counter;
	FILE *fp;
	fp=fopen("/home/shantanu/admin_files/room_resources.bin","a");
	fwrite( &object, sizeof(int), sizeof(struct book_info), fp );
	fclose(fp);

}

int bool_to_int(bool var)
{
	if(var == true)
	return 1;
	else return 0;
}
