#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <string>
#include <random>
#include <iomanip>

using namespace std;
HANDLE hStdin; // Глобальная переменная handle Текущей консоли
struct Report;




void CursorOff(void);
void CursorOn(void);
int PressKey(void);
void GotoXY(int x, int y);
void OutData(string Template, string S,int CRC = 0); // по умолчанию не проверяется контроль четности.
void ClearReport(Report& a); 
void OutReport(Report a);
random_device  rd;
int rnd(int a, int b);
const int MaxL = 80;
const int PosX=18;
//int bag = 0; 
int done = 0;


struct Report // отчет
{
    int All;              // передано блоков
    int ErrorCRC;         // повреждено блоков
    int ErrorDetectCRC;   // Обнаружено поврежденных блоков
    int ErrorNoDetectCRC; // Не обнаружено поврежденныйх блоков

    int ErrorLine;        // пореждено блоков
    int ErrorReanimator;   // восстановлено блоков с 1 ошибкой 
    int ErrorDublicate;    // восстановленно блоков с 2 ошибками
    int ErrorBad;          // невосстановимых блоков


};

int Pause = 200; // 200;

int main()
{
    const int KeyUp = 0xFF48, KeyDown = 0xFF50;
    const int KeyRight = 0xFF4D, KeyLeft = 0xFF4B;
    const int KeyEsc = 0x001B, KeySpace = 0x0020;
    const int KeyEnter = 0x000D;
    const int KeyNo = 0x0000;
    int       Key;
    //int Pause = 200; // 1000;
    string S = "";
    string S1 = "";
    string S2 = "";
    string S3 = "";

    string S4 = "";
    string S5 = "";
    string S6 = "";

    string S7 = "";
    string S8 = "";
    string S9 = "";
    string S10 = "";
    int B[6];
    int i,t;
    int d2;

    
    int Done = 1;

    int Bug = 40;
    Report Data;
    ClearReport(Data);  // Очищаем отчет 
    
   
    // устанавливаем кодировку 1251
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    system("mode con cols=120 lines=25"); // устанавливаем размер консоли 80 столбцов 25 строк
    system("cls");
    hStdin = GetStdHandle(STD_OUTPUT_HANDLE); //- получаешь handle для консоли
    SetConsoleTitle(L"Кодирование.декодирование данных. Автор: Головин И.А. 121-ИБо, 2022г.");
    while (_kbhit())  _getch();
    CursorOff(); // выключаем курсор 
    
    



    system("cls");
   

   
    cout << "Клавиши управления:  \"Влево\", \"Вправо\" - изменение количества помех, \"Пробел\" - пауза.\n";
    cout << "Клавиши управления:  \"Вверх\", \"Вниз\"   - скорость  программы, \"Esc\" - выход из программы.\n\n\n";
    
    //cout << "Данные : ";
    GotoXY(0, 15); cout << "Помех: " << Bug << " на 1000 бит, задержка в обновлении данных : " << Pause/50;



    while (Done)
    {
        S = ""; S1 = ""; S2 = ""; S3 = ""; S4 = ""; S5 = ""; S6 = ""; S7 = ""; S8 = ""; S9 = ""; S10 = ""; 
        GotoXY(PosX, 3);
        for (i = 0; i < MaxL; i++) S += (char)(48 + rnd(0, 7)); //
        //cout << S;
        GotoXY(0, 3); cout << "Данные : ";
        GotoXY(PosX, 3);  OutData(S, S);
        Data.All += MaxL; 
        S1 = "";
        for (i = 0; i < MaxL; i++)

            switch (S[i])
            {
            case '0': S1 += '0'; break;
            case '1': S1 += '3'; break;
            case '2': S1 += '5'; break;
            case '3': S1 += '6'; break;
            case '4': S1 += '9'; break;
            case '5': S1 += 'A'; break;
            case '6': S1 += 'C'; break;
            case '7': S1 += 'F'; break;

            };
        GotoXY(0, 4); cout << "Данные с CRC:";
        GotoXY(PosX, 4); OutData(S1, S1);

        cout << "\n";
        // cout << 0 ^ 3;

       
        int d1;


        S2 = ""; S3 = "";
        GotoXY(0, 13);
        for (i = 0; i < MaxL; i++)
        {
            if (((int)S1[i] >= (int)'0') && ((int)S1[i] <= (int)'9')) d1 = (int)S1[i] - (int('0'));
            if (((int)S1[i] >= (int)'A') && ((int)S1[i] <= (int)'F')) d1 = (int)S1[i] - (int('A')) + 10;
            if (Bug == 0) S3 += S1[i]; else
            {
                // начинаем вносить ошибки;
                 // выдергиваем биты
                d2 = d1;


                for (int t = 0; t < 4; t++)
                {
                    B[t] = d1 % 2; d1 = d1 >> 1;
                    if (rnd(0, 1000) < Bug) // начинавем менять биты на противоположные, если попали в процент ошибок
                    {
                        if (B[t]) B[t] = 0; else B[t] = 1;
                       
                    };
                };
                d1 = B[0] + 2 * B[1] + 4 * B[2] + 8 * B[3];


                if (d1 < 10) S2 += (char)((int)'0' + d1);
                if (d1 > 9) S2 += (char)((int)'A' + (d1 - 10));

            };

        };

        if (Bug == 0) S2 = S1;
        GotoXY(0, 5); cout << "Передача данных:";
        GotoXY(PosX, 5); OutData("", S2, 1);
        // подсчитываем число поврежденных блоков
        for (i = 0; i < MaxL; i++) if (S1[i] != S2[i]) Data.ErrorCRC++;      // считаем число поврежденных блоков

        // Начинаем декодирование данных
        S3 = "";
        for (i = 0; i < MaxL; i++)
        {
            switch (S2[i])
            {
            case '0': S3 += '0'; break;
            case '3': S3 += '1'; break;
            case '5': S3 += '2'; break;
            case '6': S3 += '3'; break;
            case '9': S3 += '4'; break;
            case 'A': S3 += '5'; break;
            case 'C': S3 += '6'; break;
            case 'F': S3 += '7'; break;
            default: S3 += 'Z';

            };

        };

        GotoXY(0, 6); cout << "Декодирование:";
        GotoXY(PosX, 6); OutData(S, S3);

        for (i = 0; i < MaxL; i++)
        {
            if (S3[i] == 'Z') Data.ErrorDetectCRC++;
            if (S3[i] != 'Z' && S[i] != S3[i]) Data.ErrorNoDetectCRC++;
        };


        //  Начинаем кодирование используя ЛК
        GotoXY(0, 8); cout << "Данные : ";

        GotoXY(PosX, 8); OutData(S, S);
        
        S4 = ""; S7 = ""; S8 = "";
        for (i = 0; i < MaxL; i++)
        {
            switch (S[i])
            {
            case '0':S4 += "00"; S7 += '0'; S8 += '0'; break;
            case '1':S4 += "51"; S7 += '5'; S8 += '1'; break;
            case '2':S4 += "32"; S7 += '3'; S8 += '2'; break;
            case '3':S4 += "63"; S7 += '6'; S8 += '3'; break;
            case '4':S4 += "64"; S7 += '6'; S8 += '4'; break;
            case '5':S4 += "35"; S7 += '3'; S8 += '5'; break;
            case '6':S4 += "56"; S7 += '5'; S8 += '6'; break;
            case '7':S4 += "07"; S7 += '0'; S8 += '7'; break;
            };

        };
        GotoXY(0, 9); cout << "Кодирование: ";
        GotoXY(PosX, 9); OutData(S7, S7);
        GotoXY(PosX, 10); OutData(S8, S8);
        // начинаем вносить помехи 
        
        S5 = "";
       
        for (i = 0; i < S4.length(); i++)
        {
            if (((int)S4[i] >= (int)'0') && ((int)S4[i] <= (int)'7')) d1 = (int)S4[i] - (int('0'));

            if (Bug == 0) S5 += S4[i]; else
            {
                // начинаем вносить ошибки;
                 // выдергиваем биты
                for (int t = 0; t < 3; t++)
                {
                    B[t] = d1 % 2; d1 = d1 >> 1;
                    if (rnd(0, 1000) < Bug) // начинавем менять биты на противоположные, если попали в процент ошибок
                    {
                        if (B[t]) B[t] = 0; else B[t] = 1;
                        
                    };
                };
                d1 = B[0] + 2 * B[1] + 4 * B[2];
                S5 += (char)((int)'0' + d1);

            };

        };
        // разбиваем блок передачи на две части

        
        S9 = ""; S10 = "";
        for (i = 0; i < S5.length(); i++) if (i % 2) S10 += S5[i]; else S9 += S5[i];

        GotoXY(0, 11); cout << "Передача данных:";

        // GotoXY(PosX, 10); OutData(S4, S5);
        // GotoXY(PosX, 11); OutData(S4.substr(MaxL), S5.substr(MaxL));

        GotoXY(PosX, 11); OutData(S7, S9);
        GotoXY(PosX, 12); OutData(S8, S10);
       


        S6 = "";
        int b1, b2, res;
        for (i = 0; i < MaxL; i++)
        {

            b1 = (int)S5[2 * i] - (int)'0';
            b2 = (int)S5[2 * i + 1] - (int)'0';
            d1 = b1 * 10 + b2; // считываем 6 бит информации;
            done = 0;
            switch (d1)
            {
            case 00: S6 += '0';  break;
            case 51: S6 += '1';  break;
            case 32: S6 += '2';  break;
            case 63: S6 += '3';  break;
            case 64: S6 += '4';  break;
            case 35: S6 += '5';  break;
            case 56: S6 += '6';  break;
            case 07: S6 += '7';  break;
            default: done = 1;

            };
            if (done == 1) // нашли баг. Пытаемся восстановить
            {
                Data.ErrorLine++;
                d1 = b1 * 8 + b2; // переводим в биты
                for (t = 0; t < 6; t++) { B[5 - t] = d1 % 2; d1 = d1 >> 1; };
                // начинаем умножать на матрицу
                res = 4 * ((B[0] + B[3] + B[5]) % 2);
                res = res + 2 * ((B[1] + B[3] + B[4]) % 2);
                res = res + ((B[2] + B[4] + B[5]) % 2);
                // вектор синдрома рассчитали. Пытаемся восстановить
                switch (res)
                {

                case 5: B[5] = (B[5] + 1) % 2; break;       // сбит бит 1
                case 3: B[4] = (B[4] + 1) % 2; break;       // сбит бит 2
                case 6: B[3] = (B[3] + 1) % 2; break;       // сбит бит 3
                case 7: B[5] = (B[5] + 1) % 2; break;       // сбит бит 1, два бита повреждено

                };
                

                d1 = 4 * B[3] + 2 * B[4] + B[5];
                if ((char)('0' + d1) != S[i]) S6 += 'Z'; else S6 += (char)('0' + d1);

                if (res > 0 && res < 7 && (char)('0' + d1) == S[i]) Data.ErrorReanimator++;
                if (res == 7 && (char)('0' + d1) == S[i]) Data.ErrorDublicate++;
                if ((char)('0' + d1) != S[i]) Data.ErrorBad++;
                
                //S6 += (char)('0' + d1);
            };

        };

        GotoXY(0, 13); cout << "Декодирование:";
        GotoXY(PosX, 13); OutData(S, S6);


        // 
        // 
        //S4.substr()

        // 
        //  
        // пишем отчет 
        /*
          S   - Сообщение
          S1  - Сообщение + CRC
          S2  - Передача сообщения (с помехами)
          S3  - Восстановление сообщения

          S4 - Закодированное сообщение
          S5 - Переданное сообщение (с помехами)
          S6 - Восстановленное сообщение;



        */
        OutReport(Data); // Выводим статистику
        
        // обрабатываем нажатые клавиши 
        
        Key = PressKey();
        switch (Key)
        {
        case KeyEsc   : Done = 0; break;
        case KeySpace :   // Пауза в выполнении программы
                       {
                         
                         GotoXY(0, 24); cout << "Работа приостановлена, для продолжения работы нажмите любую клавишу";
                         while (_kbhit())  _getch();
                         while (!PressKey());
                         GotoXY(0, 24); cout << "                                                                   ";
                         break;
                       };
        case KeyLeft:
                       {

                         // нужно обнулить статистику
                         Bug = (Bug + 990) % 1000;
                         GotoXY(0, 15); cout << "Помех: " << Bug << " на 1000 бит, задержка в обновлении данных : " << Pause / 50 << "       ";
                         ClearReport(Data);
                         break;
                       };
        case KeyRight:
                       {
                         // нужно обнулить статистику
                         Bug = (Bug + 1010) % 1000;
                         GotoXY(0, 15); cout << "Помех: " << Bug << " на 1000 бит, задержка в обновлении данных : " << Pause / 50 << "       ";
                         ClearReport(Data);
                         break;
                       
                       };
        case KeyUp: 
                       {
                          Pause = (Pause + 50) % 4000;
                          GotoXY(0, 15); cout << "Помех: " << Bug << " на 1000 бит, задержка в обновлении данных : " << Pause / 50 << "       ";
                          break;
                       };
        case KeyDown :
                       {
                         Pause = (Pause + 3950) % 4000;
                         GotoXY(0, 15); cout << "Помех: " << Bug << " на 1000 бит, задержка в обновлении данных : " << Pause / 50 << "       ";
                         break;
                       };
        
        
        default: if (Pause) Sleep(Pause);
        };
        if (Data.All >= 50000) Done = 0;

    };
        //cout << "\n";
        GotoXY(0, 24);
        cout << "Программа завершена, для закрытия окна нажмите любую клавишу";



    
    while (_kbhit())  _getch();
    _getch();
    return 0;
}


