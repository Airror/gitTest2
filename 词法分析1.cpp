
#include "stdafx.h"
#include"stack" 

#include<string>
#include<iostream>
#include "fstream"

#include"stdio.h"
#include"vector"
#include"map"



//����״̬��ΧΪ0-5
//�����״̬��ΧΪ6-20
//�ָ��״̬��ΧΪ21-40
//��һЩСС���޸�

using namespace std;
const int OTHER_ERROR = -2;
const int OPERATOR_ERROR = -3;
const int CONSTANT_ERROR=	-1;
const int KEYWORD=			1;
const int IDENTIFIER=		2;
const int OPERATOR=			3;
const int DELIMITER=		4;
const int CONSTANT=			5;
char transferTable[128][128];
vector<string> KeywordList =
{
	"auto",
	"short",
	"int",
	"long",
	"float",
	"double",
	"char",
	"struct",
	"union",
	"enum",
	"typedef",
	"const",
	"unsigned",
	"signed",
	"extern",
	"register",
	"static",
	"volatile",
	"void",
	"if",
	"else",
	"switch",
	"case",
	"for",
	"do",
	"while",
	"goto",
	"continue",
	"break",
	"default",
	"sizeof",
	"return",
	"main"
};
vector<string> OperatorList =
{
	"+",
	"-",
	"*",
	"/",
	"++",
	"--",
	">",
	">=",
	"<",
	"<=",
	"==",
	"=",
	"&",
	"|",
	"%"
};
vector<string> DelimiterList =
{
	",",
	";",
	"{",
	"}",
	"(",
	")"
};

typedef struct binTable
{
	string data;
	int type;
}binTable;

binTable constant_Automat(vector<char>::iterator *runner)
{
	binTable result;
	int intAccept = 1;
	int floatAccept = 3;
	int state = 0;
	vector<int> stateStack;
	
	result.type = CONSTANT;
	while (	(state <= 3) && (state >= -1))
	{
		result.data.push_back(**runner);
		char temp = (**runner);
		if (state == CONSTANT_ERROR)
		{
			result.type = CONSTANT_ERROR;
			return result;
		}
	
		state = transferTable[**runner][state];
		stateStack.push_back(state);
		(*runner)++;
	}
	(*runner)--;
	(*runner)--;
	result.data.pop_back();
	stateStack.pop_back();
	state = stateStack.back();
	if ((state == intAccept) || (state == floatAccept))
	{
		return result;
	}
	else if (state == CONSTANT_ERROR)
	{
		result.type = CONSTANT_ERROR;
		return result;
	}
	else
	{
		cout << "inside CONSTANT NOT ACCEPTED!\n";
		getchar();
		exit(0);
	}
}

binTable keyword_identifier_Automat(vector<char>::iterator *runner)
{
	binTable result;
	int state = 0;
	while (		((**runner >= 'A') && (**runner <= 'Z'))||
				((**runner >= 'a') && (**runner <= 'z'))||
				((**runner>='0')&&(**runner<='9'))		)
	{
		result.data.push_back(**runner);
		(*runner)++;
	}
	
	vector<string>::iterator keyRunner;
	(*runner)--;
	for (keyRunner = KeywordList.begin(); keyRunner!= KeywordList.end(); keyRunner++)
	{
		if ((*keyRunner) == result.data)
		{
			result.type = KEYWORD;
			return result;
		}
	}
	result.type = IDENTIFIER;

	return result;
}

binTable operator_Automat(vector<char>::iterator *runner)
{
	binTable result;
	int state = 5;
	vector<int> stateStack;
	while ((state >= 5) && (state <= 20))
	{
		stateStack.push_back(state);
		result.data.push_back(**runner);
		state=transferTable[**runner][state];
		(*runner)++;
	}
	if (stateStack.size() == 0)
	{
		cout << "illegality alphabet!\n";
		getchar();
		exit(0);
	}
	if (state == OPERATOR_ERROR)
	{
		cout << "OPERATOR ERROR!\n";
		getchar();
		exit(0);
	}
	else
	{
		state = stateStack.back();
		(*runner)--;
		(*runner)--;
		result.data.pop_back();
		result.type = OPERATOR;
		return result;

	}

	
	return result;
}

