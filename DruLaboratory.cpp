#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>

#define ANSI_RESET_ALL     "\x1b[0m"
#define STYLE_BOLD         "\x1b[1m"
#define COLOR_RED          "\x1b[31m"
#define COLOR_GREEN        "\x1b[32m"
#define COLOR_YELLOW       "\x1b[33m"
#define COLOR_MAGENTA      "\x1b[35m"

#define MAX_ROUTE 20
#define MAX_ROUTE_LEN 100
#define ROUTE_PROPERTIES 7
#define END_INPUT "###"

#define TAG_NUM_PHONE 0
#define TAG_PLANE 1
#define TAG_CITY 2
#define TAG_DAYS_OF_WEEK 3
#define TAG_TIME_FROM 4
#define TAG_TIME_TO 5
#define TAG_PRICE 6

/*
N3. Разработка программного обеспечения для учета данных об авиарейсах
№ 3 Массив строк «Аэропорт»

Запрос: определить все рейсы до заданного города, которые проводятся на определенном
типе самолета, и цена билетов на которые не выше некоторой заданной цены.

3 boing777 Moscow 1256 10:30 10:50 16450
12 airbus230 Moscow 1256 14:10 14:20 6710
71 boing777 Moscow 125 6:45 6:55 1300 12780
8 boing777 Novosibirsk 234 12:30 13:45 1300
26 airbus230 Moscow 1256 21:35 21:50 7600
23 airbus230 Novosibirsk 1467 6:30 7:40 1940
52 boing777 Novosibirsk 2567 17:05 18:10 2500
###
*/
void request3(
    char* response[MAX_ROUTE][ROUTE_PROPERTIES],
    int* responseLen,
    char* data[MAX_ROUTE][ROUTE_PROPERTIES],
    int len,
    char* city,
    char* plane,
    char* maxPrice)
{
    for (int i = 0; i < len; i++)
    {
        if (
            (city == NULL || strcmp(city, "-") == 0 || strcmp(data[i][TAG_CITY], city) == 0) &&
            (plane == NULL || strcmp(plane, "-") == 0 || (strcmp(data[i][TAG_PLANE], plane) == 0)) &&
            (maxPrice == NULL || strcmp(maxPrice, "-") == 0 ||(strtof(data[i][TAG_PRICE], NULL) <= strtof(maxPrice, NULL)))
            )
        {
            for (int property_i = 0; property_i < ROUTE_PROPERTIES; property_i++)
            {
                response[*responseLen][property_i] = data[i][property_i];
            }          
            (*responseLen)++;
        }
    }
}

// ввод рейсов
void addRoutes(int* len, char* data[MAX_ROUTE][ROUTE_PROPERTIES])
{
    printf("Enter the data of routes as follows:\n");
    printf(COLOR_YELLOW "number" COLOR_MAGENTA " plane" COLOR_YELLOW " city" COLOR_MAGENTA " days of week" COLOR_YELLOW " departure time" COLOR_MAGENTA " arrival time" COLOR_YELLOW " price" ANSI_RESET_ALL"\n");
    printf("Enter \"###\" to end\n");

    // пока не будет обнаружен символ ### и массив не закончился
    while (*len < MAX_ROUTE)
    {
        char* str = (char*)calloc(MAX_ROUTE_LEN, sizeof(char));
        gets_s(str, MAX_ROUTE_LEN);

        char* word = strtok(str, " ,.");
        if (strcmp(word, END_INPUT) == 0) break;

        for (int property_i = 0; property_i < ROUTE_PROPERTIES && word != NULL; property_i++)
        {
            data[*len][property_i] = word;
            //printf("> %s\n", word);
            word = strtok(NULL, " ,.");

        }
        (*len)++;
    }
}