int rnd(int a, int b)
{
    //random_device  rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(a, b); // диапазон случайеых чисел
    return dist(gen);
};


void CursorOff(void)
{
    CONSOLE_CURSOR_INFO curs = { 0 };
    curs.dwSize = sizeof(curs);
    curs.bVisible = FALSE;
    //  ::SetConsoleCursorInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &curs);
    SetConsoleCursorInfo(hStdin, &curs);
};

//--------------------------------------------------------------------------------------------------
void CursorOn(void)
{
    CONSOLE_CURSOR_INFO curs = { 0 };
    curs.dwSize = sizeof(curs);
    curs.bVisible = TRUE;
    SetConsoleCursorInfo(hStdin, &curs);

};

int PressKey(void)
{
    int key1 = 0;
    int key2 = 0;

    {
        if (_kbhit())
        {
            key1 = _getch();
            if ((key1 == 0) || (key1 == 0xE0)) key1 = 0xFF00;
            if (_kbhit())                      key2 = _getch();
     //       done = 0;
        };

    };
    return key1 + key2;


};

void GotoXY(int x, int y)
{
    COORD cPos;
    cPos.X = x;
    cPos.Y = y;
    SetConsoleCursorPosition(hStdin, cPos);
};

void OutData(string Template, string S,int CRC)  // Вывод данных
{
    if (Pause == 0) 
         return;
    // разные байты окрашиваем в красный цвет, Template -  образец, печатаем строку S
    SetConsoleTextAttribute(hStdin, 0x7); // устанавливаем белый цвет
    // если tEMPLATE =="" и CRC==1 то печатаем красным те байты, где нарушился контроль четности
    int i;
    if ((Template.length() == 0) && CRC == 1)
        for (i = 0; i < MaxL; i++)
        {
            switch (S[i])
            {
            case '0': SetConsoleTextAttribute(hStdin, 0x7); break;
            case '3': SetConsoleTextAttribute(hStdin, 0x7); break;
            case '5': SetConsoleTextAttribute(hStdin, 0x7); break;
            case '6': SetConsoleTextAttribute(hStdin, 0x7); break;
            case '9': SetConsoleTextAttribute(hStdin, 0x7); break;
            case 'A': SetConsoleTextAttribute(hStdin, 0x7); break;
            case 'C': SetConsoleTextAttribute(hStdin, 0x7); break;
            case 'F': SetConsoleTextAttribute(hStdin, 0x7); break;
            default:  SetConsoleTextAttribute(hStdin, 0x4); break;
            };
            cout << S[i];
        };
    // Если проверочный шаблон пуст, то просто печатаем строку;
    SetConsoleTextAttribute(hStdin, 0x7);
    if ((Template.length() == 0) && CRC == 0) cout << S;
    // Если есть проверочный шаблон, и CRC=0; то разные байты выделяем красасным 
    if ((Template.length() != 0) && CRC == 0)
        for (i = 0; i < MaxL; i++)
        {
           if (Template[i]==S[i]) SetConsoleTextAttribute(hStdin, 0x7); else SetConsoleTextAttribute(hStdin, 0x4); 
           cout << S[i];
        };
      
   SetConsoleTextAttribute(hStdin, 0x7);

};


