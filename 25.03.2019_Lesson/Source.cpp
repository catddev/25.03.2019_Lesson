#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include<math.h>
#include<Windows.h>
#include<ctime>
#include<cstring>
#include<fstream>

// !!!!!!!!!! под union выделяется память одна  для всех элементов и размером под самое большое поле (например, double),
// и одновременно эту память может использовать только один элемент
union example {
	int a; //4 b, в int максимально можно положить 32 единицы
	double b; //8 b
	char c; //1 b
};

using namespace std;

//47. Реализовать структуру «Машина» (цвет, модель, номер).
//Номер машины может представлять из себя или 5-ти значный номер или слово до 8 символов.
//Рекомендации: номер реализовать как объединение.
	//А)** Создать экземпляр структуры «Машина» и реализовать для него следующие функции :
	//•	Заполнение машины
	//•	Печать машины
	//Б)*** Создать массив из 10 экземпляров структуры «Машина» и реализовать для него следующие функции :
	//•Редактировать машину
	//•Печать всех машин
	//•Поиск машины по номеру

int current_size = 0;
int buffer_size = 0;
ifstream in_file("in.txt");

union number {
	// int numeric : 9; //вместо 32 битов(4 байта) используется только 9 - битовое разграничение полей, битовое поле запрещено вводить с клавиатуры,
	// т.к. нельхя работать с адресами и создавать массив битовых полей,
	// т.е. можно объявить только единичное битовое поле. Битовое поле можно только int и char
	short int numeric; //2 bites
	char str_num[9]; // при выделении 9 байтов, short int записывается только в последние 2 байта
};
//struct Car {
//	char color[30];
//	char model[30];
//	number nomer;
//};

struct Car {
	char color[30];
	char model[30];
	char flag; //флажок для проверки пользователь ввел числовое или строковое значение
	//!!!!!!!! з0авести привычку использовать флажки
	union { // если за пределами этой структуры не будем использовать, можно объявлять вложенный union
		short int numeric; //срежет любое число больще 32000, помещается только 2 в 15 степени (32768)
		char str_num[10]; //изначально по заданию 9 b, потом сами увеличили
	}nomer;

	//void enter_car(){ // метод для заполнения пользователем
	//	int choice;
	//	cin >> color >> model;

	//	cout << "Enter 1 to enter numeric value" << endl;
	//	cout << "Enter 2 to enter string value" << endl;
	//	cin >> choice;
	//	switch (choice) {
	//	case 1:
	//		flag = 'n';
	//		cin >> nomer.numeric;
	//		break;
	//	case 2:
	//		flag = 's';
	//		cin >> nomer.str_num;
	//		break;
	//	}

	void enter_car() { // метод для считывания с файла
		in_file >> color >> model; //здесь in_file вместо cin можно т.к. объявлен глобальный ifstream с in.txt

		in_file >> flag;
		switch (flag) {
		case 'n':
			in_file >> nomer.numeric;
			break;
		case 's':
			in_file >> nomer.str_num;
			break;
		}
	}
	void print_car() {
		cout << color << " " << model << " ";
		if (flag == 'n')
			cout << nomer.numeric << endl;
		else if (flag == 's')
			cout << nomer.str_num << endl;
	}
};

void add(Car*&cc, Car c) {
	if (buffer_size == 0)
	{
		buffer_size = 4;
		cc = new Car[buffer_size];
	}
	else
	{
		if (current_size == buffer_size)
		{
			buffer_size *= 2;
			Car*tmp = new Car[buffer_size];
			for (int i = 0; i < current_size; i++)
				tmp[i] = cc[i];
			delete[] cc;
			cc = tmp;
		}
	}
	cc[current_size++] = c;
}
void erase_by_number(Car*&cs, char flag) {
	char str[10];
	short int number;
	int index = -1;
	if (flag == 's')
	{
		cout << "Enter string value to erase" << endl;
		cin >> str;
		for (int i = 0; i < current_size; i++)
		{
			if (strcmp(cs[i].nomer.str_num, str) == 0)
			{
				index = i;
				break;
			}
		}
	}
	else if (flag == 'n')
	{
		cout << "Enter numeric value to erase" << endl;
		cin >> number;
		for (int i = 0; i < current_size; i++)
		{
			if (cs[i].nomer.numeric == number)
			{
				index = i;
				break;
			}
		}
	}
	if (index != -1)
	{
		for (int i = index; i < current_size - 1; i++)
		{
			cs[i] = cs[i + 1];
		}
		current_size--;
	}
}
void search(Car*cs) {
	char flag;
	cout << "Enter 'n' to enter numeric value" << endl;
	cout << "Enter 's' to enter string value" << endl;
	cin >> flag;
	int k = 0;
	char nom[10];
	int nomer = 0;

	switch (flag)
	{
	case 'n':
		cin >> nomer;
		k = 0;
		for (int i = 0; i < current_size; i++)
		{
			if (cs[i].nomer.numeric == nomer)
			{
				cs[i].print_car();
				k++;
			}
		}
		if (k == 0) cout << "No sought-for car number in the database" << endl;
		break;
	case 's':
		cin >> nom;
		k = 0;
		for (int i = 0; i < current_size; i++)
		{
			if (strcmp(cs[i].nomer.str_num, nom) == 0)
			{
				cs[i].print_car();
				k++;
			}
		}
		if (k == 0) cout << "No sought-for car number in the database" << endl;
		break;
	}
}
void edit(Car*&cs, char flag) {
	char str[10];
	short int number;
	int index = -1;
	int choice;

	cout << endl << "EDITING" << endl;
	if (flag == 's')
	{
		cout << "Enter string value to search" << endl;
		cin >> str;
		for (int i = 0; i < current_size; i++)
			if (strcmp(cs[i].nomer.str_num, str) == 0)
			{
				index = i;
				break;
			}
	}
	else if (flag == 'n')
	{
		cout << "Enter numeric value" << endl;
		cin >> number;
		for (int i = 0; i < current_size; i++)
			if (cs[i].nomer.numeric == number)
			{
				index = i;
				break;
			}
	}

	if (index != -1)
	{
		cout << "Enter 1 to change color" << endl;
		cin >> choice;
		if (choice == 1)
		{
			cin >> cs[index].color;
		}
		cout << "Enter 2 to change number" << endl;
		cin >> choice;
		if (choice == 2)
		{
			cout << "Enter s-for string number, n-for numeric number" << endl;
			cin >> cs[index].flag;

			switch (cs[index].flag)
			{
			case 's':
				cin >> cs[index].nomer.str_num;
				break;
			case 'n':
				cin >> cs[index].nomer.numeric;
				break;
			}
		}
	}
}

