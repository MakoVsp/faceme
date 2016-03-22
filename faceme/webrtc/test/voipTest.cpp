#include <stdio.h>
#include <stdlib.h>
#include   <iostream>

#include "../interface/media_stream.h"
#define MAX_MEDIA_CNT 2
#define RTP_PORT 8000

using   namespace   std   ;
void clientlog(const char*c)
{
    printf("%s\n",c);
}

int main( int argc, char * argv[] )
{
    /*  if( argc < 4 ) {
        printf("Please input command, dest_host, dest_port, local_port sequencially !\n");
        return -1;
    }
    */
    int icomd = 0;
    int ret;
    bool check = false;

    //init_media(check);

    // ret = start_audio( 0, "iLBC", 8000, argv[1], atoi( argv[2] ), atoi( argv[3] ) );

    //ret = stop_yuanxin_audio();
    //  int ret = record_audio( 0, "PCMU", 8000, argv[1], atoi( argv[2] ), atoi( argv[3] ) );
    // printf( "ret = %d \n", ret );
    //  while(true){
    ret = record_audio(102,"ILBC",8000,"124.192.38.9",12000,10000);
    ret = stop_audio();
    return 0;
    // scanf("icomd =%d ",&icomd);
    while(1)
    {

        printf( "start_audio------------------------------------------------------------------1 \n");
        //ret = start_audio(102,"ILBC",8000,"124.192.38.9",12000,"10.101.126.234",10000);

        ret = record_audio(102,"ILBC",8000,"124.192.38.9",12000,10000);

        printf( "start_audio---------------------------------------------------------------ret : %d \n",ret);

        //sleep(3);
        ret = stop_audio();
      //  sleep(1);
        /* cout<<"请出入命令:\n";
        cin>>icomd;
        cout<<icomd<<"End\n";
        switch(icomd)
        {
        case 1:
        {
            printf( "start_audio------------------------------------------------------------------1 \n");
            ret = start_audio(102,"ILBC",8000,"124.192.38.9",12000,"10.101.126.234",10000);
            printf( "start_audio---------------------------------------------------------------ret : %d \n",ret);

            sleep(3);
            ret = stop_audio();
        }
            break;
        case 2:
        {
            printf( "stop_audio------------------------------------------------------------------1 \n");

            ret = stop_audio();

            printf( "stop_audio--------------------------------------------------------------ret : %d \n",ret);
        }
            break;
        case 3:
        {
            init_media(check);
        }
            break;
        case 4:
        {
            destroy_media();
        }
            break;
        }*/
    }
    destroy_media();

    return 0;
}
