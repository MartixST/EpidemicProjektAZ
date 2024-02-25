#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


// ./epidem real_data.csv 8 0.9 output.html

double S[1000];//смоделированные данные
double I[1000];
double R[1000];

double S0[1000];//реальные данные
double I0[1000];
double R0[1000];

int infectious_days; //колич дней в теч которых чел заразен
double infections_per_day;//počet lidí, které nakažený člověk nakazí za jeden den
int days; //количество дней

double popul; //количество популяции
int sh_days; //шаг на один день по экрану

char real_data[100];//путь к входному файлу с реальными данными о эпидемии
char output[100]; //путь к выходному файлу с визуализацией предсказания и рил данных

int count_file()
{ //программа считает количество строк в файле

  FILE* fd = fopen(real_data,  "r");

   if (fd == NULL)
    {
      printf("Could not read real_data file");
      exit(1);

    }

    int i = 0;
    char str[100];

   while (!feof(fd))// читается текстовый фаил пока не достигнут конеец строки
   {
    fgets(str, 100, fd);
    //printf("Str=%s\n", str);
   
    i++;
   }
   
   fclose(fd);

   return --i; //лишнее срабатывание в цикле
}

void load_real_data()
{

  FILE* fd = fopen(real_data,  "r");

  if (fd == NULL)
  {
    printf("Could not read real_data file");
    exit(1);

  }

  int i = 0;
  char str[100];
  char sep[10]= ",";
  char *istr;//строка в которую записывается результат выделения из строки
    

  while (!feof(fd))// читается текстовый фаил пока не достигнут конец строки
  {
    fgets(str, 100, fd);
    //printf("Str=%s\n", str);
      
    //в строке стр содержатся три числа разделенные запятой, например, 800,200,250
    //нужно считать первое число с помощью strtok 
    //перевести его в целое число и записать в массив
    istr = strtok(str,sep);//записываем в строку первое число
    S0[i]=atoi(istr)/popul;
    istr = strtok(NULL,sep);//записываем в строку второе число
    I0[i]=atoi(istr)/popul;
    istr = strtok(NULL,sep);//записываем в строку первое число
    R0[i]=atoi(istr)/popul;

    i++;
  }

  fclose(fd);

  /*for (int i = 0; i < days; i++)
  {
    printf("i=%d S0=%6.4f I0=%6.4f R0=%6.4f\n", i, S0[i], I0[i], R0[i]);
    printf("sum0=%f\n", S0[i]+I0[i]+R0[i]);
  }*/
}

  //печатает в строку код линии

void draw_line(char str[], int x1, int y1, int x2, int y2 )
{
  //<line x1 = "100" y1 = "550"   x2 = "600"   y2 = "550" 
  //style = "stroke:rgb(255,0,0);stroke-width:10" /> 
  char str_x1[10];
  char str_y1[10];
  char str_x2[10];
  char str_y2[10];

  snprintf(str_x1, 10, "%d", x1);
  snprintf(str_y1, 10, "%d", y1);
  snprintf(str_x2, 10, "%d", x2);
  snprintf(str_y2, 10, "%d", y2);

  char str_line[1000] = "<line x1 = \""; // \ ставится для экранирования кавычки
  //дальше нужно прибавить само значение переменной x1, однако х1 целое. Значит преобразуем значение в строку

  strcat(str_line, str_x1);
  strcat(str_line, "\" y1 = \"");
  strcat(str_line, str_y1);
  strcat(str_line, "\" x2 = \"");
  strcat(str_line, str_x2);
  strcat(str_line, "\" y2 = \"");
  strcat(str_line, str_y2);
  strcat(str_line, "\" style = \"stroke:rgb(0,0,0);stroke-width:2\" />");
  strcat(str, str_line);

}

//добавляет в строку код графика с указанным цветом
void graph(char *str, double Y[], char color[]){

  /*<polyline points="0,40 40,40 40,80 80,80 80,120 120,120 120,160"
  style="fill:white;stroke:red;stroke-width:4" />*/
  char str_pl[10000]= "\n<polyline points=\"";
  int x = 100;
  int y;
  char str_x[10];
  char str_y[10];
  sh_days = 700/days;
  for (int i = 0; i < days; i++)
  {

    //из массива нужно извлечь данные и преобразовать их в координату у
    y = 500-Y[i]*500;
    y = abs(y);
    //коорд х формируется так

    x = sh_days*i + 100;

    //printf("x=%d y=%d\n", x, y);


    snprintf(str_x, 10, "%d", x);
    strcat(str_pl, str_x);
    strcat(str_pl, ",");
    snprintf(str_y, 10, "%d", y);
    strcat(str_pl, str_y);
    if (i!=days-1)
    {
      strcat(str_pl, " ");
    }
  }
  
  strcat(str_pl, "\" style=\"fill:none;stroke:");
  strcat(str_pl, color);
  strcat(str_pl, ";stroke-width:4\" />\n");
  strcat(str, str_pl);
}

double max_I()
{
  double max = 0;

  for (int i = 0; i < days; i++)
  {
    if (I[i]>max)
    {
      max = I[i];
    }
    
  }
  
  return max;
}

