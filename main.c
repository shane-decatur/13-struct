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
  printf("reading nyc_pop.csv");
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
  close(fd);
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
  close(fd);
  printf("Appended data to file: year: %d\t boro: %s\t pop: %d\n",entry.year,entry.boro,entry.population);
  return 0;
}

int update_data(){
  struct stat info;
  stat("./nyc_pop.data",&info);
  int fd = open("./nyc_pop.data",O_RDWR);
  struct pop_entry *data = malloc(info.st_size+sizeof(struct pop_entry));
  read(fd,data,info.st_size);

  int i;
  int count = 0;
  for (i = 0;i*sizeof(struct pop_entry) < info.st_size;i++){
    printf("%d: year %d boro: %s pop: %d\n",count,data[i].year,data[i].boro,data[i].population);
    count++;
  }

  struct pop_entry entry;

  printf("entry to update: ");
  char input1[100];
  char input2[100];
  char boro[15];
  fgets(input1,100,stdin);

  printf("enter year boro pop: ");
  fgets(input2,100,stdin);

  sscanf(input2,"%d %s %d\n", &(entry.year),boro,&(entry.population));
  strcpy(entry.boro,boro);
  int s = atoi(input1);
  lseek(fd,s*sizeof(struct pop_entry),SEEK_SET);
  write(fd,&entry,sizeof(entry));
  return 0;
}

int main(int argc, char *argv[]){
  if (argc != 2){
    printf("select an option:\n-read_csv\n-read_data\n-add_data\n-update_data\n");
    return 0;
  }
  if (!strcmp(argv[1],"-read_csv")){
    read_csv();
  }
  else if (!strcmp(argv[1],"-read_data")){
    read_data();
  }
  else if (!strcmp(argv[1],"-add_data")){
    add_data();
  }
  else if (!strcmp(argv[1],"-update_data")){
    update_data();
  }
  else{
    printf("Please enter a valid argument");
    return 0;
  }
  printf("File updated");
  return 0;
}
