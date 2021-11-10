#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

struct pop_entry{
  int year;
  int population;
  char boro[15];
};

int read_csv(){
  int fd = open("nyc_pop.csv",O_RDONLY);
  struct stat info;
  stat("./nyc_pop.csv", &info);

  char * file_text = malloc(info.st_size);
  read(fd, file_text, info.st_size);
  close(fd);

  char * pointer = file_text;
  int rows = -1;
  while (strchr(pointer+1,'\n')){
    rows++;
    pointer = strchr(pointer+1,'\n');
  }
  pointer = strchr(file_text,'\n');
  struct pop_entry *data = malloc(sizeof(struct pop_entry)*5*rows);
  int i = 0;
  int year;
  while (strchr(pointer+1,'\n')){
    char *next = strchr(pointer+1,'\n');
    if (next){
      *next = '\0';
    }
    if (i != 1){
    strcpy(data[i].boro,"Manhattan");
    strcpy(data[i+1].boro,"Brooklyn");
    strcpy(data[i+2].boro,"Queens");
    strcpy(data[i+3].boro,"Bronx");
    strcpy(data[i+4].boro,"Staten Island");
    sscanf(pointer+1,"%d,%d,%d,%d,%d,%d",&year,&(data[i].population),&(data[i+1].population),&(data[i+2].population),&(data[i+3].population),&(data[i+4].population));
    data[i].year = year;
    data[i+1].year = year;
    data[i+2].year = year;
    data[i+3].year = year;
    data[i+4].year = year;
    if (next) *next = '\n';
    i += 5;
    pointer = strchr(pointer+1,'\n');
  }
  }
  fd = open("./nyc_pop.data",O_CREAT | O_TRUNC | O_WRONLY, 0777);
  write(fd,data,sizeof(struct pop_entry)*5*rows);
  free(data);
  free(file_text);
  printf("wrote %lu bytes to nyc_pop.data\n",sizeof(struct pop_entry)*5*rows);
  return 0;
}

int read_data(){
  struct stat info;
  int fd = open("./nyc_pop.data", O_RDONLY);
  stat("nyc_pop.data",&info);
  struct pop_entry *data = malloc(info.st_size);

  read(fd,data,info.st_size);
  int i;
  int count = 0;
  for (i = 0; i * sizeof(struct pop_entry) < info.st_size; i++){
    printf("%d: year: %d boro: %s pop: %d\n",count, data[i].year,data[i].boro,data[i].population);
    count++;
  }

  return 0;
}

int add_data(){
  printf("Enter year boro pop: ");
  struct pop_entry entry;
  char input[100];
  char *p = input;
  char boro[15];
  fgets(input,100,stdin);
  sscanf(p,"%d %s %d\n", &(entry.year), boro, &(entry.population));
  strcpy(entry.boro,boro);

  int fd = open("./nyc_pop.data",O_WRONLY | O_APPEND);
  write(fd,&entry,sizeof(entry));
  printf("Appended data to file: year: %d\t boro:%s\t pop%d\n",entry.year,entry.boro,entry.population);
  return 0;
}

int main(){
  read_csv();
  read_data();
}
