#include "Tree.h"

int main()
{
	srand(time(0));
	setlocale(LC_ALL, "RUS");

	Tree<double> tree;

	int amount;

	do
	{
		cout << "Введите кол-во элементов: " << endl; cin >> amount;
	} 
	while (amount <= 0);

	tree.ConvertToSeacrhTree();

	for (int i = 0; i < amount; i++)
		tree.Add(rand() % 100);

	tree.Draw();

	tree.ConvertToBalanced();

	tree.Draw();
}
