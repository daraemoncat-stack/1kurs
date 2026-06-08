#include <iostream>
#include <clocale>
using namespace std;

void zapis(const char* FileName, int n)
{
	const char* mode = "w";
	FILE* f;
	char input[256];
	errno_t err = fopen_s(&f,FileName, mode);
	if (err != 0 or f == nullptr)
	{
		cout << "Ошибка открытия файла 1";
		return;
	}
	for (int i = 0; i < n; i++)
	{
		cin.getline(input, sizeof(input));
		fputs(input, f);
		fputs("\n", f);
	}
	fclose(f);
	return;
}

void write(const char* FileOut, const char* FileIn)
{
	char buffer[256];
	FILE* f1;
	FILE* f2;
	char stroka[256];
	errno_t err1 = fopen_s(&f1, FileOut, "r");
	errno_t err2 = fopen_s(&f2, FileIn, "w");
	if (err1 != 0 or err2 != 0 or f1 == nullptr or f2 == nullptr)
	{
		cout << "Ошибка открытия файла 2";
		return;
	}
	while (fgets(buffer, sizeof(buffer), f1))
	{
		strncpy(stroka, buffer, sizeof(stroka));
		char* FirstWord = strtok(buffer, ".,!?\n ");
		bool dubl = false;
		char* NextWord = strtok(nullptr, ".,!?\n ");
		while (NextWord)
		{
			if (strcmp(FirstWord, NextWord) == 0)
			{
				dubl = true;
			}
			NextWord = strtok(nullptr, ".,!?\n ");
		}
		if (dubl == false)
		{
			fputs(stroka, f2);
		}
	}
	fclose(f1);
	fclose(f2);
}

int sogl(const char* FileName)
{
	int kol = 0;
	FILE* f;
	errno_t err = fopen_s(&f, FileName, "r");
	if (err != 0 or f == nullptr)
	{
		cerr << "Ошибка открытия файла 3";
		return 3;
	}
	char buffer[256];
	if (fgets(buffer, sizeof(buffer), f))
	{
		for (int i = 0; buffer[i] != '\n'; i++)
		{
			if (not(buffer[i] == 'a' or buffer[i] == 'e' or buffer[i] == 'i' or buffer[i] == 'o' or buffer[i] == 'u' or buffer[i] == 'y' or buffer[i] == '\0' or buffer[i]==' '))
			{
				kol++;
			}
		}
	}
	fclose(f);
	return kol;
}

void print(const char* FileName)
{
	FILE* f;
	char buffer[256];
	errno_t err = fopen_s(&f, FileName, "r");
	if (err != 0 or f == nullptr)
	{
		cerr << "Ошибка открытия файла 4";
		fclose(f);
	}
	while (fgets(buffer, sizeof(buffer), f))
	{
		cout << buffer << endl;
	}
	fclose(f);
}

int main()
{
	setlocale(LC_ALL, "RUS");
	int kol;
	cout << "Введите количество строк" << endl;
	cin >> kol;
	cin.ignore();
	cout<<"Введите сами строки"<<endl;
	zapis("File1.txt", kol);
	write("File1.txt", "File2.txt");
	cout << "============" << endl << "File 1" << endl;
	print("File1.txt");
	cout << "============" << endl << "File 2" << endl;
	print("File2.txt");
	cout << endl << "Количество согласных: " << sogl("File2.txt") << endl;
}


