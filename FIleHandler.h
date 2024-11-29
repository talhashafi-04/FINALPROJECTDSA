#pragma once
#include <fstream>
#include "String.h"
#include "Vector.h"


using namespace std;

class FileHandler /// csv handler 
{	
	FILE* dataset;
	int headings = 0; 
	int selecteted = 0;
public :

	FileHandler(String& fileName)
	{
		headings = 0;
		selecteted = 0;
		openFile(fileName);
	}
	void openFile(String& fileName)
	{
		dataset = fopen(fileName.data , "wb");
		if (dataset == nullptr)
		{
			int a; cin >> a;
		}
		

		//set headings

	}
	void rowInput()
	{
		//display
		//intpu
		//valid

	}

	void selectTree()
	{

	}


	void insertTree()
	{
		if (selecteted == 1)
		{

		}
		else if (selecteted == 2)
		{

		}

	}
	
};