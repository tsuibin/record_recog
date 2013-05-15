#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>

char *post_req= "POST /myup/upload.php HTTP/1.1\r\n"
                             "Host: 192.168.1.180\r\n"
                             "Connection: keep-alive\r\n"
                             "User-Agent: Mozilla/5.0  AppleWebKit/532.0 (KHTML, like Gecko) Chrome/3.0.195.25 Safari/532.0\r\n"
                             "Referer: http://192.168.1.180/myup/up.html\r\n"
                             "Content-Length: 1055012\r\n"
                             "Cache-Control: max-age=0\r\n"
                             "Origin: http://192.168.1.180\r\n"
                             "Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryy+u09GS0qCiv2NKf\r\n"
                             "Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n"
                             "Accept-Encoding: gzip,deflate,sdch\r\n"
                             "Accept-Language: zh-CN,zh;q=0.8\r\n"
                             "Accept-Charset: GBK,utf-8;q=0.7,*;q=0.3\r\n";
                             
const char *data_boun_start = "----WebKitFormBoundaryy+u09GS0qCiv2NKf\r\n"; 

const char *data_boun_end = "----WebKitFormBoundaryy+u09GS0qCiv2NKf--";
const char *data_head = "Content-Disposition:form-data;name=\"upload_file\";\
						filename=\"tmp.dat\"\r\n"; 

#define req_len  strlen(post_req)
#define dst_ip "192.168.1.180"

int main()
{
       FILE *fp = NULL;
       int file_size = 0;
       int res;
       
       fp = fopen("tmp.dat", "r");
       if(fp == NULL)
       {
                fprintf(stderr, "fopen error!n");
                return -1;
       }
       fseek(fp, SEEK_SET, SEEK_END);
       file_size = ftell(fp);
      // printf("req_size: %dnfile_size:%dn", req_len, file_size/1024);

        char data_buf[file_size];
       // res = read(fp, data_buf, file_size);

       res = fread(data_buf, 1, file_size, fp);
        if(res < 0)
        {
                fprintf(stderr, "read error!n");
                return -1;
        }
        
        char *buf = NULL;
        int buf_size = 0;
        buf_size = req_len + file_size + strlen(data_head) + 2*strlen(data_boun_start) ;
        buf = (char *)malloc(buf_size);
        if(buf == NULL)
        {
                fprintf(stderr, "malloc error!n");
                return -1;
        }
        
        strcat(buf, post_req);
        strcat(buf, data_boun_start);
        strcat(buf, data_head);
        strcat(buf, data_buf);
        strcat(buf, data_boun_end);
        

     int sockfd, bytes_read;

    struct sockaddr_in dest;





        if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
        {
                printf("socket error!n");
                return -1;
        }



    /*---Initialize server address/port struct---*/

    bzero(&dest, sizeof(dest));

    dest.sin_family = AF_INET;

    dest.sin_port = htons(80); /*default HTTP Server port */

      /*

        if ( inet_addr("192.168.1.180", &dest.sin_addr.s_addr) == 0 )
                {
                        printf("inet_addr error!n");
                        return -1;
                }
          */
          dest.sin_addr.s_addr = inet_addr(dst_ip);         
                

    /*---Connect to server---*/

    if ( connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) != 0 )

        {
                printf("connect error!n");
        }
        

        if(send(sockfd, buf, buf_size, 0) < 0)
        {
                printf("send error!n");
                return -1;
        }
        else
        printf("send successfully...n");
        
        return 0;
}