//печатает график в фаил html. Используется Svg
void print_graph(){

  FILE *f = fopen(output, "w");

  if (f == NULL)
    {
        printf("Could not open file to write");
        exit(1);

    }
  char str[10000];
  strcat(str,"<html>\n");
  strcat(str,"<body>\n");
  strcat(str,"<div>\n");
  strcat(str,"<h1>Prubeh epidemii</h1>\n");
  strcat(str,"<svg height=\"600\" width=\"800\">\n");

  draw_line(str, 50,500,750,500); //ось х
  draw_line(str, 100, 50, 100, 550); 
  graph(str, I, "red");
  graph(str, R, "green");
  graph(str, S, "blue");
  graph(str, I0, "black");
  graph(str, R0, "yellow");
  graph(str, S0, "aqua");
  
  double max = max_I();
  int y;
  y = 500-max*500 - 5;
  max = max*100;
  char str_y_max[10];
  snprintf(str_y_max, 10, "%d", y);
  char str_max[20];
  sprintf (str_max, "%5.1f", max);
  //<text x="300" y="15" fill="red">I love SVG!</text>
  strcat(str,"<text x=\"200\" y=\"");
  strcat(str, str_y_max);
  strcat(str,"\" fill=\"black\">Max I =");
  strcat(str, str_max);
  strcat(str,"%</text>\n");
  strcat(str,"<text x=\"730\" y=\"530\" fill=\"black\">Dny</text>");
  strcat(str,"<text x=\"35\" y=\"30\" fill=\"black\">Procento populace</text>");

  //нарисуем штрихи на оси х

  int x = 100;
  y = 500;
  char str_x[10];
  char str_i[10];
  
  for (int i = 1; i < days-2; i++) //через равный шаг рисуем штрихи
  {
    x = x+sh_days;
    draw_line(str, x, y-5, x, y+5);
    snprintf(str_x, 10, "%d", x);
    snprintf(str_i, 10, "%d", i);
    strcat(str,"<text x=\"");
    strcat(str, str_x);
    strcat(str,"\" y=\"530\" fill=\"black\">");
    strcat(str, str_i);
    strcat(str,"</text>\n");
  }

  char str_y[10];
  x = 100;
  y = 500;

  for (int i = 1; i < 10; i++) //через равный шаг рисуем штрихи
  {
    y = y-50;
    draw_line(str, x-5, y, x+5, y);
    snprintf(str_y, 10, "%d", y);
    snprintf(str_i, 10, "%d", i*10);
    strcat(str,"<text x=\"70\" y=\"");
    strcat(str, str_y);
    strcat(str,"\" fill=\"black\">");
    strcat(str, str_i);
    strcat(str,"</text>\n");
  }
  

  strcat(str,"</svg>\n");
  strcat(str,"</div>\n");
  strcat(str,"</body>\n");
  strcat(str,"</html>\n");

  fprintf(f, "%s", str);


}

//осталось
//обработка входных параметров на предмет ошибок


int main(int argc, char** argv){

    //Нужна программа которая читает колич строк в файле

    //первый аргумент риал дата
    //число дней которые больной является опасным
    //сколько человек в день заражает
    //выходной файл

    //printf("\nargc=%d", argc);


   if (argc!=5)

    {
       printf("Wrong parameters");
       return 1;
    }

    strcpy(real_data, argv[1]);
    infectious_days = atoi(argv[2]);
    infections_per_day = atof(argv[3]);
    strcpy(output, argv[4]);
    

    //strcpy(real_data, "real_data.csv");
    //strcpy(output, "output.html");

    //infectious_days = 8; //количество дней когда человек заразен 
    //infections_per_day = 0.9;

    //printf("real_data = %s\n", real_data);
    //printf("infectious_days = %d\n", infectious_days);
    //printf("infections_per_day = %f\n", infections_per_day);
    //printf("output = %s\n", output);

    days = count_file();
    //printf("days = %d\n", days);

    //считаем первую строчку из файла для инициализации симуляции

    FILE* fd = fopen(real_data,  "r");

   if (fd == NULL)
    {
        printf("Could not read real_data file");
        exit(1);

    }
    
    char str[100];

    fgets(str, 100, fd);
    //printf("Str=%s\n", str);

    char sep[10]=",";
    char *istr;

    // Выделение первой части строки
    istr = strtok (str,sep); //все что до сеп запишет в istr
    int health_0 = atoi(istr);//преобразование в целое число значения в строке istr

    // Выделение последующих частей

    // Выделение очередной части строки
    istr = strtok (NULL,sep);
    int inf_0 = atoi(istr);
    
    istr = strtok (NULL,sep);
    int recover_0 = atoi(istr);
     
    //printf("health_0=%d\n", health_0);
    //printf("inf_0=%d\n", inf_0);
    //printf("recover_0=%d\n", recover_0);

    double b = infections_per_day;
    double y = 1.0/(double)infectious_days;

    popul = health_0+inf_0+recover_0; // все население

    S[0] = health_0/popul; // инициализация модели начальными значениями
    I[0] = inf_0/popul;
    R[0] = recover_0/popul;


    
    //работа модели в цикле
    for (int i = 1; i < days; i++)
    {
        

        /*S(d) = S(d-1) - b * S(d-1) * I(d-1)
          I(d) = I(d-1) + b * S(d-1) * I(d-1) - y * I(d-1)
          R(d) = R(d-1) + y * I(d-1)*/
        S[i]= S[i-1] - b * S[i-1] * I[i-1]; 
        I[i]= I[i-1] + b * S[i-1] * I[i-1] - y * I[i-1];
        R[i]= R[i-1] + y * I[i-1];

        //printf("i=%d S=%6.4f I=%6.4f R=%6.4f\n", i, S[i], I[i], R[i]);
        //printf("sum=%f\n", S[i]+I[i]+R[i]);
    }
    
    load_real_data();//считываем реальные данные
    /*for (int i = 0; i < days; i++)
    {
      printf("i=%d S0=%6.4f I0=%6.4f R0=%6.4f\n", i, S0[i], I0[i], R0[i]);
      printf("sum0=%f\n", S0[i]+I0[i]+R0[i]);
    }*/
    

    // сделать визуализацию (выяснить какую)
    print_graph();

    //считать еще три массива из файлов S0 I0 R0 и напечатать их в принт граф



    return 0;

}