void ClearReport(Report& a)
{
    a.All = 0;
    a.ErrorCRC=0;
    a.ErrorDetectCRC = 0;
    a.ErrorNoDetectCRC = 0;
    
    a.ErrorLine = 0;
    a.ErrorReanimator = 0;
    a.ErrorBad = 0;
    a.ErrorDublicate = 0;
};

void OutReport(Report a)
{
    GotoXY(0, 17);
    cout << "Передача трех битовых блоков с контролем четности: ";
    GotoXY(0, 18); 
    cout << "Передано : " << a.All << " Повреждено : " << a.ErrorCRC << " ("<<fixed << setprecision(1)<<setw(4) << 100.0 * a.ErrorCRC / a.All << "%) ";
    cout<<"Обнаружено: " << a.ErrorDetectCRC<<" (" <<fixed << setprecision(1) << setw(4) << 100.0 * a.ErrorDetectCRC/a.All<<"%) ";
    cout << "Не обнаружено : " << a.ErrorNoDetectCRC << " (" << fixed << setprecision(1) << setw(4) << 100.0 * a.ErrorNoDetectCRC / a.All << "%)     ";

    GotoXY(0, 20);
    cout << "Передача трех битовых блоков с использованием линейного кода (6,3): ";
    GotoXY(0, 21);
    cout << "Передано : " << a.All << " Повреждено : " << a.ErrorLine << " (" << fixed << setprecision(1) << setw(4) << 100.0 * a.ErrorLine / a.All << "%) ";
    cout << "Восстановлено: " << a.ErrorReanimator << " (" << fixed << setprecision(1) << setw(4) << 100.0 * a.ErrorReanimator / a.All << "%) ";
    cout << "Не восстановлено: " << a.ErrorBad << " (" << fixed << setprecision(1) << setw(4) << 100.0 * a.ErrorBad / a.All << "%)     ";
    GotoXY(0, 22);
    cout<<"Восстановлено с 2 поврежденными битами : " << a.ErrorDublicate<< " (" << fixed << setprecision(1) << setw(4) << 100.0 * a.ErrorDublicate / a.All << "%)     ";

};


/*
struct Report // отчет
{
    int All;              // передано блоков
    int ErrorCRC;         // повреждено блоков
    int ErrorDetectCRC;   // Обнаружено поврежденных блоков
    int ErrorNoDetectCRC; // Не обнаружено поврежденныйх блоков



    int ErrorLine;        // пореждено блоков
    int ErrorReanimator;   // восстановлено блоков
    int ErrorBad;

};
*/