#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

class CsvReader
{
public:

	char delimiter{ '\0' };
	std::string emptyData{ NULL };
	char tableDesignTopBottom{ 'X' };
	char tableDesignCross{ 'X' };
	char tableDesignLeftRight{ 'X' };

	int gapThreshold{ 0 };


	CsvReader();
	~CsvReader();

	void StoreData(std::string file_path);

private:
	std::fstream file;
	std::string word{ "" };
	std::string tempWords{ "" };


	std::vector<std::string> GetBigWords(std::vector<std::vector<std::string>> table);
	void PrintTable(std::vector<std::vector<std::string>> table);

	int GetRowNumber(std::fstream & file);
	int GetColumnNumber(std::fstream & file);
	std::string RemoveSpace(std::string word);

};

CsvReader::CsvReader()
{
}

CsvReader::~CsvReader()
{
}

std::vector<std::string> CsvReader::GetBigWords(std::vector<std::vector<std::string>> table)
{
	std::string GetBigWords{ "" };
	int len{ 0 };
	std::vector<std::string> vec;
	for (int i = 0; i < size(table); i++)
	{
		for (int j = 0; j < size(table[i]); j++)
		{
			len = size(table[i].at(j));
			if (len > size(GetBigWords))
			{
				GetBigWords = table[i].at(j);
			}


		}

		vec.push_back(GetBigWords);
		GetBigWords = "";

	}
	return vec;
}

void CsvReader::PrintTable(std::vector<std::vector<std::string>> table)
{
	int tableColumnNumber = size(table);
	int tableRowNumber = size(table[0]);


	std::string bigWord{ "\0" };
	int bigWordlength{ 0 };
	int gap{ 0 };
	int addedSize{ 0 };

	//vector of biggest words in each column
	std::vector<std::string>bigWordList;
	bigWordList = GetBigWords(table);

	//adds total table lenth
	for (int i = 0; i < size(bigWordList); i++)
	{
		addedSize = addedSize + size(bigWordList[i]);
	}

	//addedSize = addedSize + (tableColumnNumber*(gapThreshold + gapThreshold)) + (tableColumnNumber + 1);
	////print 

	//std::cout << std::string(addedSize, tableDesignTopBottom) << std::endl;


	for (int i = 0; i < size(bigWordList); i++)
	{
		std::cout << tableDesignTopBottom;
		std::cout << std::string(size(bigWordList[i]) + (gapThreshold * 2), tableDesignCross);

	}
	std::cout << tableDesignTopBottom;
	std::cout << " " << std::endl;

	//print
	for (int i = 0; i < tableRowNumber; i++)
	{
		for (int j = 0; j < tableColumnNumber; j++)
		{
			if (j == 0)
			{
				std::cout << '|';
			}
			gap = size(bigWordList[j]) - size(table[j].at(i));
			std::cout << std::string(gapThreshold, ' ') << table[j].at(i) << std::string(gap + gapThreshold, ' ') << "|";

		}
		std::cout << " " << std::endl;
	}

	//print
	for (int i = 0; i < size(bigWordList); i++)
	{
		std::cout << tableDesignTopBottom;
		std::cout << std::string(size(bigWordList[i]) + (gapThreshold * 2), tableDesignCross);

	}
	std::cout << tableDesignTopBottom;
	std::cout << " " << std::endl;


	//std::cout << std::string(addedSize, tableDesignTopBottom) << std::endl;

}

std::string CsvReader::RemoveSpace(std::string word)
{
	size_t first = word.find_first_not_of(" \t\r\f\v"); // whitespaces
	size_t last = word.find_last_not_of(" \t\r\f\v"); // whitespaces
	if (first != std::string::npos && last != std::string::npos) {
		word.erase(0, first);
		word.erase(last - first + 1);
	}
	else {
		word.clear();
	}

	return word;
}