struct my_example {
	int kod : 11; //727
	int part1 : 11; //345
	int part2 : 11; //567
	//поставили ограничение по 10 битов (т.к. максимальное трехзначное число 999 - пормещается в 10 битов + 1 запасной)
	//это можно посмотреть в калькуляторе в режиме "программист": например, максиммально двузначное число 99 занимает 8 бит)
};

// 20.	**Разработайте структуру «Квартира»(номер квартиры, кол - во комнат, общая площадь).
// Разработайте структуру «Дом»(номер, кол - во квартир, массив квартир).
// Создайте экземпляр структуры  и реализуйте для него следующие функции :
//•	Печать всех квартир
//•	Добавление квартиры
//•	Удаление квартиры
//Примечание : массив квартир можно сделать статическим.
template<typename T> //шаблонная функция для любого типа теперь
void add(T*&cc, T c) {
	if (buffer_size == 0)
	{
		buffer_size = 4;
		cc = new T[buffer_size];
	}
	else
	{
		if (current_size == buffer_size)
		{
			buffer_size *= 2;
			T*tmp = new T[buffer_size];
			for (int i = 0; i < current_size; i++)
				tmp[i] = cc[i];
			delete[] cc;
			cc = tmp;
		}
	}
	cc[current_size++] = c;
}
struct flat {
	int flat_no : 8;
	int rooms : 4;
	int area : 8;

	void print() {
		cout << flat_no << " " << rooms << " " << area << endl;
	}
};
struct home {
	int home_nom : 10; //10 битов на 400 домов по калькулятору 9+1
	int flats : 8;
	flat *fs;
	int current_size = 0;
	int buffer_size = 0;
};
void erase(flat*&fs) {

	int number;
	int index = -1;
	cout << "Enter number of the flat to erase" << endl;
	cin >> number;
	for (int i = 0; i < current_size; i++)
	{
		if (fs[i].flat_no == number)
		{
			index = i;
			break;
		}
	}
	if (index != -1)
	{
		for (int i = index; i < current_size - 1; i++)
		{
			fs[i] = fs[i + 1];
		}
		current_size--;
	}
}


int main()
{
	srand(time(NULL));

	setlocale(LC_ALL, "Rus");

	int tn = 0;

	while (true)
	{
		cout << "Задача № ";
		cin >> tn; cin.get();

		switch (tn)
		{
		case 1:
		{
			number ex_number;
			strcpy_s(ex_number.str_num, "500A"); //вытащить можно строго как записано в union
			cout << ex_number.numeric << endl;
			cout << ex_number.str_num << endl;
		}
		break;
		case 2:
		{
			Car c;
			c.enter_car();
			c.print_car();
		}
		break;
		case 3:
		{
			Car *cs = 0;
			Car tmp;

			if (!in_file)
				cerr << "File open error" << endl;
			else
			{
				while (!in_file.eof())
				{
					tmp.enter_car();
					add(cs, tmp);
				}
			}
			for (int i = 0; i < current_size; i++)
				cs[i].print_car();

			search(cs);

			edit(cs, 's');
			for (int i = 0; i < current_size; i++)
				cs[i].print_car();

			erase_by_number(cs, 'n');
			for (int i = 0; i < current_size; i++)
				cs[i].print_car();

		}
		break;
		case 4:
		{
			cout << sizeof(my_example) << endl;

			my_example ex;
			ex.kod = 727;
			ex.part1 = 345;
			ex.part2 = 567;
			cout << "(" << ex.kod << ")" << ex.part1 << "-" << ex.part2 << endl;
		}
		break;
		case 5://почему кейс в одном сеансе консоли срабатывает лишь раз? при повторном вызове ошибка
		{
			flat f;
			f.flat_no = 58;
			f.rooms = 4;
			f.area = 100;

			cout << f.flat_no << " " << f.rooms << " " << f.area << endl;

			flat *fs = 0;
			ifstream in2("in2.txt");
			int fn, rs, area;
			flat tmp;
			while (!in2.eof())
			{
				in2 >> fn >> rs >> area;
				tmp.flat_no = fn;
				tmp.rooms = rs;
				tmp.area = area;
				add(fs, tmp);
			}
			for (int i = 0; i < current_size; i++)
				fs[i].print();

			erase(fs);
			for (int i = 0; i < current_size; i++)
				fs[i].print();

			delete[] fs;

		}
		break;
		case 6:
		{

		}
		break;
		case 7:
		{

		}
		break;
		case 8:
		{

		}
		break;
		case 9:
		{

		}
		break;
		case 10:
		{

		}
		break;
		default:
			cout << "нет такой задачи" << endl << endl;
		}
	}
	system("pause");
	return 0;
}