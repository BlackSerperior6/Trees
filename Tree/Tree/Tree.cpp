﻿#include "Tree.h"

int main()
{
	srand(time(0));
	setlocale(LC_ALL, "RUS");

	int choice = -1;

	bool shouldStop = false;

	Tree<double> tree;

	while (!shouldStop)
	{
		cout << endl << endl << "1 - добавить элемент\n2 - удалить элемент\n3 - конвентировать в сбалансированое (попытка активировать дважды приведет к тому, что дерево перестанет быть таковым)\n4 - конвентировать в поисковое (попытка активировать дважды приведет к тому, что дерево перестанет быть таковым)\n5 - визуализация вертикальная\n6 - визуализация горизонатальная\n7 - сделать все три прохода\n8 - закончить работу программы";

		cout << endl << endl << "Состояние дерева:" << endl;

		int amountOfElements = tree.GetAmountOfElements();

		cout << "Сбалансировано? " << (tree.IsBalanced() ? "да" : "нет") << endl
			<< "Поисковое? " << (tree.IsSearchTree() ? "да" : "нет") << endl << "Высота: " << tree.GetHight()
			<< endl << "Кол-во элементов: " << amountOfElements << endl 
			<< "Среднее арифметическое: " << 
			(amountOfElements == 0 ? 0 : (double) (tree.GetSumOfAllElements() / amountOfElements)) 
			<< endl << endl;

		tree.PrintVerticaly();

		cin >> choice;

		double elemenet;

		bool DidIt;

		switch (choice)
		{
		case 1:

			cout << endl << "Введите элемент!" << endl;

			cin >> elemenet;

			tree.Add(elemenet);

			break;

		case 2:

			cout << endl << "Введите элемент!" << endl;

			cin >> elemenet;

			DidIt = tree.Remove(elemenet);

			if (!DidIt)
				cout << endl << "Такой элемент не найден в дереве" << endl;

			break;

		case 3:

			tree.ConvertToBalanced();

			cout << endl << "Успешно!" << endl;

			break;

		case 4:

			tree.ConvertToSeacrhTree();

			cout << endl << "Успешно!" << endl;

			break;

		case 5:

			tree.DrawVertical();

			break;

		case 6:

			tree.DrawHorizontal();

			break;

		case 7:

			tree.ReadAllThreeWays();

			break;

		case 8:

			shouldStop = true;
			
			break;

		default:

			cout << endl << "Невалидное значение" << endl;
			break;
		}
	}
}