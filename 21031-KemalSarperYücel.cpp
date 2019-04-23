#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "SearchEngine.h"

using namespace std;
template<class Object>
void printList(const List<Object> &l)
{
	if (l.isEmpty())
		cout << "The list is empty.\n";
	else
	{//Print each element one by one
		Iterator<Object> getter = l.first();
		for (; !getter.isPastEnd(); getter.advance())
		{
			cout << getter.retrieve() << " ";
			if (!l.goesBelow())
			{
				Iterator<Object> vgetter = getter;
				vgetter.gounder();
				for (; !vgetter.isPastEnd(); vgetter.gounder())
					cout << vgetter.retrieve() << ", ";
			}
			cout << endl;
		}
	}
	cout << endl;
}
int main()
{
	try{
		List<string> textlist;//Get the input
		Iterator<string> itr = textlist.zeroth();
		List<string> doclist;
		ifstream text;
		string line, textname;
		textname = "docdb.txt";
		text.open(textname.c_str());
		while (getline(text, line))
		{
			int dnum;
			string word;
			stringstream stream(line);
			stream >> word;	//The first element on each line will be a word according to the example in the pdf.
			stream >> dnum;	//The second element on each line will be a document number according to the example in the pdf.
			if (stream.fail())
				throw 'c';
			string document = to_string(dnum);//Convert it to string to add it to the list. Adding different types of variables to the list caused errors for this implementation. 
			if (textlist.find(word).isPastEnd())
			{
				textlist.insert(word, textlist.findplaces(word));
				textlist.insertBelow(document, textlist.findvplaces(document, textlist.find(word)));
			}
			else
			{
				textlist.insertBelow(document, textlist.findvplaces(document, textlist.find(word)));
			}
			if (doclist.find(document).isPastEnd())
			{
				doclist.insert(document, doclist.findplaces(document));
				doclist.insertBelow(word, doclist.findvplaces(word, doclist.find(document)));
			}
			else
			{
				doclist.insertBelow(word, doclist.findvplaces(word, doclist.find(document)));
			}
		}
		printList(textlist);
		printList(doclist);
		int num = 1;//it is equal to one the set the while loop up;
		string words1, words2;
		while (num != 0)//query loop repeats until the user wants to exit
		{
			cout << "Please enter a query(type '0' to exit)" << endl;
			cin >> num;//Get the integer that informs the program how many iterations will be performed first
			if (!cin)
			{
				throw 1;

			}
			else if (num == 0)//Check if the number is valid
			{
			}
			else
			{
				cin >> words1;//Get the first word
				cout << words1 << " ";
				List<string> output1 = textlist.getBottom(textlist.find(words1));
				for (int i = 1; i < num; i++)
				{
					cin >> words2;
					cout << words2 << " ";
					List<string> output2 = textlist.getBottom(textlist.find(words2));
					output1.intersect(output2);
					words2.clear();
				}
				words1.clear();
				Iterator<string> p = output1.first();
				List<string> Temp;
				Iterator<string> ins = Temp.zeroth();
				while (!p.isPastEnd())
				{
					Temp.insert(p.retrieve(), ins);
					Iterator<string> v = doclist.find(p.retrieve());
					v.gounder();
					ins.advance();
					while (!v.isPastEnd())
					{
						Temp.insertBelow(v.retrieve(), ins);
						v.gounder();
					}
					p.advance();
				}
				if (num != 0)
				{	int cs = 0;
					for (Iterator<string> ptr = Temp.first(); !ptr.isPastEnd(); ptr.advance(), cs++) {}
					cout << cs << endl;
					printList(Temp);}	
			}
		}
		system("pause");
		return 0;
	}
	catch (string error)
	{
		cout << error << " Exiting..." << endl;
		system("pause");
		exit(-1);
	}
	catch (int error)//catch the invalid input error during query
	{
		cout << "Invalid input in query. Exiting..." << endl;
		system("pause");
		exit(-1);
	}
	catch (char error)
	{
		cout << "Invalid orders in the file. Exiting..." << endl;
		system("pause");
		exit(-1);
	}
}