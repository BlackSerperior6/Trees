#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <map>
#include <iomanip>
#include <sstream>
#include<string>

int NodeRadius = 20;

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

	void Draw()
	{
		vector<vector<Tree<T>*>> Levels;

		int hight = GetHight();

		ReadIntoVector(Levels, hight);

		int* amountOfSpaces = new int[hight];

		amountOfSpaces[0] = 1;

		for (int i = 1; i < hight; i++)
			amountOfSpaces[i] = amountOfSpaces[i - 1] * 2 + 1;

		NodeRadius = 100 / hight;
		int HightDifference = (NodeRadius * 2) + 10;

		RenderWindow window(VideoMode(amountOfSpaces[hight - 1] * NodeRadius * 4, (hight * HightDifference) + NodeRadius), "Binary Tree");

		while (window.isOpen())
		{
			Event ev;

			while (window.pollEvent(ev))
			{
				if (ev.type == Event::Closed)
					window.close();
			}

			window.clear(Color(128, 128, 128)); //Серый

			map<Tree<T>*, Vector2f> Positions;

			int y = hight - HightDifference + (NodeRadius * 2);

			for (int i = hight - 1; i > -1; i--)
			{
				int x = NodeRadius * 2 * amountOfSpaces[i];
				y += HightDifference;

				vector<Tree<T>*> cur_Level = Levels[hight - i - 1];

				for (int k = 0; k < cur_Level.size(); k++)
				{
					x += (k == 0 ? 0 : NodeRadius * 2 * amountOfSpaces[i + 1]);

					if (cur_Level[k] != nullptr)
					{
						Positions[cur_Level[k]] = Vector2f(x, y);
						DrawNode(cur_Level[k], Positions, window);
					}

					x += NodeRadius * 2;
				}
			}

			window.display();
		}

		delete[] amountOfSpaces;
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

		for (int i = 1; i < hight; i++)
			amountOfSpaces[i] = amountOfSpaces[i - 1] * 2 + 1;

		for (int i = hight - 1; i > -1; i--)
		{
			vector<Tree<T>*> cur_Level = Levels[hight - i - 1];

			for (int k = 0; k < cur_Level.size(); k++)
			{
				bool IsNullPtr = cur_Level[k] == nullptr;
				
				int spaces = k == 0 ? amountOfSpaces[i] : amountOfSpaces[i + 1];

				if (!IsNullPtr)
				{
					ostringstream buffer;

					T element = cur_Level[k]->GetData();

					buffer << fixed <<  setprecision(1) << element;

					int fullpart = (int) element;
					
					spaces -= (buffer.str().length() - 3);
				}
					

				for (int j = 0; j < spaces; j++)
					cout << " ";

				if (!IsNullPtr)
					cout << cur_Level[k]->GetData();
				else
					cout << " ";
			}

			cout << endl << endl;
		}

		delete[] amountOfSpaces;
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
		SearchTree = false;

		if (Data == nullptr)
			return;

		vector<vector<T>*> Branches;

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
			return;

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
			}
			while (choice != 1 && choice != 2);

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

	void PrintHorizontaly()
	{
		if (Data == nullptr)
			return;

		PrintHor(0);
	}

private:

	Tree<T>* Parent;
	Tree<T>* Left;
	Tree<T>* Right;
	bool SearchTree;
	bool Balanced;

	T* Data;

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
		Vector2f position = positions[branch];

		CircleShape circle(NodeRadius);

		circle.setFillColor(Color::White);
		circle.setOutlineColor(Color::Black);
		circle.setOutlineThickness(3);
		circle.setPosition(position.x - NodeRadius, position.y - NodeRadius);

		T element = branch->GetData();

		Text text;
		ostringstream buffer;
		buffer << fixed << setprecision(1) << element;

		Font font;
		font.loadFromFile("CyrilicOld.TTF");
		text.setFont(font);

		text.setString(buffer.str());
		text.setFillColor(Color::Black);
		text.setOutlineColor(Color::White);
		text.setCharacterSize(NodeRadius);

		FloatRect textRect = text.getLocalBounds();
		text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		text.setPosition(Vector2f(position.x, position.y));

		window.draw(circle);

		window.draw(text);

		if (branch->Parent != nullptr)
		{
			Vector2f ParentPosition = positions[branch->Parent];

			if (branch->Parent->Left == branch)
			{
				ParentPosition.x -= NodeRadius - 2;
				position.x += NodeRadius + 2;
			}	
			else
			{
				ParentPosition.x += NodeRadius + 2;
				position.x -= NodeRadius - 2;
			}

			VertexArray line(Lines, 2);

			line[0].position = ParentPosition;
			line[1].position = position;

			window.draw(line);
		}
	}

	void ReadIntoVector(vector<vector<Tree<T>*>> &container, int hight)
	{
		vector<Tree<T>*> bufferOfTrees;

		bufferOfTrees.push_back(this);

		for (int i = 0; i < hight; i++)
		{
			vector<Tree<T>*> branchesOnTheLevel;

			vector<Tree<T>*> updatedBufferOfTrees;

			for (int j = 0; j < bufferOfTrees.size(); j++)
			{
				Tree<T>* current = bufferOfTrees[j];

				if (current == nullptr)
				{
					branchesOnTheLevel.push_back(nullptr);

					for (int i = 0; i < 2; i++)
						updatedBufferOfTrees.push_back(nullptr);
				}
				else
				{
					updatedBufferOfTrees.push_back(current->Left);
					updatedBufferOfTrees.push_back(current->Right);
					branchesOnTheLevel.push_back(current);
				}
			}

			container.push_back(branchesOnTheLevel);

			bufferOfTrees.clear();

			for (int k = 0; k < updatedBufferOfTrees.size(); k++)
				bufferOfTrees.push_back(updatedBufferOfTrees[k]);
		}
	}
};
