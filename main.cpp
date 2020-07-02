#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <pthread.h>

using namespace std;

/* 类 */
class Read_date {
private:
	int fd;
	int cnt;
	int buf_cnt = 0;
	char buf[2];
	char buf_read[5];
	
public:
	void open_fd(void) {
		fd = open("./english.txt",O_RDWR);
	}

	void close_fd(int ifd) {
		close(ifd);
	}
	
	int get_fd(void) {
		return fd;
	}
	
	char* get_read(void) {
		bzero(buf_read,5);
		
		for(int i = 0;i < 10;i++) {	
			read(fd,buf,1);
			//cout<<buf[0]<<endl;
			if(buf[0] > 0x20 && buf[0] < 0x7e) {
				if(buf[0] == '#') {
					cnt = 0;

					for(int j = buf_cnt;j < buf_cnt+5;j++) {
						buf_read[cnt++] = buf[0];
						read(fd,buf,1);
						if(buf[0] == '\0' || buf[0] == '\n' || buf[0] == '\r') {
							buf_cnt++;
							return buf_read;					
						}
						
					}
				}
			}
		}
		return buf_read;
	}
};

/* 全局变量区 */
char *date;
Read_date rd;
pthread_mutex_t mutex;

/* 线程函数 */
int *func1(void *j) {
	//static int tmp1 = 0;
	//tmp1 = *(int *)j;
	
	for(int i = 0;i < 20;i++) {
		pthread_mutex_lock(&mutex);
		date = rd.get_read();
		cout<<"func1,date = "<<date<<endl;
		pthread_mutex_unlock(&mutex);
	}	
	
	//cout<<__FUNCTION__<<" addr:"<<&tmp1<<"tmp1 ="<<tmp1<<endl;
	pthread_exit(NULL);
}

int *func2(void *j) {
	//static int tmp2 = 0;
	//tmp2 = *(int *)j;
	
	for(int i = 0;i < 20;i++) {
			pthread_mutex_lock(&mutex);
			date = rd.get_read();
			cout<<"func2,date = "<<date<<endl;
			pthread_mutex_unlock(&mutex);
		}
	
	//cout<<__FUNCTION__<<" addr:"<<&tmp2<<"tmp2 ="<<tmp2<<endl;
	pthread_exit(NULL);
}

/* 主函数 */
int main(int argc,char **argv) {
	//int pid,status,k;
	
	rd.open_fd();
	date = rd.get_read();
	cout<<"date = "<<date<<endl; 
	/* 线程号 */
	pthread_t tid[2];

	/* 初始化互斥锁 */
	int ret = pthread_mutex_init(&mutex,NULL);
	//int i = 5;
	//int j = 6;
	//void *tmp1 = NULL;
	//void *tmp2 = NULL;

	/*
	pid = fork();
	if(pid == 0) {
		for(int i = 0;i < 20;i++) {
			date = rd.get_read();
			cout<<"child,date = "<<date<<endl;
		}
	}
	else {
		
		for(int i = 0;i < 20;i++) {
			date = rd.get_read();
			cout<<"father,date = "<<date<<endl;
		}
		//waitpid(pid,&status,0);
		
	}
	*/
	pthread_create(&tid[0],NULL,func1,NULL);
	pthread_create(&tid[1],NULL,func2,NULL);

	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);

	pthread_mutex_destroy(&mutex);
	//cout<<__FUNCTION__<<" add:"<<tmp<<"tmp ="<<*(int *)tmp<<endl;

	
	rd.close_fd(rd.get_fd());
	
	


	
	/*
	for(k = 0;k < 5;k++) {
		pid = fork();
		if(pid == 0) {
			cout<<"s_pid["<<k<<"] = ";
			cout<<getpid()<<endl;
			break;
		}
		
		else {
			waitpid(pid,&status,0);
			cout<<"I am father,continue"<<endl;
			continue;
		}
		
	}
	*/
	
	return 0;
}
