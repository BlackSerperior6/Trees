#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <map>
#include <iomanip>
#include <sstream>
#include<string>

int NodeRadius;

using namespace sf;

using namespace std;

template <typename T>
class Tree
{
public:

	Tree() { Left = Right = Parent = nullptr; Data = nullptr; Balanced = SearchTree = false; }

	~Tree() { Clear(); }

	T& GetData()
	{
		return *Data;
	}

	bool IsBalanced() 
	{
		return Balanced;
	}

	bool IsSearchTree()
	{
		return SearchTree;
	}

	int GetHight() 
	{
		if (Data == nullptr)
			return 0;

		return GetHight(this);
	}

	double GetSumOfAllElements()
	{
		if (Data == nullptr)
			return 0;

		return SumHelper(this);
	}

	void ReadAllThreeWays()
	{
		if (Data == nullptr)
		{
			cout << "Пустое дерево" << endl;
			return;
		}

		cout << "Прямой способо: ";
		DirectWay(this);
		cout << endl << "Симметричиный способ: ";
		SymmetricWay(this);
		cout << endl << "Обратный способ: ";
		ReverseWay(this);
	}

	void Clear()
	{
		if (Left != nullptr)
			delete Left;

		if (Right != nullptr)
			delete Right;

		Left = Right = nullptr;

		delete Data;
		Data = nullptr;
	}

	int GetAmountOfElements()
	{
		if (Data == nullptr)
			return 0;

		return GetAmountofElements(this);
	}

	bool Remove(T key)
	{
		if (Data == nullptr)
			return false;

		if (*Data == key)
		{
			Clear();
			return true;
		}

		if (!SearchTree)
		{
			bool result = RemoveFromANonSearchTree(key, Left, true);

			if (!result)
				result = RemoveFromANonSearchTree(key, Right, false);

			if (result && Balanced)
			{
				Balanced = false;
				ConvertToBalanced();
			}

			return result;
		}

		Tree<T>* current;
		bool flag = false;

		if (key < *Data)
			current = Left;
		else
			current = Right;

		while (current != nullptr && !flag)
		{
			if (key < *current->Data)
				current = current->Left;

			else if (key > *current->Data)
				current = current->Right;

			else
			{
				if (current->Parent->Left == current)
					current->Parent->Left = nullptr;
				else
					current->Parent->Right = nullptr;

				delete current;

				flag = true;
			}
		}

		return flag;
	}

	void ConvertToBalanced()
	{
		if (Balanced)
		{
			Balanced = false;

			if (Left != nullptr)
				Left->ConvertToBalanced();

			if (Right != nullptr)
				Right->ConvertToBalanced();

			return;
		}

		Balanced = true;
		SearchTree = false;

		if (Data == nullptr)
			return;

		T* Buffer = new T[GetAmountOfElements()];
		int counter = 1;
		Buffer[0] = *Data;

		counter = ReadBrachIntoArray(Buffer, counter, Left);
		counter = ReadBrachIntoArray(Buffer, counter, Right);

		Clear();

		for (int i = 0; i < counter; i++)
			Add(Buffer[i]);

		delete[] Buffer;
	}

	void ConvertToSeacrhTree()
	{
		if (SearchTree)
		{
			SearchTree = false;

			if (Left != nullptr)
				Left->ConvertToSeacrhTree();

			if (Right != nullptr)
				Right->ConvertToSeacrhTree();

			return;
		}

		SearchTree = true;
		Balanced = false;

		if (Data == nullptr)
			return;

		T* Buffer = new T[GetAmountOfElements()];
		int counter = 1;
		Buffer[0] = *Data;

		counter = ReadBrachIntoArray(Buffer, counter, Left);
		counter = ReadBrachIntoArray(Buffer, counter, Right);

		Clear();

		for (int i = 0; i < counter; i++)
			Add(Buffer[i]);

		delete[] Buffer;
	}

	void Add(T data)
	{
		if (Data == nullptr)
		{
			Data = new T;
			*Data = data;
			return;
		}

		if (!Balanced && !SearchTree)
		{
			int choice;

			do
			{
				cout << endl << endl << "Куда пойдем?\n1 - влево!\n2 - вправо" << endl << endl;
				cin >> choice;
			} while (choice != 1 && choice != 2);

			if (choice == 1)
			{
				if (Left == nullptr)
				{
					Left = new Tree<T>();
					Left->Parent = this;
				}

				Left->Add(data);
			}
			else
			{
				if (Right == nullptr)
				{
					Right = new Tree<T>();
					Right->Parent = this;
				}

				Right->Add(data);
			}

			return;
		}

		else if (Balanced)
			AddBalanced(data);
		else
			AddBySearchMethod(data);
	}