int CsvReader::GetRowNumber(std::fstream &file)
{
	file.clear();
	file.seekg(0);

	int rowCounter{ 0 };

	//counts row
	while (1)
	{
		if (std::getline(file, word))
		{
			rowCounter++;
		}
		else if (file.eof())
		{
			std::cout << "------------------------" << std::endl;
			break;
		}
		else
		{
			std::cout << "ERROR" << std::endl;
		}
	}
	std::cout << "Row: " << rowCounter << std::endl;
	std::cout << "------------------------" << std::endl;

	file.clear();
	file.seekg(0);

	return rowCounter;
}

int CsvReader::GetColumnNumber(std::fstream &file)
{
	file.clear();
	file.seekg(0);

	std::vector<int>delimiter_counts;
	int columnCounter{ 0 };
	int index{ 0 };

	while (1)
	{
		std::getline(file, word);
		for (int i = 0; i < size(word); i++)
		{
			if (word[i] == delimiter)
			{
				columnCounter++;
			}
		}
		delimiter_counts.push_back(columnCounter + 1);
		columnCounter = 0;
		if (file.eof())
		{
			std::cout << "------------------------" << std::endl;
			break;
		}
		index++;
	}

	columnCounter = *max_element(delimiter_counts.begin(), delimiter_counts.end());


	std::cout << "Column: " << columnCounter << std::endl;
	std::cout << "------------------------" << std::endl;

	file.clear();
	file.seekg(0);

	return columnCounter;
}

void CsvReader::StoreData(std::string file_path)
{
	std::string tempWord;
	int flag{ 0 };
	int space{ 0 };

	//open the file
	file.open(file_path, std::ios::in);

	//gets column number
	int columnNumber = GetColumnNumber(file);
	int rowNumber = GetRowNumber(file);

	//creates dynamic vector
	//creates a vector named DataTable which has n(n=columnNumber) numbers of vector
	std::vector<std::vector<std::string>> DataTable(columnNumber);

	file.clear();
	file.seekg(0);

	//row
	for (int i = 0; i < rowNumber; i++)
	{
		for (int j = 0; j < columnNumber; j++)
		{
			std::getline(file, word, delimiter);

			//removes front and tail space
			word = RemoveSpace(word);

			//flag to increase Iterator
			if (flag != 0)
			{
				DataTable[j].push_back(tempWord);
				tempWord = emptyData;

				j++;
			}

			//word = something
			if (size(word) > 0 && word.find('\n') == std::string::npos)
			{
				DataTable[j].push_back(word);
				flag = 0;
			}

			//word contains \n 
			else if (word.find('\n') != std::string::npos)
			{

				//word = something \n 
				if (word.back() == '\n' && size(word) > 1)
				{
					size_t index = word.find('\n');
					word.erase(word.begin() + index, word.end());
					DataTable[j].push_back(word);
					tempWord = emptyData;

					flag++;
					break;
				}

				//word = \n something 
				else if (word.front() == '\n' && size(word) > 1)
				{

					DataTable[j].push_back(emptyData);
					tempWord = word;
					tempWord.erase(std::remove(tempWord.begin(), tempWord.end(), '\n'), tempWord.cend());

					flag++;
					break;

				}

				else if (word.front() == '\n' &&  size(word) == 1)
				{
					DataTable[j].push_back(emptyData);
					tempWord = emptyData;

					flag++;
					break;
				}

				//word = something \n something 
				else
				{
					tempWord = word;
					word = word.substr(0, word.find('\n', 0));
					DataTable[j].push_back(word);
					tempWord = tempWord.substr(size(word) + 1, word.find('\n', 0));

					flag++;
					break;
				}

			}

			// words = whitespaces or null 
			else if (word == "")
			{
				DataTable[j].push_back(emptyData);
				flag = 0;
			}

			//end of file
			else if (file.eof())
			{
				std::cout << word << std::endl;
			}

			//ERROR
			else
			{
				std::cout << "ERROR" << std::endl;
			}
		}
	}

	file.close();

	PrintTable(DataTable);
}

int main()
{
	std::string file_path = "F:/airtravel.txt";

	CsvReader obj;
	obj.delimiter = ',';
	obj.emptyData = "-";

	obj.tableDesignTopBottom = '+';
	obj.tableDesignCross = '-';
	obj.tableDesignLeftRight = '|';

	obj.gapThreshold = 2;

	obj.StoreData(file_path);

	return 0;
}
