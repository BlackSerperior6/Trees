#pragma once
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>

using namespace std;

const string space = " ";


template <typename T>
class Tree
{
public:

	Tree() { Left = Right = Parent = nullptr; Data = nullptr; Balanced = true; }

	~Tree() { Clear(); }

	T& GetData()
	{
		return *Data;
	}

	bool IsBalanced() 
	{
		return Balanced;
	}

	int GetHight() 
	{
		if (Data == nullptr)
			return 0;

		return GetHight(this);
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

		if (Balanced)
		{
			bool result = RemoveFromBalanced(key, Left, true);

			if (!result)
				result = RemoveFromBalanced(key, Right, false);

			if (result)
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
			return;

		Balanced = true;

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
		if (!Balanced)
			return;

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

		if (Balanced)
			AddBalanced(data);
		else
			AddBySearchMethod(data);
	}

	void PrintHorizontaly()
	{
		if (Data == nullptr)
			return;

		PrintHor(0);
	}

	void PrintVerticaly()
	{
		if (Data == nullptr)
			return;

		vector<vector<T*>> Levels;

		int hight = GetHight();

		vector<Tree<T>*> bufferOfTrees;

		bufferOfTrees.push_back(this);

		for (int i = 0; i < hight; i++)
		{
			vector<T*> elements;

			vector<Tree<T>*> updatedBufferOfTrees;

			for (int j = 0; j < bufferOfTrees.size(); j++)
			{
				Tree<T>* current = bufferOfTrees[j];

				if (current == nullptr)
				{
					elements.push_back(nullptr);

					for (int i = 0; i < 2; i++)
						updatedBufferOfTrees.push_back(nullptr);
				}
				else
				{
					updatedBufferOfTrees.push_back(current->Left);
					updatedBufferOfTrees.push_back(current->Right);
					elements.push_back(current->Data);
				}
			}

			Levels.push_back(elements);

			bufferOfTrees.clear();

			for (int k = 0; k < updatedBufferOfTrees.size(); k++)
				bufferOfTrees.push_back(updatedBufferOfTrees[k]);
		}

		int* amountOfSpaces = new int[hight];

		amountOfSpaces[0] = 0;

		for (int i = 1; i < hight; i++)
			amountOfSpaces[i] = amountOfSpaces[i - 1] * 2 + 1;

		for (int i = hight - 1; i > -1; i--)
		{
			for (int j = 0; j < amountOfSpaces[i]; j++)
				cout << space;

			vector<T*> cur_Level = Levels[hight - i - 1];

			if (cur_Level[0] == nullptr)
				cout << "■";
			else
				cout << *cur_Level[0];

			for (int k = 1; k < cur_Level.size(); k++)
			{
				for (int u = 0; u < amountOfSpaces[i + 1]; u++)
					cout << space;
					

				if (cur_Level[k] == nullptr)
					cout << "■";
				else
				{
					cout << *cur_Level[k];
				}
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

	bool Balanced;

	void PrintHor(int depth = 0)
	{
		if (Right != nullptr)
			Right->PrintHor(depth + 1);

		for (int i = 0; i < depth; i++)
			cout << "   ";

		cout << *Data << endl;


		if (Left != nullptr)
			Left->PrintHor(depth + 1);
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

	bool RemoveFromBalanced(T key, Tree<T>* branch, bool isLeft)
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

		bool result = RemoveFromBalanced(key, branch->Left, true);

		if (!result)
			result = RemoveFromBalanced(key, branch->Right, false);

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
				Left->Balanced = false;
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
				Right->Balanced = false;
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
				Right->Add(data);
			}
			else
				Right->AddBalanced(data);
		}
	}

	friend double FindAvarage(Tree<double> &tree);
};

double FindAvarage(Tree<double> &tree) 
{
	if (tree.Data == nullptr)
		return 0;

	int amountOfElement = tree.GetAmountOfElements();
	double* buffer = new double[amountOfElement];

	int counter = 1;
	buffer[0] = *tree.Data;

	counter = tree.ReadBrachIntoArray(buffer, counter, tree.Left);
	counter = tree.ReadBrachIntoArray(buffer, counter, tree.Right);

	double sum = 0;

	for (int i = 0; i < amountOfElement; i++)
		sum += buffer[i];

	return sum / amountOfElement;
}