	void DrawVertical()
	{
		vector<vector<Tree<T>*>> Levels; //Двумерный массив, изображающий собой дерево

		int hight = GetHight(); //Высота

		ReadIntoVector(Levels, hight); //Заполнение массива

		int* amountOfSpaces = new int[hight]; //Массив пробелов

		amountOfSpaces[0] = 1; //В самом низу пробел 1

		for (int i = 1; i < hight; i++)
			amountOfSpaces[i] = amountOfSpaces[i - 1] * 2 + 1; //Расчет кол-ва пробело

		NodeRadius = 100 / hight; //Расчет радиуса вершины дерева
		int HightDifference = (NodeRadius * 2) + 10; //Расчет разницы по y между уровнями

		//Создание окна, рассчет его размеров
		RenderWindow window(VideoMode(amountOfSpaces[hight - 1] * NodeRadius * 4, 
			(hight * HightDifference) + NodeRadius), "Binary Tree");

		//Базовый цикл работы окна
		while (window.isOpen())
		{
			Event ev;

			while (window.pollEvent(ev))
			{
				if (ev.type == Event::Closed)
					window.close();
			}

			window.clear(Color(128, 128, 128)); //Заливка серым

			map<Tree<T>*, Vector2f> Positions; //Словарь вершин дерева и их координат

			int y = hight - HightDifference + (NodeRadius * 2); //Изначальная y координата

			for (int i = hight - 1; i > -1; i--)
			{
				int x = NodeRadius * 2 * amountOfSpaces[i]; //Расчет x первого элм. уровня
				y += HightDifference; //Расчет y

				vector<Tree<T>*> cur_Level = Levels[hight - i - 1]; //Получаем уровень дерева

				for (int k = 0; k < cur_Level.size(); k++)
				{
					//Если элемент не первый, то перерасчет x
					x += (k == 0 ? 0 : NodeRadius * 2 * amountOfSpaces[i + 1]); 

					if (cur_Level[k] != nullptr) //Если элемент уровня не 0
					{
						Positions[cur_Level[k]] = Vector2f(x, y); //Заносим вершину в словарь
						DrawNode(cur_Level[k], Positions, window); //Рисуем вершину
					}

					x += NodeRadius * 2; //Сдвиг по x обязателен
				}
			}

			window.display(); //Отображение
		}

		delete[] amountOfSpaces; //Чистка памяти
	}

	void DrawHorizontal()
	{
		vector<vector<Tree<T>*>> Levels; //Двумерный массив, изображающий собой дерево

		int hight = GetHight(); //Высота

		ReadIntoVector(Levels, hight); //Заполнение массива

		int* amountOfSpaces = new int[hight]; //Массив пробелов

		amountOfSpaces[0] = 1; //В самом низу пробел 1

		for (int i = 1; i < hight; i++)
			amountOfSpaces[i] = amountOfSpaces[i - 1] * 2 + 1; //Расчет кол-ва пробело

		NodeRadius = 100 / (hight + 2); //Расчет радиуса вершины дерева
		int XDifference = (NodeRadius * 2) + 10; //Расчет разницы по y между уровнями

		//Создание окна, рассчет его размеров
		RenderWindow window(VideoMode((hight * XDifference) + NodeRadius,
			amountOfSpaces[hight - 1] * NodeRadius * 4), "Binary Tree");

		//Базовый цикл работы окна
		while (window.isOpen())
		{
			Event ev;

			while (window.pollEvent(ev))
			{
				if (ev.type == Event::Closed)
					window.close();
			}

			window.clear(Color(128, 128, 128)); //Заливка серым

			map<Tree<T>*, Vector2f> Positions; //Словарь вершин дерева и их координат

			int x = hight - XDifference + (NodeRadius * 2); //Изначальная x координата

			for (int i = hight - 1; i > -1; i--)
			{
				int y = NodeRadius * 2 * amountOfSpaces[i]; //Расчет y первого элм. уровня
				x += XDifference; //Расчет x

				vector<Tree<T>*> cur_Level = Levels[hight - i - 1]; //Получаем уровень дерева

				for (int k = 0; k < cur_Level.size(); k++)
				{
					//Если элемент не первый, то перерасчет x
					y += (k == 0 ? 0 : NodeRadius * 2 * amountOfSpaces[i + 1]);

					if (cur_Level[k] != nullptr) //Если элемент уровня не 0
					{
						Positions[cur_Level[k]] = Vector2f(x, y); //Заносим вершину в словарь
						DrawNode(cur_Level[k], Positions, window); //Рисуем вершину
					}

					y += NodeRadius * 2; //Сдвиг по y обязателен
				}
			}

			window.display(); //Отображение
		}

		delete[] amountOfSpaces; //Чистка памяти
	}