// печать рейсов
void printRoutes(int len, char* data[MAX_ROUTE][ROUTE_PROPERTIES])
{
    putchar(201);
    for (int i = 0; i < 5; i++) putchar(205);
    putchar(203);
    for (int i = 0; i < 11; i++) putchar(205);
    putchar(203);
    for (int i = 0; i < 14; i++) putchar(205);
    putchar(203);
    for (int i = 0; i < 9; i++) putchar(205);
    putchar(203);
    for (int i = 0; i < 8; i++) putchar(205);
    putchar(203);
    for (int i = 0; i < 8; i++) putchar(205);
    putchar(203);
    for (int i = 0; i < 10; i++) putchar(205);
    putchar(187);
    printf("\n");

    putchar(186);
    printf(STYLE_BOLD " %-3.3s " ANSI_RESET_ALL, "Num"); putchar(186);
    printf(STYLE_BOLD " %-9.9s " ANSI_RESET_ALL, "plane"); putchar(186);
    printf(STYLE_BOLD " %-12.12s " ANSI_RESET_ALL, "city"); putchar(186);
    printf(STYLE_BOLD " %-7.7s " ANSI_RESET_ALL, "days"); putchar(186);
    printf(STYLE_BOLD " %-6.6s " ANSI_RESET_ALL, "departure"); putchar(186);
    printf(STYLE_BOLD " %-6.6s " ANSI_RESET_ALL, "landing"); putchar(186);
    printf(STYLE_BOLD " %-8.8s " ANSI_RESET_ALL, "price"); putchar(186);
    putchar('\n');

    putchar(204);
    for (int i = 0; i < 5; i++) putchar(205);
    putchar(206);
    for (int i = 0; i < 11; i++) putchar(205);
    putchar(206);
    for (int i = 0; i < 14; i++) putchar(205);
    putchar(206);
    for (int i = 0; i < 9; i++) putchar(205);
    putchar(206);
    for (int i = 0; i < 8; i++) putchar(205);
    putchar(206);
    for (int i = 0; i < 8; i++) putchar(205);
    putchar(206);
    for (int i = 0; i < 10; i++) putchar(205);
    putchar(185);
    printf("\n");

    for (int i = 0, j = 0; i < len; i++)
    {
        j = 0;
        putchar(186);
        printf(" %-3.3s ", data[i][j++]); putchar(186);
        printf(" %-9.9s ", data[i][j++]); putchar(186);
        printf(" %-12.12s ", data[i][j++]); putchar(186);
        printf(" %-7.7s ", data[i][j++]); putchar(186);
        printf(" %-6.6s ", data[i][j++]); putchar(186);
        printf(" %-6.6s ", data[i][j++]); putchar(186);
        printf(" %-8.8s ", data[i][j]); putchar(186);
        putchar('\n');

        if (i + 1 != len)
        {
            putchar(204);
            for (int i = 0; i < 5; i++) putchar(205);
            putchar(206);
            for (int i = 0; i < 11; i++) putchar(205);
            putchar(206);
            for (int i = 0; i < 14; i++) putchar(205);
            putchar(206);
            for (int i = 0; i < 9; i++) putchar(205);
            putchar(206);
            for (int i = 0; i < 8; i++) putchar(205);
            putchar(206);
            for (int i = 0; i < 8; i++) putchar(205);
            putchar(206);
            for (int i = 0; i < 10; i++) putchar(205);
            putchar(185);
            printf("\n");
        }
    }

    putchar(200);
    for (int i = 0; i < 5; i++) putchar(205);
    putchar(202);
    for (int i = 0; i < 11; i++) putchar(205);
    putchar(202);
    for (int i = 0; i < 14; i++) putchar(205);
    putchar(202);
    for (int i = 0; i < 9; i++) putchar(205);
    putchar(202);
    for (int i = 0; i < 8; i++) putchar(205);
    putchar(202);
    for (int i = 0; i < 8; i++) putchar(205);
    putchar(202);
    for (int i = 0; i < 10; i++) putchar(205);
    putchar(188);
    printf("\n\n");
}

int main()
{
    char
        *data[MAX_ROUTE][ROUTE_PROPERTIES],
        *responseData[MAX_ROUTE][ROUTE_PROPERTIES];
    int 
        len = 0,
        responseLen = 0;

    addRoutes(&len, data);  

     while (1)
     {
        printf("\tALL ROUTES\n");
        printRoutes(len, data);

        responseLen = 0;

        printf("Enter necessary" COLOR_MAGENTA " city" COLOR_YELLOW " plane" COLOR_MAGENTA " max price" ANSI_RESET_ALL " or \"-\" to skip property:\n\t");
        
        char str[30];
        gets_s(str, 30);
        char
            * city = strtok(str, " .,"), 
            * plane = strtok(NULL, " .,"),
            * price = strtok(NULL, " .,");
        if (city != NULL && strcmp(city, END_INPUT) == 0) break;

        request3(responseData, &responseLen, data, len, city, plane, price);

        if (responseLen) printRoutes(responseLen, responseData);
        else printf(COLOR_RED "Data not found" ANSI_RESET_ALL "\n");
     }
}