void initTable()
{
	//��ʼ���Զ�����ת��
	int i;
	//������ʼ��
	for (i = '0'; i < '9'; i++)
	{
		transferTable[i][0] = 1;
		transferTable[i][1] = 1;
		transferTable[i][2] = 3;
		transferTable[i][3] = 3;
	}
	transferTable['.'][0] = CONSTANT_ERROR;
	transferTable['.'][1] = 2;
	transferTable['.'][2] = CONSTANT_ERROR;
	transferTable['.'][3] = CONSTANT_ERROR;
	for (i = 0; i < 4; i++)
	{
		transferTable[' '][i] = OTHER_ERROR;
		transferTable['\t'][i] = OTHER_ERROR;
		transferTable['\n'][i] = OTHER_ERROR;


		transferTable['+'][i] = OTHER_ERROR;
		transferTable['-'][i] = OTHER_ERROR;
		transferTable['*'][i] = OTHER_ERROR;
		transferTable['/'][i] = OTHER_ERROR;
		transferTable['>'][i] = OTHER_ERROR;
		transferTable['<'][i] = OTHER_ERROR;
		transferTable['='][i] = OTHER_ERROR;
		transferTable['%'][i] = OTHER_ERROR;
		transferTable['|'][i] = OTHER_ERROR;
		transferTable['&'][i] = OTHER_ERROR;


		transferTable[';'][i] = OTHER_ERROR;
		transferTable[','][i] = OTHER_ERROR;
		transferTable['}'][i] = OTHER_ERROR;
		transferTable['{'][i] = OTHER_ERROR;
		transferTable['('][i] = OTHER_ERROR;
		transferTable[')'][i] = OTHER_ERROR;

	}

	//�������ʼ��

	for (i = 0; i <= 20; i++)
	{
		transferTable['+'][i] = OPERATOR_ERROR;
		transferTable['-'][i] = OPERATOR_ERROR;
		transferTable['<'][i] = OPERATOR_ERROR;
		transferTable['>'][i] = OPERATOR_ERROR;
		transferTable['='][i] = OPERATOR_ERROR;
		transferTable['*'][i] = OPERATOR_ERROR;
		transferTable['/'][i] = OPERATOR_ERROR;
		transferTable['&'][i] = OPERATOR_ERROR;
		transferTable['|'][i] = OPERATOR_ERROR;
		transferTable['%'][i] = OPERATOR_ERROR;

	}

	transferTable['+'][5] = 6;
	transferTable['+'][6] = 7;
	transferTable['-'][5] = 8;
	transferTable['-'][8] = 9;

	transferTable['<'][5] = 10;
	transferTable['='][10] = 11;

	transferTable['>'][5] = 12;
	transferTable['='][12] = 13;

	transferTable['='][5] = 14;
	transferTable['='][14] = 15;

	transferTable['*'][5] = 16;
	transferTable['/'][5] = 17;
	transferTable['&'][5] = 18;
	transferTable['|'][5] = 19;
	transferTable['%'][5] = 20;
}
vector<binTable> C_Automat(vector<char>::iterator head,vector<char>::iterator end)
{
	string manageTemp;
	vector<char>::iterator manageRunner;
	vector<binTable> result;
	int i;
	int lineNumber = 0;
	initTable();
	char *strTemp;

	//ɨ��Դ����	
	for (
		vector<char>::iterator manageRunner = head;
		*manageRunner != '\0';
		manageRunner++
		)
	{
		//��ָ���ƶ�����һ���ǿ��ַ�
		//ͨ���Իس����������ж�����ȡ�к�
		while(	(*manageRunner == ' ')||
				(*manageRunner=='	')||
				(*manageRunner=='\n') )
		{
			if (*manageRunner == '\n')
			{
				lineNumber++;
			}
			manageRunner++;
			
		}
		//���б����ַ����ж�

		//��������
		if (	(*manageRunner==',')||
				(*manageRunner==';')||
				(*manageRunner=='{')||
				(*manageRunner=='}')||
				(*manageRunner=='(')||
				(*manageRunner==')')	)
		{
			//����������Ԫ��������
			binTable temp;
			temp.data = *manageRunner;
			temp.type = DELIMITER;
			cout <<lineNumber<< "\t" << *manageRunner << "\t" << "DELIMITER" << "\n";

			result.push_back(temp);
		}
		//���������
		else if (*manageRunner >= '0'&&*manageRunner <= '9')
		{
			//�ڳ����Զ����ڽ��н����ж�
			binTable temp = constant_Automat(&manageRunner);
			if (temp.type == -1)
			{
				cout <<lineNumber<<"\t"<< "CONSTANT NOT ACCEPTED!\n";
				getchar();
				exit(0);
			}
			else
			{
				result.push_back(temp);
				string c = "123";
				cout << lineNumber << "\t" <<temp.data << "\t" << "CONSTANT" << "\n";
				
			}
		}
		//��ʶ����ؼ��ֵ����
		else if(	(*manageRunner >= 'a'&&*manageRunner <= 'z')||
					(*manageRunner >= 'A'&&*manageRunner <= 'Z')	)
		{
			binTable temp=keyword_identifier_Automat(&manageRunner);
			result.push_back(temp);
			if (temp.type == KEYWORD)
			{


				cout << lineNumber << "\t" << temp.data << "\t" << "KEYWORD" << "\n";
			}
			else
			{
				cout << lineNumber << "\t" << temp.data << "\t" << "IDENTIFIER" << "\n";
			}
		}
		//����������
		else
		{
			binTable temp= operator_Automat(&manageRunner);
			result.push_back(temp);
			
			 cout << lineNumber << "\t" << temp.data << "\t" << "OPERATOR" << "\n";

		}

	}
	return result;
}
int main()
{
	vector<char> source;
	vector<binTable> output;
	FILE *pFile;

	if ((pFile = fopen("input.txt", "r")) == NULL)
	{
		printf("load failed!\n");
		getchar();
		return 0;
	}
	cout << "load succeed!\n";
	while (true)
	{
		int temp = fgetc(pFile);
		if (temp == EOF)
		{
			break;
		}
		source.push_back(temp);
	}
	source.push_back('\0');
	FILE *outputer;
	output = C_Automat(source.begin(), source.end());
	if ((outputer = fopen("output.txt", "w")) == NULL)
	{
		printf("write failed!\n");
		getchar();
		return 0;
	}
	cout << "now writing to output.txt\n";

	vector<binTable>::iterator writer;
	for (writer = output.begin(); writer != output.end();writer++)
	{
		fputc('(', outputer);
		string::iterator strRunner;
		{
			fputc(*strRunner, outputer);
		}
		fputc(',',outputer);
		fputc((*writer).type+'0',outputer);
		fputc(')', outputer);
		fputc('\n', outputer);
	}
	cout << "finish";
	system("pause");
	return 0;		
}										

