#include<iostream>
#include<fstream>
#include<vector>
#include<string>
using namespace std;
int k = 1;	//�ļ�����
class Node
{
public:
	int files;	//�ڼ����ļ�
	int row;	//��
	int col;	//��
	Node* next;
	Node(int f = 1, int r = -1, int c = -1) :files(f), row(r), col(c), next(NULL) {}
};

class ArrayNode
{
public:
	string word;
	int count;	//���ִ���
	ArrayNode* dirNext;	//��һλָ��
	Node* position;		//λ��ָ��

	ArrayNode() :word(""), count(1), dirNext(NULL)
	{
		position = new Node();
	}
};

class ArrayList
{
public:
	ArrayNode* head = NULL;
	ArrayNode* tail = NULL;
	void insert(ArrayNode *a);
};
ArrayList* arr = new ArrayList();

void ArrayList::insert(ArrayNode *a)
{
	if (head == NULL)
	{
		head = a;
		tail = a;
		return;
	}
	else
	{
		ArrayNode* current = head;
		while (current != NULL)
		{
			if (a->word == current->word)
			{
				current->count++;
				Node* tmp = current->position;
				while (tmp->next != NULL)
				{
					tmp = tmp->next;
				}
				tmp->next = a->position;
				return;
			}
			current = current->dirNext;
		}
		//�����Ѿ��ų�ѭ�����и�a��word��ȫһ���ĵ��ʵ������������������������ݿ϶�����a��һ��
		if (a->word < head->word)
		{
			a->dirNext = head;
			head = a;
			return;
		}
		current = head;
		while (a->word > current->word)
		{
			if (current->dirNext != NULL&&a->word < current->dirNext->word )
			{
				a->dirNext = current->dirNext;
				current->dirNext = a;
				return;
			}
			else if (current->dirNext == NULL)
			{
				current->dirNext = a;
				tail = a;
				return;
			}
			else
			{
				current = current->dirNext;
			}
		}
	}
}

//���ļ���ȡΪ���ַ���
string getfile(string path)
{
	fstream file;
	string res;
	file.open(path, ios::in);
	//cout << path << endl;
	if (file.is_open())
	{
		cout << "file open!\n";
		char ch;
		while (!file.get(ch).eof())
		{
			if (ch >= 'A' && ch <= 'Z') ch += 32;
			res += ch;
		}
		//ͳһ��Сд
	}
	else
	{
		cout << "open failed!\n";
	}
	file.close();
	return res;
}

//�Ƿ�Ϊ�����߿ո�
bool ispunc(char c)
{
	if (c>='a'&&c<='z')
	{
		return 0;
	}
	return 1;
}

//void solution(string text)
//{
//	int row = 1, col = 0, files = 1;
//	string temp = "";
//	int i = 0,textlen = text.length();
//	Node* start = new Node(files, 0, 0);
//	while (i < textlen)
//	{
//		col++;
//		if (text[i] == '\n')		//�����м�һ��Ϊ��
//		{
//			row++;
//			col = 0;
//		}
//		else if (i < textlen && ispunc(text[i]))		//����Ǳ�����
//		{
//			ArrayNode* a = new ArrayNode();
//			if (temp != "")
//			{
//				a->word = temp;
//				a->position = start;
//				arr->insert(a);
//				cout << temp << " ";
//				cout << a->position->files << a->position->col << a->position->row << " ";
//				temp = "";
//			}
//			i++;
//			while (i < textlen && (ispunc(text[i]) || text[i] == '\n'))		//���Ƕ��������
//			{
//				if (text[i] == '\n')
//				{
//					row++;
//					i++;
//					col = 0;
//				}
//				else
//				{
//					col++;
//					i++; 
//				}
//			}
//			continue;
//		}
//		else if (i >= textlen)
//		{
//			return;
//		}
//		//�������ĸ
//		else
//		{
//			//�����һ�����ʵĿ�ʼ
//			if (i == 0 || ispunc(text[i - 1]) || text[i - 1] == '\n')
//			{
//				start->files = k;
//				start->row = row;
//				start->col = col;
//			}
//			temp += text[i];
//		}
//		i++;
//	}
//}
 
 
void solution(string text)
{
	int row = 1, col = 0, files = k;
	string temp = "";
	int i = 0, textlen = text.length();
	while (i < textlen)
	{
		col++;
		if (text[i] == '\n')    // ����ʱ�м�һ��������Ϊ��
		{
			row++;
			col = 0;
		}
		else if (i < textlen && ispunc(text[i]))    // ����Ǳ�����
		{
			ArrayNode* a = new ArrayNode();
			if (temp != "")
			{
				a->word = temp;
				// Ϊ��ǰ���ʴ����µ�λ�ýڵ�
				Node* start = new Node(files, row, col - temp.length());
				a->position = start;
				arr->insert(a);
				cout << temp << " ";
				cout << a->position->files << a->position->col << a->position->row << " ";
				temp = "";
			}
			i++;
			while (i < textlen && (ispunc(text[i]) || text[i] == '\n'))    // ���Ƕ�������ŵ����
			{
				if (text[i] == '\n')
				{
					row++;
					i++;
					col = 0;
				}
				else
				{
					col++;
					i++;
				}
			}
			continue;
		}
		else if (i >= textlen)
		{
			return;
		}
		// �������ĸ
		else
		{
			temp += text[i];  // ��������
		}
		i++;
	}
}




int main(int argc,char* argv[])
{
	if (argc < 2) {
		cout << "Usage: ./main <input1.txt> <input2.txt> ..." << endl;
		return 0;
	}

	vector<string> path;
	for (int i = 1; i < argc; i++) {
		path.push_back(argv[i]);
	}

	while (k < argc)
	{
		ofstream file;
		string outname = "out" + to_string(k) + ".txt";
		file.open(outname);
		string text = getfile(path[k-1]);
		cout << text;
		solution(text);
		ArrayNode* search = arr->head;
		while(search!=NULL)
		{
			file << search->word << ';' << search->count << ';';
			cout << search->word << ';' << search->count << ';';
			Node* temper = search->position;
			while (temper != NULL)
			{
				file << '(' << temper->files << "," << temper->row << ',' << temper->col << ");";
				cout << '(' << temper->files << "," << temper->row << ',' << temper->col << ");";
				temper = temper->next;
			}
			file << endl;
			cout << endl;
			search = search->dirNext;
		}
		file.close();
		k++;
	}
	return 0;
}
//C:\Users\XY\Downloads\ 

