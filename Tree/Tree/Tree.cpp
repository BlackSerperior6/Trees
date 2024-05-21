#include "Tree.h"

int main()
{
	srand(time(0));
	setlocale(LC_ALL, "RUS");

	int choice = -1;

	bool shouldStop = false;

	Tree<double> tree;

	while (!shouldStop)
	{
		cout << endl << endl << "1 - добавить элемент\n2 - удалить элемент\n3 - конвентировать в сбалансированое\n4 - конвентировать в поисковое\n5 - визуализация вертикальная\n6 - визуализация горизонатальная\n7 - сделать все три прохода\n8 - печать в консоль вертикальная\n9 - найти среднее арифметическое\n10 - закончить работу программы";

		cout << endl << endl << "Состояние дерева:" << endl;

		cout << "Сбалансировано? " << (tree.IsBalanced() ? "да" : "нет") << endl
			<< "Поисковое? " << (tree.IsSearchTree() ? "да" : "нет") << endl << endl;

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
				cout << "Такой элемент не найден в дереве" << endl;

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

			tree.PrintVerticaly();

			break;

		case 9:
		case 10:

			shouldStop = true;
			
			break;

		default:

			cout << endl << "Невалидное значение" << endl;
			break;
		}
	}
}