	void PrintVerticaly()
	{
		if (Data == nullptr)
			return;

		vector<vector<Tree<T>*>> Levels;

		int hight = GetHight();

		ReadIntoVector(Levels, hight);

		int* amountOfSpaces = new int[hight];

		amountOfSpaces[0] = 1;

		int width = 0;

		GetMaxLenght(this, width);

		for (int i = 1; i < hight; i++)
			amountOfSpaces[i] = amountOfSpaces[i - 1] * 2 + 1;

		for (int i = hight - 1; i > -1; i--)
		{
			vector<Tree<T>*> cur_Level = Levels[hight - i - 1];

			for (int k = 0; k < cur_Level.size(); k++)
			{
				bool IsNullPtr = cur_Level[k] == nullptr;
				
				int spaces = k == 0 ? amountOfSpaces[i] : amountOfSpaces[i + 1];
					
				for (int j = 0; j < spaces; j++)
					cout << setw(width) << " ";

				if (cur_Level[k] != nullptr)
					cout << setw(width) << cur_Level[k]->GetData();
				else
					cout << setw(width) << " ";
			}

			cout << endl << endl;
		}

		delete[] amountOfSpaces;
	}

private:

	Tree<T>* Parent;
	Tree<T>* Left;
	Tree<T>* Right;

	T* Data;

	bool SearchTree;
	bool Balanced;

	double SumHelper(Tree<T>* branch)
	{
		if (branch == nullptr)
			return 0;

		return *branch->Data + SumHelper(branch->Left) + SumHelper(branch->Right);
	}

	void DirectWay(Tree<T>* branch) 
	{
		if (branch == nullptr)
			return;

		cout << *branch->Data << " ";
		DirectWay(branch->Left);
		DirectWay(branch->Right);
	}

	void SymmetricWay(Tree<T>* branch) 
	{
		if (branch == nullptr)
			return;

		SymmetricWay(branch->Left);
		cout << *branch->Data << " ";
		SymmetricWay(branch->Right);
	}

	void ReverseWay(Tree<T>* branch)
	{
		if (branch == nullptr)
			return;

		ReverseWay(branch->Left);
		ReverseWay(branch->Right);
		cout << *branch->Data << " ";
	}

	int GetHight(Tree<T>* branch)
	{
		if (branch == nullptr)
			return 0;

		int leftHight = GetHight(branch->Left);
		int rightHight = GetHight(branch->Right);

		return (rightHight > leftHight ? rightHight : leftHight) + 1;
	}

	bool RemoveFromANonSearchTree(T key, Tree<T>* branch, bool isLeft)
	{
		if (branch == nullptr)
			return false;

		if (*branch->Data == key)
		{
			if (isLeft)
				branch->Parent->Left = nullptr;
			else
				branch->Parent->Right = nullptr;

			delete branch;

			return true;
		}

		bool result = RemoveFromANonSearchTree(key, branch->Left, true);

		if (!result)
			result = RemoveFromANonSearchTree(key, branch->Right, false);

		return result;	
	}

	int GetAmountofElements(Tree<T>* branch)
	{
		if (branch == nullptr)
			return 0;

		int leftSide = GetAmountofElements(branch->Left);
		int rightSide = GetAmountofElements(branch->Right);

		return leftSide + rightSide + 1;
	}

	int ReadBrachIntoArray(T* arr, int currentIndex, Tree<T>* branch)
	{
		if (branch == nullptr)
			return currentIndex;

		arr[currentIndex] = *branch->Data;

		currentIndex++;

		currentIndex = ReadBrachIntoArray(arr, currentIndex, branch->Left);
		currentIndex = ReadBrachIntoArray(arr, currentIndex, branch->Right);

		return currentIndex;
	}

	void AddBySearchMethod(T data)
	{
		if (Balanced)
		{
			cout << "Сначала преобразуйте дерево в дерево поиска!" << endl;
			return;
		}

		if (data == *Data)
			return;

		if (data < *Data)
		{
			if (Left == nullptr)
			{
				Left = new Tree<T>();
				Left->Parent = this;
				Left->SearchTree = true;
				Left->Add(data);
			}
			else
				Left->AddBySearchMethod(data);
		}
		else
		{
			if (Right == nullptr)
			{
				Right = new Tree<T>();
				Right->Parent = this;
				Right->SearchTree = true;
				Right->Add(data);
			}
			else
				Right->AddBySearchMethod(data);
		}
	}

	void AddBalanced(T data)
	{
		if (!Balanced)
		{
			cout << "Сначало сбалансируйте дерево!" << endl;
			return;
		}

		int leftSide = GetAmountofElements(Left);
		int rightSide = GetAmountofElements(Right);

		if (rightSide > leftSide)
		{
			if (Left == nullptr)
			{
				Left = new Tree<T>();
				Left->Parent = this;
				Left->Balanced = true;
				Left->Add(data);
			}
			else
				Left->AddBalanced(data);
		}
		else
		{
			if (Right == nullptr)
			{
				Right = new Tree<T>();
				Right->Parent = this;
				Right->Balanced = true;
				Right->Add(data);
			}
			else
				Right->AddBalanced(data);
		}
	}

	void DrawNode(Tree<T>* branch, map<Tree<T>*, Vector2f>& positions, RenderWindow& window)
	{
		Vector2f position = positions[branch]; //Получаем позицию ветки

		CircleShape circle(NodeRadius); //Генерируем круг

		//Настройка внешнего вида круга
		circle.setFillColor(Color::White);
		circle.setOutlineColor(Color::Black);
		circle.setOutlineThickness(3);

		//Сдвиг круга по его же радиусу
		circle.setPosition(position.x - NodeRadius, position.y - NodeRadius);

		T element = branch->GetData(); //Получаем элемент

		//Обработка чисел с плавающей точкой
		Text text;
		ostringstream buffer;
		buffer << fixed << setprecision(1) << element;

		//Загрузка шрифта
		Font font;
		font.loadFromFile("CyrilicOld.TTF");
		text.setFont(font);

		//Настройка текста
		text.setString(buffer.str());
		text.setFillColor(Color::Black);
		text.setOutlineColor(Color::White);
		text.setCharacterSize(NodeRadius);

		//Отцентровка текста
		FloatRect textRect = text.getLocalBounds();
		text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		text.setPosition(Vector2f(position.x, position.y));

		//Рисуем круг
		window.draw(circle);

		//Рисуем текст
		window.draw(text);

		if (branch->Parent != nullptr) //Если есть родитель
		{
			Vector2f ParentPositionBoundary = CalculateBoundaryPoint(position, positions[branch->Parent], NodeRadius + 2); //Позиция родителя
			Vector2f ChildPossitionBoundary = CalculateBoundaryPoint(positions[branch->Parent], position, NodeRadius + 2);

			//Массив вершин, представляющий собой линию
			VertexArray line(Lines, 2);

			//Задаем позицию начала и конца линии
			line[0].position = ParentPositionBoundary;
			line[1].position = ChildPossitionBoundary;

			//Рисуем
			window.draw(line);
		}
	}

	Vector2f CalculateBoundaryPoint(Vector2f point1, Vector2f point2, double minus)
	{
		double Lenght = GetSideLenght(point1, point2) - minus;

		double a = atan2(point2.y - point1.y, point2.x - point1.x);

		return Vector2f(point1.x + Lenght * cos(a), point1.y + Lenght * sin(a));
	}

	void GetMaxLenght(Tree<T> *branch, int& currentLenght)
	{
		if (branch == nullptr)
			return;

		int fullPart = (int) *branch->Data;

		int lenghtToCompare;

		if (fullPart - *branch->Data == 0)
			lenghtToCompare = to_string(fullPart).length();
		else
			lenghtToCompare = to_string(*branch->Data).length();

		if (currentLenght < lenghtToCompare)
			currentLenght = lenghtToCompare;

		GetMaxLenght(branch->Left, currentLenght);
		GetMaxLenght(branch->Right, currentLenght);
	}

	double GetSideLenght(Vector2f& point1, Vector2f& point2) //Метод нахождения длины между двумя точками
	{
		return sqrt(pow(point2.x - point1.x, 2) + pow(point2.y - point1.y, 2));
	}

	void ReadIntoVector(vector<vector<Tree<T>*>> &container, int hight)
	{
		vector<Tree<T>*> bufferOfBranches; //Буфер деревьев - потомков

		bufferOfBranches.push_back(this); //Заносим корень дерева

		for (int i = 0; i < hight; i++)
		{
			vector<Tree<T>*> branchesOnTheLevel; //Вектор вершин на уровне i

			vector<Tree<T>*> updatedBufferOfTrees; //Вектор потомков

			for (int j = 0; j < bufferOfBranches.size(); j++)
			{
				Tree<T>* current = bufferOfBranches[j]; //Получаем текущую вершину

				if (current == nullptr) //Если нулевая
				{
					branchesOnTheLevel.push_back(nullptr); //Заносим в вектор нулевой элемент

					for (int i = 0; i < 2; i++)
						updatedBufferOfTrees.push_back(nullptr); //И два нулевых элемента в вектор потомков
				}
				else
				{
					//Заносим потомков
					updatedBufferOfTrees.push_back(current->Left);
					updatedBufferOfTrees.push_back(current->Right);

					//И сам элемент
					branchesOnTheLevel.push_back(current);
				}
			}

			//Записываем в двумерный массив вершины уровня
			container.push_back(branchesOnTheLevel);

			//Очистка предыдущего буффера
			bufferOfBranches.clear();

			//Занос в него новых элементов
			for (int k = 0; k < updatedBufferOfTrees.size(); k++)
				bufferOfBranches.push_back(updatedBufferOfTrees[k]);
		}
	}
};
